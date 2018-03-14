#pragma once

namespace claws
{
  template<class InputIt, class OutputIt>
  constexpr OutputIt copy(InputIt begin, InputIt end, OutputIt out)
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
