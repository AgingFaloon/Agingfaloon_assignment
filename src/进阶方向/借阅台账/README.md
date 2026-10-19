# 借阅台账

## 一、题目描述

借阅台按编号记录图书的在架册数和借出册数，需要处理 `add`、`borrow`、`return` 三种操作。

## 二、通关条件

* 提交 `main.cpp`；全部行为用例精确匹配且退出码为 0

## 三、解题过程记录

* 先用 `ls` 查看容器材料

```bash
ls -la /workspace
ls -la /workspace/samples
```

* 容器中有 `main.cpp`、`hints.md` 以及 `samples/` 里的三组样例
* 用 `nano` 打开 `main.cpp`，把处理循环里的 `TODO` 换成三种操作的分支

```bash
nano main.cpp
```

* 编译程序后用三组样例逐个比对输出

```bash
g++ -std=c++17 -Wall -Wextra -o main main.cpp
for f in samples/*.in; do
  ./main < "$f" | diff - "${f%.in}.out" && echo "PASS $f"
done
```

* 两组拒绝样例单独确认：只输出一行 `rejected`，且退出码必须是 0，不是 1

```bash
./main < samples/02-borrow-short.in; echo "exit=$?"
./main < samples/03-return-short.in; echo "exit=$?"
```

* 编号由大小写字母和数字组成，`map` 按字节比较，顺序是数字 < 大写 < 小写

```bash
printf '3\nadd b2 2\nadd A1 1\nadd 9z 3\n' | ./main
# titles=3
# onShelf=6
# borrowed=0
# 9z 3 0
# A1 1 0
# b2 2 0
```

* 补 `n=0` 的边界

```bash
printf '0\n' | ./main
# titles=0
# onShelf=0
# borrowed=0
```

* 补「借出全部归还后编号仍然保留」

```bash
printf '5\nadd A100 1\nborrow A100 1\nreturn A100\nborrow A100 1\nreturn A100\n' | ./main
# titles=1
# onShelf=1
# borrowed=0
# A100 1 0
```

* 再确认「在架为 0、借出大于 0」的编号也要出现在明细里，并计入 `titles`

```bash
printf '3\nadd B200 3\nborrow B200 3\nadd A100 1\n' | ./main
# titles=2
# onShelf=1
# borrowed=3
# A100 1 0
# B200 0 3
```

* 确认 64 位汇总：`n=1000` 且每次 1000000 时总量到 10 位，32 位实现会输出负数

```bash
awk 'BEGIN{print 1000; for(i=0;i<1000;i++) printf "add BIG 1000000\n"}' | ./main
# titles=1
# onShelf=1000000000
# borrowed=0
# BIG 1000000000 0
```

* 提交前删除本地构建和测试产物

```bash
rm -rf main
```

* 确认无误后提交文件

