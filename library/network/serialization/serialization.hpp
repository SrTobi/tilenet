#pragma once
#ifndef _SERIALIZATION_HPP
#define _SERIALIZATION_HPP


#include <type_traits>

#include "settings.hpp"
#include "access.hpp"

#include "wchar_t.hpp"
#include "enum.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "variant.hpp"
#include "pair.hpp"

namespace serialization {


namespace detail {

	typedef struct{}* ActionCommonTag;
	typedef struct{}* ActionSerializeTag;
	typedef struct{}* ActionFreeSerializationTag;

	template<typename Ty>
	class has_free_serialize_function
	{
		struct yes { char dummy[1];};
		struct no { yes dummy[3];};
		static_assert(sizeof(yes) != sizeof(no), "Types must not have equal size!");

		template<typename C>
		static C& get_ref();


		//template<typename U> static char Test(SFINAE<U, &U::used_memory>*);

		template <typename C> static yes test( decltype(::serialization::serialize(get_ref<int>(), get_ref<C>()))* ) ;
		template <typename C> static no test(...);


	public:
		static const bool value = (sizeof(test<Ty>(nullptr)) == sizeof(yes));
	};

	template<typename Ty>
	struct action_determiner
	{
		static_assert(!std::is_pointer<Ty>::value, "can not serialize pointer!");
		typedef typename access::has_serialize_function<Ty> has_serializer_check;
		static typename std::conditional<has_serializer_check::value,
										ActionSerializeTag, 
										typename std::conditional<has_free_serialize_function<Ty>::value,
											ActionFreeSerializationTag,
											ActionCommonTag>::type
			>::type determine() { return nullptr; }
	};

}


template<typename Impl, typename Stream>
class Serializer
{
public:
	Serializer(Impl& impl): mImpl(impl) {}
	~Serializer() {}

	template<typename Ty>
	Serializer& operator &(const Ty& v)
	{
		return ((*this) << v);
	}

	template<typename Ty>
	Serializer& operator << (const Ty& v)
	{
		_save(v, detail::action_determiner<Ty>::determine());

		if(mImpl.bad())
		{
			NOT_IMPLEMENTED();
		}

		return *this;
	}

	Stream& stream()
	{
		return mImpl.stream();
	}

private:
	template<typename Ty>
	void _save(const Ty& v, detail::ActionSerializeTag)
	{
		access::call_serialize(*this, v);
	}

	template<typename Ty>
	void _save(const Ty& v, detail::ActionFreeSerializationTag)
	{
		::serialization::serialize(*this, const_cast<Ty&>(v));
	}

	template<typename Ty>
	void _save(const Ty& v, detail::ActionCommonTag)
	{
		mImpl.save(v);
	}

private:
	Impl& mImpl;
};

template<typename Impl, typename Stream>
class Deserializer
{
public:
	Deserializer(Impl& impl): mImpl(impl) {}
	~Deserializer(){}

	template<typename Ty>
	Deserializer& operator &(Ty& v)
	{
		return ((*this) >> v);
	}


	template<typename Ty>
	Deserializer& operator >> (Ty& v)
	{
		_load(v, detail::action_determiner<Ty>::determine());

		if(mImpl.bad())
		{
			NOT_IMPLEMENTED();
		}

		return *this;
	}

	Stream& stream()
	{
		return mImpl.stream();
	}

private:
	template<typename Ty>
	void _load(Ty& v, detail::ActionSerializeTag)
	{
		access::call_serialize(*this, v);
	}

	template<typename Ty>
	void _load(Ty& v, detail::ActionFreeSerializationTag)
	{
		::serialization::serialize(*this, const_cast<Ty&>(v));
	}

	template<typename Ty>
	void _load(Ty& v, detail::ActionCommonTag)
	{
		mImpl.load(v);
	}
private:
	Impl& mImpl;
};


template<typename Stream = std::ostream>
class TextSerializer
	: public Serializer<TextSerializer<Stream>, Stream>
{
public:
	typedef Stream stream_type;
	typedef Serializer<TextSerializer<Stream>, Stream> base_type;

	TextSerializer(Stream& stream)
		: base_type(*this)
		, mStream(stream)
	{
	}

	template<typename Ty>
	void save(const Ty& v)
	{
		mStream << v << ' ';
	}

	bool bad() const
	{
		return (mStream.bad() || mStream.fail() || mStream.eof());
	}

	stream_type& stream()
	{
		return mStream;
	}

private:
	Stream& mStream;
};


template<typename Stream = std::istream>
class TextDeserializer
	: public Deserializer<TextDeserializer<Stream>, Stream>
{
public:
	typedef Stream stream_type;
	typedef Deserializer<TextDeserializer<Stream>, Stream> base_type;

	TextDeserializer(Stream& stream)
		: base_type(*this)
		, mStream(stream)
	{
	}

	template<typename Ty>
	void load(Ty& v)
	{
		mStream >> v;
	}

	bool bad() const
	{
		return (mStream.bad() || mStream.fail() || mStream.eof());
	}

	stream_type& stream()
	{
		return mStream;
	}
private:
	Stream& mStream;
};


template<typename Impl, typename Stream, typename Ty>
void split(Serializer<Impl, Stream>& s, const Ty& v)
{
	::serialization::save(s, v);
}

template<typename Impl, typename Stream, typename Ty>
void split(Deserializer<Impl, Stream>& s, Ty& v)
{
	::serialization::load(s, v);
}

template<typename Impl, typename Stream, typename Ty>
static void split(const Ty* v, Serializer<Impl, Stream>& s)
{
	access::call_save(s, *v);
}

template<typename Impl,typename Stream,  typename Ty>
static void split(Ty* v, Deserializer<Impl, Stream>& s)
{
	access::call_load(s, *v);
}



}









#endif
