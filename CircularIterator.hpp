#pragma once

namespace claws {
  template<class Iterator>
  class CircularIterator : private Iterator
  {
    Iterator begin;
    Iterator end;

  public:
    constexpr CircularIterator<Iterator>(Iterator begin,
					 Iterator end,
					 Iterator pos)
      : Iterator(pos),
	begin(begin),
	end(end)
    {
    }

    constexpr CircularIterator<Iterator>(Iterator begin,
					 Iterator end)
      : CircularIterator<Iterator>(begin, end, begin)
    {}

    constexpr CircularIterator<Iterator> operator++()
    {
      ++static_cast<Iterator &>(*this);
      if (static_cast<Iterator &>(*this) == end)
	static_cast<Iterator &>(*this) = begin;
      return *this;
    }

    using Iterator::operator*;
    using Iterator::operator->;

    template<class Other>
    constexpr bool operator==(Other const &other) const
    {
      return static_cast<Iterator const &>(*this) == other;
    }

    template<class Other>
    constexpr bool operator!=(Other const &other) const
    {
      return static_cast<Iterator const &>(*this) != other;
    }

    constexpr operator Iterator() const noexcept
    {
      return static_cast<Iterator>(*this);
    }
  };

  template<class Container>
  constexpr auto getCircularIterator(Container &container)
  {
    return CircularIterator<decltype(container.begin())>(container.begin(), container.end());
  }
}
