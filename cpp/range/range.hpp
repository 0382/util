#pragma once
#ifndef UTIL_RANGE_HPP
#define UTIL_RANGE_HPP

#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace util
{

// 步长为 1 的 range
// 在 -O2 优化下，似乎能够和普通的 for 循环等价
class UnitRange
{
  public:
    class iterator
    {
      public:
        constexpr iterator(int64_t pos) : position(pos) {}
        constexpr iterator &operator++()
        {
            ++position;
            return *this;
        }
        constexpr bool operator!=(const iterator &rhs) const { return position != rhs.position; }
        constexpr int64_t &operator*() { return position; }

      private:
        int64_t position;
    };

    constexpr UnitRange(int64_t begin, int64_t end) : m_begin(begin), m_end(end)
    {
        if (m_begin > m_end)
        {
            m_end = m_begin;
        }
    }

    constexpr iterator begin() const { return iterator(m_begin); }
    constexpr iterator end() const { return iterator(m_end); }

    friend std::ostream &operator<<(std::ostream &os, const UnitRange &range)
    {
        os << range.m_begin << ":" << range.m_end;
        return os;
    }

  private:
    int64_t m_begin;
    int64_t m_end;
};

// 步长任意的 range
class StepRange
{
  public:
    class iterator
    {
      public:
        constexpr iterator(int64_t pos, int64_t step) : position(pos), step(step) {}
        constexpr iterator &operator++()
        {
            position += step;
            return *this;
        }
        constexpr bool operator!=(const iterator &rhs) const { return position != rhs.position; }
        constexpr int64_t &operator*() { return position; }

      private:
        int64_t position;
        int64_t step;
    };

    constexpr StepRange(int64_t begin, int64_t end, int64_t step) : m_begin(begin), m_end(end), m_step(step)
    {
        if (m_step == 0)
            throw(std::invalid_argument("step cannot be zero"));
        if (m_end > m_begin && m_step < 0)
        {
            m_end = m_begin;
            return;
        }
        else if (m_end < m_begin && m_step > 0)
        {
            m_end = m_begin;
            return;
        }
        int64_t rem = (m_end - m_begin) % m_step;
        if (rem != 0)
        {
            m_end += m_step - rem;
        }
    }

    constexpr iterator begin() const { return iterator(m_begin, m_step); }
    constexpr iterator end() const { return iterator(m_end, m_step); }

    friend std::ostream &operator<<(std::ostream &os, const StepRange &range)
    {
        os << range.m_begin << ":" << range.m_step << ":" << range.m_end;
        return os;
    }

  private:
    int64_t m_begin;
    int64_t m_end;
    int64_t m_step;
};

inline constexpr UnitRange range(int64_t end) { return UnitRange(0, end); }
inline constexpr UnitRange range(int64_t begin, int64_t end) { return UnitRange(begin, end); }
inline constexpr StepRange range(int64_t begin, int64_t end, int64_t step) { return StepRange(begin, end, step); }

} // end namespace util

#endif // UTIL_RANGE_HPP