#pragma once

namespace claws
{
  namespace lambdaOps
  {
    template<class Op1, class Op2>
    struct Overload : private Op1, private Op2
    {
      Overload(Op1 lh, Op2 rh)
	: Op1(lh), Op2(rh)
      {
      }
      using Op1::operator();
      using Op2::operator();
    };

    template<class Op1, class Op2>
    auto operator+(Op1 lh, Op2 rh)
    {
      return Overload<Op1, Op2>{lh, rh};
    }

    template<class Op1, class Op2>
    struct Composition : private Op1, private Op2
    {
      Composition(Op1 lh, Op2 rh)
	: Op1(lh), Op2(lh)
      {
      }

      template<class... Params>
      auto operator()(Params &&... params)
      {
	return Op1::operator()(Op2::operator()(std::forward<Params>(params)...));
      }

      template<class... Params>
      auto operator()(Params &&... params) const
      {
	return Op1::operator()(Op2::operator()(std::forward<Params>(params)...));
      }
    };

    template<class Op1, class Op2>
    auto operator*(Op1 lh, Op2 rh)
    {
      return Composition<Op1, Op2>{lh, rh};
    }
  }
};
