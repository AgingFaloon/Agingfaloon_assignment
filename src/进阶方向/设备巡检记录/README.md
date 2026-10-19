# 设备巡检记录



## 一、题目描述



机房巡检员每天记录设备类型、状态和运行小时数，要求用函数和复合数据完成设备记录的过滤、统计和查询



## 二、通关条件



* 全部行为用例精确匹配且退出码为 0



## 三、解题过程记录



* 用 `rm` 删除 `c/` 目录

```bash

rm -rf c

```



* 用 `nano` 打开 `main.cpp` ，根据题目要求并结合提示3中的聚合骨架修复程序

```bash

nano main.cpp

```

* 编译代码

```bash

g++ -std=c++11 main.cpp -o main

```

* 用 `echo` 创建输入文件

```bash

echo "6
printer ok 12
scanner repair 30
router ok 45
printer repair 8
server ok 100
router repair 15" > input.txt

```

* 以及预期输出文件

```bash

echo "printer 2 20
scanner 1 30
router 2 60
server 1 100
busiest server" > expected.txt

```

* 比较样例输出

```bash

./main < input.txt | diff - expected.txt

```



* 确认无误后提交文件

