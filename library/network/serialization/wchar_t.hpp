#pragma once
#ifndef _TN_SERIALIZATION_WCHAR_HPP
#define _TN_SERIALIZATION_WCHAR_HPP



namespace serialization {


template<typename S>
void save(S& s, wchar_t wch)
{
	s << uint32(wch);
}

template<typename S>
void load(S& s, wchar_t& wch)
{
	uint32 v;
	s >> v;
	wch = (wchar_t)v;
}



template<typename S>
void serialize(S& s, wchar_t& wch)
{
	split(s, wch);
}




}




#endif
