#pragma once

#include <type_traits>

namespace claws
{
  struct no_delete
  {};

  // Can be moved to to 'forget' about ownership
  struct forget
  {
    template<class T>
    constexpr void operator()(T &&) const
    {}
  };

  template<class Type, class Deleter>
  struct handle : public Deleter, public Type
  {
    handle<Type, Deleter>() = default;

    template<class T, std::enable_if_t<!std::is_same_v<Type, T>> * = nullptr>
    explicit handle<Type, Deleter>(T &&data)
      : Deleter{}
      , Type(std::forward<T>(data))
    {}

    explicit handle<Type, Deleter>(Deleter &&deleter,
                                   Type &&data) noexcept(std::is_nothrow_constructible<Deleter>() && std::is_nothrow_move_constructible<Type>())
      : Deleter{std::forward<Deleter &&>(deleter)}
      , Type(std::forward<Type>(data))
    {}

    template<class... T, std::enable_if_t<sizeof...(T) != 1> * = nullptr>
    explicit handle<Type, Deleter>(Deleter &&deleter,
                                   T &&... data) noexcept(std::is_nothrow_move_constructible<Deleter>() && std::is_nothrow_constructible<Type, T &&...>())
      : Deleter{std::forward<Deleter &&>(deleter)}
      , Type{std::forward<T>(data)...}
    {}

    template<class First, class... T, bool ok = (sizeof...(T) > 0) && !std::is_same_v<First, Deleter>, std::enable_if_t<ok> * = nullptr>
    explicit handle<Type, Deleter>(First &&first,
                                   T &&... data) noexcept(std::is_nothrow_constructible<Deleter>() && std::is_nothrow_constructible<Type, First, T &&...>())
      : Deleter{}
      , Type{std::forward<First>(first), std::forward<T>(data)...}
    {}

    handle<Type, Deleter>(handle<Type, Deleter> const &other) = delete;

    handle<Type, Deleter>(handle<Type, Deleter> &&other) noexcept(std::is_nothrow_constructible<Deleter>() && std::is_nothrow_move_constructible<Type>()
                                                                  && std::is_nothrow_constructible<Type>() && std::is_nothrow_assignable<Type, Type>())
      : Deleter(static_cast<Deleter &&>(other))
      , Type(static_cast<Type &&>(other))
    {
      static_cast<Type &>(other) = Type{};
    }

    template<class Other_deleter, bool ok = !std::is_same_v<no_delete, Other_deleter>, std::enable_if_t<ok> * = nullptr>
    handle<Type, Deleter>(handle<Type, Other_deleter> &&other) noexcept(std::is_nothrow_move_constructible<Other_deleter>()
                                                                        && std::is_nothrow_move_constructible<Type>())
      : Deleter{}
      , Type(static_cast<Type &&>(other))
    {
      static_cast<Type &>(other) = Type{};
    }

    handle<Type, Deleter> &operator=(handle<Type, Deleter> other)
    {
      using std::swap;

      swap(static_cast<Type &>(*this), static_cast<Type &>(other));
      swap(static_cast<Deleter &>(*this), static_cast<Deleter &>(other));
      return *this;
    }

    template<class Other_deleter>
    handle<Type, Deleter> &operator=(handle<Type, Other_deleter> const other)
    {
      static_cast<Type &>(*this) = static_cast<Type &&>(other);
      static_cast<Deleter &>(*this) = static_cast<Other_deleter &&>(other);
      return *this;
    }

    Type release()
    {
      Type result(static_cast<Type &&>(*this));

      static_cast<Type &>(*this) = Type{};
      return result;
    }

    void raw_swap(Type &val)
    {
      using std::swap;

      swap(val, static_cast<Type &>(*this));
    }

    void aquire(Type val)
    {
      static_cast<Deleter &> (*this)(static_cast<Type &>(*this));
      static_cast<Type &>(*this) = {val};
    }

    auto const &get_deleter() const
    {
      return static_cast<Deleter &>(*this);
    }

    ~handle<Type, Deleter>()
    {
      static_cast<Deleter &> (*this)(static_cast<Type &>(*this));
    }
  };

  template<class Type>
  struct handle<Type, no_delete> : private no_delete, public Type
  {
    template<class... T>
    explicit handle<Type, no_delete>(T &&... data)
      : no_delete{}
      , Type(std::forward<T>(data)...)
    {}

    template<class Other_deleter>
    handle<Type, no_delete>(handle<Type, Other_deleter> const &other)
      : no_delete{}
      , Type{static_cast<Type const &>(other)}
    {}
  };

  template<class Type, class Group_type, class Group_deleter>
  struct group_handle : public handle<Group_type, Group_deleter>
  {
    using handle<Group_type, Group_deleter>::handle;

    auto operator[](std::size_t index) const
    {
      return handle<Type, no_delete>(static_cast<Group_type const &>(*this)[index]);
    }
  };
}
