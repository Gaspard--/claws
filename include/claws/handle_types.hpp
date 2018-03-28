///
/// *Defined in "HandleTypes.hpp"*
/// (TODO: rename to handle_types.hpp)*
///
#pragma once

#include <type_traits>
#include <utility>
#include <cassert>

#include "claws/box.hpp"

namespace claws
{
  /// \defgroup handles RAII handles
  /// @{
  ///
  /// \brief Collection of handles to help with RAII.
  ///

  ///
  /// \brief Tag to change a `claws::handle`
  ///
  struct no_delete;

  ///
  /// \ingroup handles
  /// \brief `deleter` used to 'forget' about ownership of a `claws::handle`.
  ///
  /// This deleter is a no-op, an thus a handle created with it will not free the ressource upon destruction.
  ///
  /// Example:
  /// \snippetlineno should_compile.cpp Forget about ownership
  ///
  struct forget
  {
    template<class T>
    constexpr void operator()(T &&) const noexcept
    {}
  };

  namespace impl
  {
    template<class T, bool is_class = std::is_class_v<T>>
    struct conversion_provider : public T
    {
    private:
      constexpr T const &get() const noexcept
      {
        return *this;
      }

      constexpr T &get() noexcept
      {
        return *this;
      }

    public:
      using T::T;

      constexpr conversion_provider() = default;

      constexpr conversion_provider(T &&t)
        : T(std::move(t))
      {}

      constexpr conversion_provider(T const &t)
        : T(t)
      {}

      constexpr operator T &() noexcept
      {
        return get();
      }

      constexpr operator T const &() const noexcept
      {
        return get();
      }
    };

    template<class T>
    struct conversion_provider<T, false> : claws::box<T>
    {
      using claws::box<T>::box;
      using box<T>::operator T &;
      using box<T>::operator T const &;
    };

    template<class _type>
    class handle_base : public conversion_provider<_type>
    {
    protected:
      using type = _type;
      using real_type = conversion_provider<type>;

      constexpr handle_base() = default;

      constexpr handle_base(type &&value) noexcept(std::is_nothrow_default_constructible_v<type> &&std::is_nothrow_move_constructible_v<type>)
        : conversion_provider<_type>(std::move(value))
      {}

      constexpr handle_base(type const &value) noexcept(std::is_nothrow_copy_constructible_v<type>)
        : real_type(value)
      {}

      template<class... T, std::enable_if_t<(sizeof...(T) > 1)> * = nullptr>
      constexpr handle_base(T &&... param)
        : real_type{std::forward<T>(param)...}
      {}

    public:
      friend constexpr void swap(handle_base &lh, handle_base &rh)
      {
        using std::swap;

        swap(static_cast<type &>(lh), static_cast<type &>(rh));
      }

      ~handle_base() = default;
    };
  }

  ///
  /// \brief A RAII wrapper which ensures that a value of type `type` is properly destroyed.
  ///
  /// @tparam _type The stored value's type.
  /// @tparam _deleter_type The deleter's type. This struct is specialised if this type is the tag type `claws::no_delete`, see `claws::handle<type, claws::no_delete>`. Inherited protectedly. This allows [empty base optimisation](http://en.cppreference.com/w/cpp/language/ebo).
  ///
  /// If the class is empty, the contained value is `type{}`
  ///
  template<class _type, class _deleter_type>
  class handle : protected _deleter_type, public impl::handle_base<_type>
  {
  public:
    /// \brief The stored value's type. The handle tries to behave as if it was inherited publicly, even for primitive types.
    using type = _type;

    /// \brief The deleter's type.
    using deleter_type = _deleter_type;

  protected:
    using impl::handle_base<_type>::operator type &;

  public:
    using impl::handle_base<_type>::operator type const &;

    ///
    /// \brief Default construction. Handle is empty.
    ///
    constexpr handle<type, deleter_type>() noexcept(std::is_nothrow_constructible<type>() && std::is_nothrow_constructible<deleter_type>()) = default;

    ///
    /// @name Deleted copy contructors
    /// Copying from another `handle` is forbidden.
    /// @{
    ///
    handle<type, deleter_type>(handle<type, deleter_type> const &other) = delete;
    template<class other_deleter_type>
    handle<type, deleter_type>(handle<type, other_deleter_type> const &other) = delete;
    /// @}
    ///

    ///
    /// @name Deleted move contructors
    /// Moving from `handle<type, claws::no_delete>` is forbidden.
    /// @{
    ///
    handle<type, deleter_type>(deleter_type &&deleter, handle<type, claws::no_delete> &&other) = delete;
    handle<type, deleter_type>(handle<type, claws::no_delete> &&other) = delete;
    handle<type, deleter_type> operator=(handle<type, claws::no_delete> other) = delete;
    /// @}
    ///

    ///
    /// \brief handle is movable if the deleter isn't `claws::no_delete`
    ///
    constexpr handle<type, deleter_type>(handle<type, deleter_type> &&other) noexcept(std::is_nothrow_constructible<deleter_type>()
                                                                                      && std::is_nothrow_move_constructible<type>()
                                                                                      && std::is_nothrow_constructible<type>()
                                                                                      && std::is_nothrow_assignable<type, type>())
      : deleter_type(static_cast<deleter_type const &>(other))
      , impl::handle_base<type>()
    {
      swap(static_cast<impl::handle_base<type> &>(*this), static_cast<impl::handle_base<type> &>(other));
    }

    ///
    /// \brief constructs `handle` by moving from a handle with another `deleter_type`, and moving in a new deleter
    ///
    /// `handle` can be moved to any other hande if both the `handle`s' `deleter_type`s aren't `claws::no_delete`
    ///
    template<class other_deleter_type>
    constexpr handle<type, deleter_type>(deleter_type &&deleter,
                                         handle<type, other_deleter_type> &&other) noexcept(std::is_nothrow_constructible<deleter_type>()
                                                                                            && std::is_nothrow_move_constructible<type>())
      : deleter_type(deleter)
      , impl::handle_base<type>()
    {
      swap(static_cast<impl::handle_base<type> &>(other), static_cast<impl::handle_base<type> &>(*this));
    }

    ///
    /// \brief constructs `handle` by moving from a handle with another `deleter_type`. The deleter is constrcuted by default.
    ///
    /// `handle` can be moved to any other hande if both the `handle`s' `deleter_type`s aren't `claws::no_delete`
    ///
    template<class other_deleter_type>
    constexpr handle<type, deleter_type>(handle<type, other_deleter_type> &&other) noexcept(std::is_nothrow_constructible<deleter_type>()
                                                                                            && std::is_nothrow_move_constructible<type>())
      : handle<type, deleter_type>({}, std::move(other))
    {}

    ///
    /// @name Assignement operators
    /// \brief handle is move-assignable if the deleter isn't `claws::no_delete`
    /// Moving from `handle<type, claws::no_delete>` is forbidden.
    /// @{
    ///

    ///
    /// \brief both the deleter and value are move-assigned
    ///
    constexpr handle<type, deleter_type> &operator=(handle<type, deleter_type> &&other)
    {
      handle<type, deleter_type> copy(std::move(other));
      using std::swap;

      swap(static_cast<deleter_type &>(*this), static_cast<deleter_type &>(copy));
      swap(static_cast<impl::handle_base<type> &>(*this), static_cast<impl::handle_base<type> &>(copy));
      return *this;
    }

    ///
    /// \brief value is moved. Deleters remain untouched.
    ///
    template<class other_deleter_type>
    constexpr handle<type, deleter_type> &operator=(handle<type, other_deleter_type> &&other)
    {
      static_assert(!std::is_same_v<claws::no_delete, other_deleter_type>, "can't move from non-owning handle!");

      std::swap(static_cast<impl::handle_base<type> &>(*this), static_cast<impl::handle_base<type> &>(other));
      return *this;
    }
    /// @}

    ///
    /// \brief Moves the deleter and the value into the handle
    ///
    constexpr explicit handle<type, deleter_type>(deleter_type &&deleter, type &&value) noexcept(std::is_nothrow_constructible<deleter_type>()
                                                                                                 && std::is_nothrow_move_constructible<type>())
      : deleter_type{std::move(deleter)}
      , impl::handle_base<type>(std::move(value))
    {}

    ///
    /// \brief Moves the deleter into the handle and forwards the other arguments to contruct the value
    ///
    template<class... T, std::enable_if_t<sizeof...(T) != 1> * = nullptr>
    constexpr explicit handle<type, deleter_type>(deleter_type &&deleter, T &&... data) noexcept(std::is_nothrow_move_constructible<deleter_type>()
                                                                                                 && std::is_nothrow_constructible<type, T &&...>())
      : deleter_type{std::move(deleter)}
      , impl::handle_base<type>{std::forward<T>(data)...}
    {}

    ///
    /// \brief Default-constructs the deleter and forward all argument to contruct the value
    ///
    template<class First, class... T, bool ok = (sizeof...(T) > 0) && !std::is_same_v<First, deleter_type>, std::enable_if_t<ok> * = nullptr>
    explicit handle<type, deleter_type>(First &&first, T &&... data) noexcept(std::is_nothrow_constructible<deleter_type>()
                                                                              && std::is_nothrow_constructible<type, First, T &&...>())
      : deleter_type{}
      , impl::handle_base<type>{std::forward<First>(first), std::forward<T>(data)...}
    {}

    ///
    /// \brief Gets the deleter
    ///
    /// Should only be used if deleter contains info which may be reusable.
    ///
    constexpr auto const &get_deleter() const
    {
      return static_cast<deleter_type &>(*this);
    }

    ///
    /// \brief On destruction, `deleter::operator(type &&)` is called, using the inherited `deleter` and `type`.
    ///
    /// `deleter::operator(type &&)` should be a no-op if the contained value is `type{}`
    ///
    ~handle<type, deleter_type>() noexcept(noexcept(std::declval<deleter_type &>()(std::declval<type &&>())))
    {
      static_cast<deleter_type &> (*this)(std::move(static_cast<type &>(*this)));
    }
  };

  ///
  /// \brief A non-owning `claws::handle`.
  ///
  /// @tparam type the contained value's type, inherited publicly.
  ///
  /// Can be created by copy from any other handle with the same `type`.
  ///
  /// Tries to behave as much as `type` as possible otherwise.
  ///
  /// Also inherits `type`'s constructors.
  ///
  /// Example:
  /// \snippetlineno should_compile.cpp Copy to non-owning handle
  ///
  template<class _type>
  struct handle<_type, no_delete> : public impl::handle_base<_type>
  {
  public:
    /// \brief The deleter's type.
    using deleter_type = no_delete;

    /// The stored value's type. Inherited publicly.
    using type = _type;

    using impl::handle_base<_type>::operator type &;

    ~handle() = default;

    constexpr handle() = default;

    constexpr handle(handle const &) = default;

    constexpr handle(type const &value)
      : impl::handle_base<type>(value)
    {}

    ///
    /// Construct from any other handle with the same `type`.
    ///
    /// @note The `other_deleter_type` can be anything since this is a non-owning handle.
    ///
    template<class other_deleter_type>
    constexpr handle(handle<type, other_deleter_type> const &other)
      : impl::handle_base<type>{static_cast<impl::handle_base<type> const &>(other)}
    {}
  };

  ///
  /// \brief provides a groupe handle, where `auto operator[](std::size_t)` returns a non-owning handle
  ///
  template<class type, class group_type, class group_deleter>
  struct group_handle : public handle<group_type, group_deleter>
  {
    using handle<group_type, group_deleter>::handle;

    ///
    /// \brief returns a non-owning handle
    ///
    constexpr auto operator[](std::size_t index) const
    {
      return handle<type, no_delete>(static_cast<group_type const &>(*this)[index]);
    }
  };
  ///
  /// @}
  ///
}
