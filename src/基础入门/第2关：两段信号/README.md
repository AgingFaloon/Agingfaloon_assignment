# 第2关：两段信号

## 一、通关条件

提交大小不超过 1 MiB 的普通文件 /workspace/flag。其字节内容必须严格等于 A 段处理结果与 B 段处理结果的有序拼接，不能缺段、重复或增加额外字节。

## 二、解题过程记录

* 使用 `find` 和 `grep` 命令找出关键文件

```bash
  find . -name "\\\*signal-A\\\*"
  find . -name "\\\*signal-B\\\*"
  grep -r "ACCESS\\\_KEY=" .
  ```

* 确定了A段和B段的位置以及ACCESS\_KEY=STAR
* 通过查看帮助信息，明确程序的用法和约定路径：

```bash
  /workspace/exam -h
  ```

* 处理 A 段

```bash
  cp /workspace/archive/2026/signal-A.bin /workspace/input
  /workspace/exam -e STAR
  mv /workspace/output /workspace/A\\\_result
  ```

* 处理 B 段

```bash
  cp /workspace/inbox/signal-B.bin /workspace/input
  /workspace/exam -e STAR
  mv /workspace/output /workspace/B\\\_result
  ```

* 按照 A段在前、B段在后的顺序拼接

```bash
  cat /workspace/A\\\_result /workspace/B\\\_result > /workspace/flag
  ```

* 检查文件大小，保证不超过 1 MiB

```bash
  ls -l /workspace/flag
  ```

* 验证 flag 字节内容等于 A 段处理结果与 B 段处理结果的有序拼接

```bash
  wc -c /workspace/A\\\_result /workspace/B\\\_result /workspace/flag
  ```

* 最后提交文件

