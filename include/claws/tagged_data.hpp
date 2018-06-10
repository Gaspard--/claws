#pragma once

namespace claws
{
  template<class _data_type, class _offset_type, class _tag>
  struct tagged_data
  {
    using tag = _tag;
    using offset_type = _offset_type;
    using data_type = _data_type;
    data_type data;

    constexpr tagged_data(tagged_data const &) noexcept = default;

    explicit constexpr tagged_data(data_type data = 0u) noexcept
      : data(data)
    {}

#define CLAWS_TAGGED_DATA_PREFIX_OP(OP)				\
    constexpr tagged_data &operator OP() noexcept		\
    {								\
      OP data;							\
      return *this;						\
    }

    CLAWS_TAGGED_DATA_PREFIX_OP(++);
    CLAWS_TAGGED_DATA_PREFIX_OP(--);

#define CLAWS_TAGGED_DATA_SUFFIX_OP(OP)				\
    constexpr tagged_data operator OP(int) noexcept		\
    {								\
      auto copy(*this);						\
      OP *this;							\
      return copy;						\
    }

    CLAWS_TAGGED_DATA_SUFFIX_OP(++);
    CLAWS_TAGGED_DATA_SUFFIX_OP(--);
    
#define CLAWS_TAGGED_DATA_COMPARE(OP)					\
    constexpr bool operator OP(tagged_data const &other) const noexcept	\
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
    constexpr auto operator OP(_offset_type const &other) const noexcept \
    {									\
      return tagged_data(static_cast<data_type>(data OP other));		\
    }

    CLAWS_TAGGED_DATA_BINARY_OP(+);
    CLAWS_TAGGED_DATA_BINARY_OP(-);
  };
}
