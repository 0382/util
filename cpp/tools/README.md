# 一些小工具

### count_down
比如编译后放在`/home/username/myprogram/bin/count_down`，在这里写一个`count_down.txt`：
```text
2024-06-24 暑假
2024-09-09 新学期注册
2025-01-28 今年结束
```
然后添加`/home/username/myprogram/bin/count_down`到`~/.bashrc`的最后一行。那么每次登录都会给出一个提醒，例如
```text
还有104.541667天到暑假(2024-06-24)
还有181.541667天到新学期注册(2024-09-09)
还有322.541667天到今年结束(2025-01-28)
```