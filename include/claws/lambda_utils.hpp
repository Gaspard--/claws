#pragma once

#include <type_traits>

namespace claws
{
  /// \brief template to help with recursive lambdas
  ///
  /// The basic problem is that a lambda can't refer to itself.
  /// This class takes a lambda as parameter, and it's `operator()` injects
  /// the class itself into the lambda as first parameter,
  /// thus allowing the lambda to call the first of it's parameters to call itself
  /// This class is usable with any functor, even though the readablity gain only realy applies to lambdas
  ///
  /// \tparam functor_type the funtor's type
  template<class functor_type>
  struct inject_self
  {
    functor_type functor;

    inject_self() = default;
    inject_self(inject_self const &) = default;
    inject_self(inject_self &) = default;
    inject_self& operator=(inject_self const &) = default;
    inject_self& operator=(inject_self &) = default;

    inject_self(functor_type const &functor) noexcept(std::is_nothrow_copy_constructible_v<functor_type>)
      : functor(functor)
    {
    }

    inject_self(functor_type &&functor) noexcept(std::is_nothrow_move_constructible_v<functor_type>)
      : functor(std::move(functor))
    {
    }

    template<class... param_types>
    decltype(auto) operator()(param_types &&... params)
    {
      return functor(*this, std::forward<param_types>(params)...);
    }
  };
};
