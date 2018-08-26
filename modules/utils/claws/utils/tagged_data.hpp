#pragma once

namespace claws
{
  /// \brief typify a type, helping with type safety
  /// \tparam _data_type the stored type
  /// \tparam _offset_type used for offsets (operators `+=`, `+`, `-=`, `-`)
  /// \tparam _tag a struct used as tag
  ///
  /// Stores a `tagged_data::data_type` value.
  /// If `offset_type` is the same as `data_type` this behaves like an index
  /// If `offset_type` is not the same as `data_type` this behaves like a pointer
  /// This class can't be assigned a `data_type` by default,
  /// helping with accidental promotion
  /// It should be typically used to help not making mistakes when handling
  /// different indexes or pointers of the same underlying type.
  /// The underling type should be cheap to copy and assign,
  /// and never throw in such operations. (Ideally, a primitive type)
  ///
  template<class _data_type, class _offset_type, class _tag>
  struct tagged_data
  {
    /// the used as tag
    using tag = _tag;
    /// used for offsets (operators `+=`, `+`, `-=`, `-`)
    using offset_type = _offset_type;
    /// the stored type
    using data_type = _data_type;
    /// \brief the actual data
    /// access this when you need the actual value (typically inside your api)
    data_type data;

    constexpr tagged_data(tagged_data const &) noexcept = default;
    constexpr tagged_data &operator=(tagged_data const &) noexcept = default;

    /// Construtor is explicit, to avoid conversion
    explicit constexpr tagged_data(data_type data = 0u) noexcept
      : data(data)
    {}

#define CLAWS_TAGGED_DATA_PREFIX_OP(OP)                                                                                                                        \
  constexpr tagged_data &operator OP() noexcept                                                                                                                \
  {                                                                                                                                                            \
    OP data;                                                                                                                                                   \
    return *this;                                                                                                                                              \
  }

    CLAWS_TAGGED_DATA_PREFIX_OP(++);
    CLAWS_TAGGED_DATA_PREFIX_OP(--);

#define CLAWS_TAGGED_DATA_SUFFIX_OP(OP)                                                                                                                        \
  constexpr tagged_data operator OP(int) noexcept                                                                                                              \
  {                                                                                                                                                            \
    auto copy(*this);                                                                                                                                          \
    OP *this;                                                                                                                                                  \
    return copy;                                                                                                                                               \
  }

    CLAWS_TAGGED_DATA_SUFFIX_OP(++);
    CLAWS_TAGGED_DATA_SUFFIX_OP(--);

#define CLAWS_TAGGED_DATA_COMPARE(OP)                                                                                                                          \
  constexpr bool operator OP(tagged_data const &other) const noexcept                                                                                          \
  {                                                                                                                                                            \
    return data OP other.data;                                                                                                                                 \
  }

    CLAWS_TAGGED_DATA_COMPARE(==);
    CLAWS_TAGGED_DATA_COMPARE(!=);
    CLAWS_TAGGED_DATA_COMPARE(<=);
    CLAWS_TAGGED_DATA_COMPARE(>=);
    CLAWS_TAGGED_DATA_COMPARE(<);
    CLAWS_TAGGED_DATA_COMPARE(>);

#define CLAWS_TAGGED_DATA_BINARY_OP(OP)                                                                                                                        \
  constexpr auto operator OP(_offset_type const &other) const noexcept                                                                                         \
  {                                                                                                                                                            \
    return tagged_data(static_cast<data_type>(data OP other));                                                                                                 \
  }

    CLAWS_TAGGED_DATA_BINARY_OP(+);
    CLAWS_TAGGED_DATA_BINARY_OP(-);
  };
}
