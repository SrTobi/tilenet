#pragma once
#ifndef _TN_SERIALIZATION_ACCESS_HPP
#define _TN_SERIALIZATION_ACCESS_HPP


namespace serialization {

	
template<typename Impl, typename Stream>
class Serializer;
template<typename Impl, typename Stream>
class Deserializer;

class access
{
public:
	template<typename Ty>
	class has_serialize_function
	{
		struct yes { char dummy[1];};
		struct no { yes dummy[3];};
		static_assert(sizeof(yes) != sizeof(no), "Types must not have equal size!");
		
		template<typename C>
		static C& get_ref();


		//template<typename U> static char Test(SFINAE<U, &U::used_memory>*);

		template <typename C> static yes test( decltype(get_ref<C>().serialize(get_ref<int>()))* ) ;
		template <typename C> static no test(...);


	public:
		static const bool value = (sizeof(test<Ty>(nullptr)) == sizeof(yes));
	};

private:
	template<typename Impl, typename Stream>
	friend class Serializer;
	template<typename Impl, typename Stream>
	friend class Deserializer;

	template<typename Impl, typename Stream, typename Ty>
	static void call_serialize(Serializer<Impl, Stream>& s, const Ty& v)
	{
		const_cast<Ty&>(v).serialize(s);
	}

	template<typename Impl, typename Stream, typename Ty>
	static void call_serialize(Deserializer<Impl, Stream>& s, Ty& v)
	{
		v.serialize(s);
	}

	template<typename Impl, typename Stream, typename Ty>
	static void call_save(Serializer<Impl, Stream>& s, const Ty& v)
	{
		v.save(s);
	}

	template<typename Impl, typename Stream, typename Ty>
	static void call_load(Deserializer<Impl, Stream>& s, Ty& v)
	{
		v.load(s);
	}
};

}

#endif