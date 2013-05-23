#pragma once
#ifndef _PROTOCOL_SERIALIZER_HPP
#define _PROTOCOL_SERIALIZER_HPP

#include <vector>
#include <locale>
#include <boost/concept_check.hpp>
#include <boost/noncopyable.hpp>
#include "settings.hpp"

namespace net {

typedef std::vector<char> default_serialization_buffer;



template<typename Buffer = default_serialization_buffer>
class ProtocolSerializer
	: boost::noncopyable
{
public:
	typedef Buffer								buffer_type;
	typedef typename buffer_type::value_type	value_type;
	BOOST_CONCEPT_ASSERT((boost::BackInsertionSequence<buffer_type>));
	static_assert(std::is_same<typename buffer_type::value_type, char>::value, "Buffer must contain char");

public:
	template<typename Buffer_>
	friend class SpecificInserter;
	template<typename Buffer_>
	friend class DirectInserter;
	template<typename Buffer_>
	friend class StringInserter;
	template<typename Buffer_>
	friend class SequenceInserter;
	template<typename Buffer_>
	friend class TypeInserter;
	template<typename Buffer_>
	friend class BinaryInserter;

public:
	ProtocolSerializer(buffer_type& buffer)
		: mBuffer(buffer)
		, mEndSign(0)
	{
	}

	~ProtocolSerializer()
	{
		if(mBuffer.back() == ' ')
			mBuffer.pop_back();
		if(mEndSign)
			mBuffer.push_back(mEndSign);
	}

	template<typename T>
	ProtocolSerializer& operator &(const T& v)
	{
		return *this << v;
	}

private:
	void beginInserting()
	{
		// Do nothing
	}

	void endInserting()
	{
		mBuffer.push_back(' ');
	}

	ProtocolSerializer(buffer_type& buffer, char endSign)
		: mBuffer(buffer)
		, mEndSign(endSign)
	{
	}

	buffer_type& buffer() { return mBuffer; }
private:
	buffer_type&	mBuffer;
	char			mEndSign;
};


template<typename Buffer = default_serialization_buffer>
class SpecificInserter
	: boost::noncopyable
{
public:
	typedef Buffer buffer_type;
	typedef ProtocolSerializer<buffer_type> serializer_type;

	SpecificInserter(serializer_type& serializer)
		: mSerializer(serializer)
	{
		mSerializer.beginInserting();
	}

	~SpecificInserter()
	{
		mSerializer.endInserting();
	}

protected:

	void insert_back(char c)
	{
		mSerializer.buffer().push_back(c);
	}

	template<typename Iter>
	void insert_back(Iter begin, Iter end)
	{
		std::copy(begin, end, std::back_inserter(mSerializer.buffer()));
	}

	serializer_type& serializer() { return mSerializer; }
protected:
	serializer_type&	mSerializer;
};

template<typename Buffer = default_serialization_buffer>
class DirectInserter
	: public SpecificInserter<Buffer>
{
public:
	typedef typename SpecificInserter<Buffer>::buffer_type buffer_type;
	typedef typename SpecificInserter<Buffer>::serializer_type serializer_type;

	DirectInserter(serializer_type& serializer)
		: SpecificInserter<Buffer>(serializer)
	{
	}

	void operator ()(char c)
	{
		tnAssert(c > 0 && c <= 127);
		this->insert_back(c);
	}

	template<typename Iter>
	void operator ()(Iter begin, Iter end)
	{
		for(;begin != end; ++begin)
		{
			(*this)(*begin);
		}
	}
};

template<typename Buffer = default_serialization_buffer>
class StringInserter
	: public SpecificInserter<Buffer>
{
public:
	typedef SpecificInserter<Buffer> base_type;
	typedef typename base_type::buffer_type buffer_type;
	typedef typename base_type::serializer_type serializer_type;

	StringInserter(serializer_type& serializer)
		: base_type(serializer)
	{
		this->insert_back('\"');
	}

	~StringInserter()
	{
		this->insert_back('\"');
	}

	// Do not use this version!
	void operator ()(const std::string& str)
	{
		(*this)(lexical_convert<std::wstring>(str));
	}

	void operator ()(const std::wstring& wstr)
	{
		std::string utf8 = to_utf8_string(wstr);

		for(char c : utf8)
		{
			switch(c)
			{
			case '\\':
				this->insert_back('\\');
				this->insert_back('\\');
				break;
			case '\n':
				this->insert_back('\\');
				this->insert_back('n');
				break;
			case '\t':
				this->insert_back('\\');
				this->insert_back('t');
				break;
			case '\"':
				this->insert_back('\\');
				this->insert_back('\"');
				break;
			default:
				this->insert_back(c);
				break;
			}
		}
	}
};



template<typename Buffer = default_serialization_buffer>
class SequenceInserter
	: public SpecificInserter<Buffer>
{
public:
	typedef SpecificInserter<Buffer> base_type;
	typedef typename base_type::buffer_type buffer_type;
	typedef typename base_type::serializer_type serializer_type;

	SequenceInserter(serializer_type& serializer)
		: base_type(serializer)
	{
		this->insert_back('{');
	}

	~SequenceInserter()
	{
		mChildSerializer.reset();
		if(this->serializer().buffer().back() == ',')
			this->serializer().buffer().pop_back();
		this->insert_back('}');
	}

	serializer_type& newElement()
	{
		mChildSerializer.reset(new serializer_type(this->serializer().buffer(), ','));
		return *mChildSerializer;
	}

private:
	std::unique_ptr<serializer_type> mChildSerializer;
};


template<typename Buffer = default_serialization_buffer>
class TypeInserter
	: public SpecificInserter<Buffer>
{
public:
	typedef SpecificInserter<Buffer> base_type;
	typedef typename base_type::buffer_type buffer_type;
	typedef typename base_type::serializer_type serializer_type;

	TypeInserter(serializer_type& serializer, const std::string& name)
		: base_type(serializer)
		, mChildSerializer(new serializer_type(serializer.buffer()))
	{
		//TODO: check name
		{
			serializer_type interSerializer(serializer.buffer());
			DirectInserter<> insertDirect(interSerializer);
			insertDirect(name.begin(), name.end());
		}
		this->insert_back('{');
	}

	~TypeInserter()
	{
		mChildSerializer.reset();
		this->insert_back('}');
	}

	serializer_type& serializer()
	{
		return *mChildSerializer;
	}

private:
	std::unique_ptr<ProtocolSerializer<Buffer>> mChildSerializer;
};


template<typename Buffer, typename T>
void serialize_primitive(ProtocolSerializer<Buffer>& serializer, const T& v)
{
	std::ostringstream ss;
	ss << v;

	std::string str = ss.str();

	DirectInserter<Buffer> insert(serializer);
	insert(str.begin(), str.end());
}

template<typename Buffer>
void serialize_primitive(ProtocolSerializer<Buffer>& serializer, const unsigned char& uc)
{
	serialize_primitive(serializer, (unsigned int)(uc));
}

#define PRIMITIVE_SERIALIZATION(_type) template<typename Buffer> ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, _type v) { serialize_primitive(serializer, v); return serializer; }


template<typename Buffer>
ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, char c)
{
	DirectInserter<Buffer> insert(serializer);
	insert(c);

	return serializer;
}

template<typename Buffer>
ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, bool v)
{
	serializer << (v ? '1' : '0');
	return serializer;
}

PRIMITIVE_SERIALIZATION(unsigned char);
PRIMITIVE_SERIALIZATION(short);
PRIMITIVE_SERIALIZATION(unsigned short);
PRIMITIVE_SERIALIZATION(int);
PRIMITIVE_SERIALIZATION(unsigned int);
PRIMITIVE_SERIALIZATION(long);
PRIMITIVE_SERIALIZATION(unsigned long);
PRIMITIVE_SERIALIZATION(long long);
PRIMITIVE_SERIALIZATION(unsigned long long);
#undef PRIMITIVE_SERIALIZATION

template<typename Buffer>
ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, const wchar_t* str)
{
	serializer << std::wstring(str);

	return serializer;
}


template<typename Buffer>
ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, const std::wstring& str)
{
	StringInserter<Buffer> insert(serializer);
	insert(str);

	return serializer;
}
/*
template<typename C>
struct SerializationContainer
{

	SerializationContainer(const C& container)
		: container(container)
	{
	}

	const C& container;
};

template<typename C>
SerializationContainer<C> serialize_container(const C& container)
{
	return SerializationContainer<C>(container);
}

template<typename Buffer, typename C>
ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, const SerializationContainer<C>& cont)
{
	SequenceInserter<> sequence(serializer);

	for(auto& v : cont.container)
	{
		sequence.newElement() << v;
	}

	return serializer;
}*/

/*
 * Serializer for classes
 */
template<typename Buffer, typename M>
ProtocolSerializer<Buffer>& operator <<(ProtocolSerializer<Buffer>& serializer, const M& msg)
{
	const_cast<M&>(msg).serialize(serializer);

	return serializer;
}


}




#endif
