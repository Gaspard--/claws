#pragma once

namespace claws
{
  template<class input_it, class output_it>
  constexpr output_it copy(input_it begin, input_it end, output_it out)
  {
    while (begin != end)
      {
        *out = *begin;
        ++begin;
        ++out;
      }
    return out;
  }
};
