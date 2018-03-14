#pragma once

#include <optional>

namespace claws
{
  template<class Func>
  auto onScopeExit(Func func)
  {
    struct Ret
    {
      std::optional<Func> func;

      Ret(Func func)
        : func(std::make_optional(func))
      {}

      Ret(Ret const &ret) = delete;

      Ret(Ret &&ret)
        : func(ret.func)
      {
        ret.func.reset();
      };

      ~Ret()
      {
        if (func)
          (*func)();
      }
    };

    return Ret{func};
  }
};
