# inifile

一个简单的读取ini配置文件的库。

```julia
# 获取某个section中的数据
getString(ini::IniFile, section::String, key::String)
getBool(ini::IniFile, section::String, key::String)
getInt(ini::IniFile, section::String, key::String)
getFloat(ini::IniFile, section::String, key::String)

# 获取默认section中的数据，
# 如果ini文件没有section，即全部都是默认section
# 一个ini文件开头如果没有指定section，则开头部分为默认section，直到第一个section为止
getValue(ini::IniFile, key::String)
getString(ini::IniFile, key::String)
getBool(ini::IniFile, key::String)
getInt(ini::IniFile, key::String)
getFloat(ini::IniFile, key::String)
```
