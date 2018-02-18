#pragma once

#include <tuple>

namespace claws
{
  template<template<class...> class DestTemplate, class Src>
  class CopyParamPack
  {
    template<class... T,  template<class...> class SrcTemplate>
    static DestTemplate<T...> convert(SrcTemplate<T...>);

  public:
    ~CopyParamPack() = delete;
    using Type = decltype(convert(std::declval<Src>()));
  };

  template<template<class...> class DestTemplate, class Src>
  using CopyParamPack_T = typename CopyParamPack<DestTemplate, Src>::Type;
  
  template<class Src>
  using ToTuple = CopyParamPack<std::tuple, Src>;

  template<class Src>
  using ToTuple_T = typename ToTuple<Src>::Type;
}
