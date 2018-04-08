#pragma once

namespace claws
{
  ///
  /// \brief Makes a primitive look like a class.
  ///
  /// \tparam _type the primitive type. Specifically asserted to be primtive.
  ///
  /// `claws::box` is fully `constexpr` and `noexcept`
  ///
  template<class _type>
  class box
  {
  public:
    /// stored type
    using type = _type;

    static_assert(!std::is_class_v<type>, "box should store a primitive, not a class");

  private:
    type value{};

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
      : value{std::move(value)}
    {}

    constexpr box(type const &value) noexcept
      : value{value}
    {}
    /// @}

    ~box() = default;

    /// \name User defined conversions
    /// All declared `noexcept` and `constexpr`.
    /// Casting `claws::box<type>` to `type` is possible.
    /// @{
    constexpr operator type const &() const &noexcept
    {
      return value;
    }

    constexpr operator type &() & noexcept
    {
      return value;
    }
    /// @}
  };
}
