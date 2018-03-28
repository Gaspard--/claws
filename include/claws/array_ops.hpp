#pragma once

#include <array>

namespace claws
{
/// \defgroup array_ops Array operations
/// @{
/// \brief  Provides array operations

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

#define ARRAY_UNARY_OP_DEF(OP)						\
  template<class T, std::size_t dim>					\
  constexpr std::array<T, dim> operator OP(std::array<T, dim> const &array) noexcept(noexcept(OP std::declval<T>())) \
  {									\
    return map([](T const &t) noexcept(noexcept(OP std::declval<T>()))	\
	       {							\
		 return OP t;						\
	       }, array);						\
  }

  template<class Mapper, class T, std::size_t dim, std::size_t... indices>
  constexpr auto map(Mapper mapper, std::array<T, dim> const &src, std::index_sequence<indices...>) noexcept(noexcept(std::declval<Mapper>()(std::declval<T const &>())))
  {
    return std::array<decltype(mapper(std::declval<T const &>())), dim>{mapper(src[indices])...};
  }

  template<class Mapper, class T, std::size_t dim>
  constexpr auto map(Mapper mapper, std::array<T, dim> const &src) noexcept(noexcept(std::declval<Mapper>()(std::declval<T const &>())))
  {
    return map(mapper, src, std::make_index_sequence<dim>{});
  }

  template<class T, std::size_t dim>
  constexpr auto scalar(std::array<T, dim> const &lh, std::array<T, dim> const &rh) noexcept(std::is_nothrow_constructible_v<T> && noexcept(std::declval<T &>() += std::declval<T const &>() * std::declval<T const &>()))
  {
    T result{};

    for (std::size_t i(0u); i < dim; ++i)
      result += lh[i] * rh[i];
    return result;
  }

  template<class T, std::size_t dim>
  constexpr auto length2(std::array<T, dim> const &val) noexcept(std::is_nothrow_constructible_v<T> && noexcept(std::declval<T &>() += std::declval<T const &>() * std::declval<T const &>()))
  {
    T result{};

    for (auto const &elem : val)
      result += elem * elem;
    return result;
  }

  template<class T, std::size_t dim>
  constexpr auto equals(std::array<T, dim> const &lh, std::array<T, dim> const &rh) noexcept(noexcept(std::declval<T &>() != std::declval<T &>()))
  {
    for (std::size_t i(0u); i < dim; ++i)
      if (lh[i] != rh[i])
        return false;
    return true;
  }

  ///
  /// \brief Provides per-component array operators
  ///
  /// For a given binary operator \f$ \circ \f$, the following holds:
  /// - \f$
  /// A \, \circ \!\! = B \iff \forall x: A_x \, \circ \!\! = B_x
  /// \f$
  /// - \f$
  /// out = A \circ B \iff \forall x: out_x = A_x \circ B_x
  /// \f$
  ///
  /// Defined binary operators are `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|`, `<<`, `>>`
  /// and all their corresponding side-effect versions.
  ///
  /// For a given unary operator \f$ \circ \f$:
  ///
  /// - \f$
  /// out = \circ A \iff \forall x: out_x = \circ A_x
  /// \f$
  ///
  /// Defined unary operators are `+`, `-`, `~`, and `!`.
  ///
  /// All operators are constexpr and have strict noexcept specification
  ///
  namespace array_ops
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

    ARRAY_UNARY_OP_DEF(!);
  }

  ///
  /// \brief Provides per-component scalar on array operators
  ///
  /// For a given binary operator \f$ \circ \f$, the following holds:
  /// - \f$
  /// array \, \circ \!\! = scalar \iff \forall x: array_x \, \circ \!\! = scalar
  /// \f$
  /// - \f$
  /// out = array \circ scalar \iff \forall x: out_x = array_x \circ scalar
  /// \f$
  ///
  /// Defined binary operators are `+`, `-`, `*`, `/`, `%`, `^`, `&`, `|`, `<<`, `>>`
  /// and all their corresponding side-effect versions.
  ///
  /// All operators are constexpr and have strict noexcept specification
  ///
  namespace scalar_array_ops
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

  /// @}
}
