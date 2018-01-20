#include <array>

/// Still experimental, don't hesitate to add anything missing
/// Todo: noexcept specification on unary operators
namespace arrayOps
{
#define ARRAY_OPERATOR_DEF(OP)						\
  template<class T, class U, std::size_t dim>				\
  constexpr std::array<T, dim>& operator OP##=(std::array<T, dim> &lh, std::array<U, dim> const &rh) noexcept(noexcept(std::declval<T &>() OP##= std::declval<U>())) \
  {									\
    for (size_t i(0u); i != dim; ++i)					\
      lh[i] OP##= rh[i];						\
    return lh;								\
  };									\
									\
  									\
  template<class T, class U, std::size_t dim>				\
  constexpr std::array<T, dim>& operator OP##=(std::array<T, dim> &lh, U const &rh) noexcept(noexcept(std::declval<T &>() OP##= std::declval<U>())) \
  {									\
    for (auto &elem : lh)						\
      elem OP##= rh;							\
    return lh;								\
  };									\
									\
  template<class T, class U, std::size_t dim>				\
  constexpr auto operator OP(std::array<T, dim> lh, U const &rh)  noexcept(noexcept(std::declval<std::array<T, dim> &>() OP##= std::declval<U>())) \
  {									\
    lh OP##= rh;							\
    return lh;								\
  };									\

  ARRAY_OPERATOR_DEF(+);
  ARRAY_OPERATOR_DEF(-);
  ARRAY_OPERATOR_DEF(*);
  ARRAY_OPERATOR_DEF(/);
  ARRAY_OPERATOR_DEF(%);
  ARRAY_OPERATOR_DEF(^);
  ARRAY_OPERATOR_DEF(&);
  ARRAY_OPERATOR_DEF(|);
  ARRAY_OPERATOR_DEF(<<);
  ARRAY_OPERATOR_DEF(>>);

  template<class Mapper, class T, std::size_t dim, std::size_t... indices>
  constexpr std::array<T, dim> map(Mapper mapper, std::array<T, dim> const &src, std::index_sequence<indices...>)
  {
    return {mapper(src[indices])...};
  }

#define ARRAY_UNARY_OP_DEF(OP)						\
  template<class T, std::size_t dim>					\
  constexpr std::array<T, dim> operator OP(std::array<T, dim> const &array)	\
  {									\
    struct Mapper							\
    {									\
      constexpr T operator()(T const &t)				\
      {									\
	return OP t;							\
      }									\
    };									\
    return map(Mapper{}, array, std::make_index_sequence<dim>());	\
  }									\

  ARRAY_UNARY_OP_DEF(-);
  ARRAY_UNARY_OP_DEF(+);
  ARRAY_UNARY_OP_DEF(~);
  ARRAY_UNARY_OP_DEF(!);
}
