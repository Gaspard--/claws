#pragma once

#include <type_traits>

namespace claws
{
  struct NoDelete
  {
  };

  template<class Type, class Deleter>
  struct Handle : private Deleter, public Type
  {
    Handle<Type, Deleter>() = default;

    template<class T, std::enable_if_t<!std::is_same_v<Type, T>> * = nullptr>
    explicit Handle<Type, Deleter>(T &&data)
      : Deleter{}, Type(std::forward<T>(data))
    {
    }

    explicit Handle<Type, Deleter>(Deleter &&deleter, Type &&data)
      : Deleter{std::forward<Deleter &&>(deleter)}, Type(std::forward<Type>(data))
    {
    }

    template<class... T, std::enable_if_t<sizeof...(T) != 1> * = nullptr>
    explicit Handle<Type, Deleter>(Deleter &&deleter, T &&... data)
      : Deleter{std::forward<Deleter &&>(deleter)}, Type{std::forward<T>(data)...}
    {
    }

    template<class First, class... T, bool ok = (sizeof...(T) > 0) && !std::is_same_v<First, Deleter>, std::enable_if_t<ok> * = nullptr>
    explicit Handle<Type, Deleter>(First &&first, T &&... data)
      : Deleter{}, Type{std::forward<First>(first), std::forward<T>(data)...}
    {
    }

    Handle<Type, Deleter>(Handle<Type, Deleter> const &other) = delete;

    Handle<Type, Deleter>(Handle<Type, Deleter> &&other) noexcept(noexcept(Deleter(std::declval<Deleter &&>())) && noexcept(Type(std::declval<Type &&>())))
      : Deleter(static_cast<Deleter &&>(other)), Type(static_cast<Type &&>(other))
    {
      static_cast<Type &>(other) = Type{};
    }

  
    template<class OtherDeleter, bool ok = !std::is_same_v<NoDelete, OtherDeleter>, std::enable_if_t<ok> * = nullptr>
    Handle<Type, Deleter>(Handle<Type, OtherDeleter> &&other) noexcept(noexcept(Deleter(std::declval<OtherDeleter &&>())) && noexcept(Type(std::declval<Type &&>())))
      : Deleter(static_cast<OtherDeleter &&>(other)), Type(static_cast<Type &&>(other))
    {
      static_cast<Type &>(other) = Type{};
    }

    Handle<Type, Deleter> &operator=(Handle<Type, Deleter> other)
    {
      using std::swap;

      swap(static_cast<Type &>(*this), static_cast<Type &>(other));
      swap(static_cast<Deleter &>(*this), static_cast<Deleter &>(other));
      return *this;
    }

    template<class OtherDeleter>
    Handle<Type, Deleter> &operator=(Handle<Type, OtherDeleter> other)
    {
      using std::swap;

      swap(static_cast<Type &>(*this), static_cast<Type &>(other));
      swap(static_cast<Deleter &>(*this), static_cast<OtherDeleter &>(other));
      return *this;
    }

    Type release()
    {
      Type result(static_cast<Type>(*this));

      static_cast<Type &>(*this) = Type{};
      return result;
    }

    void rawSwap(Type &type)
    {
      using std::swap;

      swap(type, static_cast<Type &>(*this));
    }

    void aquire(Type type)
    {
      static_cast<Deleter &>(*this)(static_cast<Type &>(*this));
      static_cast<Type &>(*this) = {type};
    }

    ~Handle<Type, Deleter>()
    {
      // std::cerr << "Destroying " << typeid(Type).name() << " with " << typeid(Deleter).name() << std::endl;
      static_cast<Deleter &>(*this)(static_cast<Type &>(*this));
    }
  };

  template<class Type>
  struct Handle<Type, NoDelete> : private NoDelete, public Type
  {
    template<class... T>
    explicit Handle<Type, NoDelete>(T &&... data)
      : NoDelete{}, Type(std::forward<T>(data)...)
    {
    }

    template<class OtherDeleter>
    Handle<Type, NoDelete>(Handle<Type, OtherDeleter> const &other)
      : NoDelete{}, Type{static_cast<Type const &>(other)}
    {
    }
  };

  template<class Type, class GroupType, class GroupDeleter>
  struct GroupHandle : public Handle<GroupType, GroupDeleter>
  {
    using Handle<GroupType, GroupDeleter>::Handle;

    auto operator[](std::size_t index) const
    {
      return Handle<Type, NoDelete>(static_cast<GroupType>(*this)[index]);
    }

    // auto operator[](std::size_t index)
    // {
    //   class Proxy
    //   {
    // 	friend class GroupHandle<Type, GroupType, GroupDeleter>;
    //   private:
    // 	GroupType &groupHandle;
    // 	std::size_t index;

    // 	Proxy(GroupHandle<Type, GroupType, GroupDeleter> &groupHandle,
    // 	      std::size_t index)
    // 	  : groupHandle(groupHandle)
    // 	  , index(index)
    // 	{}
	  
    //   public:
    // 	Proxy(Proxy const &proxy)
    // 	  : groupHandle(proxy.groupHandle)
    // 	  , index(proxy.index)
    // 	{
    // 	}

    // 	auto &operator=(Proxy const &proxy) = delete;
    // 	auto &operator=(Proxy &&proxy) = delete;

    // 	operator Handle<Type, NoDelete>() const
    // 	{
    // 	  return Handle<Type, NoDelete>(groupHandle[index]);
    // 	}

    // 	void swap(Proxy proxy)
    // 	{
    // 	  using std::swap;

    // 	  swap(groupHandle[index], proxy.groupHandle[proxy.index]);
    // 	}
    //   };
    //   return Proxy{*this, index};
    // }
  };
}
