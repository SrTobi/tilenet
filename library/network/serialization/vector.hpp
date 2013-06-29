#pragma once
#ifndef _TN_SERIALIZATION_VECTOR_HPP
#define _TN_SERIALIZATION_VECTOR_HPP




namespace serialization {


	
template<typename S, typename E>
void save(S& s, const std::vector<E>& v)
{
	s << v.size();
	for(auto& e : v) s << e;
}



template<typename S, typename E>
void load(S& s, std::vector<E>& v)
{
	typename std::vector<E>::size_type size;
	s >> size;
	for(typename std::vector<E>::size_type i = 0; i < size; ++i)
	{
		v.emplace_back();
		s >> v.back();
	}
}





template<typename S, typename E>
void serialize(S& s, std::vector<E>& v)
{
	split(s, v);
}




}




#endif
