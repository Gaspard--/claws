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
    using reference = decltype(func(*it));
    using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;
    using pointer = std::remove_reference_t<reference> *;
    using iterator_category = typename std::iterator_traits<raw_it_type>::iterator_category;

    iterator_view() = default;
    iterator_view(iterator_view const &) = default;
    iterator_view(iterator_view &&) = default;
    iterator_view& operator=(iterator_view const &) = default;
    iterator_view& operator=(iterator_view &&) = default;

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

#define CONTAINER_VIEW_ITERATOR_OP(OP)					\
    constexpr iterator_view &operator OP##=(difference_type val) noexcept(noexcept(it OP## = std::declval<difference_type>())) \
    {									\
      it OP## = val;							\
      return *this;							\
    };									\
									\
    constexpr iterator_view operator OP(difference_type val)		\
      const noexcept(noexcept(std::declval<iterator_view<raw_it_type, func_type>>() OP## = std::declval<difference_type>()) \
		     && std::is_nothrow_copy_constructible_v<iterator_view<raw_it_type, func_type>>) \
    {									\
      auto tmp(*this);							\
									\
      tmp OP## = val;							\
      return tmp;							\
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

#define CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(OP)			\
    constexpr bool operator OP(iterator_view other) const noexcept(noexcept(it OP std::declval<raw_it_type>()))	\
    {									\
      return it OP other.it;						\
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

  ///
  /// \brief a class providing an easy way to work with a mapped view of a container
  ///
  /// \snippetlineno main.cpp Example usage
  ///
  template<class container_type, class func_type>
  class container_view
  {
    container_type container;
    func_type _func;

  public:
    /// \name constructors and assignement operators
    ///
    /// Strict noexcept, and constexpr specifiers.
    /// @{

    ///
    /// \brief contructs a container view
    ///
    /// This is the constructor that should fit most use cases.
    /// Thanks to deduction guidelines, it's possible to write
    /// ```cpp
    /// for (auto value : container_view(begin, end, func))
    /// ```
    /// which is really useful. It may be necessary to qualify the auto properly depending on use-cases.
    ///
    template<class it_type, class end_type>
    constexpr container_view(it_type const &begin, end_type const &end, func_type const &func)
    : container{begin, end}
      , _func(func)
    {}

    ///
    /// \brief contructs a container view
    ///
    /// This is the constructor that should fit most use cases. Not that the container isn't stored, only it's begin(), and end().
    /// If these get invalidated, the view will be too.
    /// Thanks to the added deduction guidelines, it's possible to write
    /// ```cpp
    /// for (auto value : container_view(container, func))
    /// ```
    /// which is really useful. It may be necessary to qualify the auto properly depending on use-cases.
    ///
    template<class constructor_container_type>
    constexpr container_view(constructor_container_type &container, func_type const &func)
      : container_view(container.begin(), container.end(), func)
    {}

    ///
    /// \brief contructs a container view
    ///
    /// This constructor stores the recieved container. This gives the useer more flexibillity.
    /// The parameter will be moved from.
    /// Thanks to the added deduction guidelines, it's possible to write
    /// ```cpp
    /// for (auto value : container_view(create_container(), func))
    /// ```
    /// which is really useful. It may be necessary to qualify the auto properly depending on use-cases.
    ///
    constexpr container_view(container_type &&container, func_type const &func)
    : container(std::move(container))
      , _func(func)
    {}

    constexpr container_view() = default;
    constexpr container_view(container_view const &) = default;
    constexpr container_view(container_view &&) = default;
    constexpr container_view &operator=(container_view const &) = default;
    constexpr container_view &operator=(container_view &&) = default;
    /// @}

    template<class index_type>
    constexpr decltype(auto) operator[](index_type index)
    {
      return begin()[index];
    }

    template<class index_type>
    constexpr decltype(auto) operator[](index_type index) const
    {
      return begin()[index];
    }

    constexpr auto size() const
    {
      return container.size();
    }

    /// \name iterators to the view
    ///
    /// Return `iterator_view`s constructed with func and the corresponding iterator.
    /// @{
    constexpr auto begin() const noexcept(noexcept(iterator_view(container.begin(), _func)))
    {
      return iterator_view(container.begin(), _func);
    }

    constexpr auto end() const noexcept(noexcept(iterator_view(container.end(), _func)))
    {
      return iterator_view(container.end(), _func);
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


  // Deduction guideline for second constructor
  template<class container_type, class func_type>
  container_view(container_type &container, func_type const &func)
    -> container_view<iterator_pair<decltype(container.begin()), decltype(container.begin())>, func_type>;

  template<class container_type, class func_type>
  container_view(container_type &&container, func_type const &func)
    -> container_view<container_type, func_type>;

  template<class it_type, class end_type, class func_type>
  container_view(it_type const &begin, end_type const &end, func_type const &func)
    -> container_view<iterator_pair<it_type, end_type>, func_type>;

}
