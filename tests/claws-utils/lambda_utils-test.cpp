#include <utility>
#include <type_traits>
#include <gtest/gtest.h>
#include <claws/utils/lambda_utils.hpp>

TEST(inject_self, recursion)
{
  constexpr auto repeat_step = [](auto &&step, int nb, auto &&call) -> void {
    if (nb > 0)
      {
        call();
        step(nb - 1, std::forward<decltype(call)>(call));
      }
  };
  constexpr claws::inject_self repeat = repeat_step;

  int counter = 0;
  repeat(3, [&counter]() { ++counter; });
  ASSERT_EQ(counter, 3);
}

TEST(inject_self, constexpr_correctness)
{
  constexpr auto do_nothing_step = [](auto &&step, int nb) -> void {
    if (nb > 0)
      {
        step(nb - 1);
      }
  };
  constexpr claws::inject_self do_nothing = do_nothing_step;
  constexpr claws::inject_self do_nothing2 = do_nothing;
  constexpr claws::inject_self do_nothing3 = std::move(do_nothing2);
  (void)do_nothing3;
}

namespace
{
  struct ThrowingFunctor
  {
    ThrowingFunctor(const ThrowingFunctor &);
    ThrowingFunctor(ThrowingFunctor &&);
    ThrowingFunctor &operator=(const ThrowingFunctor &);
    ThrowingFunctor &operator=(ThrowingFunctor &&);
    template<typename Callable>
    void operator()(Callable &&step, int nb) const;
  };
}

TEST(inject_self, noexcept_correctness)
{
  constexpr auto do_nothing_step = [](auto &&step, int nb) -> void {
    if (nb > 0)
      {
        step(nb - 1);
      }
  };
  using with_lambda_t = claws::inject_self<decltype(do_nothing_step)>;
  using with_funcptr_t = claws::inject_self<void (*)(void (*)(int), int)>;
  using with_throw_t = claws::inject_self<ThrowingFunctor>;

  static_assert(noexcept(with_funcptr_t()));

  static_assert(noexcept(with_lambda_t(std::declval<const with_lambda_t &>())));
  static_assert(noexcept(with_lambda_t(std::declval<with_lambda_t &&>())));
  static_assert(noexcept(std::declval<with_funcptr_t &>() = std::declval<const with_funcptr_t &>()));
  static_assert(noexcept(std::declval<with_funcptr_t &>() = std::declval<with_funcptr_t &&>()));

  static_assert(noexcept(with_lambda_t(std::declval<const with_lambda_t &>())));
  static_assert(noexcept(with_lambda_t(std::declval<with_lambda_t &&>())));

  static_assert(not noexcept(with_throw_t(std::declval<const with_throw_t &>())));
  static_assert(not noexcept(with_throw_t(std::declval<with_throw_t &&>())));
  static_assert(not noexcept(std::declval<with_throw_t &>() = std::declval<const with_throw_t &>()));
  static_assert(not noexcept(std::declval<with_throw_t &>() = std::declval<with_throw_t &&>()));

  static_assert(not noexcept(with_throw_t(std::declval<const ThrowingFunctor &>())));
  static_assert(not noexcept(with_throw_t(std::declval<ThrowingFunctor &&>())));
}
