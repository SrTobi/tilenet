#pragma once
#ifndef _PROTOCOL_TILE
#define _PROTOCOL_TILE


#include <boost/variant/variant.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/variant.hpp>
#include "settings.hpp"



namespace net {



class PTile
{
	friend class boost::serialization::access;
public:
	enum TileType
	{
		NullTileType = 0,
		StdTileType = 1,
		CharTileType = 2
	};

private:
	typedef boost::variant<	TNTILE::nullset_type,
							TNTILE::stddata_type,
							TNTILE::chardata_type> value_type;

public:
	PTile();
	PTile(const TNTILE& tile);
	PTile(const PTile& other);
	PTile(PTile&& other);
	~PTile();

	PTile& operator =(const PTile& other);
	PTile& operator =(PTile&& other);
	PTile& operator =(const TNTILE& tile);

	void assign(const PTile& other);
	void assign(PTile&& other);
	void assign(const TNTILE& tile);

	TileType type() const;

	template<typename T>
	const T& data() const
	{
		return boost::get<T>(mData);
	}

private:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & mData;
	}

private:
	value_type mData;
};

}


namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, TNTILE::nullset_type& data, const unsigned int version)
{
}

template<class Archive>
void serialize(Archive & ar, TNTILE::stddata_type& data, const unsigned int version)
{
	ar	& data.id
		& data.color
		& data.modifier;
}

template<class Archive>
void serialize(Archive & ar, TNTILE::chardata_type& data, const unsigned int version)
{
	ar	& data.ch
		& data.color
		& data.modifier;
}



} // namespace serialization
} // namespace boost



#endif
