# util

收集整理我写过的一些乱七八糟的代码。整个代码并不是一个项目，部分代码会力求做成一个head only的头文件。

## c++

- [argparse](./cpp/argparse)，一个简单的命令行参数解析库。
- [inifile](./cpp/inifile)，一个简单的ini文件读写库。
- [logger](./cpp/logger)，一个简单的日志库。
- [range](./cpp/range)，模仿python定义的range函数。
- [radix_sort](./cpp/radix_sort)，基数排序，对于基本类型比`std::sort`更快。
- [io](./cpp/io)，定义了输入输出和字符串相关的一些函数。
- [windows](./cpp/windows)，windows系统相关的一些东西。
- [matrix](./cpp/matrix)，计算物理作业相关代码。
- [snippet](./cpp/snippet)，一些乱七八糟的代码片段。
- [statistics](./cpp/statistics)，统计相关的代码。
- [nucleus](./cpp/nucleus)，原子核物理相关，其中包含了元素周期表。


目前这些代码普遍基于`c++17`标准，如果没有用到`c++17`特性，可能可以直接编译；否则，需要指定`c++17`的选项。

## latex

- [latex](./latex)，定义的一些常用的命令。

## Julia

转移到[MyTools](https://github.com/0382/MyTools.jl)，自己安装自己用，真好玩。
