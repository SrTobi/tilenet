#pragma once
#ifndef _PROTOCOL_DESERIALIZER_HPP
#define _PROTOCOL_DESERIALIZER_HPP

#include <codecvt>
#include "settings.hpp"




namespace net {

namespace helper {
bool is_direct_protocol_char(char c);
}


typedef std::vector<char>::const_iterator default_deserialization_iterator;


template<typename Iter = default_deserialization_iterator>
class ProtocolDeserializer
	: boost::noncopyable
{
public:
	typedef Iter														input_iterator;
	typedef std::pair<input_iterator, input_iterator>					range_type;
	typedef typename std::iterator_traits<input_iterator>::value_type	value_type;
	BOOST_CONCEPT_ASSERT((boost::ForwardIterator<input_iterator>));
	static_assert(std::is_same<value_type, char>::value, "Iterator must iterate over char");

public:
	template<typename Iter>
	friend class SpecificExtractor;
	template<typename Iter>
	friend class DirectExtractor;
	template<typename Iter>
	friend class StringExtractor;
	template<typename Iter>
	friend class SequenceExtractor;
	template<typename Iter>
	friend class TypeExtractor;
	template<typename Iter>
	friend class BinaryExtractor;

public:
	ProtocolDeserializer(range_type& input)
		: mInput(input)
	{
	}

	~ProtocolDeserializer()
	{
		/*if(mBuffer.back() == ' ')
			mBuffer.pop_back();
		if(mEndSign)
			mBuffer.push_back(mEndSign);*/
	}

	template<typename T>
	ProtocolDeserializer& operator &(T& v)
	{
		return *this >> v;
	}

private:
	void beginExtracting()
	{

	}

	void endExtracting()
	{
		if(begin() != end() && *begin() == ' ')
		{
			next();
		}
	}

	bool next()
	{
		input_iterator& it = begin();
		if(it == end())
			NOT_IMPLEMENTED(/* throw something*/);

		++it;

		return it != end() && helper::is_direct_protocol_char(*it);
	}

	range_type& range() { return mInput; }
	input_iterator& begin() { return mInput.first; }
	input_iterator& end() { return mInput.second; }
private:
	range_type&		mInput;
};





template<typename Iter = default_deserialization_iterator>
class SpecificExtractor
	: boost::noncopyable
{
public:
	typedef Iter input_iterator;
	typedef ProtocolDeserializer<input_iterator> deserializer_type;
	typedef typename deserializer_type::range_type range_type;

	SpecificExtractor(deserializer_type& deserializer)
		: mDeserializer(deserializer)
	{
		deserializer.beginExtracting();
	}

	~SpecificExtractor()
	{
		deserializer().endExtracting();
	}

protected:
	bool next()
	{
		return deserializer().next();
	}

	deserializer_type& deserializer() { return mDeserializer; }
	input_iterator& input_begin() { return deserializer().begin(); }
	input_iterator& input_end() { return deserializer().end(); }
	range_type input_range() const { return deserializer().range(); }
private:
	deserializer_type&	mDeserializer;
};



template<typename Iter = default_deserialization_iterator>
class DirectExtractor
	: public SpecificExtractor<Iter>
{
public:
	typedef typename SpecificExtractor<Iter>::input_iterator input_iterator;
	typedef typename SpecificExtractor<Iter>::deserializer_type deserializer_type;
	typedef typename SpecificExtractor<Iter>::range_type range_type;

	DirectExtractor(deserializer_type& deserializer)
		: SpecificExtractor(deserializer)
		, mBegin(deserializer.begin())
	{
	}

	range_type operator ()()
	{
		while (next());

		return range_type(mBegin, input_begin());
	}

private:
	const input_iterator mBegin;
};



template<typename Iter = default_deserialization_iterator>
class StringExtractor
	: public SpecificExtractor<Iter>
{
public:
	typedef typename SpecificExtractor<Iter>::input_iterator input_iterator;
	typedef typename SpecificExtractor<Iter>::deserializer_type deserializer_type;
	typedef typename SpecificExtractor<Iter>::range_type range_type;

	StringExtractor(deserializer_type& deserializer)
		: SpecificExtractor(deserializer)
	{
		if(*input_begin() != '\"')
			NOT_IMPLEMENTED();
		std::string utf8;

		char c;
		do {

			c = consume();

			if(c == '\\')
			{
				c = consume();
				switch(c)
				{
				case 'n':
					utf8.push_back('\n');
					break;
				case 't':
					utf8.push_back('\t');
					break;
				case '\\':
					utf8.push_back('\\');
					break;
				case '\"':
					utf8.push_back('\"');
					break;
				default:
					NOT_IMPLEMENTED();
				}
			}else{
				if(c != '\"')
					utf8.push_back(c);
			}

		} while(c != '\"');

		++input_begin();


		std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
		mExtractedString = converter.from_bytes(utf8);
	}

	const std::wstring& operator ()() const
	{
		return mExtractedString;
	}
	 
private:
	char consume()
	{
		++input_begin();
		if(input_begin() == input_end())
			NOT_IMPLEMENTED();

		return *input_begin();
	}

private:
	std::wstring mExtractedString;
};



template<typename Iter>
class SequenceExtractor
	: public SpecificExtractor<Iter>
{
public:
	class SequenceIterator
		: public std::iterator<std::input_iterator_tag, ProtocolDeserializer<Iter>, void>
	{
		template<typename Iter>
		friend class SequenceExtractor;
	public:
		typedef std::iterator<std::input_iterator_tag, ProtocolDeserializer<Iter>, void> base_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::value_type value_type;
		typedef SequenceIterator this_type;


		SequenceIterator()
			: mExtractor(nullptr)
		{
		}

		SequenceIterator(const SequenceIterator& it)
			: mExtractor(it.mExtractor)
		{
		}

		reference operator *()
		{
			tnAssert(mExtractor);
			return mExtractor->currentElement();
		}

		this_type& operator ++()
		{
			if(mExtractor->hasNext())
				mExtractor->newElement();
			else
				mExtractor = nullptr;
			return *this;
		}

		void operator ++(int)
		{
			if(mExtractor->hasNext())
				mExtractor->newElement();
			else
				mExtractor = nullptr;
		}

		bool operator ==(const SequenceIterator& other)
		{
			return equals(other);
		}

		bool operator !=(const SequenceIterator& other)
		{
			return !equals(other);
		}

	private:
		SequenceIterator(SequenceExtractor<Iter>* extractor)
			: mExtractor(extractor)
		{
		}

		bool equals(const SequenceIterator& other)
		{
			auto* oex = other.mExtractor;

			if(mExtractor == oex)
				return true;

			return false;
		}
	private:
		SequenceExtractor<Iter>* mExtractor;
	};
public:
	typedef SequenceIterator									iterator;
	typedef typename SpecificExtractor<Iter>::input_iterator input_iterator;
	typedef typename SpecificExtractor<Iter>::deserializer_type deserializer_type;
	typedef typename SpecificExtractor<Iter>::range_type range_type;

	SequenceExtractor(deserializer_type& deserializer)
		: SpecificExtractor(deserializer)
	{
		if(*input_begin() != '{')
			NOT_IMPLEMENTED(/* thorw */);

		// Skip '{'
		next();
	}

	~SequenceExtractor()
	{
		if(*input_begin() != '}')
			NOT_IMPLEMENTED(/*Throw something*/);

		next();
	}

	iterator begin() { return iterator(this); }
	iterator end() { return iterator(); }

	deserializer_type& newElement()
	{
		if(mChildDeserializer)
		{
			char separator = *deserializer().begin();

			if(separator == ',')
			{
				next(); // consume ','
			}else{
				NOT_IMPLEMENTED(/* throw! */)
			}
		}
		mChildDeserializer.reset(new deserializer_type(deserializer().range()));
		return *mChildDeserializer;
	}

private:
	bool hasNext()
	{
		return (*input_begin() != '}');
	}

	deserializer_type& currentElement()
	{
		if(!mChildDeserializer)
			newElement();
		return *mChildDeserializer;
	}

private:
	std::unique_ptr<deserializer_type> mChildDeserializer;
};



template<typename Iter, typename T>
void deserialize_primitive(ProtocolDeserializer<Iter>& deserializer, T& v)
{
	Iter begin, end;
	DirectExtractor<Iter> extract(deserializer);
	std::tie(begin, end) = extract();

	std::istringstream ss(std::string(begin, end));
	ss >> v;
}


template<typename Iter>
void deserialize_primitive(ProtocolDeserializer<Iter>& deserializer, unsigned char& uc)
{
	unsigned int i;
	deserialize_primitive(deserializer, i);

	if(i > WCHAR_MAX)
	{
		NOT_IMPLEMENTED();
	}

	uc = i;
}

#define PRIMITIVE_DESERIALIZATION(_type) template<typename Iter> ProtocolDeserializer<Iter>& operator >>(ProtocolDeserializer<Iter>& deserializer, _type& v) { deserialize_primitive(deserializer, v); return deserializer; }


template<typename Iter>
ProtocolDeserializer<Iter>& operator >>(ProtocolDeserializer<Iter>& deserializer, char& c)
{
	net::DirectExtractor<Iter> extrat(deserializer);

	Iter begin, end;
	std::tie(begin, end) = extrat();

	if(begin != end)
		c = *begin;
	else
		NOT_IMPLEMENTED();

	return deserializer;
}

template<typename Iter>
ProtocolDeserializer<Iter>& operator >>(ProtocolDeserializer<Iter>& deserializer, bool& b)
{
	char c;
	deserializer >> c;

	switch(c)
	{
	case '0':
		b = false;
		break;
	case '1':
		b = true;
		break;
	default:
		NOT_IMPLEMENTED();
	}

	return deserializer;
}



PRIMITIVE_DESERIALIZATION(unsigned char);
PRIMITIVE_DESERIALIZATION(short);
PRIMITIVE_DESERIALIZATION(unsigned short);
PRIMITIVE_DESERIALIZATION(int);
PRIMITIVE_DESERIALIZATION(unsigned int);
PRIMITIVE_DESERIALIZATION(long);
PRIMITIVE_DESERIALIZATION(unsigned long);
PRIMITIVE_DESERIALIZATION(long long);
PRIMITIVE_DESERIALIZATION(unsigned long long);

#undef PRIMITIVE_DESERIALIZATION

template<typename Iter>
ProtocolDeserializer<Iter>& operator >>(ProtocolDeserializer<Iter>& deserializer, std::wstring& wstr)
{
	StringExtractor<Iter> extract(deserializer);
	wstr = extract();

	return deserializer;
}

template<typename Iter, typename M>
ProtocolDeserializer<Iter>& operator >>(ProtocolDeserializer<Iter>& deserializer, M& msg)
{
	msg.serialize(deserializer);

	return deserializer;
}


}




#endif