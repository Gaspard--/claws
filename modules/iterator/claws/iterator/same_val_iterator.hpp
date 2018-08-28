#pragma once

#include <iterator>
#include <type_traits>

namespace claws
{
  template<class T, class tag = std::input_iterator_tag>
  class same_val_iterator
  {
    static_assert(std::is_same_v<tag, std::input_iterator_tag>, "Given iterator tag hasn't been implemented");

    T t;

  public:
    using difference_type = std::size_t;
    using value_type = T;
    using pointer = T const *;
    using reference = T const &;
    using iterator_category = std::input_iterator_tag;

    constexpr same_val_iterator(T const &t) noexcept(std::is_nothrow_copy_constructible<T>())
      : t(t)
    {}

    constexpr same_val_iterator() = default;

    constexpr same_val_iterator(same_val_iterator const &) = default;

    constexpr same_val_iterator(same_val_iterator &&) = default;

    constexpr same_val_iterator &operator=(same_val_iterator const &) = default;

    constexpr same_val_iterator &operator=(same_val_iterator &&) = default;

    constexpr auto &operator*() const noexcept
    {
      return t;
    }

    constexpr auto &operator[](std::size_t) const noexcept
    {
      return t;
    }

    constexpr auto &operator++() noexcept
    {
      return *this;
    }

    constexpr auto operator++(int) noexcept(std::is_nothrow_copy_constructible<T>())
    {
      return *this;
    }

    constexpr bool operator==(same_val_iterator const &) noexcept
    {
      return true;
    }
  };
}
