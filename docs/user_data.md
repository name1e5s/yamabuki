# 实验平台管理模块：在线监测与数据获取

[TOC]

## 需求列表

- [x] 监测长时间占用较多 CPU 资源的进程并做记录，必要时可删除进程
- [x] 扫描用户账户下的文件，对可疑文件作出记录
- [ ] 监测恶意频繁提交的学生，作出记录并发出警告信息*

- 收集用户信息
  - [x] 首次开始实验的时间
  - [x] 登陆次数
  - [x] 在线时长
  - [ ] 用户在线时执行的全部指令，以此判断是否存在恶意行为
  - [ ] 提交答案的次数、每次提交的时间点*
  - [ ] 每个 Phase 的提交次数*
  - [ ] 每个 Phase 的排名*

其中，需要处理 `lablogdata` 内的日志文件的需求（以星号标记）由陈璇实现，其余需求由于海鑫实现（代码URL https://github.com/name1e5s/yamabuki）。

## 实现记录

### 耗费 CPU 资源进程的记录（high_cpu_cost_process.sh）

在一段时间内每秒执行一次 `ps`指令获取全部正在执行的进程的 CPU 占用，并在之后统计出平均 CPU 占用高于一定限额的进程作出记录。

输出格式： `学生ID PID 程序名 平均CPU占有率`

```bash
➜  yamabuki git:(main) ✗ bash high_cpu_cost_process.sh
_windowserver 131 /System/Library/ 27.98
name1e5s 6853 /Applications/Ne 13.745
root 6826 /usr/sbin/blueto 5.515
```

### 可疑文件的记录（file_scanner.c）

按照规则查找目录中的可疑文件，并输出到 stdout。
当前可疑文件的规则为：

- 文件大小为 2MiB 以上（同时考虑在 `/etc/security/limits.conf`中添加对于最大文件大小的限制）
- 若文件为可执行文件，文件名不能为 `radare2` 等知名 Linux 平台逆向软件的文件名

输出格式：`学生ID:可疑文件路径:可疑文件类别`

```bash
➜  yamabuki git:(main) ✗ ./file_scanner /Users/name1e5s/Documents/Temp/CV
2021919810:/Users/name1e5s/Documents/Temp/CV/hw.tar.gz:INVALID_BIG_FILE
2021919810:/Users/name1e5s/Documents/Temp/CV/r2:INVALID_EXE_NAME
```

### 首次开始实验的时间（first_login_time.sh）

用户首次开始实验的时间可以被视为是以下三种时间之一：

- 用户初次登陆系统的时间（通过分析 `wtmp` 日志获得，**当前方案**）
- 用户初次运行实验 elf 文件的时间（可以通过 `auditd` 监控系统调用或是实验文件获取 - 最精确，实现难度以及对系统负担可能较大）
- 用户初次提交结果的时间（可以通过分析实验日志获取 - 考虑到解题速度，偏差可能较大）

这里采用用户初次登陆系统的时间近似视为开始实验的时间。

输出格式：`学生ID 首次登陆时间（ISO-8601）`

```bash
➜  yamabuki git:(main) ✗ bash first_login_time.sh
frank 2021-07-28T08:03:34+0000
yhx 2021-07-29T01:12:59+0000
```

### 登陆次数（login_count.sh）

通过读取 `/var/log/wtmp`，获取各个用户的登陆次数

输出格式：`学生ID 登陆次数`

```bash
➜  yamabuki git:(main) ✗ bash login_count.sh
name1e5s 38
reboot 10
root 4
shutdown 8
```

### 在线时间（online_time.sh）

通过读取 `/var/log/wtmp`，获取各个用户的在线时间(以小时为单位)，并统一格式

输出格式：`学生ID 在线时间(h)`

```
➜  yamabuki git:(main) ✗ bash online_time.sh
root 0.02
name1e5s 3352.29
```

### 用户运行指令记录（实现中）

可以采用如下几种方案实现用户运行的全部指令：

- 修改 `.bash_logout`，在用户退出时将其执行过的指令（通过 `history` 指令获得）做记录（极容易被绕过）
- 采用 [Snoopy](https://github.com/a2o/snoopy/) hook `execve`/`execv` 系统调用，记录用户执行的指令（对静态链接的程序无效，可以被绕过）
- 采用 Linux 内置的[审计框架](https://documentation.suse.com/sles/12-SP4/html/SLES-all/part-audit.html)记录（很难被绕过，日志完善，同时可以对 `/etc/passwd`等文件做监控，以此监控是否发生提权；缺点是可能会影响服务器性能，同时管理较为复杂）

考虑采用Linux 内置的审计框架实现，目前仍在了解其效果。

