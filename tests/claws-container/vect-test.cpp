#include <gtest/gtest.h>
#include <type_traits>
#include <claws/container/vect.hpp>

TEST(vect, member_types)
{
  using vec = claws::vect<int, 2>;

  static_assert(std::is_same_v<typename vec::value_type, int>);
  static_assert(std::is_same_v<typename vec::reference, int &>);
  static_assert(std::is_same_v<typename vec::const_reference, const int &>);
  static_assert(std::is_same_v<typename vec::pointer, int *>);
  static_assert(std::is_same_v<typename vec::const_pointer, const int *>);
  static_assert(std::is_same_v<typename vec::const_pointer, const int *>);
}

// Array iterators are pointers, so they cannot be constexpr for stack-local variables.
// However, they are for static variables.
static constexpr claws::vect<int, 2> static_vect{{1, 2}};

TEST(vect, constexpr_correctness)
{
  constexpr claws::vect<int, 2> v1{{1, 2}};

  static_assert(v1[0] == 1);
  static_assert(v1[1] == 2);

  static_assert(v1.x() == 1);
  static_assert(v1.y() == 2);

  static_assert(v1.size() == 2);
  static_assert(v1.max_size() == 2);
  static_assert(!v1.empty());

  constexpr auto it = static_vect.begin();
  static_assert(*it == 1);

  static_assert(static_vect == static_vect);
  static_assert(!(static_vect < static_vect));
  static_assert(!(static_vect > static_vect));
  static_assert(static_vect <= static_vect);
  static_assert(static_vect >= static_vect);
}

template<typename T, typename U>
static constexpr const bool check_same_noexceptness = std::is_nothrow_destructible_v<
                                                        T> == std::is_nothrow_destructible_v<U> &&std::is_nothrow_move_constructible_v<T> == std::is_nothrow_move_constructible_v<U> &&std::is_nothrow_copy_constructible_v<T> == std::is_nothrow_copy_constructible_v<U> &&std::is_nothrow_move_assignable_v<T> == std::is_nothrow_move_assignable_v<U> &&std::is_nothrow_copy_assignable_v<T> == std::is_nothrow_copy_assignable_v<U>;

namespace
{
  class throw_copy_constructible
  {
  public:
    throw_copy_constructible(const throw_copy_constructible &) noexcept(false)
    {}
  };
}

TEST(vect, noexcept_correctness)
{
  static_assert(check_same_noexceptness<int, claws::vect<int, 3>>);
  static_assert(check_same_noexceptness<throw_copy_constructible, claws::vect<throw_copy_constructible, 3>>);
}

TEST(vect, construction)
{
  constexpr int array[] = {5, 4, 3, 2, 1};
  constexpr claws::vect v1(array);
  constexpr claws::vect v2(v1);
  constexpr claws::vect<long, 5> v3(v2);
  constexpr claws::vect<int, 3> v4;
  static_assert(v4[0] == 0);
  static_assert(v4[1] == 0);
  static_assert(v4[2] == 0);
  constexpr claws::vect<int, 4> vec5(1, 2, 3, 4);
  constexpr claws::vect<int, 4> vec6{1, 2, 3, 4};
}

TEST(vect, copy)
{
  constexpr claws::vect<int, 5> from{{1, 2, 3, 4, 5}};
  claws::vect<int, 5> to(from);

  ASSERT_EQ(to, from);
}

TEST(vect, move)
{
  claws::vect<int, 5> from{{1, 2, 3, 4, 5}};
  claws::vect<int, 5> to(std::move(from));

  ASSERT_EQ(to, (claws::vect<int, 5>{{1, 2, 3, 4, 5}}));
}

namespace
{
  class CastableToInt
  {
  public:
    constexpr CastableToInt(int value)
      : _value(value)
    {}

    explicit constexpr operator int() const noexcept
    {
      return _value;
    }

  private:
    int _value;
  };
}

TEST(vect, cast)
{
  constexpr claws::vect<CastableToInt, 3> vec1{{CastableToInt(1), CastableToInt(2), CastableToInt(3)}};
  constexpr auto vec2 = claws::vect_cast<int>(vec1);

  static_assert(vec2[0] == 1);
  static_assert(vec2[1] == 2);
  static_assert(vec2[2] == 3);
}

TEST(vect, sum)
{
  constexpr claws::vect<int, 5> vec{{1, 2, 3, 4, 5}};

  static_assert(vec.sum() == 15);
}

TEST(vect, unary_operators)
{
  constexpr claws::vect<int, 4> vec{{1, 2, 3, 4}};
  constexpr auto vec_opposite = -vec;

  static_assert(vec_opposite == claws::vect{{-1, -2, -3, -4}});
}
