# yamabuki

### File Scanner

按照规则查找目录中的可疑文件，并输出到 stdout。
当前可疑文件的规则为：
- 文件大小为 2MiB 以上
- 若文件为可执行文件，文件名不能为 `radare2` 等知名逆向工程软件的文件名

输出格式：`学生ID:可疑文件路径:可疑文件类别`

```
➜  yamabuki git:(main) ✗ ./file_scanner /Users/name1e5s/Documents/Temp/CV
2021919810:/Users/name1e5s/Documents/Temp/CV/hw.tar.gz:INVALID_BIG_FILE
2021919810:/Users/name1e5s/Documents/Temp/CV/r2:INVALID_EXE_NAME
```

### Onlink Time

通过读取 /var/log/wtmp，获取各个用户的在线时间(以小时为单位)，并统一格式

输出格式：`学生ID:在线时间(h)`

```
➜  yamabuki git:(main) ✗ bash online_time.sh
root:0.02
name1e5s:3352.29
```

### Login Count

通过读取 /var/log/wtmp，获取各个用户的登陆次数

输出格式：`学生ID:登陆次数`

```
➜  yamabuki git:(main) ✗ bash login_count.sh
name1e5s:38
reboot:10
root:4
shutdown:8
```

### First Login Time

通过读取 /var/log/wtmp，获取各个用户的首次登陆时间，可近似视为首次开始实验的时间

输出格式：`学生ID 首次登陆时间（ISO-8601）`

```
➜  yamabuki git:(main) ✗ bash first_login_time.sh
frank 2021-07-28T08:03:34+0000
yhx 2021-07-29T01:12:59+0000
```