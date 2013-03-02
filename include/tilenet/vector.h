#pragma once
#ifndef _TN_VECTOR_H
#define _TN_VECTOR_H

#include <vector>
#include <sstream>
namespace tn {

template<typename T>
class basic_vector2
{
public:
	basic_vector2()	: x(T(0)), y(T(0))									{}
	basic_vector2(const basic_vector2& _v)	: x(_v.x), y(_v.y)			{}
	basic_vector2(const T _v)				: x(_v), y(_v)				{}
	basic_vector2(const T _x, const T _y)	: x(_x), y(_y)				{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}

	// Zuweisungsoperatoren
	basic_vector2& operator = (const basic_vector2& v)	{x = v.x; y = v.y;		return (*this);}
	basic_vector2& operator += (const basic_vector2& v)	{x += v.x; y += v.y;	return (*this);}
	basic_vector2& operator -= (const basic_vector2& v)	{x -= v.x; y -= v.y;	return (*this);}
	basic_vector2& operator *= (const basic_vector2& v)	{x *= v.x; y *= v.y;	return (*this);}
	basic_vector2& operator *= (const T f)				{x *= f; y *= f;		return (*this);}
	basic_vector2& operator /= (const basic_vector2& v)	{x /= v.x; y /= v.y;	return (*this);}
	basic_vector2& operator /= (const T f)				{x /= f; y /= f;		return (*this);}

	// Arithmetische Operatoren
	inline basic_vector2 operator + (const basic_vector2& v)	const	{return basic_vector2(x + v.x, y + v.y);}
	inline basic_vector2 operator - (const basic_vector2& v)	const	{return basic_vector2(x - v.x, y - v.y);}
	inline basic_vector2 operator - ()							const	{return basic_vector2(-x, -y);}
	inline basic_vector2 operator * (const basic_vector2& v)	const	{return basic_vector2(x * v.x, y * v.y);}
	inline basic_vector2 operator * (const T f)					const	{return basic_vector2(x * f, y * f);}
	inline basic_vector2 operator / (const basic_vector2& v)	const	{return basic_vector2(x / v.x, y / v.y);}
	inline basic_vector2 operator / (const T f)					const	{return basic_vector2(x / f, y / f);}

	// Vergleichsoperatoren
	inline bool operator == (const basic_vector2& v) const {if(x != v.x) return false;	return (y == v.y);}
	inline bool operator != (const basic_vector2& v) const {if(x != v.x) return true;	return (y != v.y);}

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
			T w;
			T h;
		};
		
		T c[2];
	};
};


// Arithmetische Operatoren
template<typename T>
inline basic_vector2<T> operator * (const T f, const basic_vector2<T>& v)		{return basic_vector2<T>(v.x * f, v.y * f);}
template<typename T>
inline basic_vector2<T> operator / (const T f, const basic_vector2<T>& v)		{return basic_vector2<T>(v.x / f, v.y / f);}





template<typename T, typename C>
inline std::basic_istream<C>& operator >> (std::basic_istream<C>& is, basic_vector2<T>& v)
{
	C op;

	is >> v.x;
	is >> op;
	is >> v.y;

	if(op !=  TN_CHAR_TAKE(C, ',') && op != TN_CHAR_TAKE(C, 'x') && op != TN_CHAR_TAKE(C, '|') || is.bad())
	{
		throw TnException(L"Not able to convert from stream to vector!");
	}

	return is;
}

template<typename T, typename C>
std::basic_ostream<C>& operator << (std::basic_ostream<C>& os, const basic_vector2<T>& v)
{
	os << v.x << TN_STRING_TAKE(C, ", ") << v.y;
	return os;
}


typedef basic_vector2<int>			Rect;
typedef basic_vector2<int>			Point;
typedef basic_vector2<unsigned int> Ratio;
typedef basic_vector2<float>		Vector;
}

#endif
