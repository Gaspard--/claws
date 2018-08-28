#pragma once

#include <cstddef>

namespace claws
{
  template<class Data, class Container, class Context>
  struct contextful_container
  {
    Context context;
    Container container;

    auto operator[](std::size_t i)
    {
      return Data(container[i], context);
    }

    auto operator[](std::size_t i) const
    {
      return Data(container[i], context);
    }

    auto operator*()
    {
      return (*this)[0];
    }

    auto operator*() const
    {
      return (*this)[0];
    }

    auto operator-> () const
    {
      return &**this;
    }
  };
}
