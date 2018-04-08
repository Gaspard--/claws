#pragma once

namespace claws
{
  template<class raw_it_type, class func_type>
  class iterator_view
  {
    raw_it_type it;
    func_type func;

  public:
    using difference_type = typename std::iterator_traits<raw_it_type>::difference_type;
    using value_type = std::remove_cv_t<decltype(func(*it))>;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = typename std::iterator_traits<raw_it_type>::iterator_category;

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

    constexpr iterator_view operator++(int)noexcept(noexcept(++it) && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>)
    {
      auto tmp(*this);
      ++it;
      return tmp;
    }

    constexpr iterator_view operator--(int)noexcept(noexcept(--it) && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>)
    {
      auto tmp(*this);
      --it;
      return tmp;
    }

#define CONTAINER_VIEW_ITERATOR_OP(OP)                                                                                                                         \
  constexpr iterator_view &operator OP##=(difference_type val) noexcept(noexcept(it OP## = std::declval<difference_type>()))                                   \
  {                                                                                                                                                            \
    it OP## = val;                                                                                                                                             \
    return *this;                                                                                                                                              \
  };                                                                                                                                                           \
                                                                                                                                                               \
  constexpr iterator_view operator OP(difference_type val)                                                                                                     \
    const noexcept(noexcept(std::declval<iterator_view<raw_it_type, func_type>>() OP## = std::declval<difference_type>())                                      \
                   && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>)                                                             \
  {                                                                                                                                                            \
    auto tmp(*this);                                                                                                                                           \
                                                                                                                                                               \
    tmp OP## = val;                                                                                                                                            \
    return tmp;                                                                                                                                                \
  };

    CONTAINER_VIEW_ITERATOR_OP(-);
    CONTAINER_VIEW_ITERATOR_OP(+);

    constexpr difference_type operator-(iterator_view const &other) const noexcept(noexcept(std::declval<raw_it_type>() - std::declval<difference_type>()))
    {
      return it - other.it;
    };

    constexpr decltype(auto) operator*() const noexcept(noexcept(func(*it)))
    {
      return func(*it);
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

#define CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(OP)                                                                                                           \
  constexpr bool operator OP(iterator_view other) const noexcept(noexcept(it OP std::declval<raw_it_type>()))                                                  \
  {                                                                                                                                                            \
    return it OP other.it;                                                                                                                                     \
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

  template<class it_type, class end_type, class func_type>
  class container_view
  {
    it_type _begin;
    end_type _end;
    func_type _func;

    static constexpr bool constructors_are_noexcept{
      std::is_nothrow_copy_constructible_v<it_type> && std::is_nothrow_copy_constructible_v<end_type> && std::is_nothrow_copy_constructible_v<func_type>};

  public:
    /// \name constructors and assignement operators
    ///
    /// Strict noexcept, and constexpr specifiers.
    /// @{

    ///
    /// \brief contructs a container view
    ///
    /// This is the constructor that should fit most use cases.
    /// Thanks to deduction guidelines, it's possible to right
    /// ```cpp
    /// for (auto value : container_view(begin, end, func))
    /// ```
    /// which is really usefull. It may be necessary to qualify the auto properly depending on use-cases.
    ///
    constexpr container_view(it_type const &begin, end_type const &end, func_type const &func) noexcept(constructors_are_noexcept)
      : _begin(begin)
      , _end(end)
      , _func(func)
    {}

    constexpr container_view() = default;
    container_view(container_view const &) = default;
    container_view(container_view &&) = default;
    container_view &operator=(container_view const &) = default;
    container_view &operator=(container_view &&) = default;
    /// @}

    /// \name iterators to the view
    ///
    /// Return `iterator_view`s constructed with func and the corresponding iterator.
    /// @{
    constexpr auto begin() const noexcept(noexcept(iterator_view(_begin, _func)))
    {
      return iterator_view(_begin, _func);
    }

    constexpr auto end() const noexcept(noexcept(iterator_view(_end, _func)))
    {
      return iterator_view(_end, _func);
    }
    /// @}
  };

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

    constexpr auto operator++(int)noexcept(std::is_nothrow_copy_constructible<T>())
    {
      return *this;
    }

    constexpr bool operator==(same_val_iterator const &) noexcept
    {
      return true;
    }
  };
}
