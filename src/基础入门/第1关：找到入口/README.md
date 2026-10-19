# 第1关：找到入口

## 一、通关条件

在 /workspace/ 目录下生成名为 flag 的普通文件，大小不超过 1 MiB，且内容必须与输入文件经密钥和 exam 处理后的结果逐字节相同。

## 二、 解题过程记录

* 首先使用 `ls` 命令查看工作区中的所有材料：

```bash
  ls -la /workspace/
  ```

* 发现有一个 cabinet 文件夹、一个 exam 程序，以及 input.bin
* 根据题目要求，使用 `cp` 命令将 input.bin 复制一份，生成 input 文件

```bash
  cp /workspace/input.bin /workspace/input
  ```

* 接着进入 cabinet 目录寻找密钥：

```bash
  ls -la /workspace/cabinet/
  ```

* 发现key.txt文件
* 打开key.txt文件

```bash
  cat /workspace/cabinet/key.txt
  ```

* 成功获取到密钥：MOON。
* 运行exam程序

```bash
  /workspace/exam
  ```

* 程序提示“参数无效，请使用 --help 查看用法”。于是运行

```bash
  /workspace/exam --help
  ```

* 了解了参数格式后，带上密钥执行

```bash
  /workspace/exam -e MOON
  ```

* 程序成功执行，并生成了 /workspace/output 文件
* 按照通关要求，复制出输出文件 flag

```bash
  cp /workspace/output /workspace/flag
  ```

* 执行以下命令确认flag 是一个普通文件且大小不超过 1 MiB

```bash
  file /workspace/flag

  ls -lh /workspace/flag
  ```

* 确认无误后提交文件

