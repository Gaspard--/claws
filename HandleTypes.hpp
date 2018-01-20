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

  template<class Type, class Deleter, class GroupeType, class GroupeDeleter>
  struct GroupeHandle : private Deleter, protected Handle<GroupeType, GroupeDeleter>
  {
    using Handle<GroupeType, GroupeDeleter>::Handle;

    auto operator[](std::size_t index) const
    {
      return Handle<Type, NoDelete>(static_cast<GroupeType>(*this)[index]);
    }

    auto operator[](std::size_t index)
    {
      class Proxy : public Handle<NoDelete, Type>
      {
	friend class GroupeHandle<Type, Deleter, GroupeType, Deleter>;
      private:
	GroupeHandle<Type, Deleter, GroupeType, Deleter> &groupHandle;
	std::size_t index;

	// protected:
	Proxy(GroupeHandle<Type, Deleter, GroupeType, Deleter> &groupHandle,
	      std::size_t index)
	  : groupHandle(groupHandle)
	  , index(index)
	{}
	  
      public:
	Proxy(Proxy const &proxy)
	{
	  groupHandle = proxy.groupHandle;
	  index = proxy.index;
	}

	auto &operator=(Proxy const &proxy) = delete;

	auto &operator=(Proxy &&proxy)
	{
	  static_cast<Deleter>(groupHandle)(groupHandle[index]);
	  groupHandle[index] = proxy.groupHandle[proxy.index];
	  proxy.groupHandle[proxy.index] = 0;
	  return *this;
	}

	auto &operator=(Handle<Type, Deleter> other)
	{
	  swap(other);
	  return *this;
	}

	operator Handle<Type, Deleter>()
	{
	  Handle<Type, Deleter> handle{static_cast<Deleter>(groupHandle)};

	  swap(handle);
	  return handle;
	}

	void swap(Handle<Type, Deleter> handle)
	{
	  handle.swap(groupHandle[index]);
	}

	void swap(Proxy proxy)
	{
	  swap(groupHandle[index], proxy.groupHandle[proxy.index]);
	}
      };
      return Proxy{*this, index};
    }

    // template<class Type>
    // struct Iterator
    // {
    //   Type &type;
    //   std::size_t index;

    //   auto operator*()
    //   {
    //     return type[index];
    //   }

    //   auto operator->()
    //   {
    //     return &type[index];
    //   }

    //   auto operator++()
    //   {
    //     ++index;
    //   }

    //   auto operator--()
    //   {
    //     --index;
    //   }
    // }

    // auto moveAt(Handle<Type, Deleter> &handle, std::size_t index)
    // {
    //   Handle<Type, Deleter> out(std::move(data[index]));

    //   data[index] = Type{};
    //   hanlde = std::move(out);
    // }

    // template<class Index>
    // auto release(Index const &index)
    // {
    //   auto &ref((*this)[index]);
    //   auto result(std::move(ref));

    //   ref = std::move(back())
    //   resize(size() - 1);
    //   return std::move(result);
    // }

    // auto acquire()
    // {
    // }
  };
}
