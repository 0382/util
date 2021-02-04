# inifile

读写ini文件的一个简单的库，只有一个头文件。这个库只是为了能够用ini文件为某个更大的程序做初始化，因此设计为遇到错误就终止程序，并打印错误信息。

## 使用

```cpp
inifile ini("test.ini"); // 用文件名初始化

// 读取数据
ini.get_string("key");
ini.section("section1").get_string("key");

// 设置数据
ini.set_string("key", "value");
ini.section("section1").set_string("key", "value");

// 打印和保存
ini.show();
ini.save_as("file.ini");
```

其中`string`还可以换成`int,double,bool`，支持这四类数据。ini文件可以分成几个`section`，使用`[section-name]`来标识`section`。这里仅支持`ini.section("section-name")`然后对某`section`的数据进行读写。对于没有标识`section-name`的部分，可以直接读写。实际上代码实现是指定了一个默认的`section`，名字是空字符串。


