#pragma once

namespace claws
{
  template<class... T, class Func>
  void forEachTupleElement(std::tuple<T...> &tuple, Func func)
  {
    (func(std::get<T>(tuple)), ...);
  }
};
