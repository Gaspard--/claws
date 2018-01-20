#pragma once

namespace claws
{
  template<class Func, class IT>
  constexpr auto containerView(IT begin, IT end, Func func)
  {

    struct ContainerView
    {
      struct iterator
      {
	IT it;
	Func func;

	constexpr iterator &operator++()
	{
	  ++it;
	  return *this;
	}

	constexpr iterator &operator--()
	{
	  --it;
	  return *this;
	}

	constexpr iterator operator++(int)
	{
	  auto tmp(*this);
	  ++it;
	  return tmp;
	}

	constexpr iterator operator--(int)
	{
	  auto tmp(*this);
	  --it;
	  return tmp;
	}

#define CONTAINER_VIEW_ITERATOR_OP(OP)				\
	constexpr iterator &operator OP##=(std::size_t val)	\
	{							\
	  it OP##= val;						\
	  return *this;						\
	};							\
								\
	constexpr iterator operator OP(std::size_t val) const	\
	{							\
	  auto tmp(*this);					\
								\
	  tmp OP##= val;					\
	  return tmp;						\
	};

	CONTAINER_VIEW_ITERATOR_OP(-);
	CONTAINER_VIEW_ITERATOR_OP(+);

	constexpr std::size_t operator-(iterator const &other)
	{
	  return it - other.it;
	};

	constexpr auto operator*() const
	{
	  return func(*it);
	}

	constexpr auto operator->() const
	{
	  struct SelfPointerLike
	  {
	    decltype(func(*it)) data;

	    auto operator->()
	    {
	      return &data;
	    }
	  };
	  return SelfPointerLike{func(*it)};
	}

#define CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(OP)		\
	constexpr bool operator OP(iterator other) const	\
	{							\
	  return it OP other.it;				\
	}

	CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(==);
	CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(!=);
	CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(<=);
	CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(>=);
	CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(<);
	CONTAINER_VIEW_ITERATOR_BINARY_PREDICATE(>);
      };

      IT begin2;
      IT end2;
      Func func;

      constexpr iterator begin() const
      {
	return iterator{begin2, func};
      }

      constexpr iterator end() const
      {
	return iterator{end2, func};
      }
    };

    return ContainerView{begin, end, func};
  }
}
