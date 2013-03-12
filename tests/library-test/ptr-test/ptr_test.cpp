#include "test_settings.hpp"
#include "utils/tilenet_object.hpp"

enum ptr_test_events
{
	ObjectCreated,
	ObjectDestroyed
};

typedef MockObserver<ptr_test_events> PtrMockObs;


class PtrTestObject
	: public TilenetObject
{
public:
	PtrTestObject(PtrMockObs mock)
		: mock(mock)
	{
		mock.expect(ObjectCreated);
	}

	~PtrTestObject()
	{
		mock.expect(ObjectDestroyed);
	}

	PtrMockObs mock;
};


BOOST_AUTO_TEST_CASE(simple_ptr_test)
{
	PtrMockObs mock;
	mock.set()	<< ObjectCreated
				<< ObjectDestroyed;

	{
		ptr<PtrTestObject> p = new PtrTestObject(mock);
	}
}


BOOST_AUTO_TEST_CASE(nested_ptr_test_1)
{
	PtrMockObs mock;
	mock.set()	<< ObjectCreated
				<< ObjectDestroyed;

	{
		ptr<PtrTestObject> gp;
		BOOST_CHECK(!gp);
		{
			ptr<PtrTestObject> p = new PtrTestObject(mock);
			BOOST_REQUIRE(p);
			BOOST_CHECK_EQUAL(p->refCount(), 1);
			BOOST_CHECK_EQUAL(p->weakCount(), 0);
			{
				ptr<PtrTestObject> ip = p;
				BOOST_REQUIRE(ip);
				BOOST_CHECK_EQUAL(ip.get(), p.get());
				BOOST_CHECK_EQUAL(ip->refCount(), 2);
				BOOST_CHECK_EQUAL(ip->weakCount(), 0);

				gp = ip;
				BOOST_REQUIRE(gp);
				BOOST_CHECK_EQUAL(ip.get(), gp.get());
				BOOST_CHECK_EQUAL(gp->refCount(), 3);
				BOOST_CHECK_EQUAL(gp->weakCount(), 0);
			}

			BOOST_CHECK_EQUAL(p.get(), gp.get());
			BOOST_CHECK_EQUAL(p->refCount(), 2);
			BOOST_CHECK_EQUAL(p->weakCount(), 0);
		}
		BOOST_CHECK(gp);
		BOOST_CHECK_EQUAL(gp->refCount(), 1);
		BOOST_CHECK_EQUAL(gp->weakCount(), 0);
	}
}