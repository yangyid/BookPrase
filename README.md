windows 运行
# 首次运行
```
执行指令： python.exe .\book_auto.py
正在创建项目目录: D:\Test_AI\book_prease\book_ai_learning_engine ...
   ⏩ 跳过（已存在非空文件）: run.py
   ✅ 已生成: book_ai_learning_engine\config.yaml
正在生成提示词文件...
   ⏩ 跳过（已存在非空文件）: C++.md
   ⏩ 跳过（已存在非空文件）: Algorithms.md
   ⏩ 跳过（已存在非空文件）: ROS.md
   ⏩ 跳过（已存在非空文件）: OS.md
   ⏩ 跳过（已存在非空文件）: Networking.md
   ⏩ 跳过（已存在非空文件）: Design_Patterns.md
   ⏩ 跳过（已存在非空文件）: Linux.md
   ⏩ 跳过（已存在非空文件）: AI.md
   ⏩ 跳过（已存在非空文件）: Generic.md

============================================================
✅ 项目初始化完成！
============================================================
接下来操作步骤：
1. 进入目录: cd book_ai_learning_engine
2. 编辑 config.yaml，填入你的 DeepSeek API Key
3. 根据需要修改 category 字段
4. 运行生成引擎: python run.py
============================================================
```

# 配置 
1. 配置Dsp 的key
2. 使用dsp的识图模式，识别出章节目录
章节的目录严格按照如下格式
```
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
```
3. 配置解析功能
```
mode: "all"          # generate（仅生成） / merge（仅合并） / all （生成加合并）
```
4. 将需要解析的文件放在book_ai_learning_engine 目录下

# 最后合并后的文档可见
https://blog.csdn.net/MMTS_yang/article/details/162654473?sharetype=blogdetail&sharerId=162654473&sharerefer=PC&sharesource=MMTS_yang&spm=1011.2480.3001.8118
