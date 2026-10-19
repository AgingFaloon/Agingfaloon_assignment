# 固定容量读数窗口

## 一、题目描述

监控面板只保留最近 8 个读数。工作区里的程序试图实现这个功能，但容量用尽后会越界写入。

## 二、通关条件

* 全部行为用例精确匹配且退出码为 0

## 三、解题过程记录

* 先用 `ls` 查看容器材料

```bash
ls -la /workspace
ls -la /workspace/samples
```

* 容器中有 `main.cpp`、 `c`以及 `samples` 中的三组样例
* 编译程序后按工作区说明先跑超过容量的那组样例

```bash
g++ -std=c++17 -Wall -Wextra -o main main.cpp
./main < samples/02-over-capacity.in
```

* 程序被栈保护终止，并没有输出结果，与题面"容量用尽后会越界写入"一致
* 再跑容量未满的那组做对照

```bash
./main < samples/01-under-capacity.in
```

* 容量未满时结果正确，只有超过 8 个读数才失败，说明问题集中在容量边界，而不是输出格式
* 用 `rm` 删除 `c/`

```bash
rm -rf c
```

* 打开 `main.cpp`，对照已知缺陷定位三处问题：写入下标直接用了递增的循环变量、保留计数没有上限、最早保留值永远取 `window\[0]`

```bash
nano main.cpp
```

* 按提示 3 的修复方向改成回绕写入：容量记为 `CAPACITY = 8`，第 `i` 个值写入 `read % CAPACITY`，下标始终落在 0..7，不会越界
* `kept` 增加到 8 后不再增加；覆盖某一格之前先把该格里的旧值从窗口和中减去，保证 `sum` 只统计实际保留的 8 个（或更少）值
* 最早保留值不能永远取 `window\[0]`：绕回之后最早的值位于下一个即将被覆盖的位置，所以用 `oldestSlot = (read >= CAPACITY) ? (read % CAPACITY) : 0` 取下标
* 重新编译，确认没有警告

```bash
g++ -std=c++17 -Wall -Wextra -o main main.cpp
```

* 用三组样例逐个比对输出

```bash
for f in samples/\*.in; do
  ./main < "$f" | diff - "${f%.in}.out" \&\& echo "PASS $f"
done
```

* 单独确认 `n=9` 这个临界点，最早的值必须真的被挤出窗口

```bash
echo "9
1 2 3 4 5 6 7 8 9" > edge.in
./main < edge.in
```

* 补齐隐藏输入的上限 `n=100000`，确认大数据下也不越界

```bash
awk 'BEGIN{print 100000; for(i=0;i<100000;i++) printf "1000 "; print ""}' > big.in
./main < big.in
```

* 再次确认容量缺陷确实消除

```bash
g++ -std=c++17 -g -fsanitize=address -o main\_asan main.cpp
./main\_asan < samples/02-over-capacity.in
./main\_asan < big.in
```

* 提交前删除本地构建和测试产物

```bash
rm -rf main main\_asan edge.in big.in
```

* 确认无误后提交文件

