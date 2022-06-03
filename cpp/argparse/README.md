# argparse

一个解析命令行参数的纯头文件库，仅需要c++11支持。

一个简单的例子
```c++
int main(int argc, char const *argv[])
{
    auto args = util::argparser("A quantum physics calculation program.");
    args.set_program_name("test")
        .add_help_option()
        .add_sc_option("-v", "--version", "show version info", []() {
            std::cout << "version " << VERSION << std::endl;
        })
        .add_option("-o", "--openmp", "use openmp or not")
        .add_option("-m", "--mpi", "use mpi or not")
        .add_option<int>("-t", "--threads", "if openmp it set,\nuse how many threads,\ndefault is 4", 4)
        .add_option<util::StepRange>("-r", "--range", "range", util::range(0, 10, 2))
        .add_named_argument<std::string>("input", "initialize file")
        .add_named_argument<std::string>("output", "output file")
        .parse(argc, argv);
}
```

## 解析规则

在我的库里面，命令行参数分为两大类（可选的**选项**，和必选的**参数**），每种又分为两种，即共四种命令行参数。

### `option`（选项）

选项，从语义上来说是**可选**的。它分成两种：一般选项和短路选项。

#### 一般选项

用如下方式添加一般选项：
```c++
args.add_option<int>("-t", "--threads", "threads number, default is 4", 4);
```
其中第一个是短选项名，是可以为空字符串的，如果非空，则必须是一个`'-'`后接单个字符；第二个是长选项名，不能为空字符串，必须以`"--"`开头。第三个是帮助信息，最后一个是该选项的默认值。

一般选项默认支持：`bool, int, int64_t, double, std::string`这五种类型。本来不想加入`int`的，但是考虑到多数人还是习惯默认用`int`，所以还是加上吧。

除了`bool`型的`option`，其余的`option`添加时都要给定默认的值。
对于`bool`型，不需要默认值，检查到命令行参数有这个选项，就是`true`否则为`false`。例如

```bash
ls -l
```
此时`-l`选项为`true`。而其他类型选项需要在其后面加上参数，比如
```bash
greet --name Alice --age 24
```
于是`--name`选项的值为`"Alice"`，`--age`选项的值为`24`。

#### 短路选项

短路选项（short circuit option）按照如下方式添加
```c++
args.add_sc_option("-v", "--version", "show version info", []() {
            std::cout << "version " << VERSION << std::endl;
        });
```

短路选项仅支持`bool`类型，添加该选项时需要给定一个回调函数。短路选项是最先搜索解析一种命令行参数。比如
```bash
git --help
gcc -v
```
只要命令行参数包含了这类参数，则调用回调函数，并立即（正常）退出程序。

如果有多个短路选项，按照添加的顺序搜索，仅调用第一个找到的短路选项的回调函数。

### `argument`参数

参数，和选项相反是必须提供的。如果某个参数没有提供，则程序会报错并退出。参数分为位置参数和命名参数两种

#### 位置参数

按照位置获取的参数，例如
```bash
dir /usr/bin
```
`/usr/bin`就是一个位置参数。一般而言位置参数不应该过多，不然这个命令行程序很难使用。


按照如下方式添加位置参数
```c++
args.add_argument<std::string>("input", "initialize file");
```
该函数的第一个参数是该参数的名字，用于获取该参数值时使用，第二个是帮助信息。参数不支持`bool`类型，默认支持`int, int64_t, double, std::string`四种类型。

#### 命名参数

这是为了解决我个人工作中遇到的情况而定义的。它的使用方式例如
```bash
greet name=Alice age=24
```

使用如下函数添加命名参数
```c++
args.add_named_argument<std::string>("output", "output file");
```

显然这样使用参数非常繁琐，不应该作为轻量级的命令行程序使用。但是可以放在一个较重的工程中，并且运行的时候是用脚本调用程序而不是直接在命令行使用。这个时候，使用命名参数可以让你的脚本更具可读性。

解析命令行参数时，先解析命名参数，剩下的自动按照顺序赋值给位置参数。命名参数不必按照设置的顺序指定。

## 杂项

### 短选项名聚合指定

这是为了实现linux一些基本命令行工具类似的效果。比如
```bash
ls -lh
```
同时指定了`-l`和`-h`选项。（这也是为什么选项的短名字仅允许一个字符。）

### 帮助选项

使用
```c++
args.add_help_option();
```
添加默认的帮助选项，它实际上相当于
```c++
args.add_sc_option("-?", "--help", "show this help message", [&args](){
            args.print_help();
        });
```
使用`"-?"`而不是`"-h"`是为了给其他选项留出空间，如果你不喜欢的话，可以用`add_sc_option`自行添加。

### 冲突

短路选项和一般选项的名字不允许冲突。命名参数和位置参数的名字也不允许冲突。

### 多行的帮助信息

有的时候我们的帮助信息很长，如果写在一行但是控制台的宽度不够造成换行会很难看。你可以在帮助信息里面加上换行符，它会根据的换行符在换行前加空格使得帮助信息看起来更好看。


## 获取结果

获取结果的方式举例如下
```c++
if(args.has_option("--openmp"))
{
    std::cout << "openmp is used, and we use " << args.get_option_int("--threads") << " threads" << std::endl;
}
if(args.has_option("--mpi"))
{
    std::cout << "mpi is used" << std::endl;
}
std::cout << "calculate range is " << args.get_option<util::StepRange>("--range") << std::endl;
std::cout << "the input file is " << args.get_argument<std::string>("input") << std::endl;
std::cout << "the output file is " << args.get_argument<std::string>("output") << std::endl;
```

### 获取选项

短路选项通常用于一些程序非正常运行的情况，并且检测到就调用回调函数并立即退出。所以我们不需要获取短路选项的值。对于一般选项，你可以用模板函数
```c++
args.get_option<bool>("-o");
```
获取，也可以用我提供的一些别名，比如`get_option_int`。对于`bool`型选项，特别提供了`has_option`函数。

### 获取参数

命名参数和位置参数在设置和解析阶段有区别，但是在获取结果时没有区别，所以统一使用
`get_argument<T>`获取，同样提供一些类似`get_argument_int`的别名。

### 保存各种参数的结果

使用
```c++
void print_as_ini(std::ostream &os, bool comments = false)
```
函数打印解析结果为`ini`文件格式。如果`comments == true`则将帮助信息作为注释打印。

## 拓展

你可以拓展支持你想要的类型。只要你实现了如下三个模板特化（均在命名空间`util`下）
```c++
template <> inline std::string type_string<T>() {return "you type name"};
template <> inline std::string to_string<T>(const T &value) {...}
template <> inline T parse_value<T>(const std::string &str) {...}
```
然后使用`add_option<T>`等模板函数就可以了。我前面给出代码中，`util::StepRange`就是我自定义的一个类型。

其中`to_string`默认实现是符号重载了`std::ostream`的`<<`，如果你有可以不用特化。`parse_value`默认实现是符号重载了`std::istream`的`>>`，如果你有，并且有默认构造函数，那么也可以不用特化。


## 相似的项目

我的库最开始借鉴了[cmdline](https://github.com/tanakh/cmdline)的一些设计思路，也借鉴了python标准库的`argparse`的一些思路。随着用于我自己的工作中，逐渐修改成了现在的版本。