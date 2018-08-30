#pragma once

#include <type_traits>
#include <utility>

namespace claws
{
  template<typename T>
  inline constexpr bool is_boxable_v = not std::is_class_v<T>;

  template<typename T>
  struct is_boxable : std::bool_constant<is_boxable_v<T>>
  {};

  ///
  /// \brief Makes a primitive look like a class.
  ///
  /// \tparam _type the primitive type. Specifically asserted to be primitive.
  ///
  /// `claws::box` is fully `constexpr` and `noexcept`
  ///
  template<class _type>
  class box
  {
  public:
    /// stored type
    using type = _type;

    static_assert(is_boxable_v<type>, "box should store a primitive, not a class");

  private:
    type _value{};

  public:
    /// \name Contructors and assignement operators for box.
    /// All declared `noexcept` and `constexpr`.
    /// @{

    /// Primitive is default initialised. (IE worth 0, or nullptr etc.)
    constexpr box() = default;
    constexpr box(box const &) = default;
    constexpr box(box &&) = default;
    constexpr box &operator=(box const &) = default;
    constexpr box &operator=(box &&) = default;

    constexpr box(type &&value) noexcept
      : _value{std::move(value)}
    {}

    constexpr box(type const &value) noexcept
      : _value{value}
    {}
    /// @}

    ~box() = default;

    /// \name User defined conversions
    /// All declared `noexcept` and `constexpr`.
    /// Casting `claws::box<type>` to `type` is possible.
    /// @{
    constexpr operator type const &() const & noexcept
    {
      return _value;
    }

    constexpr operator type &() & noexcept
    {
      return _value;
    }
    /// @}
  };
}
