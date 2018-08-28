#pragma once

#include <tuple>

namespace claws
{
  namespace tuple_helper
  {
    ///
    /// \brief Allows iteration on `std::tuple`s.
    ///
    template<class... T, class func_type>
    constexpr void for_each_tuple_element(std::tuple<T...> &tuple, func_type func)
    {
      (func(std::get<T>(tuple)), ...);
    }
  }
};
