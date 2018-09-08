#pragma once

#include <type_traits>
#include <utility>

namespace claws
{
  /// \brief template to help with recursive lambdas
  ///
  /// The basic problem is that a lambda cannot refer to itself.
  /// This class takes a lambda as parameter, and its `operator()` injects
  /// the class itself into the lambda as first parameter,
  /// thus allowing the lambda to call the first of it's parameters to call itself
  /// This class is usable with any functor, even though the readability gain only really applies to lambdas
  ///
  /// \tparam functor_type the functor's type
  template<class functor_type>
  class inject_self
  {
  private:
    functor_type wrapped_functor;

  public:
    inject_self() = default;
    inject_self(inject_self const &) = default;
    inject_self(inject_self &&) = default;
    inject_self &operator=(inject_self const &) = default;
    inject_self &operator=(inject_self &&) = default;

    constexpr inject_self(functor_type const &functor) noexcept(std::is_nothrow_copy_constructible_v<functor_type>)
      : wrapped_functor(functor)
    {}

    constexpr inject_self(functor_type &&functor) noexcept(std::is_nothrow_move_constructible_v<functor_type>)
      : wrapped_functor(std::move(functor))
    {}

    template<class... param_types>
    constexpr decltype(auto) operator()(param_types &&... params) const
    {
      return wrapped_functor(*this, std::forward<param_types>(params)...);
    }

    template<class... param_types>
    decltype(auto) operator()(param_types &&... params)
    {
      return wrapped_functor(*this, std::forward<param_types>(params)...);
    }
  };
};
