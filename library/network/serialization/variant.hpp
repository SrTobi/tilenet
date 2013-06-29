#pragma once
#ifndef _TN_SERIALIZATION_VARIANT_HPP
#define _TN_SERIALIZATION_VARIANT_HPP



#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/empty.hpp>

#include <boost/variant.hpp>


namespace serialization {

namespace detail {

template<class S>
struct variant_save_visitor: boost::static_visitor<> 
{
	variant_save_visitor(S& ar) :
		m_ar(ar)
	{}
	template<class T>
	void operator()(T const & value) const
	{
		m_ar << value;
	}
private:
	S& m_ar;
};



template<class S>
struct variant_impl {

	struct load_null {
		template<class Archive, class V>
		static void invoke(
			Archive & /*ar*/,
			int /*which*/,
			V & /*v*/
			){}
	};

	struct load_impl {
		template<class Archive, class V>
		static void invoke(
			Archive & ar,
			int which,
			V & v
			){
				if(which == 0){
					// note: A non-intrusive implementation (such as this one)
					// necessary has to copy the value.  This wouldn't be necessary
					// with an implementation that de-serialized to the address of the
					// aligned storage included in the variant.
					typedef BOOST_DEDUCED_TYPENAME boost::mpl::front<S>::type head_type;
					head_type value;
					ar >> value;
					v = value;
					//ar.reset_object_address(& boost::get<head_type>(v), & value);
					return;
				}
				typedef BOOST_DEDUCED_TYPENAME boost::mpl::pop_front<S>::type type;
				variant_impl<type>::load(ar, which - 1, v);
		}
	};

	template<class Archive, class V>
	static void load(
		Archive & ar,
		int which,
		V & v
		){
			typedef BOOST_DEDUCED_TYPENAME boost::mpl::eval_if<boost::mpl::empty<S>,
				boost::mpl::identity<load_null>,
				boost::mpl::identity<load_impl>
			>::type typex;
			typex::invoke(ar, which, v);
	}

};

}

template<class S, BOOST_VARIANT_ENUM_PARAMS(/* typename */ class T)>
void save(S& ar, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> const & v)
{
	int which = v.which();
	ar << which;
	typedef BOOST_DEDUCED_TYPENAME  boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>::types types;
	detail::variant_save_visitor<S> visitor(ar);
	v.apply_visitor(visitor);
}



template<class S, BOOST_VARIANT_ENUM_PARAMS(/* typename */ class T)>
void load(S& ar,boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& v)
{
		int which;
		typedef BOOST_DEDUCED_TYPENAME boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>::types types;
		ar >> which;

		if(which >=  boost::mpl::size<types>::value)
			// this might happen if a type was removed from the list of variant types
		{
			NOT_IMPLEMENTED();
		}

		detail::variant_impl<types>::load(ar, which, v);
}




template<class S, BOOST_VARIANT_ENUM_PARAMS(/* typename */ class T)>
inline void serialize(S & ar, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> & v)
{	
	split(ar,v);
}




}




#endif
