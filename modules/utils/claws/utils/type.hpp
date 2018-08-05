#pragma once

#include <tuple>

namespace claws
{
  /// \defgroup type Type support
  /// @{
  /// \brief Provides type support

  /// \brief Provides a member `type` which is `dest_template` with the template parameters of `src`
  ///
  /// @tparam dest_template the destintation variadic template
  /// @tparam src a specialisation of a variadic template (such as std::tuple<int, double, float>)
  ///
  template<template<class...> class dest_template, class src>
  class copy_param_pack
  {
    template<class... T, template<class...> class src_template>
    static dest_template<T...> convert(src_template<T...>);

  public:
    ~copy_param_pack() = delete;

    /// type is `dest_template<...>` with the template parameters of `src`
    using type = decltype(convert(std::declval<src>()));
  };

  /// convenience typedef, see `claws::copy_param_pack::type`
  template<template<class...> class dest_template, class src>
  using copy_param_pack_t = typename copy_param_pack<dest_template, src>::type;

  /// convenience typedef, see `claws::copy_param_pack`
  template<class src>
  using to_tuple = copy_param_pack<std::tuple, src>;

  /// convenience typedef, see `claws::to_tuple`
  template<class src>
  using to_tuple_t = typename to_tuple<src>::type;

  ///
  /// @}
  ///
}
