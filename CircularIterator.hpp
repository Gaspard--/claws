#pragma once

namespace claws
{
  template<class Iterator>
  class CircularIterator
  {
    Iterator current;
    Iterator begin;
    Iterator end;

  public:
    constexpr CircularIterator<Iterator>(Iterator begin, Iterator end, Iterator pos)
      : current(pos)
      , begin(begin)
      , end(end)
    {}

    constexpr CircularIterator<Iterator>(Iterator begin, Iterator end)
      : CircularIterator<Iterator>(begin, end, begin)
    {}

    constexpr CircularIterator<Iterator> operator++()
    {
      ++current;
      if (current == end)
        current = begin;
      return *this;
    }

    decltype(auto) operator*()
    {
      return *current;
    }

    decltype(auto) operator*() const
    {
      return *current;
    }

    decltype(auto) operator-> ()
    {
      return current;
    }

    decltype(auto) operator-> () const
    {
      return current;
    }

    template<class Other>
    constexpr bool operator==(Other const &other) const
    {
      return current == other;
    }

    template<class Other>
    constexpr bool operator!=(Other const &other) const
    {
      return current != other;
    }

    constexpr operator Iterator() const noexcept
    {
      return current;
    }
  };

  template<class Container>
  constexpr auto getCircularIterator(Container &container)
  {
    return CircularIterator<decltype(container.begin())>(container.begin(), container.end());
  }
}
