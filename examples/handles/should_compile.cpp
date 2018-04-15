#include <iostream>

#include "claws/HandleTypes.hpp"
#include "claws/Box.hpp"

inline void forget_example()
{
  /// [Forget about ownership]
  // Simple deleter:
  auto deleter{[](int *a) {
    std::cout << "deleting " << a << std::endl;
    delete a;
  }};
  claws::handle a(std::move(deleter), new int(15));
  int *raw = claws::handle<int *, claws::no_delete>(claws::handle<int *, claws::forget>(std::move(a))); // Forget about ownership
  claws::handle(std::move(deleter), std::move(raw));                                                    // Re-acquire (and therefor delete)
  /// [Forget about ownership]
}

inline void move_ownership_example()
{
  /// [Move ownership]
  // Simple deleter:
  auto deleter{[](int *a) {
    std::cout << "deleting " << a << std::endl;
    delete a;
  }};
  claws::handle a(std::move(deleter), new int(15));
  claws::handle b(std::move(a)); // owning handles can be moved
  /// [Move ownership]
}

inline void move_to_other_deleter_example()
{
  /// [Move ownership to other deleter]
  // Simple deleter:
  auto deleter1{[](int *a) {
    std::cout << "deleter 1 is deleting " << a << std::endl;
    delete a;
  }};
  auto deleter2{[](int *a) {
    std::cout << "deleter 2 is deleting " << a << std::endl;
    delete a;
  }};
  claws::handle a(std::move(deleter1), new int(15));
  claws::handle alt_deleter(std::move(deleter2), std::move(a));
  static_assert(std::is_same_v<decltype(alt_deleter)::type, int *>, "Wrong deduced type.");
  /// [Move ownership to other deleter]
}

inline void copy_example()
{
  /// [Copy to non-owning handle]
  // Simple deleter:
  auto deleter{[](int *a) {
    std::cout << "deleting " << a << std::endl;
    delete a;
  }};
  claws::handle a(std::move(deleter), new int(15));
  claws::handle<int *, claws::no_delete> non_owning(a);           // non-owning handles can copy
  claws::handle<int *, claws::no_delete> non_owning2(non_owning); // non-owning handles can copy from non-owning
  /// [Copy to non-owning handle]
}

int main()
{
  auto deleter{[](int *a) {
    std::cout << "deleting " << a << std::endl;
    delete a;
  }};
  claws::handle a(std::move(deleter), new int(15));
  claws::handle b(std::move(a));                                  // owning handles can be moved
  claws::handle<int *, claws::no_delete> non_owning(b);           // non-owning handles can copy
  claws::handle<int *, claws::no_delete> non_owning2(non_owning); // non-owning handles can copy
  auto deleter2{[](int *a) { delete a; }};
  claws::handle<int *, decltype(deleter2)> alt_deleter(std::move(deleter2));
  alt_deleter = std::move(b); // owning handles can be move-assigned, as long as the deleter isn't claws::no_delete
  struct deleter3
  {
    deleter3() = default;

    void operator()(int *a)
    {
      std::cout << "deleting " << a << std::endl;
      delete a;
    }
  };

  claws::handle<int *, deleter3> alt_deleter2(std::move(alt_deleter)); // owning handle can move-construct another handle
  forget_example();
  move_ownership_example();
  move_to_other_deleter_example();
  copy_example();
}
