/**
 * @author: 0.382
 * @description: 元素周期表和原子核物理相关
 * @url: https://github.com/0382/util/tree/main/cpp/nucleus
 */

#pragma once
#ifndef UTIL_NUCLEUS_HPP
#define UTIL_NUCLEUS_HPP

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
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

constexpr const char *l_name_map = "spdfghijklmno";

// j-shceme 下的单粒子轨道。该结构体以及下面的`MOrbit`中，
// 总角动量和同位旋相关量都是使用物理上量子数的两倍来表示，以回避半整数
struct JOrbit
{
    int n;
    int l;
    int j;
    int tz;
    JOrbit(int _n, int _l, int _j, int _tz) : n(_n), l(_l), j(_j), tz(_tz) {}
};

struct MOrbit
{
    int n;
    int l;
    int j;
    int m;
    int tz;
    MOrbit(int _n, int _l, int _j, int _m, int _tz) : n(_n), l(_l), j(_j), m(_m), tz(_tz) {}
    MOrbit(JOrbit orb, int _m) : n(orb.n), l(orb.l), j(orb.j), m(_m), tz(orb.tz) {}
};

// 单粒子轨道的名字。例如`p(0d3/2)`，其中最外面的`p, n`表示质子或者中子，
// `l > 12`以后的轨道角动量都用`"x"`表示。（我想也没人对这之后的轨道角动量感兴趣。）
inline std::string name(const JOrbit &orb)
{
    char nucleon_type = orb.tz == -1 ? 'p' : 'n';
    char l_name = orb.l > 12 ? 'x' : l_name_map[orb.l];
    return nucleon_type + std::to_string(orb.n) + l_name + std::to_string(orb.j) + "/2";
}

struct NuclearShell
{
    std::vector<JOrbit> orbits;
    NuclearShell(std::vector<JOrbit> _orbits) : orbits(std::move(_orbits)) {}
    // 获得壳内所有的j-scheme轨道
    std::vector<JOrbit> j_orbits() const { return orbits; }
    // 获得壳内所有的m-scheme轨道
    std::vector<MOrbit> m_orbits() const
    {
        std::vector<MOrbit> result;
        for (auto &&orb : orbits)
        {
            for (int m = -orb.j; m <= orb.j; m += 2)
            {
                result.emplace_back(orb, m);
            }
        }
        return result;
    }
    // 壳内有几条j-scheme轨道。
    int jsize() const { return orbits.size(); }
    // 壳内有几条m-scheme轨道
    int msize() const
    {
        int sum = 0;
        for (auto &&orb : orbits)
            sum += orb.j + 1;
        return sum;
    }
    // 计算壳内能够容纳几个质子。
    int psize() const
    {
        int sum = 0;
        for (auto &&orb : orbits)
        {
            if (orb.tz == -1)
                sum += orb.j + 1;
        }
        return sum;
    }
    // 计算壳内能够容纳几个中子。
    int nsize() const
    {
        int sum = 0;
        for (auto &&orb : orbits)
        {
            if (orb.tz == 1)
                sum += orb.j + 1;
        }
        return sum;
    }
    // 计算壳内质子最大角动量
    int max_pj() const
    {
        int maxj = 1;
        for (auto &&orb : orbits)
        {
            if (orb.tz == -1)
                maxj = std::max(maxj, orb.j);
        }
        return maxj;
    }
    // 计算壳内中子最大角动量
    int max_nj() const
    {
        int maxj = 1;
        for (auto &&orb : orbits)
        {
            if (orb.tz == 1)
                maxj = std::max(maxj, orb.j);
        }
        return maxj;
    }
};

inline const NuclearShell s_shell = std::vector<JOrbit>{
    JOrbit(0, 0, 1, -1), JOrbit(0, 0, 1, 1) // 0s1/2
};
inline const NuclearShell p_shell = std::vector<JOrbit>{
    JOrbit(0, 1, 3, -1), JOrbit(0, 1, 3, 1), // 0p3/2
    JOrbit(0, 1, 1, -1), JOrbit(0, 1, 1, 1)  // 0p1/2
};
inline const NuclearShell sd_shell = std::vector<JOrbit>{
    JOrbit(0, 2, 5, -1), JOrbit(0, 2, 5, 1), // 0d5/2
    JOrbit(1, 0, 1, -1), JOrbit(1, 0, 1, 1), // 1s1/2
    JOrbit(0, 2, 3, -1), JOrbit(0, 2, 3, 1)  // 0d3/2
};

inline const NuclearShell pf_shell = std::vector<JOrbit>{
    JOrbit(0, 3, 7, -1), JOrbit(0, 3, 7, 1), // 0f7/2
    JOrbit(1, 1, 3, -1), JOrbit(1, 1, 3, 1), // 1p3/2
    JOrbit(0, 3, 5, -1), JOrbit(0, 3, 5, 1), // 0f5/2
    JOrbit(1, 1, 1, -1), JOrbit(1, 1, 1, 1)  // 1p1/2
};

inline NuclearShell merge(const NuclearShell &ns1, const NuclearShell &ns2)
{
    std::vector<JOrbit> orbits;
    std::copy(ns1.orbits.cbegin(), ns1.orbits.cend(), std::back_inserter(orbits));
    std::copy(ns2.orbits.cbegin(), ns2.orbits.cend(), std::back_inserter(orbits));
    return NuclearShell(orbits);
}

// 单粒子轨道将 m 量子数转化为正数作为索引
inline int m2index(int m) { return m >= 0 ? m : ~m; }
// 将单粒子轨道映射成的索引转化为 m 量子数
inline int index2m(int idx) { return (idx & 0x01) ? idx : ~idx; }

// 计算谐振子基 Nmax = N 的 m-scheme 轨道数目。注意仅计算质子，
// 因为计算中通常是对质子和中子分开算的，省的后面再手动除以 2 了。
constexpr int HO_size(int N) { return (N + 1) * (N + 2) * (N + 3) / 3; }

template <typename T>
T binominal(int n, int k)
{
    if (k < 0)
        return 0;
    int sgn = 1; // 保存符号
    if (n < 0)
    {
        n = -n + k - 1;
        if (k & 0x01)
            sgn = -sgn;
    }
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return sgn;
    if (k == 1)
        return sgn * n;
    // 如果 k 大于 n 的一半
    if (k > (n >> 1))
        k = (n - k);
    T nn = n - k + 1;
    T x = nn;
    nn += 1;
    T rr = 2;
    while (rr <= k)
    {
        x = (x * nn) / rr;
        rr += 1;
        nn += 1;
    }
    return sgn * x;
}

// 通过斯特林公式估计的 binominal 函数溢出的可能
// 注意是上述算法溢出的可能，而不是二项式系数的溢出可能
template <typename T>
bool binominal_safe(int n, int m)
{
    int k = n - m;
    double f = (n + 0.5) * std::log(n) - (m + 0.5) * std::log(m) - (k - 0.5) * std::log(k) - 1;
    return f < std::log(std::numeric_limits<T>::max());
}

// 将 `n` 个小球放入 `capacities.size()` 个容器内，每个容器的容量由 `capacities` 数组指定
// 生成所有可能的放置方式
inline std::vector<std::vector<int>> partision(int n, std::vector<int> capacities)
{
    std::vector<std::vector<int>> result;
    auto next = std::function<void(int, std::vector<int>, std::vector<int>)>();
    next = [&result, &next](int n, const std::vector<int> &caps, const std::vector<int> &line)
    {
        // 最后一个容器
        if (caps.size() == 1)
        {
            if (n <= caps.front())
            {
                auto xline = line;
                xline.push_back(n);
                result.push_back(xline);
            }
        }
        else
        {
            // 对第一个容器，放入所有可能的情况
            for (int i = 0; i <= std::min(n, caps.front()); ++i)
            {
                auto xline = line;
                xline.push_back(i);
                next(n - i, std::vector<int>(caps.begin() + 1, caps.end()), xline);
            }
        }
    };
    std::vector<int> line;
    line.reserve(capacities.size());
    next(n, capacities, line);
    return result;
}

// 虽然需要指定空间大小有点奇怪
// 凑合能用就行
inline __int128_t m_config_size(const NuclearShell &ns, int Z, int N)
{
    auto orbits = ns.m_orbits();
    auto orbit_number = ns.msize();

    std::vector<int> p_m_map, n_m_map;
    p_m_map.resize(ns.max_pj() + 1, 0);
    n_m_map.resize(ns.max_nj() + 1, 0);

    for (auto &&orb : orbits)
    {
        if (orb.tz == -1)
        {
            p_m_map[m2index(orb.m)] += 1;
        }
        else
        {
            n_m_map[m2index(orb.m)] += 1;
        }
    }

    int max_p_bin = *std::max_element(p_m_map.cbegin(), p_m_map.cend());
    if (!binominal_safe<int64_t>(max_p_bin, Z))
    {
        std::cout << "binominal of int64_t may overflow\n";
        std::exit(0);
    }
    int max_n_bin = *std::max_element(n_m_map.cbegin(), n_m_map.cend());
    if (!binominal_safe<int64_t>(max_n_bin, N))
    {
        std::cout << "binominal of int64_t may overflow\n";
        std::exit(0);
    }

    std::unordered_map<int, __int128_t> hist_pM, hist_nM;

    auto p_ptn = partision(Z, p_m_map);
    for (auto &&line : p_ptn)
    {
        __int128_t num = 1;
        int m = 0;
        for (int idx = 0; idx < line.size(); ++idx)
        {
            m += line[idx] * index2m(idx);
            num *= binominal<int64_t>(p_m_map[idx], line[idx]);
        }
        auto pos = hist_pM.find(m);
        if (pos == hist_pM.end())
        {
            hist_pM[m] = num;
        }
        else
        {
            pos->second += num;
        }
    }
    p_ptn.clear();
    p_ptn.shrink_to_fit();

    auto n_ptn = partision(N, n_m_map);
    for (auto &&line : n_ptn)
    {
        __int128_t num = 1;
        int m = 0;
        for (int idx = 0; idx < line.size(); ++idx)
        {
            m += line[idx] * index2m(idx);
            num *= binominal<int64_t>(n_m_map[idx], line[idx]);
        }
        auto pos = hist_nM.find(m);
        if (pos == hist_nM.end())
        {
            hist_nM[m] = num;
        }
        else
        {
            pos->second += num;
        }
    }
    n_ptn.clear();
    n_ptn.shrink_to_fit();

    __int128_t sum = 0;
    int target_MM = (N + Z) % 2;
    for (auto &&[pm, pnum] : hist_pM)
    {
        for (auto &&[nm, nnum] : hist_nM)
        {
            if (pm + nm == target_MM)
            {
                sum += pnum * nnum;
            }
        }
    }
    return sum;
}

inline std::ostream &operator<<(std::ostream &os, __int128_t x)
{
    if (x < 0)
    {
        os << '-';
        x = -x;
    }
    if (x > 9)
        os << x / 10;
    os << int(x % 10);
    return os;
}

// 构建一个`N = 2n + l`的谐振子壳层。
inline NuclearShell HO_shell(int N)
{
    std::vector<JOrbit> orbits;
    for (int n = 0; n <= N / 2; ++n)
    {
        int l = N - 2 * n;
        for (int j = 2 * l + 1; j >= std::max(2 * l - 1, 0); j -= 2)
        {
            orbits.emplace_back(n, l, j, -1);
            orbits.emplace_back(n, l, j, 1);
        }
    }
    return NuclearShell(orbits);
}

// 构建从`N = 0`到`N = Nmax`的所有谐振子轨道。
inline NuclearShell HO_orbits(int Nmax)
{
    std::vector<JOrbit> orbits;
    for (int N = 0; N <= Nmax; ++N)
    {
        for (int n = 0; n <= N / 2; ++n)
        {
            int l = N - 2 * n;
            for (int j = 2 * l + 1; j >= std::max(2 * l - 1, 0); j -= 2)
            {
                orbits.emplace_back(n, l, j, -1);
                orbits.emplace_back(n, l, j, 1);
            }
        }
    }
    return NuclearShell(orbits);
}

// 表示价空间的内。这个还是要有一定物理意义的，价轨道应该在核芯之上。
struct ValenceSpace
{
    int core_proton;
    int core_neutron;
    NuclearShell ns;
    ValenceSpace(int cp, int cn, const NuclearShell &_ns) : core_proton(cp), core_neutron(cn), ns(_ns) {}
};

inline const ValenceSpace p_space(2, 2, p_shell);
inline const ValenceSpace sd_space(8, 8, sd_shell);
inline const ValenceSpace pf_space(20, 20, pf_shell);

// 同位素
struct Isotope
{
    // 直接根据核子数构造同位素
    Isotope(int _Z, int _N) : Z(_Z), N(_N) {}

    // 根据核素名称构造同位素，仅支持形如 `Ne20` 这样的名称
    Isotope(const std::string &name)
    {
        std::regex pattern("([A-Z][a-z]*)(\\d{1,3})");
        std::smatch isotope_match;
        if (!std::regex_search(name, isotope_match, pattern))
        {
            Z = -1;
            N = -1;
            return;
        }
        std::string sym = isotope_match[1];
        auto ele = find_element_with_symbol(sym);
        Z = ele.Z();
        N = std::stoi(isotope_match[2]) - Z;
    }

    // 给定某个价空间，及其价核子数，构造同位素
    Isotope(const ValenceSpace &vs, int _Z, int _N) : Isotope(_Z + vs.core_proton, _N + vs.core_neutron) {}
    Isotope(const Isotope &iso, int _Z, int _N) : Isotope(_Z + iso.Z, _N + iso.N) {}
    Isotope(const std::string &core, int _Z, int _N)
    {
        Isotope core_iso(core);
        Z = core_iso.Z + _Z;
        N = core_iso.N + _N;
    }

    // 某个核素在某个壳内的价核子数目
    std::tuple<int, int> valence(const ValenceSpace &vs) const
    {
        return std::make_tuple(Z - vs.core_proton, N - vs.core_neutron);
    }

    // 不存在的同位素
    bool is_none() const { return N < 0 || Z < 0; }

    int getA() const { return N + Z; }
    int getZ() const { return Z; }
    int getN() const { return N; }

    friend std::ostream &operator<<(std::ostream &os, const Isotope &iso)
    {
        os << find_element_with_Z(iso.getZ()).symbol() << iso.getA();
        return os;
    }

    int Z;
    int N;
};

} // end namespace util

#endif // UTIL_NUCLEUS_HPP