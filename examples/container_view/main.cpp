#include <array>
#include <cassert>
#include "claws/iterator_util.hpp"

int main()
{
  /// [Example usage]
  std::array<int, 7u> data{{0, 1, 2, 3, 4, 5, 6}};
  size_t call_count(0);
  auto data_times_two(claws::container_view(data, [&call_count](int x) noexcept
					    {
					      ++call_count;
					      return 2 * x;
					    }));
  assert(data.size() == data_times_two.size());
  for (size_t i(0ul); i < data.size(); ++i)
    {
      assert(call_count == i); // values are computed when queried
      assert(data[i] * 2 == data_times_two[i]); // the returned is the function applied to the `i`th element
    }
  data_times_two[0]; // dummy access to show `call_count` increase
  assert(call_count == data_times_two.size() + 1ul); // container doesn't store result
  /// [Example usage]
}
