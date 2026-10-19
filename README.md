# NCUSCC2026考核报告

本文件汇总内容：基础入门 2 关、进阶方向 6 题、1 份报告，以及各题在本地重新编译、运行、对拍后的实测结论。

## 一、总览

| 分类 | 数量 | 内容 |
| --- | --- | --- |
| 基础入门 | 2 关 | 找到入口、两段信号 |
| 进阶方向 | 6 题 | 读数统计、设备巡检记录、固定容量读数窗口、借阅台账、多文件统计工具、四线程快速排序观察 |
| 报告 | 1 份 | 技术&感悟报告.pdf |

## 二、目录结构

```text
考核/
├── README.md
├── 技术&感悟报告.pdf
└── src/
    ├── 基础入门/
    │   ├── 第1关：找到入口/README.md
    │   └── 第2关：两段信号/README.md
    └── 进阶方向/
        ├── 读数统计/
        │   ├── README.md
        │   └── main.cpp
        ├── 设备巡检记录/
        │   ├── README.md
        │   └── main.cpp
        ├── 固定容量读数窗口/
        │   ├── README.md
        │   └── main.cpp
        ├── 借阅台账/
        │   ├── README.md
        │   └── main.cpp
        ├── 多文件统计工具/
        │   ├── README.md
        │   ├── CMakeLists.txt
        │   └── format.cpp
        └── 四线程快速排序观察/
            └── report.md
```

## 三、基础入门

### 3.1 第1关：找到入口

- **通关条件**：在 `/workspace/` 生成普通文件 `flag`，大小 ≤ 1 MiB，字节内容等于 `input.bin` 经密钥与 `exam` 处理后的结果。
- **关键路径**：`ls -la /workspace/` 摸清材料 → `cp input.bin input` 准备输入 → `cabinet/key.txt` 取到密钥 `MOON` → `exam --help` 确认参数格式 → `exam -e MOON` 生成 `output` → `cp output flag` → `file` / `ls -lh` 确认是普通文件且未超限。

### 3.2 第2关：两段信号

- **通关条件**：`flag` 的字节内容严格等于「A 段处理结果 + B 段处理结果」的顺序拼接，不缺段、不重复、无额外字节。
- **关键路径**：`find . -name "*signal-A*"`、`find . -name "*signal-B*"` 定位两段二进制 → `grep -r "ACCESS_KEY=" .` 取到密钥 `STAR` → 分别把两段复制成 `input` 后各跑一次 `exam -e STAR`，产物另存为 `A_result`、`B_result` → `cat A_result B_result > flag` → `wc -c` 核对三段字节数满足 `size(A)+size(B)==size(flag)`。

## 四、进阶方向

### 4.0 一览

| 题目 | 提交文件 | 实现要点 |
| --- | --- | --- | --- |
| 读数统计 | `main.cpp` | 一次扫描求 count/sum/min/max |
| 设备巡检记录 | `main.cpp` | 结构体聚合 + 类型索引 + 最忙设备 |
| 固定容量读数窗口 | `main.cpp` | 环形窗口 `read % 8`、回绕 oldest |
| 借阅台账 | `main.cpp` | 双 `map` + `long long`，失败即 `rejected` |
| 多文件统计工具 | `CMakeLists.txt`、`format.cpp` | CMake 入口 + 格式化签名修正 |
| 四线程快速排序观察 | `report.md` | 五数取中 + 三路划分 + 名额 CAS |

### 4.1 读数统计

- **题目**：读入一批温度读数，输出统计概要。
- **实现**：读入 `n` 后单趟循环累加 `sum` 并维护 `min_val`/`max_val`，最后输出 `count`、`sum`、`min`、`max` 四行。
- **实测**：

  | 输入 | 输出 |
  | --- | --- |
  | `5` / `1 2 3 4 5` | `count=5 sum=15 min=1 max=5` |
  | `3` / `-5 0 7` | `count=3 sum=2 min=-5 max=7` |
  | `0` | `count=0 sum=0 min=1001 max=-1001` |

- **说明**：本题未对 `n=0` 的输出结果做要求，输出哨兵值 `1001`/`-1001` 也可接受。

### 4.2 设备巡检记录

- **题目**：用函数和复合数据完成设备记录的过滤、统计与查询。
- **实现**：`struct Totals { int count; int hours; }` 按 4 类设备（printer/scanner/router/server）聚合；`typeIndex()` 把设备名映射为下标，非法类型直接跳过；`busiestIndex()` 取 `hours` 最大者，并列时取靠前者；先输出 4 行「类型 次数 小时数」，再输出 `busiest <类型>`。
- **实测**：用题面样例输入（6 条记录）运行，输出与预期逐行一致：

  ```text
  printer 2 20
  scanner 1 30
  router 2 60
  server 1 100
  busiest server
  ```

- **说明**：本题要求累计`repairHours` 但不要求打印，选择以 `(void)repairHours;` 抑制未使用告警。

### 4.3 固定容量读数窗口

- **题目**：监控面板只保留最近 8 个读数。工作区里的程序试图实现这个功能，但容量用尽后会越界写入。
- **定位到的三处缺陷**：写入下标直接用递增循环变量、`kept` 无上限、最早保留值恒取 `window[0]`。
- **修复**：容量记为 `CAPACITY = 8`，第 `read` 个值写入 `read % CAPACITY`，下标恒落在 0..7；`kept` 达到 8 后不再增长；覆盖某格前先把该格旧值从 `sum` 中减去，保证 `sum` 只统计实际保留的 ≤8 个值；最早值改用 `oldestSlot = (read >= CAPACITY) ? (read % CAPACITY) : 0`。
- **实测**：

  | 检查段 | 规模 | 结果 |
  | --- | --- | --- |
  | 题面三组样例 | 3 项 | 全部通过 |
  | 边界与极值（n=1/2/7/8/9/13/16、n=100000 全 ±1000/全 0/正负交替） | 15 项 | 全部通过 |
  | 随机对拍 | 300 组 | 全部通过 |
  | AddressSanitizer 复跑 | 13 项 | 无越界、无泄漏 |

### 4.4 借阅台账

- **题目**：按编号维护在架册数与借出册数，处理 `add` / `borrow` / `return`。
- **实现**：`std::map<std::string, long long>` 两张表分别存 `onShelf`、`borrowed`；`return` 只有两个字段，只有 `add`/`borrow` 读第三个字段 `copies`；任一操作不合法立即输出单行 `rejected` 并 `return 0`（退出码必须为 0，不是 1）；全部成功后先输出 `titles`/`onShelf`/`borrowed` 三行，再按编号字节序输出明细，两边都为 0 的编号跳过；册数统一 `long long`，避免累加溢出。
- **实测**（`check/check_jieyue.py`，独立参考实现逐步模拟）：9 组固定用例（含 borrow 超量、return 无借出、borrow 恰好清空、单册借还、字典序、未登记编号、1e9 总量、n=0）+ 500 组随机对拍，以及“`rejected` 只有一行且退出码 0”“空台账三行无明细”两项抽查，**全部通过**。

### 4.5 多文件统计工具

- **题目**：统计功能拆成入口、统计、格式化三部分，在不改动 `main` 调用语义的前提下修好多文件工程。
- **两处修改**：
  1. `CMakeLists.txt` 显式设 `CMAKE_CXX_STANDARD 17`，并把漏掉的 `format.cpp` 补进 `add_executable(main ...)`，目标名保持 `main`；
  2. `format.cpp` 的 `print_report` 参数顺序由 `count, smallest, largest, total` 改回 `main` 调用的 `count, total, smallest, largest`。
- **说明**：文件夹内仅放 `README.md` 与作出修改的 `CMakeLists.txt` 与 `format.cpp` 文件，因而未做本地复测

### 4.6 四线程快速排序观察

- **题目**：用不超过 4 个工作线程实现快速排序，并提交 `report.md `
- **实现要点**：
  - C++17 标准库 `std::thread` / `std::atomic` / `std::mutex`，未用 OpenMP、TBB、MPI、CUDA；
  - 五数取中基准（区间内 5 个等距采样点排序后取中位），配三路划分，重复元素一次划分全部就位；
  - 区间长度 ≤ 3 转插入排序；递归深度 ≥ 32 作为安全阀；
  - **线程上限**：名额计数器“只增不减” + CAS 抢名额，`kMaxSpawned = 3`，保证“每次排序期间创建的子线程总数 ≤ 3”，进而保证“同一时刻线程总数 ≤ 4 个”（main + 最多 3 个子线程）；名额不回收，所以这既是并发上限也是累计上限；抢不到名额直接串行递归，`std::thread` 构造抛异常时归还名额；
  - **口径边界**：`main()` 里的自测循环（8 个用例 + 1000 次复核 A + 100 次复核 B）在整个进程里累计创建约 2,300 个线程，属自测框架，不是题面的单次运行口径。
  - **汇合**：父线程创建子线程后先处理较小子区间，函数返回前 `join`；子线程再 `join` 自己的子线程，形成 join 树；全程无 `detach()`。
- **正确性**：每个用例排序后做四项检查（`std::is_sorted` 且相邻逆序对为 0、与 `std::sort` 逐元素一致、与 `std::qsort` 逐元素一致、存活线程回到 1 且子线程数 ≤ 3 / 峰值 ≤ 4），程序外再用 Python `sorted()` 独立核对。
- **16 个数的结果**：

  ```text
  输入: 42 7 19 42 3 88 15 7 61 0 54 23 91 15 36 100
  输出: 0 3 7 7 15 15 19 23 36 42 42 54 61 88 91 100
  ```

- **本地复测**：四次运行均以 `=== 全部用例通过 ===` 结束；16 个数重复 1000 次、1000 个随机数重复 100 次全部正确，线程峰值均不超过 4；四次记录只有线程 ID（每次运行不同）和“复核 B”里的峰值分布（`2:0 3:100` 与 `2:1 3:99`）有差异，属调度抖动，不影响结论。
- **环境限制**：`--cpu0` 把进程绑定到单个逻辑 CPU，所有线程分时复用，耗时仅作观察记录，报告不据此给出加速比。

## 五、本地复测汇总

本机可用工具：MSVC 14.51（VS 18 Community）、MinGW-w64 g++ 16.2.0（`C:\Users\lenovo\tools\mingw64\bin`）、Python 3（`D:\python\python.exe`）。五份源码在两套编译器下以 `-Wall -Wextra` 复编均为 0 条告警。

| # | 对象 | 检查方式 | 结果 |
| --- | --- | --- | --- |
| 1 | 固定容量读数窗口 | `python check/check_guding.py --src … --root … --random 300` | 331 项全部通过（含 ASan） |
| 2 | 借阅台账 | `python check/check_jieyue.py --exe build/ledger.exe --random 500` | 9 组固定 + 500 组随机全通过 |
| 3 | 设备巡检记录 | 题面样例输入、逐行 diff | 完全一致 |
| 4 | 读数统计 | 手工构造 3 组输入 | 输出符合预期；`n=0` 输出哨兵值 |
| 5 | 四线程快速排序观察 | `qsort4.exe --cpu0 data.txt` | 全部用例通过，线程峰值 ≤ 4 |

## 六、构建与复现

```batch
:: 需要先进入 VS x64 命令行，设置 INCLUDE / LIB
cl /nologo /std:c++17 /EHsc /O2 /utf-8 /Fe:build\ledger.exe   "src\进阶方向\借阅台账\main.cpp"
cl /nologo /std:c++17 /EHsc /O2 /utf-8 /Fe:build\window.exe   "src\进阶方向\固定容量读数窗口\main.cpp"
cl /nologo /std:c++17 /EHsc /O2 /utf-8 /Fe:build\patrol.exe   "src\进阶方向\设备巡检记录\main.cpp"
cl /nologo /std:c++17 /EHsc /O2 /utf-8 /Fe:build\stats.exe    "src\进阶方向\读数统计\main.cpp"
```

平台侧等价命令（单文件题）：

```bash
g++ -std=c++17 -Wall -Wextra -o main main.cpp
for f in samples/*.in; do ./main < "$f" | diff - "${f%.in}.out" && echo "PASS $f"; done
```

多文件题按 CMake 入口：

```bash
cmake -S . -B build && cmake --build build
./build/main < samples/01-mixed.in | diff - samples/01-mixed.out
```

四线程题：

```bash
g++ -std=c++17 -O2 -pthread qsort4/main.cpp -o qsort4 && ./qsort4 data.txt
```

## 七、说明

- 各题目录下的 `README.md` 是逐题的解题过程记录，本文件只做汇总与结论，不重复其细节。
- 基础入门两关的通关文件 `flag` 通过平台提交，仓库中只保留解题记录。