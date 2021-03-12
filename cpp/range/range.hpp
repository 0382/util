#pragma once
#ifndef UTIL_RANGE_HPP
#define UTIL_RANGE_HPP

#include <cstdint>
#include <stdexcept>

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
        iterator(int64_t pos) : position(pos) {}
        iterator &operator++()
        {
            ++position;
            return *this;
        }
        bool operator!=(const iterator &rhs) const { return position != rhs.position; }
        int64_t &operator*() { return position; }

      private:
        int64_t position;
    };

    UnitRange(int64_t begin, int64_t end) : _begin(begin), _end(end) {}

    iterator begin() const { return iterator(_begin); }
    iterator end() const { return iterator(_end); }

  private:
    int64_t _begin;
    int64_t _end;
};

// 步长任意的 range
class StepRange
{
  public:
    class iterator
    {
      public:
        iterator(int64_t pos, int64_t _step) : position(pos), step(_step) {}
        iterator &operator++()
        {
            position += step;
            return *this;
        }
        bool operator!=(const iterator &rhs) const { return position != rhs.position; }
        int64_t &operator*() { return position; }

      private:
        int64_t position;
        int64_t step;
    };

    StepRange(int64_t begin, int64_t end, int64_t step) : _begin(begin), _end(end), _step(step)
    {
        if (_step == 0)
            throw(std::invalid_argument("step cannot be zero"));
        if (_end > _begin && _step < 0)
        {
            _end = _begin;
            return;
        }
        else if (_end < _begin && _step > 0)
        {
            _end = _begin;
            return;
        }
        auto temp = (_end - _begin) % _step;
        if (temp != 0)
        {
            _end += _step - temp;
        }
    }

    iterator begin() const { return iterator(_begin, _step); }
    iterator end() const { return iterator(_end, _step); }

  private:
    int64_t _begin;
    int64_t _end;
    int64_t _step;
};

inline UnitRange range(int64_t end) { return UnitRange(0, end); }
inline UnitRange range(int64_t begin, int64_t end) { return UnitRange(begin, end); }
inline StepRange range(int64_t begin, int64_t end, int64_t step) { return StepRange(begin, end, step); }

} // end namespace util

#endif // UTIL_RANGE_HPP