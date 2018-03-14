#pragma once

namespace claws
{
  template<class Data, class Container, class Context>
  struct ContextfulContainer
  {
    Context context;
    Container container;

    // public:
    //   template<class ...U>
    //   ContextfulContainer(Context context, U &&... u)
    //     : context(context)
    //     , container(std::forward<U>(u)...)
    //   {
    //   }

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
