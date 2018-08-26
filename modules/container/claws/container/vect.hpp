#pragma once

#include <type_traits>
#include <utility>
#include <math.h>
#include <cstddef>

namespace claws
{
  /// Deprecated since c++17, use std::array and util/ArrayOps.hpp instead (by using arraOps)
  /// This existed because before c++17 std::array::operator[] wasn't constexpr.
  template<std::size_t dim, class T>
  class vect
  {
  private:
    T data[dim];

    template<class V, std::size_t... indices>
    constexpr vect(V const &other, std::index_sequence<indices...>)
      : data{static_cast<T>(other[indices])...}
    {}

    template<std::size_t... indices>
    constexpr vect(std::index_sequence<indices...>)
      : vect(((void)indices, T{})...)
    {}

    template<class func, std::size_t... indices>
    static auto apply_op(func &&f, std::index_sequence<indices...>)
    {
      return vect{f(indices)...};
    }

  public:
    template<class func>
    static vect<dim, T> apply_op(func &&f)
    {
      return apply_op(f, std::make_index_sequence<dim>{});
    }

    template<class V>
    constexpr vect(V const (&other)[dim])
      : vect(other, std::make_index_sequence<dim>{})
    {}

    template<class V>
    constexpr vect(vect<dim, V> const &other)
      : vect(other, std::make_index_sequence<dim>{})
    {}

    constexpr vect()
      : vect(std::make_index_sequence<dim>{})
    {}

    template<class... U, typename std::enable_if<sizeof...(U) == dim>::type * = nullptr>
    constexpr vect(U &&... ts)
      : data{std::forward<U>(ts)...}
    {}

    constexpr T *begin()
    {
      return data;
    }

    constexpr T *end()
    {
      return data + dim;
    }

    constexpr T const *begin() const
    {
      return data;
    }

    constexpr T const *end() const
    {
      return data + dim;
    }

    template<class operation>
    void apply_on_self(operation op)
    {
      for (unsigned int i(0); i < dim; ++i)
        data[i] = op(i);
    }

    constexpr T &operator[](std::size_t index)
    {
      return (data[index]);
    }

    constexpr T const &operator[](std::size_t index) const
    {
      return (data[index]);
    }

#define VECT_OPERATOR_DEF(OP)                                       \
  template<class U>                                                 \
  constexpr vect<dim, T> &operator OP##=(vect<dim, U> const &other) \
  {                                                                 \
    for (std::size_t i(0u); i != dim; ++i)                          \
      data[i] OP## = other[i];                                      \
    return (*this);                                                 \
  };                                                                \
                                                                    \
  template<class U>                                                 \
  constexpr auto operator OP(vect<dim, U> const &other) const       \
  {                                                                 \
    vect<dim, decltype(data[0] OP other[0])> result{*this};         \
                                                                    \
    result OP## = other;                                            \
    return result;                                                  \
  };                                                                \
                                                                    \
  template<class U>                                                 \
  constexpr vect<dim, T> &operator OP##=(U const &other)            \
  {                                                                 \
    for (auto &elem : *this)                                        \
      elem OP## = other;                                            \
    return (*this);                                                 \
  };                                                                \
                                                                    \
  template<class U>                                                 \
  constexpr auto operator OP(U const &other) const                  \
  {                                                                 \
    vect<dim, decltype(data[0] OP other)> result{*this};            \
                                                                    \
    result OP## = other;                                            \
    return result;                                                  \
  }

    VECT_OPERATOR_DEF(+);
    VECT_OPERATOR_DEF(-);
    VECT_OPERATOR_DEF(*);
    VECT_OPERATOR_DEF(/);
    VECT_OPERATOR_DEF(%);
    VECT_OPERATOR_DEF (^);
    VECT_OPERATOR_DEF(|);
    VECT_OPERATOR_DEF(&);

    // TODO optimize ?
    constexpr bool equals(vect<dim, T> const &other) const
    {
      unsigned int i(0);

      for (; i != dim && data[i] == other.data[i]; ++i)
        ;
      return i == dim;
    }

  private:
    template<class FUNC, size_t... indices>
    constexpr vect<dim, T> map_impl(FUNC &&func, std::index_sequence<indices...>) const
    {
      return {func(data[indices])...};
    }

  public:
    template<class FUNC>
    constexpr vect<dim, T> map(FUNC &&func) const
    {
      return (map_impl(func, std::make_index_sequence<dim>{}));
    }

#define VECT_UNARY_OP_DEF(OP)                    \
  constexpr vect<dim, T> operator OP(void) const \
  {                                              \
    struct mapper                                \
    {                                            \
      T operator()(T const &t)                   \
      {                                          \
        return OP t;                             \
      }                                          \
    };                                           \
    return map(mapper{});                        \
  }

    VECT_UNARY_OP_DEF(-);
    VECT_UNARY_OP_DEF(+);
    VECT_UNARY_OP_DEF(~);
    VECT_UNARY_OP_DEF(!);

  public:
    constexpr T sum(void) const
    {
      T result{0u};

      for (auto const &t : *this)
        result += t;
      return (result);
    }

    constexpr T scalar(vect<dim, T> const &other) const
    {
      return ((*this * other).sum());
    }

    constexpr T length2() const
    {
      return ((*this * *this).sum());
    }

    vect<dim, T> normalized() const
    {
      return length2() > 0 ? ((*this) / sqrt(length2())) : *this;
    }

#define VECT_NAMED_COMPONENT(NAME, INDEX)                                                  \
  template<unsigned int _dim = dim, typename std::enable_if<(_dim > 0)>::type * = nullptr> \
  T NAME() const                                                                           \
  {                                                                                        \
    return (data[INDEX]);                                                                  \
  }                                                                                        \
                                                                                           \
  template<unsigned int _dim = dim, typename std::enable_if<(_dim > 0)>::type * = nullptr> \
  T &NAME()                                                                                \
  {                                                                                        \
    return (data[INDEX]);                                                                  \
  }

    VECT_NAMED_COMPONENT(x, 0);
    VECT_NAMED_COMPONENT(y, 1);
    VECT_NAMED_COMPONENT(z, 2);
    VECT_NAMED_COMPONENT(w, 3);

    constexpr bool all() const
    {
      unsigned int i(0);

      while (i != dim && data[i])
        i = i + 1;
      return (i == dim);
    }
  };
}
