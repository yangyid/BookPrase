import os
from pathlib import Path

# ============================================================
# 优化后的提示词（通俗版）
# ============================================================
PROMPTS = {
    "C++.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍、客套话。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可编译运行代码**(要求使用 `g++ -std=c++17 -O2` 编译,严禁使用 Python)
3. **内核/性能验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "Algorithms.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可运行代码**(必须使用 C++ 或 C,严禁使用 Python)
3. **性能基准实验**
4. **理论-实践映射表**
5. **可深挖的知识点与进阶应用**
6. **代码练习任务**""",

    "ROS.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可运行代码**(必须使用 C++,严禁使用 Python)
3. **系统/网络验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "OS.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可编译运行代码**(必须使用 C 或 C++,严禁使用 Python)
3. **内核验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "Networking.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可编译运行代码**(必须使用 C 或 C++,严禁使用 Python)
3. **内核验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "Design_Patterns.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可编译运行代码**(必须使用 C++,严禁使用 Java/Python)
3. **架构静态分析实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "Linux.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可编译运行代码**(必须使用 C 或 C++,严禁使用 Python)
3. **内核/系统验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "AI.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言,白话类比与大师避坑指南**
2. **保姆级可运行代码**
3. **GPU/性能验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**""",

    "Generic.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出:
1. **核心理论断言**
2. **仿真代码示例**
3. **内核/系统验证实验**
4. **理论-实践映射表**
5. **可深挖的知识点与学习链接**
6. **代码练习任务**"""
}

# ============================================================
# 引擎代码 – 彻底修复三引号冲突
# ============================================================
ENGINE_CODE = """
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

BASE_DIR = Path.cwd()

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
        self.category: str = data.get("category", "auto")
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
        allowed_categories = ["C++", "Algorithms", "ROS", "OS", "Networking",
                              "Design_Patterns", "Linux", "AI", "Generic", "auto"]
        if self.category not in allowed_categories:
            return False, f"category 配置无效: '{self.category}'，允许的值: {allowed_categories}"
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
    valid_categories = ["C++", "Algorithms", "ROS", "OS", "Networking",
                        "Design_Patterns", "Linux", "AI", "Generic"]
    try:
        client = OpenAI(api_key=cfg.api_key, base_url="https://api.deepseek.com")
        prompt = (
            "请判断以下书籍所属的技术分类，只回答一个分类名，不要解释。\\n"
            "可选项：C++, Algorithms, ROS, OS, Networking, Design_Patterns, Linux, AI, Generic\\n"
            f"书名：{cfg.book_title}"
        )
        resp = client.chat.completions.create(
            model=cfg.model,
            messages=[{"role": "user", "content": prompt}],
            temperature=0.0,
            max_tokens=20,
            timeout=30
        )
        raw = resp.choices[0].message.content.strip()
        for cat in valid_categories:
            if cat.lower() in raw.lower():
                category = cat
                break
        else:
            cleaned = re.sub(r'[^A-Za-z_+]', '', raw)
            for cat in valid_categories:
                if cat.lower() == cleaned.lower():
                    category = cat
                    break
            else:
                category = "Generic"
                logger.warning(f"自动分类返回未知值: '{raw}'，使用 Generic")
    except Exception as e:
        logger.warning(f"分类选择失败，使用 Generic 模板: {e}")
        category = "Generic"
    prompt_path = PROMPTS_DIR / f"{category}.md"
    return prompt_path

def safe_filename(title: str, max_len: int = 40) -> str:
    cleaned = re.sub('[<>:"/\\\\\\\\|?*]', '_', title)
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

def format_chapter_content(raw: str, chapter_title: str) -> str:
    lines = raw.split('\\n')
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    if lines and lines[0].startswith('# '):
        lines[0] = f"## {chapter_title}"
    elif lines and lines[0].startswith('## '):
        lines[0] = f"## {chapter_title}"
    else:
        lines.insert(0, f"## {chapter_title}")
        lines.insert(1, "")
    new_lines = []
    for line in lines:
        match = re.match(r'^(\\d+)\\.\\s*\\**(.+?)\\**\\s*$', line)
        if match:
            num = match.group(1)
            title = match.group(2).strip()
            title = re.sub(r'\\*+', '', title)
            line = f"### {num}. {title}"
        new_lines.append(line)
    return '\\n'.join(new_lines)

def generate_chapter_content(
    client, cfg, system_prompt, book_title, chapter_title
) -> Optional[str]:
    format_instruction = (
        "[格式重申] 请使用 ### 三级标题标注各个部分，例如：### 1. 核心理论断言,白话类比与大师避坑指南。开头不要加总标题，我们会自动处理。严禁AI自我介绍。\\n"
        "严格按照以下6部分输出：\\n"
        "1. **核心理论断言,白话类比与大师避坑指南**\\n"
        "2. **保姆级可编译运行代码**\\n"
        "3. **内核验证实验**\\n"
        "4. **理论-实践映射表**\\n"
        "5. **可深挖的知识点与学习链接**\\n"
        "6. **代码练习任务**"
    )
    messages = [
        {"role": "system", "content": system_prompt + "\\n\\n" + format_instruction},
        {"role": "user", "content": f"书籍标题: {book_title}\\n章节标题: {chapter_title}"}
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
        '```(?:c|cpp|cc|C)\\n(.*?)```',
        '```c(?:.*?)\\n(.*?)```'
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

def merge_chapters(output_dir: Path, book_title: str, ordered_titles: List[str]):
    existing_files = {}
    for f in output_dir.glob("*.md"):
        existing_files[f.stem] = f
    ordered_files = []
    missing = []
    for title in ordered_titles:
        safe = safe_filename(title)
        if safe in existing_files:
            ordered_files.append(existing_files[safe])
        else:
            missing.append(title)
    if missing:
        logger.warning(f"以下章节文件未找到，将跳过合并: {missing}")
    if not ordered_files:
        logger.warning("没有找到任何章节文件，无法生成合并文档。")
        return
    merged_file_name = safe_filename(book_title) + "_合集.md"
    merged_path = output_dir.parent / merged_file_name
    with open(merged_path, "w", encoding="utf-8") as outfile:
        outfile.write(f"# {book_title}\\n\\n")
        outfile.write(f"*生成日期: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}*\\n\\n")
        outfile.write(f"*总章节数: {len(ordered_files)}*\\n\\n")
        outfile.write("## 目录\\n\\n")
        for ch_file in ordered_files:
            content = ch_file.read_text(encoding="utf-8")
            first_line = content.split('\\n')[0]
            if first_line.startswith('## '):
                ch_title = first_line[3:]
            else:
                ch_title = ch_file.stem
            outfile.write(f"- [{ch_title}](#{ch_title.replace(' ', '-')})\\n")
        outfile.write("\\n---\\n\\n")
        for ch_file in ordered_files:
            content = ch_file.read_text(encoding="utf-8")
            outfile.write(content)
            outfile.write("\\n\\n---\\n\\n")
    logger.info(f"✅ 所有章节已按配置顺序合并为完整书籍文档: {merged_path}")

def main():
    start_time = time.time()
    logger.info("=" * 60)
    logger.info(" 书籍学习蓝图生成器 - 最终工程版 (统一格式)")

    cfg = load_config()
    if not cfg:
        sys.exit(1)

    logger.info(f" 模式: {cfg.mode}")
    logger.info("=" * 60)

    if cfg.mode in ["generate", "all"]:
        if cfg.category != "auto":
            prompt_path = PROMPTS_DIR / f"{cfg.category}.md"
            if not prompt_path.exists():
                logger.warning(f"指定的分类模板 {cfg.category}.md 不存在，回退为自动判断")
                prompt_path = select_category(cfg)
        else:
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

        for idx, chapter_title in enumerate(chapters):
            safe_name = safe_filename(chapter_title) or f"Chapter_{idx+1:02d}"
            chapter_dir = CODE_ROOT / safe_name
            chapter_dir.mkdir(exist_ok=True)
            md_path = OUTPUT_DIR / f"{safe_name}.md"

            if chapter_title in completed or md_path.exists():
                logger.info(f"⏩ 跳过已生成章节 [{idx+1}/{len(chapters)}]: {chapter_title}")
                total_success += 1
                continue

            logger.info(f"⏳ 生成中 [{idx+1}/{len(chapters)}]: {chapter_title}")
            try:
                raw_content = generate_chapter_content(client, cfg, system_prompt, cfg.book_title, chapter_title)
                if raw_content is None:
                    raise Exception("重试失败, 跳过本章")
                formatted = format_chapter_content(raw_content, chapter_title)
                with open(md_path, "w", encoding="utf-8") as f:
                    f.write(formatted)
                code_count = extract_code_blocks(formatted, chapter_dir)
                if code_count > 0:
                    logger.info(f"  ✅ 提取 {code_count} 个代码文件")
                else:
                    logger.warning(f"  ⚠️ 未找到代码块, 已记录占位")
                completed.add(chapter_title)
                save_progress(completed)
                total_success += 1
                time.sleep(cfg.rate_limit_delay)
            except Exception as e:
                logger.error(f"❌ 第 {idx+1} 章生成失败: {chapter_title}")
                logger.error(f"   错误: {e}")
                failed_chapters.append(f"{idx+1}. {chapter_title}: {e}")

        elapsed = time.time() - start_time
        logger.info("=" * 60)
        logger.info(f"✅ 生成执行完成！耗时: {elapsed:.1f} 秒")
        logger.info(f"✅ 成功: {total_success} 章 / 共 {len(chapters)} 章")
        if failed_chapters:
            logger.warning(f"⚠️ 失败: {len(failed_chapters)} 章")
            with open(LOG_DIR / "failed_chapters.log", "w", encoding="utf-8") as f:
                f.write("\\n".join(failed_chapters))
            logger.info(f"失败详情已保存至: {LOG_DIR / 'failed_chapters.log'}")
        else:
            logger.info("🎉 所有章节均生成成功！")

        logger.info(f"📄 输出目录: {OUTPUT_DIR}")
        logger.info(f"💻 代码目录: {CODE_ROOT}")
        logger.info(f"📋 日志文件: {LOG_FILE}")

    if cfg.mode in ["merge", "all"]:
        logger.info("📚 正在按配置顺序合并所有章节...")
        merge_chapters(OUTPUT_DIR, cfg.book_title, cfg.table_of_contents)
    else:
        logger.info("模式为 generate, 不执行合并。")

    logger.info("=" * 60)

if __name__ == "__main__":
    main()
"""


def create_project():
    base_dir = Path.cwd() / "book_ai_learning_engine"
    subdirs = ["prompts", "output", "code_labs", "logs"]

    print(f"正在创建项目目录: {base_dir} ...")
    base_dir.mkdir(exist_ok=True)
    for sub in subdirs:
        (base_dir / sub).mkdir(exist_ok=True)

    def safe_write(file_path: Path, content: str):
        if file_path.exists() and file_path.stat().st_size > 0:
            print(f"   ⏩ 跳过（已存在非空文件）: {file_path.name}")
            return
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"   ✅ 已生成: {file_path.relative_to(Path.cwd())}")

    safe_write(base_dir / "run.py", ENGINE_CODE.strip() + "\n")

    config_content = """# 书籍信息
book_title: "C++17 完整指南"
category: "C++"        # 强制使用 "C++" 提示词模板
api_key: "sk-你的DeepSeekAPI密钥"
model: "deepseek-chat"
output_dir: "./output"
max_concurrent_requests: 5
api_timeout: 120
mode: "all"          # generate / merge / all

# 章节列表（标准 YAML 列表）
table_of_contents:
  - "第 1 章 结构化绑定"
  - "第 2 章 带初始化的if和switch语句"
  - "第 3 章 内联变量"
  - "第 4 章 聚合体扩展"
  - "第 5 章 强制省略拷贝或传递未实质化的对象"
  - "第 6 章 lambda表达式扩展"
  - "第 7 章 新属性和属性特性"
  - "第 8 章 其他语言特性"
  - "第 9 章 类模板参数推导"
  - "第 10 章 编译期if语句"
  - "第 11 章 折叠表达式"
  - "第 12 章 处理字符串字面量模板参数"
  - "第 13 章 占位符类型作为模板参数"
  - "第 14 章 扩展的using声明"
  - "第 15 章 std::optional<>"
  - "第 16 章 std::variant<>"
  - "第 17 章 std::any"
  - "第 18 章 std::byte"
  - "第 19 章 字符串视图"
  - "第 20 章 文件系统库"
  - "第 21 章 类型特征扩展"
  - "第 22 章 并行STL算法"
  - "第 23 章 新的STL算法详解"
  - "第 24 章 子串和子序列搜索器"
  - "第 25 章 其他工具函数和算法"
  - "第 26 章 容器和字符串扩展"
  - "第 27 章 多线程和并发"
  - "第 28 章 标准库的其他微小特性和修改"
  - "第 29 章 多态内存资源(PMR)"
  - "第 30 章 使用new和delete管理超对齐数据"
  - "第 31 章 std::to_chars()和std::from_chars()"
  - "第 32 章 std::launder()"
  - "第 33 章 编写泛型代码的改进"
  - "第 34 章 总体性的C++17事项"
  - "第 35 章 废弃和移除的特性"
"""
    safe_write(base_dir / "config.yaml", config_content)

    prompts_dir = base_dir / "prompts"
    print("正在生成提示词文件...")
    for name, content in PROMPTS.items():
        safe_write(prompts_dir / name, content)

    print("\n" + "=" * 60)
    print("✅ 项目初始化完成！")
    print("=" * 60)
    print("接下来操作步骤：")
    print("1. 进入目录: cd book_ai_learning_engine")
    print("2. 编辑 config.yaml，填入你的 DeepSeek API Key")
    print("3. 根据需要修改 category 字段")
    print("4. 运行生成引擎: python run.py")
    print("=" * 60)


if __name__ == "__main__":
    create_project()