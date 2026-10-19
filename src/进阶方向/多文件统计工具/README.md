# 多文件统计工具

## 一、题目描述

统计功能被拆成入口、统计和格式化三个部分，初始工程能读懂但不能正确构建和输出，需要在不改动 main 调用语义的前提下修好多文件工程

## 二、通关条件

按 CMake 入口重新构建后，全部行为用例通过

## 三、解题过程记录

* 先用 `ls` 查看容器材料

```bash
ls -la /workspace
```

* 看到 `CMakeLists.txt`、`main.cpp`、`stats.h`、`stats.cpp`、`format.h`、`format.cpp`、目录 `samples/`以及 `c/`
* 按工作区 README 的提示先跑一次构建，看失败停在哪一层

```bash
cmake -S . -B build \&\& cmake --build build
```

* 报错停在链接层：`undefined reference to print\_report(int, long long, int, int)`。说明有两处问题，构建入口没有列全实现文件，格式化函数的定义也不是 `main` 调用的那个签名
* 先用 `rm` 删除 `c/`

```bash
rm -rf c
```

* 用 `nano` 打开 `CMakeLists.txt`，按提示 3 的最小结构显式设置 C++17，并把漏掉的 `format.cpp` 补进可执行目标，目标名保持 `main`

```bash
nano CMakeLists.txt
```

* 用 `nano` 打开 `format.cpp`，对照 `format.h` 的声明与 `main` 的调用，把参数顺序由 `count, smallest, largest, total` 改回 `count, total, smallest, largest`
* `stats.cpp` 本身没有问题，`sum\_values` 已经用 `long long` 累加，`min\_value`、`max\_value` 返回 `int`，保持不动
* 删掉旧构建目录重新配置，确认生成 `build/main`

```bash
rm -rf build
cmake -S . -B build
cmake --build build
ls -l build/main
```

* 用三个公开样例逐个比对输出

```bash
for f in samples/\*.in; do
  ./build/main < "$f" | diff - "${f%.in}.out" \&\& echo "PASS $f"
done
```

* 单独确认 64 位求和这一项，32 位实现会输出负数

```bash
./build/main < samples/02-overflow.in
# count=5
# sum=5000000000
# min=1000000000
# max=1000000000
```

* 提交前删除本地构建产物

```bash
rm -rf build
```

* 确认无误后提交文件

