# 书籍学习蓝图生成器 - 最终工程版 (独立运行引擎)
# 本文件由 starter.py 自动生成, 请勿手动修改

import os
import sys
import re
import time
import json
import yaml
import logging
from pathlib import Path
from datetime import datetime
from typing import List, Dict, Optional, Tuple
from openai import OpenAI, RateLimitError, APITimeoutError, APIError

PROJECT_NAME = "book_ai_learning_engine"
BASE_DIR = Path.cwd()
BASE_DIR.mkdir(exist_ok=True)

PROMPTS_DIR = BASE_DIR / "prompts"
PROMPTS_DIR.mkdir(exist_ok=True)
OUTPUT_DIR = BASE_DIR / "output"
OUTPUT_DIR.mkdir(exist_ok=True)
CODE_ROOT = BASE_DIR / "code_labs"
CODE_ROOT.mkdir(exist_ok=True)
LOG_DIR = BASE_DIR / "logs"
LOG_DIR.mkdir(exist_ok=True)

LOG_FILE = LOG_DIR / f"generation_{datetime.now().strftime('%Y%m%d_%H%M%S')}.log"
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
    handlers=[
        logging.FileHandler(LOG_FILE, encoding='utf-8'),
        logging.StreamHandler(sys.stdout)
    ]
)
logger = logging.getLogger(__name__)

PROGRESS_FILE = BASE_DIR / ".generation_progress.json"

CONFIG_PATH = BASE_DIR / "config.yaml"

class Config:
    def __init__(self, data: dict):
        self.api_key: str = data.get("api_key", "")
        self.model: str = data.get("model", "deepseek-chat")
        self.output_dir: str = data.get("output_dir", "./output")
        self.book_title: str = data.get("book_title", "")
        self.max_concurrent: int = data.get("max_concurrent_requests", 5)
        self.timeout: int = data.get("api_timeout", 120)
        self.max_retries: int = 3
        self.rate_limit_delay: float = 1.0
        self.table_of_contents: List[str] = data.get("table_of_contents", [])
        self.mode: str = data.get("mode", "all")

    def validate(self) -> Tuple[bool, str]:
        if self.mode not in ["generate", "merge", "all"]:
            return False, f"mode 配置错误: '{self.mode}', 允许的值: generate, merge, all"
        if self.mode in ["generate", "all"]:
            if not self.api_key or self.api_key == "sk-你的DeepSeekAPI密钥":
                return False, "API Key 未填写。请在 config.yaml 中填入真实的 DeepSeek API Key."
            if not self.api_key.startswith("sk-"):
                return False, "API Key 格式错误, 应以 sk- 开头"
        if not self.book_title:
            return False, "book_title 为空, 请在 config.yaml 中填写书籍名称"
        if not isinstance(self.table_of_contents, list):
            return False, "table_of_contents 必须是 YAML 列表格式, 请检查缩进和 '-'"
        if not self.table_of_contents:
            return False, "table_of_contents 列表为空, 请至少包含一个章节"
        return True, ""

def load_config() -> Optional[Config]:
    if not CONFIG_PATH.exists():
        logger.error(f"配置文件不存在: {CONFIG_PATH}")
        return None
    try:
        with open(CONFIG_PATH, "r", encoding="utf-8-sig") as f:
            raw_data = yaml.safe_load(f)
        cfg = Config(raw_data)
        valid, msg = cfg.validate()
        if not valid:
            logger.error(f"配置校验失败: {msg}")
            return None
        return cfg
    except Exception as e:
        logger.error(f"读取配置文件失败: {e}")
        return None

def ensure_prompts():
    pass

def select_category(cfg: Config) -> Optional[Path]:
    try:
        client = OpenAI(api_key=cfg.api_key, base_url="https://api.deepseek.com")
        prompt = f"判断书籍分类: C++, Algorithms, ROS, OS, Networking, Design_Patterns, Linux, AI, Generic。书名:{cfg.book_title}"
        resp = client.chat.completions.create(
            model=cfg.model,
            messages=[{"role": "user", "content": prompt}],
            temperature=0.1,
            max_tokens=100,
            timeout=30
        )
        content = resp.choices[0].message.content.strip()
        # 修正正则匹配, 确保不会因为换行符断裂
        content = re.sub('```json\\n?|```\\n?', '', content).strip()
        result = json.loads(content)
        category = result.get("category", "Generic")
    except Exception as e:
        logger.warning(f"分类选择失败, 使用 Generic 模板: {e}")
        category = "Generic"
    prompt_path = PROMPTS_DIR / f"{category}.md"
    return prompt_path

def safe_filename(title: str, max_len: int = 40) -> str:
    cleaned = re.sub('[<>:"/\\\\|?*]', '_', title)
    cleaned = cleaned.strip('_')
    return cleaned[:max_len]

def call_with_retry(client, model, messages, max_retries=3, timeout=120):
    for attempt in range(1, max_retries + 1):
        try:
            resp = client.chat.completions.create(
                model=model,
                messages=messages,
                temperature=0.7,
                max_tokens=4096,
                timeout=timeout
            )
            return resp.choices[0].message.content
        except RateLimitError:
            wait = 2 ** attempt
            logger.warning(f"触发限流, 等待 {wait} 秒后重试 (第 {attempt}/{max_retries} 次)")
            time.sleep(wait)
        except APITimeoutError:
            wait = 2 ** attempt
            logger.warning(f"API 超时, 等待 {wait} 秒后重试 (第 {attempt}/{max_retries} 次)")
            time.sleep(wait)
        except APIError as e:
            if e.status_code in [500, 502, 503, 504]:
                wait = 2 ** attempt
                logger.warning(f"服务端错误 {e.status_code}, 等待 {wait} 秒后重试")
                time.sleep(wait)
            else:
                logger.error(f"API 错误 (不可重试): {e}")
                raise
        except Exception as e:
            logger.error(f"未知错误: {e}")
            raise
    logger.error("重试次数耗尽, 返回 None")
    return None

def load_progress() -> set:
    if PROGRESS_FILE.exists():
        try:
            with open(PROGRESS_FILE, "r", encoding="utf-8") as f:
                data = json.load(f)
                return set(data.get("completed", []))
        except:
            return set()
    return set()

def save_progress(completed: set):
    with open(PROGRESS_FILE, "w", encoding="utf-8") as f:
        json.dump({"completed": list(completed)}, f, ensure_ascii=False, indent=2)

def generate_chapter_content(
    client, cfg, system_prompt, book_title, chapter_title
) -> Optional[str]:
    format_instruction = '''
[格式重申] 允许开头以[章节知识点概述]开头, 但禁止AI自我介绍、禁废话。随后严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可编译运行代码**
3. **内核验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**
'''
    messages = [
        {"role": "system", "content": system_prompt + "\n\n" + format_instruction},
        {"role": "user", "content": f"书籍标题: {book_title}\n章节标题: {chapter_title}"}
    ]
    return call_with_retry(
        client,
        cfg.model,
        messages,
        max_retries=cfg.max_retries,
        timeout=cfg.timeout
    )

def extract_code_blocks(content: str, chapter_dir: Path) -> int:
    patterns = [
        '```(?:c|cpp|cc|C)\n(.*?)```',
        '```c(?:.*?)\n(.*?)```'
    ]
    code_blocks = []
    for pattern in patterns:
        blocks = re.findall(pattern, content, re.DOTALL)
        if blocks:
            code_blocks = blocks
            break
    if not code_blocks:
        with open(chapter_dir / "NO_CODE_BLOCK.txt", "w", encoding="utf-8") as f:
            f.write("AI 未输出有效的 C 代码块。")
        return 0
    for i, code in enumerate(code_blocks):
        c_file = chapter_dir / f"sim_lab_{i+1}.c"
        with open(c_file, "w", encoding="utf-8") as f:
            f.write(code.strip())
    return len(code_blocks)

def merge_chapters(output_dir: Path, book_title: str):
    chapters = sorted(output_dir.glob("*.md"))
    if not chapters:
        logger.warning("没有找到章节文件, 无法生成合并文档。")
        return

    merged_file_name = safe_filename(book_title) + "_合集.md"
    chapters = [f for f in chapters if f.name != merged_file_name]

    if not chapters:
        logger.info("已检测到合并文档, 无需重复合并。")
        return

    merged_path = output_dir.parent / merged_file_name

    with open(merged_path, "w", encoding="utf-8") as outfile:
        outfile.write(f"# {book_title}\n\n")
        outfile.write(f"*生成日期: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}*\n\n")
        outfile.write(f"*总章节数: {len(chapters)}*\n\n")

        outfile.write("## 目录\n\n")
        toc_list = []
        for ch_file in chapters:
            content = ch_file.read_text(encoding="utf-8")
            first_line = content.split('\n')[0]
            if first_line.startswith('# '):
                title = first_line[2:]
                toc_list.append(f"- [{title}](#{title.replace(' ', '-')})")
        if toc_list:
            outfile.write("\n".join(toc_list) + "\n\n")
        else:
            outfile.write("*(未检测到可用目录索引)*\n\n")

        outfile.write("---\n\n")
        for ch_file in chapters:
            content = ch_file.read_text(encoding="utf-8")
            outfile.write(content)
            outfile.write("\n\n---\n\n")

    logger.info(f"✅ 所有章节已合并为完整书籍文档: {merged_path}")

def main():
    start_time = time.time()
    logger.info("=" * 60)
    logger.info(" 书籍学习蓝图生成器 - 最终工程版 (允许章节前言)")

    cfg = load_config()
    if not cfg:
        sys.exit(1)

    logger.info(f" 模式: {cfg.mode}")
    logger.info("=" * 60)

    if cfg.mode in ["generate", "all"]:
        prompt_path = select_category(cfg)
        with open(prompt_path, "r", encoding="utf-8") as f:
            system_prompt = f.read().strip()
        logger.info(f"使用提示词模板: {prompt_path.name}")

        client = OpenAI(api_key=cfg.api_key, base_url="https://api.deepseek.com")
        logger.info(f"使用模型: {cfg.model}")

        completed = load_progress()
        chapters = cfg.table_of_contents
        logger.info(f"已生成 {len(completed)}/{len(chapters)} 章")

        total_success = 0
        failed_chapters = []

        for idx, title in enumerate(chapters):
            safe_name = safe_filename(title) or f"Chapter_{idx+1:02d}"
            chapter_dir = CODE_ROOT / safe_name
            chapter_dir.mkdir(exist_ok=True)
            md_path = OUTPUT_DIR / f"{safe_name}.md"

            if title in completed or md_path.exists():
                logger.info(f"⏩ 跳过已生成章节 [{idx+1}/{len(chapters)}]: {title}")
                total_success += 1
                continue

            logger.info(f"⏳ 生成中 [{idx+1}/{len(chapters)}]: {title}")
            try:
                content = generate_chapter_content(client, cfg, system_prompt, cfg.book_title, title)
                if content is None:
                    raise Exception("重试失败, 跳过本章")

                with open(md_path, "w", encoding="utf-8") as f:
                    f.write(content)

                code_count = extract_code_blocks(content, chapter_dir)
                if code_count > 0:
                    logger.info(f"  ✅ 提取 {code_count} 个代码文件")
                else:
                    logger.warning(f"  ⚠️ 未找到代码块, 已记录占位")

                completed.add(title)
                save_progress(completed)
                total_success += 1
                time.sleep(cfg.rate_limit_delay)

            except Exception as e:
                logger.error(f"❌ 第 {idx+1} 章生成失败: {title}")
                logger.error(f"   错误: {e}")
                failed_chapters.append(f"{idx+1}. {title}: {e}")

        elapsed = time.time() - start_time
        logger.info("=" * 60)
        logger.info(f"✅ 生成执行完成！耗时: {elapsed:.1f} 秒")
        logger.info(f"✅ 成功: {total_success} 章 / 共 {len(chapters)} 章")
        if failed_chapters:
            logger.warning(f"⚠️ 失败: {len(failed_chapters)} 章")
            with open(LOG_DIR / "failed_chapters.log", "w", encoding="utf-8") as f:
                f.write("\n".join(failed_chapters))
            logger.info(f"失败详情已保存至: {LOG_DIR / 'failed_chapters.log'}")
        else:
            logger.info("🎉 所有章节均生成成功！")

        logger.info(f"📄 输出目录: {OUTPUT_DIR}")
        logger.info(f"💻 代码目录: {CODE_ROOT}")
        logger.info(f"📋 日志文件: {LOG_FILE}")

    if cfg.mode in ["merge", "all"]:
        logger.info("📚 正在合并所有章节为一个完整的文档...")
        merge_chapters(OUTPUT_DIR, cfg.book_title)
    else:
        logger.info("模式为 generate, 不执行合并。")

    logger.info("=" * 60)

if __name__ == "__main__":
    main()
