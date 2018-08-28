#pragma once

#include <iterator>

namespace claws
{
  template<class raw_it_type, class func_type>
  class iterator_view
  {
    raw_it_type it;
    func_type func;

  public:
    using difference_type = typename std::iterator_traits<raw_it_type>::difference_type;
    using reference = decltype(func(*it));
    using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;
    using pointer = std::remove_reference_t<reference> *;
    using iterator_category = typename std::iterator_traits<raw_it_type>::iterator_category;

    iterator_view() = default;

    iterator_view(iterator_view const &) = default;

    iterator_view(iterator_view &&) = default;

    iterator_view &operator=(iterator_view const &) = default;

    iterator_view &operator=(iterator_view &&) = default;

    constexpr iterator_view(raw_it_type const &it,
                            func_type const &func) noexcept(std::is_nothrow_copy_constructible_v<raw_it_type> &&std::is_nothrow_copy_constructible_v<func_type>)
      : it(it)
      , func(func)
    {}

    constexpr iterator_view &operator++() noexcept(noexcept(++it))
    {
      ++it;
      return *this;
    }

    constexpr iterator_view &operator--() noexcept(noexcept(--it))
    {
      --it;
      return *this;
    }

    constexpr iterator_view operator++(int) noexcept(noexcept(++it) && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>

    )
    {
      auto tmp(*this);
      ++it;
      return tmp;
    }

    constexpr iterator_view operator--(int) noexcept(noexcept(--it) && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>

    )
    {
      auto tmp(*this);
      --it;
      return tmp;
    }

#define CONTAINER_VIEW_ITERATOR_OP(OP)                                                                                       \
  constexpr iterator_view &operator OP##=(difference_type val) noexcept(noexcept(it OP## = std::declval<difference_type>())) \
  {                                                                                                                          \
    it OP## = val;                                                                                                           \
    return *this;                                                                                                            \
  };                                                                                                                         \
                                                                                                                             \
  constexpr iterator_view operator OP(difference_type val)                                                                   \
    const noexcept(noexcept(std::declval<iterator_view<raw_it_type, func_type>>() OP## = std::declval<difference_type>())    \
                   && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>)                           \
  {                                                                                                                          \
    auto tmp(*this);                                                                                                         \
                                                                                                                             \
    tmp OP## = val;                                                                                                          \
    return tmp;                                                                                                              \
  };

    CONTAINER_VIEW_ITERATOR_OP(-);

    CONTAINER_VIEW_ITERATOR_OP(+);

    constexpr difference_type operator-(iterator_view const &other) const noexcept(noexcept(std::declval<raw_it_type>() - std::declval<difference_type>()))
    {
      return it - other.it;
    };

    constexpr reference operator*() const noexcept(noexcept(func(*it)))
    {
      return func(*it);
    }

    template<class index_type>
    constexpr reference operator[](index_type index) const noexcept(noexcept(func(*it)))
    {
      return func(*(it + index));
    }

    /// \todo add noexcept specification
    constexpr decltype(auto) operator-> () const
    {
      struct self_pointer_like
      {
        value_type data;

        auto operator-> ()
        {
          return &data;
        }
      };
      return self_pointer_like{func(*it)};
    }

#define CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(OP)                                                          \
  constexpr bool operator OP(iterator_view other) const noexcept(noexcept(it OP std::declval<raw_it_type>())) \
  {                                                                                                           \
    return it OP other.it;                                                                                    \
  }

    /// \name comparision operators
    /// comparaisions are inherited from
    /// @{
    CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(==);

    CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(!=);

    CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(<=);

    CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(>=);

    CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(<);

    CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(>);
    /// @}
  };
}
