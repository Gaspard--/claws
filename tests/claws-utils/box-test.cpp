#include <type_traits>
#include <gtest/gtest.h>
#include <claws/utils/box.hpp>

TEST(box, type_traits)
{
  using int_box = claws::box<int>;
  static_assert(std::is_class_v<int_box>);
  static_assert(std::is_same_v<typename int_box::type, int>);
}

TEST(box, constexpr_correctness)
{
  constexpr claws::box<int> i{2};
  static_assert(i == 2);
  constexpr claws::box<int> i2 = i;
  static_assert(i2 == 2);
  constexpr claws::box<int> i3 = std::move(i);
  static_assert(i3 == 2);
  constexpr int value = i3;
  static_assert(value == 2);
}

TEST(box, noexcept_correctness)
{
  using int_box = claws::box<int>;
  static_assert(noexcept(int_box()));
  static_assert(noexcept(int_box(std::declval<const int_box &>())));
  static_assert(noexcept(int_box(std::declval<int_box &&>())));

  static_assert(noexcept(std::declval<int_box &>() = std::declval<const int_box &>()));
  static_assert(noexcept(std::declval<int_box &>() = std::declval<int_box &&>()));

  static_assert(noexcept(int_box(std::declval<const int &>())));
  static_assert(noexcept(int_box(std::declval<int &&>())));

  static_assert(noexcept(static_cast<const int &>(std::declval<const int_box &>())));
  static_assert(noexcept(static_cast<int &>(std::declval<int_box &>())));
}
