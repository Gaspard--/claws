#pragma once

namespace claws
{
  template<class... T, class Func>
  void for_each_tuple_element(std::tuple<T...> &tuple, Func func)
  {
    (func(std::get<T>(tuple)), ...);
  }
};
