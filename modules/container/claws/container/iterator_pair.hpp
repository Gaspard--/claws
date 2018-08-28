#pragma once

#include <type_traits>

namespace claws
{
  template<class it_type, class end_type>
  struct iterator_pair
  {
    it_type _begin;
    end_type _end;

    constexpr auto begin() const noexcept(std::is_nothrow_copy_constructible_v<it_type>)
    {
      return _begin;
    }

    constexpr auto end() const noexcept(std::is_nothrow_copy_constructible_v<end_type>)
    {
      return _end;
    }

    constexpr auto size() const noexcept(noexcept(std::declval<iterator_pair>().end() - std::declval<iterator_pair>().begin()))
    {
      return end() - begin();
    }
  };
}
