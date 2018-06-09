#pragma once

namespace claws
{
  template<class _DataType, class _OffestType, class _Tag>
  struct TaggedData
  {
    using Tag = _Tag;
    using DataType = _DataType;
    DataType data;

    constexpr TaggedData(TaggedData const &) noexcept = default;

    explicit constexpr TaggedData(DataType data = 0u) noexcept
      : data(data)
    {}

#define CLAWS_TAGGED_DATA_PREFIX_OP(OP)				\
    constexpr TaggedData &operator OP() noexcept		\
    {								\
      OP data;							\
      return *this;						\
    }

    CLAWS_TAGGED_DATA_PREFIX_OP(++);
    CLAWS_TAGGED_DATA_PREFIX_OP(--);

#define CLAWS_TAGGED_DATA_SUFFIX_OP(OP)				\
    constexpr TaggedData operator OP(int) noexcept		\
    {								\
      auto copy(*this);						\
      OP *this;							\
      return copy;						\
    }

    CLAWS_TAGGED_DATA_SUFFIX_OP(++);
    CLAWS_TAGGED_DATA_SUFFIX_OP(--);
    
#define CLAWS_TAGGED_DATA_COMPARE(OP)					\
    constexpr bool operator OP(TaggedData const &other) const noexcept	\
    {									\
      return data OP other.data;					\
    }
    
    CLAWS_TAGGED_DATA_COMPARE(==);
    CLAWS_TAGGED_DATA_COMPARE(!=);
    CLAWS_TAGGED_DATA_COMPARE(<=);
    CLAWS_TAGGED_DATA_COMPARE(>=);
    CLAWS_TAGGED_DATA_COMPARE(<);
    CLAWS_TAGGED_DATA_COMPARE(>);

#define CLAWS_TAGGED_DATA_BINARY_OP(OP)					\
    constexpr auto operator OP(_OffestType const &other) const noexcept \
    {									\
      return TaggedData(static_cast<DataType>(data OP other));		\
    }

    CLAWS_TAGGED_DATA_BINARY_OP(+);
    CLAWS_TAGGED_DATA_BINARY_OP(-);
  };
}
