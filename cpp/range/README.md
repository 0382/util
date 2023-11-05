# range

模仿python定义的`range`函数，包含起始位置而不包含结束位置。

具体实现则参考了julia的做法。定义如下
```cpp
inline constexpr UnitRange range(int64_t end) { return UnitRange(0, end); }
inline constexpr UnitRange range(int64_t begin, int64_t end) { return UnitRange(begin, end); }
inline constexpr StepRange range(int64_t begin, int64_t end, int64_t step) { return StepRange(begin, end, step); }
```
单参数和两参数的`range`函数构造`UnitRange`类，每次增加1，三参数的`range`函数构造`StepRange`类，使用自定义的步长。

使用方法见`test.cpp`.