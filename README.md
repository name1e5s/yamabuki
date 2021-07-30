# yamabuki

### File Scanner

按照规则查找目录中的可疑文件，并输出到 stdout。
当前可疑文件的规则为：
- 文件大小为 2MiB 以上
- 若文件为可执行文件，文件名不能为 `radare2` 等知名逆向工程软件的文件名