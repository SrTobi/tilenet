#pragma once
#ifndef _PROTOCOL_TILE
#define _PROTOCOL_TILE


#include <boost/serialization/access.hpp>
#include "settings.hpp"



namespace net {



class PTile
{
	friend class boost::serialization::access;
public:
	enum TileType
	{
		NullTileType,
		StdTileType,
		CharTileType
	};

private:
	struct DataContainer
	{
		virtual ~DataContainer() {}
		template<typename Archive>
		void serialize(Archive& ar, unsigned int)
		{
		}

		virtual TileType type() const = 0;
		virtual std::unique_ptr<DataContainer> clone() const = 0;
	};

	template<typename T>
	struct TileDataContainer
		: public DataContainer
	{
		virtual TileType type() const
		{
			return T::Type;
		}

		virtual std::unique_ptr<DataContainer> clone() const
		{
			return std::unique_ptr<DataContainer>(new TileDataContainer(*this));
		}

		T data;
	};

public:
	PTile();
	PTile(const PTile& other);
	PTile(PTile&& other);
	PTile(TNID tileset_id, TNID tile_id, TNFLAG modifier, TNCOLOR color);
	PTile(TNID tileset_id, wchar_t c, TNFLAG modifier, TNCOLOR color);
	~PTile();

	PTile& operator =(const PTile& other);
	PTile& operator =(const PTile&& other);

	void assign(const PTile& other);
	void assign(PTile&& other);

	TileType type() const;

	template<typename T>
	const T& data() const
	{
		const TileDataContainer<T>* data = dynamic_cast<const TileDataContainer<T>*>(mData.get());
		tnAssert(data);
		return data->data;
	}

private:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int version)
	{

	}


private:
	std::unique_ptr<DataContainer> mData;
};

struct StdTileData
{
	static const PTile::TileType Type = PTile::StdTileType;

	TNID tileset_id;
	TNID tile_id;
	TNFLAG modifier;
	TNCOLOR color;
};


struct CharTileData
{
	static const PTile::TileType Type = PTile::CharTileType;

	TNID tileset_id;
	wchar_t character;
	TNFLAG modifier;
	TNCOLOR color;
};







}


#endif