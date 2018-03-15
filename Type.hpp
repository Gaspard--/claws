#pragma once

#include <tuple>

namespace claws
{
  template<template<class...> class dest_template, class src>
  class copy_param_pack
  {
    template<class... T, template<class...> class src_template>
    static dest_template<T...> convert(src_template<T...>);

  public:
    ~copy_param_pack() = delete;
    using type = decltype(convert(std::declval<src>()));
  };

  template<template<class...> class dest_template, class src>
  using copy_param_pack_t = typename copy_param_pack<dest_template, src>::type;

  template<class src>
  using to_tuple = copy_param_pack<std::tuple, src>;

  template<class src>
  using to_tuple_t = typename to_tuple<src>::type;
}
