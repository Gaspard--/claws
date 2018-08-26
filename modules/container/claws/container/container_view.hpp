#pragma once

#include <utility>
#include <claws/iterator/iterator_pair.hpp>

namespace claws
{
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

  // Deduction guideline for second constructor
  template<class container_type, class func_type>
  container_view(container_type &container, func_type const &func)
    ->container_view<iterator_pair<decltype(container.begin()), decltype(container.begin())>, func_type>;

  template<class container_type, class func_type>
  container_view(container_type &&container, func_type const &func)->container_view<container_type, func_type>;

  template<class it_type, class end_type, class func_type>
  container_view(it_type const &begin, end_type const &end, func_type const &func)->container_view<iterator_pair<it_type, end_type>, func_type>;
}
