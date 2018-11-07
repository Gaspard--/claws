#pragma once

#include <math.h>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <iterator>
#include <claws/algorithm/constexpr_algorithm.hpp>

namespace claws
{
  template<typename T, std::size_t Size>
  class vect;

  namespace impl
  {
    template<typename T, size_t Size, typename Transformer, size_t... indexes>
    constexpr inline auto vect_transform_impl(vect<T, Size> const &from, Transformer &&transf, std::index_sequence<indexes...>)
    {
      return vect{transf(from[indexes])...};
    };
  }

  template<typename T, size_t Size, typename Transformer>
  constexpr inline auto vect_transform(vect<T, Size> const &from, Transformer &&transf) noexcept
  {
    return impl::vect_transform_impl(from, std::forward<Transformer>(transf), std::make_index_sequence<Size>{});
  }

  template<typename To, typename T, size_t Size>
  constexpr inline vect<To, Size> vect_cast(vect<T, Size> const &from) noexcept
  {
    return vect_transform(from, [](auto const &value) constexpr { return static_cast<To>(value); });
  }

  template<typename T, std::size_t Size>
  class vect
  {
  public:
    using value_type = T;
    using reference = T &;
    using const_reference = T const &;
    using pointer = T *;
    using const_pointer = T const *;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

  private:
    value_type array[Size];

    template<size_t... indexes>
    constexpr vect(value_type const (&arr)[Size], std::index_sequence<indexes...>) noexcept
      : array{arr[indexes]...}
    {}

    template<typename Iterator, size_t... indexes>
    constexpr vect(Iterator it, std::index_sequence<indexes...>) noexcept
      : array{((void)indexes, *it++)...}
    {}

  public:
    constexpr vect() noexcept
      : array{}
    {}

    constexpr vect(value_type const (&arr)[Size]) noexcept
      : vect(arr, std::make_index_sequence<Size>{})
    {}

    template<typename... U, typename = std::enable_if_t<sizeof...(U) == Size>>
    constexpr vect(U &&... u) noexcept
      : array{std::forward<U>(u)...}
    {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    constexpr vect(vect<U, Size> const &other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
      : vect(other.begin(), std::make_index_sequence<Size>{})
    {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    constexpr vect(vect<U, Size> &&other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
      : vect(std::make_move_iterator(other.begin()), std::make_index_sequence<Size>{})
    {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    constexpr vect &operator=(vect<U, Size> const &other) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
    {
      claws::copy(other.begin(), other.end(), begin());
      return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    constexpr vect &operator=(vect<U, Size> &&other) noexcept(std::is_nothrow_move_assignable_v<value_type>)
    {
      claws::move(other.begin(), other.end(), begin());
      return *this;
    }

    constexpr iterator begin() noexcept
    {
      return std::begin(array);
    }

    constexpr iterator end() noexcept
    {
      return std::end(array);
    }

    constexpr const_iterator cbegin() const noexcept
    {
      return std::cbegin(array);
    }

    constexpr const_iterator cend() const noexcept
    {
      return std::cend(array);
    }

    constexpr const_iterator begin() const noexcept
    {
      return cbegin();
    }

    constexpr const_iterator end() const noexcept
    {
      return cend();
    }

    constexpr reverse_iterator rbegin() noexcept
    {
      return std::reverse_iterator(end());
    }

    constexpr reverse_iterator rend() noexcept
    {
      return std::reverse_iterator(begin());
    }

    constexpr const_reverse_iterator crbegin() const noexcept
    {
      return std::reverse_iterator(cend());
    }

    constexpr const_reverse_iterator crend() const noexcept
    {
      return std::reverse_iterator(cbegin());
    }

    constexpr const_reverse_iterator rbegin() const noexcept
    {
      return crbegin();
    }

    constexpr const_reverse_iterator rend() const noexcept
    {
      return crend();
    }

    constexpr size_type size() const noexcept
    {
      return Size;
    }

    constexpr size_type max_size() const noexcept
    {
      return Size;
    }

    constexpr bool empty() const noexcept
    {
      return size() == 0;
    }

    constexpr const_reference operator[](size_t idx) const noexcept
    {
      return array[idx];
    }

    constexpr reference operator[](size_t idx) noexcept
    {
      return array[idx];
    }

#define CLAWS_VECT_OPERATOR_DEF(OP)                                   \
  template<typename U>                                                \
  constexpr vect<T, Size> &operator OP##=(vect<U, Size> const &other) \
  {                                                                   \
    for (std::size_t i = 0u; i < Size; ++i)                           \
      array[i] OP## = other[i];                                       \
    return (*this);                                                   \
  }                                                                   \
                                                                      \
  template<typename U>                                                \
  constexpr auto operator OP(vect<U, Size> const &other) const        \
  {                                                                   \
    vect<decltype(array[0] OP other[0]), Size> result{*this};         \
                                                                      \
    result OP## = other;                                              \
    return result;                                                    \
  }                                                                   \
                                                                      \
  template<typename U>                                                \
  constexpr vect<T, Size> &operator OP##=(U const &other)             \
  {                                                                   \
    for (auto &elem : *this)                                          \
      elem OP## = other;                                              \
    return *this;                                                     \
  }                                                                   \
                                                                      \
  template<typename U>                                                \
  constexpr auto operator OP(U const &other) const                    \
  {                                                                   \
    vect<decltype(array[0] OP other), Size> result{*this};            \
                                                                      \
    result OP## = other;                                              \
    return result;                                                    \
  }

    CLAWS_VECT_OPERATOR_DEF(+);

    CLAWS_VECT_OPERATOR_DEF(-);

    CLAWS_VECT_OPERATOR_DEF(*);

    CLAWS_VECT_OPERATOR_DEF(/);

    CLAWS_VECT_OPERATOR_DEF(%);

    CLAWS_VECT_OPERATOR_DEF (^);

    CLAWS_VECT_OPERATOR_DEF(|);

    CLAWS_VECT_OPERATOR_DEF(&);

#undef CLAWS_VECT_OPERATOR_DEF

    template<typename U>
    constexpr bool operator==(vect<U, Size> const &other) const noexcept
    {
      size_t i = 0;

      for (; i < Size && array[i] == other.array[i]; ++i)
        ;
      return i == Size;
    }

    template<typename U>
    constexpr bool operator!=(vect<U, Size> const &other) const noexcept
    {
      return !(*this == other);
    }

#define CLAWS_VECT_ORDER_OPERATOR_DEF(OP)                               \
  template<typename U>                                                  \
  constexpr bool operator OP(const vect<U, Size> &other) const noexcept \
  {                                                                     \
    auto it1 = begin();                                                 \
    auto it2 = other.begin();                                           \
                                                                        \
    for (; it1 != end(); ++it1, ++it2)                                  \
      {                                                                 \
        if (*it1 OP * it2)                                              \
          return true;                                                  \
        if (*it2 OP * it1)                                              \
          return false;                                                 \
      }                                                                 \
    return false;                                                       \
  }

    CLAWS_VECT_ORDER_OPERATOR_DEF(<);
    CLAWS_VECT_ORDER_OPERATOR_DEF(<=);
    CLAWS_VECT_ORDER_OPERATOR_DEF(>);
    CLAWS_VECT_ORDER_OPERATOR_DEF(>=);

#undef CLAWS_VECT_ORDER_OPERATOR_DEF

  private:
    template<typename U, typename Pred, size_t... indexes>
    constexpr auto compare_impl(vect<U, Size> const &other, Pred &&pred, std::index_sequence<indexes...>) const noexcept
    {
      return vect<bool, Size>{pred((*this)[indexes], other[indexes])...};
    };

  public:
    template<typename U, typename Pred>
    constexpr vect<bool, Size> compare(vect<U, Size> const &other, Pred &&pred) const noexcept
    {
      return compare_impl(other, std::forward<Pred>(pred), std::make_index_sequence<Size>{});
    }

#define CLAWS_VECT_ORDER_COMPARATOR_DEF(OP, NAME)                                         \
  template<typename U>                                                                    \
  constexpr vect<bool, Size> is_##NAME(vect<U, Size> const &other) const noexcept         \
  {                                                                                       \
    return compare(other, [](const auto &a, const auto &b) constexpr { return a OP b; }); \
  }

    CLAWS_VECT_ORDER_COMPARATOR_DEF(==, equal);

    CLAWS_VECT_ORDER_COMPARATOR_DEF(!=, not_equal);

    CLAWS_VECT_ORDER_COMPARATOR_DEF(<, less);

    CLAWS_VECT_ORDER_COMPARATOR_DEF(<=, less_or_equal);

    CLAWS_VECT_ORDER_COMPARATOR_DEF(>, greater);

    CLAWS_VECT_ORDER_COMPARATOR_DEF(>=, greater_or_equal);

#undef CLAWS_VECT_ORDER_COMPARATOR_DEF

#define CLAWS_VECT_UNARY_OP_DEF(OP)                                       \
  constexpr vect<T, Size> operator OP() const                             \
  {                                                                       \
    return vect_transform(*this, [](auto const &cur) { return OP cur; }); \
  }

    CLAWS_VECT_UNARY_OP_DEF(-);

    CLAWS_VECT_UNARY_OP_DEF(+);

    CLAWS_VECT_UNARY_OP_DEF(~);

    CLAWS_VECT_UNARY_OP_DEF(!);

#undef CLAWS_VECT_UNARY_OP_DEF

#define CLAWS_VECT_NAMED_COMPONENT(NAME, INDEX)                               \
  template<size_t _Size = Size, typename = std::enable_if_t<(_Size > INDEX)>> \
  constexpr const_reference NAME() const noexcept                             \
  {                                                                           \
    return array[INDEX];                                                      \
  }                                                                           \
                                                                              \
  template<size_t _Size = Size, typename = std::enable_if_t<(_Size > INDEX)>> \
  constexpr reference NAME() noexcept                                         \
  {                                                                           \
    return array[INDEX];                                                      \
  }

    CLAWS_VECT_NAMED_COMPONENT(x, 0);

    CLAWS_VECT_NAMED_COMPONENT(y, 1);

    CLAWS_VECT_NAMED_COMPONENT(z, 2);

    CLAWS_VECT_NAMED_COMPONENT(w, 3);

#undef CLAWS_VECT_NAMED_COMPONENT

    constexpr value_type sum() const noexcept
    {
      value_type result{0u};

      for (auto const &t : *this)
        result += t;
      return result;
    }

    constexpr value_type scalar(vect<T, Size> const &other) const noexcept
    {
      return (*this * other).sum();
    }

    constexpr value_type length2() const noexcept
    {
      return (*this * *this).sum();
    }

    vect<T, Size> normalized() const noexcept
    {
      return length2() > 0 ? (*this) / sqrt(length2()) : *this;
    }

    constexpr bool all() const noexcept
    {
      size_t i = 0;

      while (i < Size && array[i])
        i = i + 1;
      return i == Size;
    }
  };

  template<typename T, typename... Ts>
  vect(T &&t, Ts &&... ts)->vect<T, 1 + sizeof...(Ts)>;
}
