#pragma once

namespace claws
{
  template<class Value>
  class SelfIterator
  {
    Value value;

  public:
    constexpr SelfIterator<Value>(Value value) noexcept
      : value(std::move(value))
    {}

    constexpr SelfIterator<Value>() noexcept = default;
    constexpr SelfIterator<Value>(SelfIterator<Value> const &) noexcept = default;
    constexpr SelfIterator<Value>(SelfIterator<Value> &&) noexcept = default;
    constexpr SelfIterator<Value> &operator=(SelfIterator<Value> const &) noexcept = default;
    constexpr SelfIterator<Value> &operator=(SelfIterator<Value> &&) noexcept = default;

#define SELF_ITERATOR_UNARY_PREFIX(OP)                                                                                                                         \
  constexpr auto &operator OP() noexcept                                                                                                                       \
  {                                                                                                                                                            \
    OP value;                                                                                                                                                  \
    return *this;                                                                                                                                              \
  }

    SELF_ITERATOR_UNARY_PREFIX(++);
    SELF_ITERATOR_UNARY_PREFIX(--);

#define SELF_ITERATOR_UNARY_SUFFIX(OP)                                                                                                                         \
  constexpr auto operator OP(int) noexcept                                                                                                                     \
  {                                                                                                                                                            \
    auto copy{*this};                                                                                                                                          \
                                                                                                                                                               \
    OP value;                                                                                                                                                  \
    return copy;                                                                                                                                               \
  }

    SELF_ITERATOR_UNARY_SUFFIX(++);
    SELF_ITERATOR_UNARY_SUFFIX(--);

#define SELF_ITERATOR_BINARY_ASSIGN_OP(OP)                                                                                                                     \
  constexpr auto &operator OP(Value val) noexcept                                                                                                              \
  {                                                                                                                                                            \
    value OP val;                                                                                                                                              \
    return *this;                                                                                                                                              \
  }

    SELF_ITERATOR_BINARY_ASSIGN_OP(+=);
    SELF_ITERATOR_BINARY_ASSIGN_OP(-=);

#define SELF_ITERATOR_BINARY_OP(OP)                                                                                                                            \
  constexpr auto operator OP(Value val) noexcept                                                                                                               \
  {                                                                                                                                                            \
    return SelfIterator<Value>(value OP val);                                                                                                                  \
  }

    SELF_ITERATOR_BINARY_OP(+);
    SELF_ITERATOR_BINARY_OP(-);

#define SELF_ITERATOR_COMPARE_OP(OP)                                                                                                                           \
  constexpr auto operator OP(SelfIterator<Value> const &other) noexcept                                                                                        \
  {                                                                                                                                                            \
    return value OP other.value;                                                                                                                               \
  }

    SELF_ITERATOR_COMPARE_OP(==);
    SELF_ITERATOR_COMPARE_OP(!=);
    SELF_ITERATOR_COMPARE_OP(<=);
    SELF_ITERATOR_COMPARE_OP(>=);
    SELF_ITERATOR_COMPARE_OP(<);
    SELF_ITERATOR_COMPARE_OP(>);

    constexpr auto operator-(SelfIterator<Value> const &other) const noexcept
    {
      return value - other.value;
    }

    constexpr auto operator*() const noexcept
    {
      return value;
    }

    constexpr auto operator[](Value index) const noexcept
    {
      return value + index;
    }
  };

  template<class Value>
  class Range
  {
    SelfIterator<Value> pBegin;
    SelfIterator<Value> pEnd;

  public:
    constexpr Range(Value begin, Value end) noexcept
      : pBegin(begin)
      , pEnd(end)
    {}

    constexpr auto const &begin() const noexcept
    {
      return pBegin;
    }

    constexpr auto const &end() const noexcept
    {
      return pEnd;
    }
  };
}
