#pragma once

#include <math.h>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <algorithm>

namespace claws
{
  template<typename T, std::size_t Size>
  class vect;

  namespace details
  {
    template<typename T, size_t Size, typename Transformer, size_t... indexes>
    constexpr inline auto vect_transform_impl(const vect<T, Size> &from, Transformer &&transf, std::index_sequence<indexes...>)
    {
      return vect{transf(from[indexes])...};
    };
  }

  template<typename T, size_t Size, typename Transformer>
  constexpr inline auto vect_transform(const vect<T, Size> &from, Transformer &&transf) noexcept
  {
    return details::vect_transform_impl(from, std::forward<Transformer>(transf), std::make_index_sequence<Size>{});
  }

  template<typename To, typename T, size_t Size>
  constexpr inline vect<To, Size> vect_cast(const vect<T, Size> &from) noexcept
  {
    return vect_transform(from, [](const auto &value) { return static_cast<To>(value); });
  }

  template<typename T, std::size_t Size>
  class vect
  {
  public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

  private:
    value_type array[Size];

    template<size_t... indexes>
    constexpr vect(const value_type (&arr)[Size], std::index_sequence<indexes...>) noexcept
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

    constexpr vect(const value_type (&arr)[Size]) noexcept
      : vect(arr, std::make_index_sequence<Size>{})
    {}

    template<typename... U, typename = std::enable_if_t<sizeof...(U) == Size>>
    constexpr vect(U &&... u) noexcept
      : array{std::forward<U>(u)...}
    {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    constexpr vect(const vect<U, Size> &other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
      : vect(other.begin(), std::make_index_sequence<Size>{})
    {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    constexpr vect(vect<U, Size> &&other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
      : vect(std::make_move_iterator(other.begin()), std::make_index_sequence<Size>{})
    {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    vect &operator=(const vect<U, Size> &other) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
    {
      std::copy(other.begin(), other.end(), begin());
      return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
    vect &operator=(vect<U, Size> &&other) noexcept(std::is_nothrow_move_assignable_v<value_type>)
    {
      std::move(other.begin(), other.end(), begin());
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
  constexpr auto operator OP(const vect<U, Size> &other) const        \
  {                                                                   \
    vect<decltype(array[0] OP other[0]), Size> result{*this};         \
                                                                      \
    result OP## = other;                                              \
    return result;                                                    \
  }                                                                   \
                                                                      \
  template<typename U>                                                \
  constexpr vect<T, Size> &operator OP##=(const U &other)             \
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
    constexpr bool operator==(const vect<U, Size> &other) const noexcept
    {
      size_t i = 0;

      for (; i < Size && array[i] == other.array[i]; ++i)
        ;
      return i == Size;
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

#define CLAWS_VECT_UNARY_OP_DEF(OP)                                       \
  constexpr vect<T, Size> operator OP() const                             \
  {                                                                       \
    return vect_transform(*this, [](const auto &cur) { return OP cur; }); \
  }

    CLAWS_VECT_UNARY_OP_DEF(-);

    CLAWS_VECT_UNARY_OP_DEF(+);

    CLAWS_VECT_UNARY_OP_DEF(~);

    CLAWS_VECT_UNARY_OP_DEF(!);

#undef CLAWS_VECT_UNARY_OP_DEF

#define CLAWS_VECT_NAMED_COMPONENT(NAME, INDEX)                           \
  template<size_t _Size = Size, typename = std::enable_if_t<(_Size > 0)>> \
  constexpr const_reference NAME() const noexcept                         \
  {                                                                       \
    return array[INDEX];                                                  \
  }                                                                       \
                                                                          \
  template<size_t _Size = Size, typename = std::enable_if_t<(_Size > 0)>> \
  constexpr reference NAME() noexcept                                     \
  {                                                                       \
    return array[INDEX];                                                  \
  }

    CLAWS_VECT_NAMED_COMPONENT(x, 0);

    CLAWS_VECT_NAMED_COMPONENT(y, 1);

    CLAWS_VECT_NAMED_COMPONENT(z, 2);

    CLAWS_VECT_NAMED_COMPONENT(w, 3);

#undef CLAWS_VECT_NAMED_COMPONENT

    constexpr value_type sum() const noexcept
    {
      value_type result{0u};

      for (const auto &t : *this)
        result += t;
      return result;
    }

    constexpr value_type scalar(const vect<T, Size> &other) const noexcept
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
