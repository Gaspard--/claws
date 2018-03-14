#pragma once

#include <array>

namespace claws
{
/// Still experimental, don't hesitate to add anything missing
/// Todo: noexcept specification on unary operators
#define ARRAY_OPERATOR_DEF(OP)                                                                                                                                 \
  template<class T, class U, std::size_t dim>                                                                                                                  \
  constexpr std::array<T, dim> &operator OP##=(std::array<T, dim> &lh,                                                                                         \
                                               std::array<U, dim> const &rh) noexcept(noexcept(std::declval<T &>() OP## = std::declval<U const &>()))          \
  {                                                                                                                                                            \
    for (size_t i(0u); i != dim; ++i)                                                                                                                          \
      lh[i] OP## = rh[i];                                                                                                                                      \
    return lh;                                                                                                                                                 \
  };                                                                                                                                                           \
                                                                                                                                                               \
  template<class T, class U, std::size_t dim>                                                                                                                  \
  constexpr auto operator OP(std::array<T, dim> lh, std::array<U, dim> const &rh) noexcept(                                                                    \
    noexcept(std::declval<std::array<T, dim> &>() OP## = std::declval<std::array<U, dim> const &>()))                                                          \
  {                                                                                                                                                            \
    lh OP## = rh;                                                                                                                                              \
    return lh;                                                                                                                                                 \
  };

#define SCALAR_ARRAY_OPERATOR_DEF(OP)                                                                                                                          \
  template<class T, class U, std::size_t dim>                                                                                                                  \
  constexpr std::array<T, dim> &operator OP##=(std::array<T, dim> &lh, U const &rh) noexcept(noexcept(std::declval<T &>() OP## = std::declval<U const &>()))   \
  {                                                                                                                                                            \
    for (auto &elem : lh)                                                                                                                                      \
      elem OP## = rh;                                                                                                                                          \
    return lh;                                                                                                                                                 \
  };                                                                                                                                                           \
                                                                                                                                                               \
  template<class T, class U, std::size_t dim>                                                                                                                  \
  constexpr auto operator OP(std::array<T, dim> lh, U const &rh) noexcept(noexcept(std::declval<std::array<T, dim> &>() OP## = std::declval<U const &>()))     \
  {                                                                                                                                                            \
    return lh OP## = rh;                                                                                                                                       \
  };

#define ARRAY_UNARY_OP_DEF(OP)                                                                                                                                 \
  template<class T, std::size_t dim>                                                                                                                           \
  constexpr std::array<T, dim> operator OP(std::array<T, dim> array)                                                                                           \
  {                                                                                                                                                            \
    for (auto &t : array)                                                                                                                                      \
      t = OP t;                                                                                                                                                \
    return array;                                                                                                                                              \
  }

  template<class Mapper, class T, std::size_t dim, std::size_t... indices>
  constexpr auto map(Mapper mapper, std::array<T, dim> const &src, std::index_sequence<indices...>)
  {
    return std::array<decltype(mapper(std::declval<T const &>())), dim>{mapper(src[indices])...};
  }

  template<class Mapper, class T, std::size_t dim>
  constexpr auto map(Mapper mapper, std::array<T, dim> const &src)
  {
    return map(mapper, src, std::make_index_sequence<dim>{});
  }

  template<class T, std::size_t dim>
  constexpr auto scalar(std::array<T, dim> const &lh, std::array<T, dim> const &rh) noexcept
  {
    T result{};

    for (std::size_t i(0u); i < dim; ++i)
      result += lh[i] * rh[i];
    return result;
  }

  template<class T, std::size_t dim>
  constexpr auto length2(std::array<T, dim> const &val) noexcept
  {
    T result{};

    for (auto const &elem : val)
      result += elem * elem;
    return result;
  }

  template<class T, std::size_t dim>
  constexpr auto equals(std::array<T, dim> const &lh, std::array<T, dim> const &rh) noexcept
  {
    for (std::size_t i(0u); i < dim; ++i)
      if (lh[i] != rh[i])
        return false;
    return true;
  }

  namespace arrayOps
  {
    ARRAY_OPERATOR_DEF(+);
    ARRAY_OPERATOR_DEF(-);
    ARRAY_UNARY_OP_DEF(+);
    ARRAY_UNARY_OP_DEF(-);

    ARRAY_OPERATOR_DEF(*);
    ARRAY_OPERATOR_DEF(/);
    ARRAY_OPERATOR_DEF(%);

    ARRAY_OPERATOR_DEF (^);
    ARRAY_OPERATOR_DEF(&);
    ARRAY_OPERATOR_DEF(|);
    ARRAY_OPERATOR_DEF(<<);
    ARRAY_OPERATOR_DEF(>>);
    ARRAY_UNARY_OP_DEF(~);
  }

  namespace scalarArrayOps
  {
    SCALAR_ARRAY_OPERATOR_DEF(+);
    SCALAR_ARRAY_OPERATOR_DEF(-);

    SCALAR_ARRAY_OPERATOR_DEF(*);
    SCALAR_ARRAY_OPERATOR_DEF(/);
    SCALAR_ARRAY_OPERATOR_DEF(%);

    SCALAR_ARRAY_OPERATOR_DEF (^);
    SCALAR_ARRAY_OPERATOR_DEF(&);
    SCALAR_ARRAY_OPERATOR_DEF(|);
    SCALAR_ARRAY_OPERATOR_DEF(<<);
    SCALAR_ARRAY_OPERATOR_DEF(>>);
  }
}
