#include "includes.hpp"


#include "test_settings.hpp"
#include "utils/link_graph.hpp"



class SimpleLinkHolder
{
public:

	graph::Link<SimpleLinkHolder> link;

	static shared_ptr<SimpleLinkHolder> Create()
	{
		shared_ptr<SimpleLinkHolder> p = std::make_shared<SimpleLinkHolder>();
		p->link.bind(p);

		return p;
	}
};

typedef SimpleLinkHolder SLH;



BOOST_AUTO_TEST_CASE(simple_link_test1)
{
	auto h1 = SLH::Create();
	auto h2 = SLH::Create();

	h1->link.link(h2->link);
	BOOST_CHECK_EQUAL(h1->link.size(), 1);
	BOOST_CHECK_EQUAL(h2->link.size(), 1);
	BOOST_CHECK(h1->link.has(h2));
	BOOST_CHECK(h2->link.has(h1));
	BOOST_CHECK(h1->link.has(h2->link));
	BOOST_CHECK(h2->link.has(h1->link));
}


BOOST_AUTO_TEST_CASE(simple_link_test2)
{
	auto h1 = SLH::Create();
	auto h2 = SLH::Create();
	auto h3 = SLH::Create();

	h1->link.link(h2->link);
	h3->link.link(h2->link);

	BOOST_CHECK_EQUAL(h1->link.size(), 1);
	BOOST_CHECK_EQUAL(h2->link.size(), 2);
	BOOST_CHECK_EQUAL(h3->link.size(), 1);

	BOOST_CHECK(h2->link.has(h1));
	BOOST_CHECK(h2->link.has(h3));
	BOOST_CHECK(!h1->link.has(h3));
	BOOST_CHECK(!h3->link.has(h1));
	BOOST_CHECK(h1->link.has(h2));
	BOOST_CHECK(h3->link.has(h2));

	h2.reset();

	BOOST_CHECK_EQUAL(h1->link.size(), 0);
	BOOST_CHECK_EQUAL(h3->link.size(), 0);
}


BOOST_AUTO_TEST_CASE(simple_ulink_test1)
{
	auto h1 = SLH::Create();
	auto h2 = SLH::Create();

	h1->link.link(h2->link);
	h1->link.unlink(h2);
	BOOST_CHECK_EQUAL(h1->link.size(), 0);
	BOOST_CHECK_EQUAL(h2->link.size(), 0);
}

BOOST_AUTO_TEST_CASE(simple_destructor_test1)
{
	auto h1 = SLH::Create();
	auto h2 = SLH::Create();

	h1->link.link(h2->link);

	h2.reset();

	BOOST_CHECK_EQUAL(h1->link.size(), 0);
}