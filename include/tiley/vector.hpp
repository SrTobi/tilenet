#pragma once
#ifndef _TILEY_VECTOR_HPP
#define _TILEY_VECTOR_HPP

#include <cmath>
#include <sstream>


namespace tiley {



template<typename T>
class basic_vector2
{
public:
	basic_vector2()	: x(T(0)), y(T(0))									{}
	basic_vector2(const basic_vector2& _v)	: x(_v.x), y(_v.y)			{}
	basic_vector2(const T _v)				: x(_v), y(_v)				{}
	basic_vector2(const T _x, const T _y)	: x(_x), y(_y)				{}


	operator float* () {return (float*)(c);}


	basic_vector2& operator = (const basic_vector2& v)	{x = v.x; y = v.y;		return (*this);}
	basic_vector2& operator += (const basic_vector2& v)	{x += v.x; y += v.y;	return (*this);}
	basic_vector2& operator -= (const basic_vector2& v)	{x -= v.x; y -= v.y;	return (*this);}
	basic_vector2& operator *= (const basic_vector2& v)	{x *= v.x; y *= v.y;	return (*this);}
	basic_vector2& operator *= (const T f)				{x *= f; y *= f;		return (*this);}
	basic_vector2& operator /= (const basic_vector2& v)	{x /= v.x; y /= v.y;	return (*this);}
	basic_vector2& operator /= (const T f)				{x /= f; y /= f;		return (*this);}


	inline basic_vector2 operator + (const basic_vector2& v)	const	{return basic_vector2(x + v.x, y + v.y);}
	inline basic_vector2 operator - (const basic_vector2& v)	const	{return basic_vector2(x - v.x, y - v.y);}
	inline basic_vector2 operator - ()							const	{return basic_vector2(-x, -y);}
	inline basic_vector2 operator * (const basic_vector2& v)	const	{return basic_vector2(x * v.x, y * v.y);}
	inline basic_vector2 operator * (const T f)					const	{return basic_vector2(x * f, y * f);}
	inline basic_vector2 operator / (const basic_vector2& v)	const	{return basic_vector2(x / v.x, y / v.y);}
	inline basic_vector2 operator / (const T f)					const	{return basic_vector2(x / f, y / f);}


	inline bool operator == (const basic_vector2& v) const {return (x == v.x && y == v.y);}
	inline bool operator != (const basic_vector2& v) const {return (x != v.x || y != v.y);}

	inline double distance(const basic_vector2& v) const { return std::sqrt(distanceQuad(v));}
	inline double distanceQuad(const basic_vector2& v) const { basic_vector2<double> delta(v - *this); delta*=delta; return delta.x + delta.y; }
	inline bool isInRange(const basic_vector2& v, double epsilon = 0.000000001) const { return distanceQuad(v) < epsilon; }
	inline bool isIn(const basic_vector2& size) const { return 0 <= x && x < size.w && 0 <= y && y < size.h; }
	inline bool isIn(const basic_vector2& pos, const basic_vector2& size) const { return (*this - pos).isIn(size); }

	operator basic_vector2<int>	 ()		const {return basic_vector2<int>(static_cast<int>(x), static_cast<int>(y));}
	operator basic_vector2<float> ()	const {return basic_vector2<float>(static_cast<float>(x), static_cast<float>(y));}
	operator basic_vector2<double>()	const {return basic_vector2<double>(static_cast<double>(x), static_cast<double>(y));}

	inline T area() const { return w * h; }

	union
	{
		struct
		{
			T x;
			T y;
		};

		struct
		{
			T lon;
			T lat;
		};

		struct
		{
			T w;
			T h;
		};
		
		T c[2];
	};
};


template<typename T>
inline basic_vector2<T> operator * (const T f, const basic_vector2<T>& v)		{return basic_vector2<T>(v.x * f, v.y * f);}
template<typename T>
inline basic_vector2<T> operator / (const T f, const basic_vector2<T>& v)		{return basic_vector2<T>(v.x / f, v.y / f);}


template<typename T>
inline basic_vector2<T> compmin(const basic_vector2<T> first, const basic_vector2<T>& second)
{
	return basic_vector2<T>(std::min(first.x, second.x), std::min(first.y, second.y));
}

template<typename T>
inline basic_vector2<T> compmax(const basic_vector2<T> first, const basic_vector2<T>& second)
{
	return basic_vector2<T>(std::max(first.x, second.x), std::max(first.y, second.y));
}


/*
template<typename T, typename C>
inline std::basic_istream<C>& operator >> (std::basic_istream<C>& is, basic_vector2<T>& v)
{
	C op;

	is >> v.x;
	is >> op;
	is >> v.y;

	if(op !=  C(',') && op != C('x') && op != C('|') || is.bad())
	{
		throw std::exception("Not able to convert from stream to vector!");
	}

	return is;
}*/

template<typename T, typename C>
std::basic_ostream<C>& operator << (std::basic_ostream<C>& os, const basic_vector2<T>& v)
{
	os << v.x << C(',') << C(' ') << v.y;
	return os;
}

typedef basic_vector2<int>			Point;
typedef basic_vector2<int>			Rect;
typedef basic_vector2<float>		Ratio;
typedef basic_vector2<float>		FPoint;


}




#endif
