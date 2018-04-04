#pragma once

namespace claws
{
  /// \defgroup lambda_ops Lambda operations
  /// @{
  /// \brief provides lambda overloading and composition through operators

  ///
  /// \brief provides lambda overloading and composition through operators
  ///
  namespace lambda_ops
  {
    ///
    /// Create a lambda with 2 `operator()` inherited from both lambda params.
    /// \tparam op1 first lambda type
    /// \tparam op2 second lambda type
    ///
    template<class op1, class op2>
    struct overload : private op1, private op2
    {
      ///
      /// \brief Constructs a lambda storing both lamdas' data.
      ///
      constexpr overload(op1 lh, op2 rh)
        : op1(lh)
        , op2(rh)
      {}
      using op1::operator();
      using op2::operator();
    };

    ///
    /// \brief Constructs a lambda storing both lamdas' data, and overloading the `operator()`
    ///
    template<class op1, class op2>
    constexpr auto operator+(op1 lh, op2 rh)
    {
      return overload<op1, op2>{lh, rh};
    }

    ///
    /// Composes two lambdas together.
    /// \tparam op1 first lambda type
    /// \tparam op2 second lambda type
    ///
    template<class op1, class op2>
    struct composition : private op1, private op2
    {
      ///
      /// \brief Constructs a lambda equivalent to *x -> op1(op2(x))*
      ///
      constexpr composition(op1 lh, op2 rh)
        : op1(lh)
        , op2(rh)
      {}

      template<class... param_types>
      constexpr auto operator()(param_types &&... params)
      {
        return op1::operator()(op2::operator()(std::forward<param_types>(params)...));
      }

      template<class... param_types>
      constexpr auto operator()(param_types &&... params) const
      {
        return op1::operator()(op2::operator()(std::forward<param_types>(params)...));
      }
    };

    ///
    /// \brief Returns a lambda equivalent to *x -> op1(op2(x))*
    ///
    template<class op1, class op2>
    constexpr auto operator*(op1 lh, op2 rh)
    {
      return composition<op1, op2>{lh, rh};
    }
  }

  /// @}
};
