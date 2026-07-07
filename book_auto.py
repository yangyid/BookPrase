import os
from pathlib import Path

# ============================================================
# 优化后的提示词：强制要求完整代码练习
# ============================================================
PROMPTS = {
    "C++.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍、客套话。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可编译运行代码(要求使用 g++ -std=c++17 -O2 编译,严禁使用 Python)
### 3. 内核/性能验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "Algorithms.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可运行代码(必须使用 C++ 或 C,严禁使用 Python)
### 3. 性能基准实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与进阶应用
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C/C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "ROS.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可运行代码(必须使用 C++,严禁使用 Python)
### 3. 系统/网络验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "OS.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可编译运行代码(必须使用 C 或 C++,严禁使用 Python)
### 3. 内核验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C/C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "Networking.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可编译运行代码(必须使用 C 或 C++,严禁使用 Python)
### 3. 内核验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C/C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "Design_Patterns.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可编译运行代码(必须使用 C++,严禁使用 Java/Python)
### 3. 架构静态分析实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "Linux.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可编译运行代码(必须使用 C 或 C++,严禁使用 Python)
### 3. 内核/系统验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接编译运行的 C/C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "AI.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言,白话类比与大师避坑指南
### 2. 保姆级可运行代码
### 3. GPU/性能验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接运行的 Python/C++ 代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）""",

    "Generic.md": """[指令] 允许开头包含一段简洁的[章节知识点概述]。请用通俗易懂的语言写作，多使用生活化比喻，让零基础读者也能轻松理解。绝对禁止任何AI自我介绍。直接输出要点概述后,严格按以下6部分输出（每部分必须使用 ### 数字. 标题 的格式，严禁使用 # 或 ##）:
### 1. 核心理论断言
### 2. 仿真代码示例
### 3. 内核/系统验证实验
### 4. 理论-实践映射表
### 5. 可深挖的知识点与学习链接
### 6. 代码练习任务（必须提供完整的、可直接运行的代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）"""
}

# ============================================================
# 引擎代码：强化格式后处理 + 自然排序合并 + 代码练习检查
# ============================================================
ENGINE_CODE = r"""
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
        self.api_key = data.get("api_key", "")
        self.model = data.get("model", "deepseek-v4-pro")
        self.output_dir = data.get("output_dir", "./output")
        self.book_title = data.get("book_title", "")
        self.category = data.get("category", "auto")
        self.max_concurrent = data.get("max_concurrent_requests", 5)
        self.timeout = data.get("api_timeout", 120)
        self.max_retries = 3
        self.rate_limit_delay = 1.0
        self.table_of_contents = data.get("table_of_contents", [])
        self.mode = data.get("mode", "all")

    def validate(self):
        if self.mode not in ["generate", "merge", "all"]:
            return False, f"mode 配置错误: '{self.mode}'"
        if self.mode in ["generate", "all"]:
            if not self.api_key or self.api_key == "sk-你的DeepSeekAPI密钥":
                return False, "API Key 未填写。"
            if not self.api_key.startswith("sk-"):
                return False, "API Key 格式错误"
        if not self.book_title:
            return False, "book_title 为空"
        allowed = ["C++", "Algorithms", "ROS", "OS", "Networking",
                   "Design_Patterns", "Linux", "AI", "Generic", "auto"]
        if self.category not in allowed:
            return False, f"category 无效: {self.category}"
        if not isinstance(self.table_of_contents, list) or not self.table_of_contents:
            return False, "table_of_contents 配置错误"
        return True, ""

def load_config():
    if not CONFIG_PATH.exists():
        logger.error(f"配置文件不存在: {CONFIG_PATH}")
        return None
    try:
        with open(CONFIG_PATH, "r", encoding="utf-8-sig") as f:
            raw = yaml.safe_load(f)
        cfg = Config(raw)
        ok, msg = cfg.validate()
        if not ok:
            logger.error(msg)
            return None
        return cfg
    except Exception as e:
        logger.error(f"配置读取失败: {e}")
        return None

def select_category(cfg):
    valid = ["C++", "Algorithms", "ROS", "OS", "Networking",
             "Design_Patterns", "Linux", "AI", "Generic"]
    try:
        client = OpenAI(api_key=cfg.api_key, base_url="https://api.deepseek.com")
        prompt = f"只回答一个分类名（C++, Algorithms, ROS, OS, Networking, Design_Patterns, Linux, AI, Generic）：{cfg.book_title}"
        resp = client.chat.completions.create(
            model=cfg.model,
            messages=[{"role": "user", "content": prompt}],
            temperature=0.0,
            max_tokens=10,
            timeout=30
        )
        raw = resp.choices[0].message.content.strip()
        logger.info(f"分类API返回: '{raw}'")
        for cat in valid:
            if cat.lower() in raw.lower():
                return PROMPTS_DIR / f"{cat}.md"
        first_word = raw.split()[0].rstrip(',.')
        if first_word in valid:
            return PROMPTS_DIR / f"{first_word}.md"
        logger.warning("无法从返回值解析分类，使用 Generic")
        return PROMPTS_DIR / "Generic.md"
    except Exception as e:
        logger.warning(f"自动分类失败 ({e})，使用 Generic")
        return PROMPTS_DIR / "Generic.md"

def safe_filename(title, max_len=100):
    cleaned = re.sub(r'[<>:"/\\|?*]', '_', title).strip('_')
    return cleaned[:max_len]

def call_with_retry(client, model, messages, max_retries=3, timeout=120):
    for attempt in range(1, max_retries+1):
        try:
            resp = client.chat.completions.create(
                model=model, messages=messages,
                temperature=0.7, max_tokens=4096, timeout=timeout
            )
            return resp.choices[0].message.content
        except RateLimitError:
            wait = 2 ** attempt
            logger.warning(f"限流，等待{wait}s重试")
            time.sleep(wait)
        except APITimeoutError:
            wait = 2 ** attempt
            logger.warning(f"超时，等待{wait}s重试")
            time.sleep(wait)
        except APIError as e:
            if e.status_code in [500,502,503,504]:
                wait = 2 ** attempt
                logger.warning(f"服务端错误{e.status_code}，重试")
                time.sleep(wait)
            else:
                raise
        except Exception:
            raise
    return None

def load_progress():
    if PROGRESS_FILE.exists():
        try:
            with open(PROGRESS_FILE, "r", encoding="utf-8") as f:
                data = json.load(f)
            return set(data.get("completed", []))
        except:
            pass
    return set()

def save_progress(completed):
    with open(PROGRESS_FILE, "w", encoding="utf-8") as f:
        json.dump({"completed": list(completed)}, f, ensure_ascii=False, indent=2)

# ========== 统一格式化 + 代码练习完整性检查 ==========
def format_chapter_content(raw_text, chapter_title):
    lines = raw_text.split('\n')
    cleaned = []
    for line in lines:
        if line.startswith('# ') or line.startswith('## '):
            continue
        cleaned.append(line)
    text = '\n'.join(cleaned).strip()

    def replace_num_heading(m):
        num = m.group(1)
        title = m.group(2).strip()
        title = re.sub(r'\*+', '', title)
        return f"### {num}. {title}"

    text = re.sub(
        r'^(\d+)[\.\)、]\s*\**(.+?)\**\s*$',
        replace_num_heading,
        text,
        flags=re.MULTILINE
    )

    h3_count = len(re.findall(r'^### \d+\.', text, flags=re.MULTILINE))
    if h3_count < 6:
        logger.warning(f"章节 '{chapter_title}' 只包含 {h3_count} 个三级标题，请检查内容完整性。")

    # 检查第六部分是否包含代码块
    # 定位 ### 6. 之后的文本
    match = re.search(r'^### 6\. .*?(?=^### \d+\.|\Z)', text, flags=re.MULTILINE | re.DOTALL)
    if match:
        section = match.group(0)
        if not re.search(r'```', section):
            logger.warning(f"章节 '{chapter_title}' 的代码练习任务中未发现代码块，可能缺少完整代码。")
    else:
        logger.warning(f"章节 '{chapter_title}' 没有找到第六部分标题。")

    final = f"## {chapter_title}\n\n{text}"
    return final

def generate_chapter_content(client, cfg, system_prompt, book_title, chapter_title):
    format_instruction = (
        "【格式要求】不要添加任何一级或二级标题（不要用 # 或 ##）。"
        "直接写概述文字，然后按顺序输出以下6部分，每部分必须使用 '### 数字. 标题' 的格式（三级标题）。\n"
        "### 1. 核心理论断言,白话类比与大师避坑指南\n"
        "### 2. 保姆级可编译运行代码\n"
        "### 3. 内核验证实验\n"
        "### 4. 理论-实践映射表\n"
        "### 5. 可深挖的知识点与学习链接\n"
        "### 6. 代码练习任务（必须提供完整的、可直接编译运行的代码解决方案，包含必要的注释和解释，不要只给思路或伪代码）"
    )
    messages = [
        {"role": "system", "content": system_prompt + "\n\n" + format_instruction},
        {"role": "user", "content": f"书籍标题: {book_title}\n章节标题: {chapter_title}"}
    ]
    return call_with_retry(client, cfg.model, messages, max_retries=cfg.max_retries, timeout=cfg.timeout)

def extract_code_blocks(content, chapter_dir):
    patterns = [
        r'```(?:c|cpp|cc|C)\n(.*?)```',
        r'```c(?:.*?)\n(.*?)```'
    ]
    for pat in patterns:
        blocks = re.findall(pat, content, re.DOTALL)
        if blocks:
            for i, code in enumerate(blocks):
                fname = chapter_dir / f"sim_lab_{i+1}.c"
                with open(fname, "w", encoding="utf-8") as f:
                    f.write(code.strip())
            return len(blocks)
    with open(chapter_dir / "NO_CODE_BLOCK.txt", "w", encoding="utf-8") as f:
        f.write("AI 未输出有效的 C 代码块。")
    return 0

# ========== 合并函数（自然排序） ==========
def merge_chapters(output_dir, book_title):
    all_md = [f for f in output_dir.glob("*.md") if "_合集.md" not in f.name]
    if not all_md:
        logger.warning("没有找到任何章节文件，无法生成合并文档。")
        return

    def sort_key(filepath):
        m = re.search(r'(\d+)', filepath.name)
        return int(m.group(1)) if m else float('inf')
    all_md.sort(key=sort_key)

    logger.info(f"合并文件顺序: {[f.name for f in all_md]}")

    merged_name = safe_filename(book_title) + "_合集.md"
    merged_path = output_dir.parent / merged_name

    with open(merged_path, "w", encoding="utf-8") as out:
        out.write(f"# {book_title}\n\n")
        out.write(f"*生成日期: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}*\n\n")
        out.write(f"*总章节数: {len(all_md)}*\n\n")
        out.write("## 目录\n\n")
        for f in all_md:
            content = f.read_text(encoding="utf-8")
            first_line = content.split('\n')[0]
            if first_line.startswith('## '):
                title = first_line[3:]
            else:
                title = f.stem
            out.write(f"- [{title}](#{title.replace(' ', '-')})\n")
        out.write("\n---\n\n")
        for f in all_md:
            out.write(f.read_text(encoding="utf-8"))
            out.write("\n\n---\n\n")
    logger.info(f"合并完成（按数字自然排序）: {merged_path}")

def main():
    start = time.time()
    logger.info("="*60)
    logger.info("书籍学习蓝图生成器 - 完整代码练习版")

    cfg = load_config()
    if not cfg:
        sys.exit(1)

    logger.info(f"模式: {cfg.mode} | 模型: {cfg.model}")

    if cfg.mode in ["generate", "all"]:
        if cfg.category != "auto":
            prompt_path = PROMPTS_DIR / f"{cfg.category}.md"
            if not prompt_path.exists():
                logger.warning(f"指定的分类模板 {cfg.category}.md 不存在，使用自动分类")
                prompt_path = select_category(cfg)
        else:
            prompt_path = select_category(cfg)

        system_prompt = prompt_path.read_text(encoding="utf-8").strip()
        logger.info(f"模板: {prompt_path.name}")

        client = OpenAI(api_key=cfg.api_key, base_url="https://api.deepseek.com")
        completed = load_progress()
        chapters = cfg.table_of_contents
        logger.info(f"进度: {len(completed)}/{len(chapters)}")

        success = 0
        failed = []

        for i, chap_title in enumerate(chapters, 1):
            safe = safe_filename(chap_title) or f"Chapter_{i:02d}"
            cdir = CODE_ROOT / safe
            cdir.mkdir(exist_ok=True)
            md_path = OUTPUT_DIR / f"{safe}.md"

            if chap_title in completed or md_path.exists():
                logger.info(f"⏩ 跳过 [{i}/{len(chapters)}]: {chap_title}")
                success += 1
                continue

            logger.info(f"⏳ 生成 [{i}/{len(chapters)}]: {chap_title}")
            try:
                raw = generate_chapter_content(client, cfg, system_prompt, cfg.book_title, chap_title)
                if raw is None:
                    raise Exception("无内容")
                formatted = format_chapter_content(raw, chap_title)
                md_path.write_text(formatted, encoding="utf-8")
                code_count = extract_code_blocks(formatted, cdir)
                if code_count:
                    logger.info(f"  ✅ {code_count}个代码文件")
                else:
                    logger.warning("  ⚠️ 未提取到代码")
                completed.add(chap_title)
                save_progress(completed)
                success += 1
                time.sleep(cfg.rate_limit_delay)
            except Exception as e:
                logger.error(f"❌ 失败: {chap_title} - {e}")
                failed.append(f"{i}. {chap_title}: {e}")

        elapsed = time.time() - start
        logger.info("="*60)
        logger.info(f"完成！耗时{elapsed:.1f}s，成功{success}/{len(chapters)}")
        if failed:
            with open(LOG_DIR / "failed_chapters.log", "w", encoding="utf-8") as f:
                f.write("\n".join(failed))

    if cfg.mode in ["merge", "all"]:
        logger.info("📚 合并中（自然排序）...")
        merge_chapters(OUTPUT_DIR, cfg.book_title)

    logger.info("="*60)

if __name__ == "__main__":
    main()
"""


def create_project():
    base_dir = Path.cwd() / "book_ai_learning_engine"
    subdirs = ["prompts", "output", "code_labs", "logs"]

    print(f"创建项目: {base_dir}")
    base_dir.mkdir(exist_ok=True)
    for sub in subdirs:
        (base_dir / sub).mkdir(exist_ok=True)

    def safe_write(path, content):
        if path.exists() and path.stat().st_size > 0:
            print(f"   ⏩ 跳过: {path.name}")
            return
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"   ✅ {path.relative_to(Path.cwd())}")

    safe_write(base_dir / "run.py", ENGINE_CODE.strip() + "\n")

    config = """# 书籍信息
book_title: "C++17 完整指南"
category: "C++"      # 建议填写具体分类以避免自动分类失败
api_key: "sk-你的DeepSeekAPI密钥"
model: "deepseek-v4-pro"
output_dir: "./output"
max_concurrent_requests: 5
api_timeout: 120
mode: "all"          # generate / merge / all

# 章节列表（生成顺序）
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
    safe_write(base_dir / "config.yaml", config)

    prompts_dir = base_dir / "prompts"
    print("生成提示词...")
    for name, content in PROMPTS.items():
        safe_write(prompts_dir / name, content)

    print("\n" + "="*60)
    print("✅ 一键脚本初始化完成！")
    print("1. cd book_ai_learning_engine")
    print("2. 编辑 config.yaml，填入 API Key，建议设置 category")
    print("3. python run.py")
    print("="*60)


if __name__ == "__main__":
    create_project()