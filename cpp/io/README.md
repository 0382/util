# io

输入输出和字符串相关的一些函数

### `easyprint.hpp`
定义了
```c++
print(args...)
println(args...)
sep_print(sep, args...)
sep_println(sep, args...)
```
等函数，用于模仿其他语言的输出行为。

### `tryparse.hpp`

封装了字符串转数字的函数，返回`std::optional<T>`，用于处理解析失败的情况。使用方法

```c++
auto a = tryparse<int>("1234", 16);
if (a)
    print(a.value());
```