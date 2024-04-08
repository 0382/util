# hpctools

## 使用方法

我喜欢写成一个函数来使用。通常将某个脚本里面的函数，比如`module-setup`函数添加到`~/.bashrc`里面去，重新`source`一下即可。

或者也可以在`~/.bashrc`里面添加一行`source /path/to/module-setup.sh`。

## module-setup

一个简易的environment module导入软件组的工具。module是很好用的，但是我们自己工作时可能会用到不同的环境，此时手动`module load`还是比较麻烦。

这个工具默认使用`~/.module-setup`作为配置文件，示例如下
```text
[imsrg]
gsl/2.7.1
boost/1.81.0

[kshell]
compiler/2023.0.0
mpi/2021.8.0
mkl/2023.0.0
```
其实就是定义一些环境，每个环境需要load哪些软件。


一些命令
```
module-setup help   # 显示帮助文档
module-setup list   # 显示`~/.module-setup`所有已经定义的环境
module-setup kshell # 设置kshell环境
```
注意，使用`module-setup`会调用`module purge`来清空所有已经load的软件。

## my-sbatch

有用的函数（命令）为`sb`，代替原来的`sbatch`，即`sb run-job.sh`。

其主要目的是生成`jobid git-hash`并添加到当前目录的`log/jobhash.txt`文件中去。由此可以追踪每次任务使用的是哪个版本的代码。（当然，如果你忘了编译最新代码，那就没办法了。）