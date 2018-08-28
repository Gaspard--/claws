#pragma once

#include <utility>
#include <optional>

namespace claws
{
  ///
  /// \brief Guaranties a functor will be called once and only once.
  ///
  /// Contrary to `claws::on_scope_exit`, can be moved to change location of call, but is slightly less performant, and uses slightly more space.
  ///
  /// Can therefor also be empty.
  ///
  template<class func_type>
  class on_destruction
  {
    std::optional<func_type> func;

  public:
    /// Contructs empty `claws::on_destruction`
    on_destruction() = default;

    ///
    /// \brief constructs a `claws::on_destruction` containing `func`.
    ///
    /// @param func Moved, will be called once and only once
    ///
    on_destruction(func_type &&func) noexcept(std::optional(std::declval<func_type &&>()))
      : func(std::move(func))
    {}

    on_destruction(on_destruction const &) = delete;

    ///
    /// \brief Steals the contained functor from other.
    ///
    /// @param other will be left empty and won't call a functor on destruction
    ///
    /// Current object will call functor that param contained once, or do nothing if `other` was empty.
    ///
    on_destruction(on_destruction &&other) noexcept(noexcept(func_type(std::declval<func_type const &>())))
      : func(other.func)
    {
      other.func.reset();
    };

    /// Calls functor if handle isn't empty
    ~on_destruction() noexcept(noexcept(std::declval<func_type &>()()))
    {
      if (func)
        (*func)();
    }
  };

  ///
  /// \brief Guaranties a functor will be called before exiting the scope.
  ///
  /// For a more flexible call guaranty, see `claws::on_destruction`
  ///
  template<class func_type>
  class on_scope_exit
  {
    func_type func;

  public:
    ///
    /// \brief Sole contructor.
    ///
    /// @param func Moved, will be called on destruction
    ///
    on_scope_exit(func_type &&func) noexcept(std::is_nothrow_move_constructible_v<func_type>)
      : func(std::move(func))
    {}

    on_scope_exit(on_scope_exit const &) = delete;
    on_scope_exit(on_scope_exit &&) = delete;

    /// invokes the functor
    ~on_scope_exit() noexcept(noexcept(std::declval<func_type &>()()))
    {
      func();
    }
  };
};
