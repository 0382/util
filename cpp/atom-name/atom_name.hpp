/**
 * @author: 0.382
 * @description: parse command line args
 * @url: https://github.com/0382/util/tree/main/cpp/atom-name
 */

#pragma once
#ifndef UTIL_ATOM_NAME_HPP
#define UTIL_ATOM_NAME_HPP

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace util
{

class Element
{
  public:
    Element(int _Z, std::string _short, std::string _zh, std::string _en, std::string _latin, std::string _pinyin)
        : atomic_number(_Z), name_short(std::move(_short)), name_zh(std::move(_zh)), name_en(std::move(_en)),
          name_latin(std::move(_latin)), name_pinyin(std::move(_pinyin))
    {}

    bool is_None() const { return atomic_number == -1; }

    int Z() const { return atomic_number; }
    int get_atomic_number() const { return atomic_number; }
    std::string symbol() const { return name_short; }
    std::string chinese() const { return name_zh; }
    std::string english() const { return name_en; }
    std::string latin() const { return name_latin; }
    std::string pinyin() const { return name_pinyin; }

  private:
    int atomic_number;       // 原子序数
    std::string name_short;  // 元素符号
    std::string name_zh;     // 中文名
    std::string name_en;     // 英文名
    std::string name_latin;  // 拉丁文名
    std::string name_pinyin; // 汉语拼音
};

// 不存在的元素，查找失败的时候返回
inline const Element NoneElement(-1, "", "", "", "", "");

// clang-format off
inline const std::vector<Element> element_table = {
    Element(1, "H", "氢", "Hydrogen", "Hydrogenium", "qīng"),
    Element(2, "He", "氦", "Helium", "Helium", "hài"),
    Element(3, "Li", "锂", "Lithium", "Lithium", "lǐ"),
    Element(4, "Be", "铍", "Beryllium", "Beryllium", "pí"),
    Element(5, "B", "硼", "Boron", "Borum", "péng"),
    Element(6, "C", "碳", "Carbon", "Сarbonium (Carboneum)", "tàn"),
    Element(7, "N", "氮", "Nitrogen", "Nitrogenium", "dàn"),
    Element(8, "O", "氧", "Oxygen", "Oxygenium", "yǎng"),
    Element(9, "F", "氟", "Fluorine", "Fluorum", "fú"),
    Element(10, "Ne", "氖", "Neon", "Neon", "nǎi"),
    Element(11, "Na", "钠", "Sodium", "Natrium", "nà"),
    Element(12, "Mg", "镁", "Magnesium", "Magnesium", "měi"),
    Element(13, "Al", "铝", "Aluminum", "Aluminium", "lǚ"),
    Element(14, "Si", "硅", "Silicon", "Silicium", "guī"),
    Element(15, "P", "磷", "Phosphorus", "Phosphorus", "lín"),
    Element(16, "S", "硫", "Sulfur", "Sulphuris", "liú"),
    Element(17, "Cl", "氯", "Chlorine", "Сhlorum", "lǜ"),
    Element(18, "Ar", "氩", "Argon", "Argon", "yà"),
    Element(19, "K", "钾", "Potassium", "Kalium", "jiǎ"),
    Element(20, "Ca", "钙", "Calcium", "Сalcium", "gài"),
    Element(21, "Sc", "钪", "Scandium", "Scandium", "kàng"),
    Element(22, "Ti", "钛", "Titanium", "Titanium", "tài"),
    Element(23, "V", "钒", "Vanadium", "Vanadium", "fán"),
    Element(24, "Cr", "铬", "Chromium", "Chromium", "gè"),
    Element(25, "Mn", "锰", "Manganese", "Manganum", "měng"),
    Element(26, "Fe", "铁", "Iron", "Ferrum", "tiě"),
    Element(27, "Co", "钴", "Cobalt", "Cobaltum", "gǔ"),
    Element(28, "Ni", "镍", "Nickel", "Niccolum", "niè"),
    Element(29, "Cu", "铜", "Copper", "Cuprum", "tóng"),
    Element(30, "Zn", "锌", "Zinc", "Zincum", "xīn"),
    Element(31, "Ga", "镓", "Gallium", "Gallium", "jiā"),
    Element(32, "Ge", "锗", "Germanium", "Germanium", "zhě"),
    Element(33, "As", "砷", "Arsenic", "Arsenicum", "shēn"),
    Element(34, "Se", "硒", "Selenium", "Selenium", "xī"),
    Element(35, "Br", "溴", "Bromine", "Bromum", "xiù"),
    Element(36, "Kr", "氪", "Krypton", "Krypton", "kè"),
    Element(37, "Rb", "铷", "Rubidium", "Rubidium", "rú"),
    Element(38, "Sr", "锶", "Strontium", "Strontium", "sī"),
    Element(39, "Y", "钇", "Yttrium", "Yttrium", "yǐ"),
    Element(40, "Zr", "锆", "Zirconium", "Zirconium", "gào"),
    Element(41, "Nb", "铌", "Niobium", "Niobium", "ní"),
    Element(42, "Mo", "钼", "Molybdenum", "Molybdaenum", "mù"),
    Element(43, "Tc", "锝", "Technetium", "Technetium", "dé"),
    Element(44, "Ru", "钌", "Ruthenium", "Ruthenium", "liǎo"),
    Element(45, "Rh", "铑", "Rhodium", "Rhodium", "lǎo"),
    Element(46, "Pd", "钯", "Palladium", "Palladium", "bǎ"),
    Element(47, "Ag", "银", "Silver", "Argentum", "yín"),
    Element(48, "Cd", "镉", "Cadmium", "Cadmium", "gé"),
    Element(49, "In", "铟", "Indium", "Indium", "yīn"),
    Element(50, "Sn", "锡", "Tin", "Stannum", "xī"),
    Element(51, "Sb", "锑", "Antimony", "Stibium", "tī"),
    Element(52, "Te", "碲", "Tellurium", "Tellurium", "dì"),
    Element(53, "I", "碘", "Iodine", "Iodium", "diǎn"),
    Element(54, "Xe", "氙", "Xenon", "Xenon", "xiān"),
    Element(55, "Cs", "铯", "Caesium", "Caesium", "sè"),
    Element(56, "Ba", "钡", "Barium", "Barium", "bèi"),
    Element(57, "La", "镧", "Lanthanum", "Lanthanum", "lán"),
    Element(58, "Ce", "铈", "Cerium", "Cerium", "shì"),
    Element(59, "Pr", "镨", "Praseodymium", "Praseodymium", "pǔ"),
    Element(60, "Nd", "钕", "Neodymium", "Neodymium", "nǚ"),
    Element(61, "Pm", "钷", "Promethium", "Promethium", "pǒ"),
    Element(62, "Sm", "钐", "Samarium", "Samarium", "shān"),
    Element(63, "Eu", "铕", "Europium", "Europium", "yǒu"),
    Element(64, "Gd", "钆", "Gadolinium", "Gadolinium", "gá"),
    Element(65, "Tb", "铽", "Terbium", "Terbium", "tè"),
    Element(66, "Dy", "镝", "Dysprosium", "Dysprosium", "dī"),
    Element(67, "Ho", "钬", "Holmium", "Holmium", "huǒ"),
    Element(68, "Er", "铒", "Erbium", "Erbium", "ěr"),
    Element(69, "Tm", "铥", "Thulium", "Thulium", "diū"),
    Element(70, "Yb", "镱", "Ytterbium", "Ytterbium", "yì"),
    Element(71, "Lu", "镥", "Lutetium", "Lutetium", "lǔ"),
    Element(72, "Hf", "铪", "Hafnium", "Hafnium", "hā"),
    Element(73, "Ta", "钽", "Tantalum", "Tantalum", "tǎn"),
    Element(74, "W", "钨", "Tungsten", "Wolframium", "wū"),
    Element(75, "Re", "铼", "Rhenium", "Rhenium", "lái"),
    Element(76, "Os", "锇", "Osmium", "Osmium", "é"),
    Element(77, "Ir", "铱", "Iridium", "Iridium", "yī"),
    Element(78, "Pt", "铂", "Platinum", "Platinum", "bó"),
    Element(79, "Au", "金", "Gold", "Aurum", "jīn"),
    Element(80, "Hg", "汞", "Mercury", "Hydrargyrum", "gǒng"),
    Element(81, "Tl", "铊", "Thallium", "Thallium", "tā"),
    Element(82, "Pb", "铅", "Lead", "Plumbum", "qiān"),
    Element(83, "Bi", "铋", "Bismuth", "Bisemutum (Bismuthum, Bismutum)", "bì"),
    Element(84, "Po", "钋", "Polonium", "Polonium", "pō"),
    Element(85, "At", "砹", "Astatine", "Astatum", "ài"),
    Element(86, "Rn", "氡", "Radon", "Radon", "dōng"),
    Element(87, "Fr", "钫", "Francium", "Francium", "fāng"),
    Element(88, "Ra", "镭", "Radium", "Radium", "léi"),
    Element(89, "Ac", "锕", "Actinium", "Actinium", "ā"),
    Element(90, "Th", "钍", "Thorium", "Thorium", "tǔ"),
    Element(91, "Pa", "镤", "Protactinium", "Protactinium", "pú"),
    Element(92, "U", "铀", "Uranium", "Uranium", "yóu"),
    Element(93, "Np", "镎", "Neptunium", "Neptunium", "ná"),
    Element(94, "Pu", "钚", "Plutonium", "Plutonium", "bù"),
    Element(95, "Am", "镅", "Americium", "Americium", "méi"),
    Element(96, "Cm", "锔", "Curium", "Curium", "jú"),
    Element(97, "Bk", "锫", "Berkelium", "Berkelium", "péi"),
    Element(98, "Cf", "锎", "Californium", "Californium", "kāi"),
    Element(99, "Es", "锿", "Einsteinium", "Einsteinium", "āi"),
    Element(100, "Fm", "镄", "Fermium", "Fermium", "fèi"),
    Element(101, "Md", "钔", "Mendelevium", "Mendelevium", "mén"),
    Element(102, "No", "锘", "Nobelium", "Nobelium", "nuò"),
    Element(103, "Lr", "铹", "Lawrencium", "Lawrencium", "láo"),
    Element(104, "Rf", "𬬻", "Rutherfordium", "Rutherfordium", "lú"),
    Element(105, "Db", "𬭊", "Dubnium", "Dubnium", "dù"),
    Element(106, "Sg", "𬭳", "Seaborgium", "Seaborgium", "xǐ"),
    Element(107, "Bh", "𬭛", "Bohrium", "Bohrium", "bō"),
    Element(108, "Hs", "𬭶", "Hassium", "Hassium", "hēi"),
    Element(109, "Mt", "鿏", "Meitnerium", "Meitnerium", "mài"),
    Element(110, "Ds", "𫟼", "Darmstadtium", "Darmstadtium", "dá"),
    Element(111, "Rg", "𬬭", "Roentgenium", "Roentgenium", "lún"),
    Element(112, "Cn", "鎶", "Copernicium", "Copernecium", "gē"),
    Element(113, "Nh", "鉨", "Nihonium", "Nihonium", "nǐ"),
    Element(114, "Fl", "𫓧", "Flerovium", "Flerovium", "fū"),
    Element(115, "Mc", "镆", "Moscovium", "Moscovium", "mò"),
    Element(116, "Lv", "鉝", "Livermorium", "Livermorium", "lì"),
    Element(117, "Ts", "石田", "Tennessine", "Tennessine", "tián"),
    Element(118, "Og", "气奥", "Oganesson", "Oganneson", "ào")
};
// clang-format on

// 通过原子序数获得元素
inline Element find_element_with_Z(int Z)
{
    if (Z < 1 || Z > 118)
        return NoneElement;
    return element_table[Z - 1];
}

// 通过元素符号获得元素
inline Element find_element_with_symbol(const std::string &sym)
{
    auto pos = std::find_if(element_table.cbegin(), element_table.cend(),
                            [&sym](const Element &ele) { return ele.symbol() == sym; });
    if (pos == element_table.cend())
        return NoneElement;
    return *pos;
}

// 通过中文名获得元素
inline Element find_element_with_chinese(const std::string &chinese)
{
    auto pos = std::find_if(element_table.cbegin(), element_table.cend(),
                            [&chinese](const Element &ele) { return ele.chinese() == chinese; });
    if (pos == element_table.cend())
        return NoneElement;
    return *pos;
}

// 通过英文名获得元素
inline Element find_element_with_english(const std::string &english)
{
    auto pos = std::find_if(element_table.cbegin(), element_table.cend(),
                            [&english](const Element &ele) { return ele.english() == english; });
    if (pos == element_table.cend())
        return NoneElement;
    return *pos;
}

// 通过拉丁名获得元素
inline Element find_element_with_latin(const std::string &latin)
{
    auto pos = std::find_if(element_table.cbegin(), element_table.cend(),
                            [&latin](const Element &ele) { return ele.latin() == latin; });
    if (pos == element_table.cend())
        return NoneElement;
    return *pos;
}

// 通过拼音获得元素
inline Element find_element_with_pinyin(const std::string &pinyin)
{
    auto pos = std::find_if(element_table.cbegin(), element_table.cend(),
                            [&pinyin](const Element &ele) { return ele.pinyin() == pinyin; });
    if (pos == element_table.cend())
        return NoneElement;
    return *pos;
}

// 通过随便什么名获得元素
// 虽然这效率不高，可是这里又不是经常用的东西，不用在意效率
inline Element find_element(const std::string &name)
{
    auto ele = find_element_with_symbol(name);
    if (ele.is_None())
        ele = find_element_with_chinese(name);
    if (ele.is_None())
        ele = find_element_with_english(name);
    if (ele.is_None())
        ele = find_element_with_latin(name);
    if (ele.is_None())
        ele = find_element_with_pinyin(name);
    return ele;
}

struct NuclearShell
{
    int core;
    int valence;
};

constexpr auto p_shell = NuclearShell{2, 6};
constexpr auto sd_shell = NuclearShell{8, 12};
constexpr auto pf_shell = NuclearShell{20, 20};

// 同位素
struct Isotope
{
    // 直接根据核子数构造同位素
    Isotope(int Z, int N) : _Z(Z), _N(N) {}

    // 根据核素名称构造同位素，仅支持形如 `Ne20` 这样的名称
    Isotope(const std::string &name)
    {
        std::regex pattern("([A-Z][a-z]*)(\\d{1,3})");
        std::smatch isotope_match;
        if (!std::regex_search(name, isotope_match, pattern))
        {
            _Z = -1;
            _N = -1;
            return;
        }
        _name = isotope_match[1];
        auto ele = find_element_with_symbol(_name);
        _Z = ele.Z();
        _N = std::stoi(isotope_match[2]) - _Z;
    }

    // 给定某个壳，及其价核子数，构造同位素
    Isotope(const NuclearShell &ns, int Z, int N) : Isotope(Z + ns.core, N + ns.core) {}

    // 某个核素在某个壳内的价核子数目
    std::tuple<int, int> valence(const NuclearShell &ns) const { return std::make_tuple(_Z - ns.core, _N - ns.core); }

    bool is_none() const { return _N < 0; }

    int A() const { return _N + _Z; }
    int Z() const { return _Z; }
    int N() const { return _N; }
    std::string name() const { return _name; }

    friend std::ostream &operator<<(std::ostream &os, const Isotope &iso)
    {
        os << iso._name << iso.A();
        return os;
    }

  private:
    int _Z;
    int _N;
    std::string _name;
};

} // end namespace util

#endif // UTIL_ATOM_NAME_HPP