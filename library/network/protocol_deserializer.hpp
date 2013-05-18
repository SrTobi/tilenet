#pragma once
#ifndef _PROTOCOL_DESERIALIZER_HPP
#define _PROTOCOL_DESERIALIZER_HPP

#include "settings.hpp"




namespace net {

typedef std::vector<char>::const_iterator default_deserialization_iterator;


template<typename Iter = default_deserialization_iterator>
class ProtocolDeserializer
	: boost::noncopyable
{
public:
	typedef Iter												iterator;
	typedef std::pair<iterator, iterator>						range_type;
	typedef typename std::iterator_traits<iterator>::value_type	value_type;
	BOOST_CONCEPT_ASSERT((boost::ForwardIterator<iterator>));
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
	ProtocolDeserializer(iterator begin, iterator end)
		: mInput(begin, end)
		, mEndSign(0)
	{
	}

	~ProtocolDeserializer()
	{
		if(range().first != range().second)
		{
			NOT_IMPLEMENTED();
		}
		/*if(mBuffer.back() == ' ')
			mBuffer.pop_back();
		if(mEndSign)
			mBuffer.push_back(mEndSign);*/
	}

	template<typename T>
	ProtocolDeserializer& operator &(const T& v)
	{
		return *this >> v;
	}

private:
	void endExtracting(const iterator& it)
	{
		mInput.first = it;
		if(mInput.first != mInput.second)
		{
			if(*it == ' ' || *it == mEndSign)
			{
				++(mInput.first);
			}else{

				NOT_IMPLEMENTED(/*throw*/)
			}
		}
	}

	ProtocolDeserializer(range_type& input, char endSign)
		: mInput(input)
		, mEndSign(endSign)
	{
	}

	bool next(iterator& it)
	{
		if(it == range().second)
			NOT_IMPLEMENTED(/* throw something*/);

		++it;

		return it != range().second && *it != ' ' && *it != mEndSign;
	}

	const range_type& range() { return mInput; }
private:
	range_type		mInput;
	char			mEndSign;
};





template<typename Iter = default_deserialization_iterator>
class SpecificExtractor
	: boost::noncopyable
{
public:
	typedef Iter iterator;
	typedef ProtocolDeserializer<iterator> deserializer_type;
	typedef typename deserializer_type::range_type range_type;

	SpecificExtractor(deserializer_type& deserializer)
		: mDeserializer(deserializer)
		, mBegin(deserializer.range().first)
		, mEnd(deserializer.range().first)
	{
		//mSerializer.beginInserting();
	}

	~SpecificExtractor()
	{
		deserializer().endExtracting(mEnd);
	}

protected:
	bool expand()
	{
		return deserializer().next(mEnd);
	}

	deserializer_type& deserializer() { return mDeserializer; }
	const iterator& begin() const { return mBegin; }
	const iterator& end() const { return mEnd; }
	range_type range() const { return range_type(begin(), end()); }
private:
	deserializer_type&	mDeserializer;
	const iterator mBegin;
	iterator mEnd;
};



template<typename Iter = default_deserialization_iterator>
class DirectExtractor
	: public SpecificExtractor<Iter>
{
public:
	typedef typename SpecificExtractor<Iter>::iterator iterator;
	typedef typename SpecificExtractor<Iter>::deserializer_type deserializer_type;
	typedef typename SpecificExtractor<Iter>::range_type range_type;

	DirectExtractor(deserializer_type& deserializer)
		: SpecificExtractor(deserializer)
	{
	}

	range_type operator ()()
	{
		while (expand());

		return range();
	}
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

#define PRIMITIVE_SERIALIZATION(_type) template<typename Iter> ProtocolDeserializer<Iter>& operator >>(ProtocolDeserializer<Iter>& deserializer, _type& v) { deserialize_primitive(deserializer, v); return deserializer; }


PRIMITIVE_SERIALIZATION(int);


}




#endif