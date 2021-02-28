// 处理身份证校验码

#include "easyprint.hpp"
#include <iostream>
#include <string>
#include <string_view>

// 1,2 省市自治区
// 3,4 所在地级市
// 5,6 所在区，县，自治县，县级市
// 7-14 出生日期
// 15,16 所在地派出所代码
// 17 性别
// 18 校验码
static constexpr int _id_weight[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
static constexpr char _id_check_code[] = "10X98765432";

char get_id_check_code(std::string_view id17)
{
    if (id17.size() < 17)
        util::stop("should give at least 17 number");
    int sum = 0;
    for (int i = 0; i < 17; ++i)
    {
        sum += (id17[i] - '0') * _id_weight[i];
    }
    return _id_check_code[sum % 11];
}

bool check_idcard(std::string_view id)
{
    if (id.size() != 18)
        util::stop("invalid id number size ", id.size());
    int sum = 0;
    for (int i = 0; i < 17; ++i)
    {
        sum += (id[i] - '0') * _id_weight[i];
    }
    return _id_check_code[sum % 11] == id[17];
}

int main(int argc, char const *argv[])
{
    util::println(check_idcard("100110145305291438"));
    return 0;
}
