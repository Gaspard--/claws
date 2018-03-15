#pragma once

#include <optional>

namespace claws
{
  template<class Func>
  auto on_scope_exit(Func func)
  {
    struct ret
    {
      std::optional<Func> func;

      ret(Func func)
        : func(std::make_optional(func))
      {}

      ret(ret const &ret) = delete;

      ret(ret &&ret)
        : func(ret.func)
      {
        ret.func.reset();
      };

      ~ret()
      {
        if (func)
          (*func)();
      }
    };

    return ret{func};
  }
};
