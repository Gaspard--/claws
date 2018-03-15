#pragma once

namespace claws
{
  template<class iterator>
  class circular_iterator
  {
    iterator current;
    iterator begin;
    iterator end;

  public:
    constexpr circular_iterator<iterator>(iterator begin, iterator end, iterator pos)
      : current(pos)
      , begin(begin)
      , end(end)
    {}

    constexpr circular_iterator<iterator>(iterator begin, iterator end)
      : circular_iterator<iterator>(begin, end, begin)
    {}

    constexpr circular_iterator<iterator> operator++()
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

    template<class other_type>
    constexpr bool operator==(other_type const &other) const
    {
      return current == other;
    }

    template<class other_type>
    constexpr bool operator!=(other_type const &other) const
    {
      return current != other;
    }

    constexpr operator iterator() const noexcept
    {
      return current;
    }
  };

  template<class container_type>
  constexpr auto get_circular_iterator(container_type &container)
  {
    return circular_iterator<decltype(container.begin())>(container.begin(), container.end());
  }
}
