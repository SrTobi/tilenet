#pragma once
#ifndef _PROTOCOL_TILE
#define _PROTOCOL_TILE

#include <boost/variant.hpp>

#include "settings.hpp"

#include "network/serialization/serialization.hpp"
#include "network/serialization/variant.hpp"


#include "utils/additional_comparison.hpp"

namespace net {



class PTile
{
	friend class serialization::access;
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

	bool operator ==(const PTile& other) const;
	bool operator !=(const PTile& other) const;

	template<typename T>
	const T& data() const
	{
		return boost::get<T>(mData);
	}

private:
	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar & mData;
	}

private:
	value_type mData;
};

}


namespace serialization {

template<class Archive>
void serialize(Archive & ar, TNTILE::nullset_type& data)
{
}

template<class Archive>
void serialize(Archive & ar, TNTILE::stddata_type& data)
{
	ar	& data.id
		& data.color
		& data.modifier;
}

template<class Archive>
void serialize(Archive & ar, TNTILE::chardata_type& data)
{
	ar	& data.ch
		& data.color
		& data.modifier;
}



} // namespace serialization



#endif
