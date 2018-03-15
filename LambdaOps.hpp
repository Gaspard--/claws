#pragma once

namespace claws
{
  namespace lambda_ops
  {
    template<class op1, class op2>
    struct overload : private op1, private op2
    {
      constexpr overload(op1 lh, op2 rh)
        : op1(lh)
        , op2(rh)
      {}
      using op1::operator();
      using op2::operator();
    };

    template<class op1, class op2>
    constexpr auto operator+(op1 lh, op2 rh)
    {
      return overload<op1, op2>{lh, rh};
    }

    template<class op1, class op2>
    struct composition : private op1, private op2
    {
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

    template<class op1, class op2>
    constexpr auto operator*(op1 lh, op2 rh)
    {
      return composition<op1, op2>{lh, rh};
    }
  }
};
