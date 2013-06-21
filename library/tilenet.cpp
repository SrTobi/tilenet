#include "includes.hpp"

#include <locale>
#include <assert.h>
#include <boost/thread/tss.hpp>
#include <boost/unordered_map.hpp>
#include <boost/variant/variant.hpp>

#include "tilenet.h"
#include "settings.hpp"

#include "server/server.hpp"
#include "server/service.hpp"

#include "server/layer.hpp"
#include "server/tile_layer.hpp"

#include "server/std_tileset.hpp"

#include "server/participant.hpp"

// Acceptors
#include "server/acceptors/listen_acceptor.hpp"
#include "server/acceptors/local_acceptor.hpp"

// Jobs
#include "server/jobs/service_job.hpp"
#include "server/jobs/attach_layer_job.hpp"

// Key stuff
#include "network/protocol.hpp"

/// @cond DEV

struct ThreadErrorInfo
{
	typedef boost::variant<string, int> value_type;


	ThreadErrorInfo(TNERROR code, const string& description)
		: errorcode(code)
	{
		infos.insert(std::make_pair(TNERRI_DESCRIPTION, description));
	}


	const TNERROR errorcode;
	boost::unordered_map<TNERRINFO, value_type> infos;
};

boost::thread_specific_ptr<ThreadErrorInfo> LastThreadError;


void add_errinfo(TNERRINFO info, int i)
{
	tnAssert(LastThreadError.get());
	tnAssert(LastThreadError->infos.count(info) == 0);
	LastThreadError->infos.insert(std::make_pair(info, i));
}

void add_errinfo(TNERRINFO info, const string& str)
{
	tnAssert(LastThreadError.get());
	tnAssert(LastThreadError->infos.count(info) == 0);
	LastThreadError->infos.insert(std::make_pair(info, str));
}

void reset_error(TNERROR code, const string& description)
{
	LastThreadError.reset(new ThreadErrorInfo(code, description));
}


void reset_error(const excp::CodeException& exp)
{
	reset_error(exp.getErrorCode(), exp.why());
}

template<typename Type>
class VisitorForInfoType{};

template<>
class VisitorForInfoType<int> : public boost::static_visitor<int>
{
public:
	int operator()(int i)				const { return i; }
	int operator()(const string& str)	const { BOOST_THROW_EXCEPTION(excp::WrongInfoTypeException()); }
};

template<>
class VisitorForInfoType<string> : public boost::static_visitor<string>
{
public:
	string operator()(int i)				const { BOOST_THROW_EXCEPTION(excp::WrongInfoTypeException()); }
	string operator()(const string& str)	const { return str; }
};

template<typename Target>
Target get_error(TNERRINFO code)
{
	if(!LastThreadError.get())
		BOOST_THROW_EXCEPTION(excp::NoErrorException());

	auto it = LastThreadError->infos.find(code);

	if(it == LastThreadError->infos.end())
		BOOST_THROW_EXCEPTION(excp::InfoNotSetException() << excp::InfoCode(code));

	return boost::apply_visitor(VisitorForInfoType<Target>(), it->second);
}

#define EXTRACT_ERROR_INFO(_expType)									\
	{																	\
		auto const*const p = boost::get_error_info<_expType>(*codeexcp);\
		if(p)															\
			add_errinfo(excp::get_infocode<_expType>::value, *p);		\
	}

TNERROR process_exception(const excp::ExceptionBase& exp, bool reset = true)
{
	const excp::CodeException* codeexcp = dynamic_cast<const excp::CodeException*>(&exp);

	TNERROR errorcode = TNINTERNALERROR;
	if(codeexcp)
	{
		errorcode = codeexcp->getErrorCode();
	}

	
	static Log log(L"Tilenet");
	log.error() << L"Err(" << errorcode << L"): " << exp.what();

	if(reset)
	{
		reset_error(errorcode, exp.why());

		if(codeexcp)
		{
			// Extract error infos
			EXTRACT_ERROR_INFO(excp::InfoCode);
			EXTRACT_ERROR_INFO(excp::CopiedEements);
			EXTRACT_ERROR_INFO(excp::BadId);
		}

#ifdef TILENET_DEBUG

		string info = lexical_convert<string>(boost::diagnostic_information(exp));

		add_errinfo(TNERRI_INTERNALDEBUGDESCRIPTION, info);
		log.error()
			<< "Error information:"
			<< "\n---------------------------\n"
			<< info
			<< "\n---------------------------\n";

#endif // TILENET_DEBUG
	}


	return errorcode;
}
#undef EXTRACT_ERROR_INFO


struct TrivialExtractor
{
	template<typename Type>
	Type operator ()(const Type& v)
	{
		return v;
	}
};

template<typename Iterator, typename Type, typename Extractor>
void copy_its(Iterator begin, const Iterator end, Type* dest, size_t buflen, size_t* copied, Extractor ex)
{
	tnAssert(copied);
	tnAssert(dest);
	tnAssert(buflen);
	size_t idx = 0;

	Iterator it;
	for(it = begin; it != end && idx < buflen; ++it)
	{
		dest[idx] = ex(*it);
		++idx;
	}

	*copied = idx;

	if(it != end)
		BOOST_THROW_EXCEPTION(excp::BufferUndersizedException() << excp::CopiedEements(idx));
}

void copy_string(const string& src, wchar_t* dest, size_t buflen)
{
	tnAssert(dest);
	tnAssert(buflen);

	size_t len = std::min(src.size(), buflen-1);

	memcpy(dest, src.c_str(), len * sizeof(wchar_t));
	dest[len] = L'\0';

	if(len < src.size())
		BOOST_THROW_EXCEPTION(excp::BufferUndersizedException() << excp::CopiedEements(len+1));
}


#define AUTO_CATCH(_reset_error)																		\
	catch(const excp::ExceptionBase& e) {																		\
		return process_exception(e, _reset_error);														\
	} catch(const std::exception& e)																			\
	{ 																									\
		if(_reset_error)																				\
			reset_error(TNINTERNALERROR, lexical_convert<string>(e.what()));							\
		return TNINTERNALERROR;																			\
	} catch(const boost::exception& e)																		\
	{ 																									\
		if(_reset_error)																				\
			reset_error(TNINTERNALERROR, lexical_convert<string>(boost::diagnostic_information(e)));	\
		return TNINTERNALERROR;																			\
	} catch(...)																						\
	{ 																									\
		return TNUNKNOWN;																				\
	}

#define CHECK_RETURN(_cond, _ret)	if(!(_cond)){return (_ret);}
#define CHECK_NULL(_expr)			CHECK_RETURN(_expr, TNNULLARG)
#define CHECK_EMPTY(_str)			CHECK_RETURN(*(_str), TNEMPTY)
#define CHECK_IF_ERROR()			CHECK_RETURN(LastThreadError.get(), TNNOERROR)
#define CHECK_CAST(_dest, _src, _type)	auto* _dest = dynamic_cast<_type*>(_src); CHECK_RETURN(_dest, TNBADTYPE);
#define CHECK_OBJ(_dest, _src, _type)	CHECK_CAST(_tmp_##_dest, _src, _type); auto _dest = _tmp_##_dest->self<_type>();
/// @endcond

/**
 * \defgroup Tilenet Global Tilenet functions
 * @{
 */


/**
 * @brief Returns an error info, which is a string
 *
 * \param infono The number, identifying the certain info
 * \param dest A buffer used as destination
 * \param buflen Size of the buffer indicating the maximum size of the output string
 * \return TNOK, if info was copied successfully
 * \return TNNOERROR, if there was no error in this thread
 * \return TNINFONOTSET, if no info with infono exists in the current error
 * \return TNWRONGINFOTYPE, if the error info is an int
 * \return TNBUFFERUNDERSIZED, if buflen was smaller then the string, which should be copied!
 *
 * \note This function will not modify the internal (error)state, even if it fails!
 *
 **/
TNAPI TNERROR tilenet_get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen )
{
	CHECK_NULL(dest);
	CHECK_NULL(buflen);
	CHECK_IF_ERROR();

	try {
		string infoString = get_error<string>(infono);
		copy_string(infoString, dest, buflen);

		return TNOK;

	} AUTO_CATCH(false);
}

/**
 * @brief Returns an error info, which is an int
 *
 * \param infono The number, identifying the certain info
 * \param dest A buffer used as destination
 * \return TNOK, if info was copied successfully
 * \return TNNOERROR, if there was no error in this thread
 * \return TNINFONOTSET, if no info with infono exists in the current error
 * \return TNWRONGINFOTYPE, if the error info is a string
 *
 * \note This function will not modify the internal (error)state, even if it fails!
 *
 **/
TNAPI TNERROR tilenet_get_error_int(TNERRINFO infono, int* dest )
{
	CHECK_NULL(dest);
	CHECK_IF_ERROR();

	try {
		*dest = get_error<int>(infono);

		return TNOK;

	} AUTO_CATCH(false);
}

/**
 * @brief Fills a list with all error information codes available at the current error
 *
 * \param dest A buffer list used as destination
 * \param buflen The size of the buffer, indicating the maximum codes that will be copied
 * \param copied Variable set to the number of info codes copied by this function
 * \return TNOK, if all codes were successfully copied.
 * \return TNNOERROR, if there was no error in this thread
 * \return TNBUFFERUNDERSIZED, if buflen was smaller then the number of codes, which should be copied!
 *
 *
 **/
TNAPI TNERROR tilenet_get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied )
{
	CHECK_NULL(dest);
	CHECK_NULL(buflen);
	CHECK_NULL(copied);
	CHECK_IF_ERROR();

	struct {
		TNERRINFO operator ()(const std::pair<TNERRINFO, ThreadErrorInfo::value_type>& p)
		{
			return p.first;
		}
	} extractor;

	try {
		copy_its(LastThreadError->infos.begin(), LastThreadError->infos.end(), dest, buflen, copied, extractor);

		return TNOK;
	} AUTO_CATCH(false);
}


/**
 * @brief Returns the last error code
 *
 * If no error occurred in this thread TNOK will be the code returned.
 *
 * \return The code of the last error occurred in the current thread.
 *
 * \note This function can not fail.
 * \note This function will not modify the internal (error)state!
 * \note Trivial argument errors are not stored
 *
 **/
TNAPI TNERROR tilenet_get_last_error()
{
	if(LastThreadError.get())
		return TNOK;

	return LastThreadError->errorcode;
}

TNAPI TNERROR tilenet_destroy(TNOBJ obj)
{
	CHECK_NULL(obj);

	try {
		TilenetObject::Destroy(obj);
		return TNOK;

	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_clone(TNOBJ src, TNOBJ* dest)
{
	CHECK_NULL(src);
	CHECK_NULL(dest);

	try {
		*dest = src->clone().get();
		return TNOK;

	} AUTO_CATCH(true);
}



TNAPI TNERROR tilenet_set_service_thread_count(size_t count)
{

	try {
		srv::Service::Inst().setThreadCount(count);
		return TNOK;

	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_create_server(TNSERVER* server, const TNSVRCONFIG* init)
{
	CHECK_NULL(server);
	CHECK_NULL(init);
	CHECK_NULL(init->name);
	CHECK_NULL(init->info);
	CHECK_NULL(init->pkg);
	CHECK_NULL(init->pkgi);

	try {
		*server = new ::srv::Server(init);
		return TNOK;

	} AUTO_CATCH(true);
}


TNAPI TNERROR tilenet_add_listen_acceptor(TNSERVER server, unsigned short port, unsigned int maxc)
{
	CHECK_NULL(server);
	CHECK_OBJ(_server, server, srv::Server);

	try {
		auto* acceptor = new srv::ListenAcceptor(port, maxc);
		acceptor->start();

		return TNOK;
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_add_local_acceptor(TNACCEPTOR* acceptor, TNSERVER server)
{
	CHECK_NULL(server);
	CHECK_OBJ(_server, server, srv::Server);

	try {
		auto* _acceptor = new srv::LocalAcceptor(_server->self<srv::Server>());
		_acceptor->start();

		if(acceptor)
		{
			*acceptor = _acceptor;
		}else{
			_acceptor->uncouple();
			tilenet_destroy(_acceptor);
		}

		return TNOK;
	} AUTO_CATCH(true);
}



TNAPI TNERROR tilenet_fetch_events(TNSERVER server, TNEVENT* dest, size_t buflen, size_t* fetched, size_t* timeout)
{
	CHECK_NULL(server);
	CHECK_NULL(dest);
	CHECK_NULL(buflen);
	CHECK_NULL(fetched);
	CHECK_OBJ(_server, server, srv::Server);

	try {
		size_t zero_time = 0;

		*fetched = 0;
		while(*fetched < buflen && _server->fetchNextEvent(dest + *fetched, timeout))
		{
			++(*fetched);
			timeout = &zero_time;
		}

		return TNOK;
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_kick( TNPARTICIPANT participant, const wchar_t* reason )
{
	try {
		shared_ptr<srv::Participant> p = srv::Participant::Resolve(participant);

		tnAssert(p);

		p->kick(reason);

		return TNOK;

	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_attach_layer( TNPARTICIPANT participant, TNLAYER layer )
{
	CHECK_NULL(layer);
	CHECK_CAST(_layer, layer, srv::Layer);

	try {
		shared_ptr<srv::Participant> p = srv::Participant::Resolve(participant);
		
		p->attachLayer(_layer->self<srv::Layer>());
		return TNOK;
	} AUTO_CATCH(true);
}


TNAPI TNERROR tilenet_create_frame( TNLAYER* frame, TNFLAG flags )
{
	try {
		NOT_IMPLEMENTED();

	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_clear_frame( TNLAYER frame )
{
	CHECK_NULL(frame);

	try {
		NOT_IMPLEMENTED();
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_frame_add( TNLAYER frame, TNLAYER layer, TNVIEW* view )
{
	CHECK_NULL(frame);
	CHECK_NULL(layer);

	try {
		NOT_IMPLEMENTED();
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_frame_remove( TNLAYER frame, TNLAYER layer )
{
	CHECK_NULL(frame);
	CHECK_NULL(layer);

	try {
		NOT_IMPLEMENTED();
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_update_view( TNVIEW view )
{
	CHECK_NULL(view);

	try {
		NOT_IMPLEMENTED();
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_create_tilelayer( TNLAYER* layer, unsigned int width, unsigned int height, TNRATIO xr, TNRATIO yr, TNFLAG flags )
{
	CHECK_NULL(layer);

	try {
		auto* l = new srv::TileLayer(Rect(width, height), Ratio(xr, yr), flags);
		srv::TileLayer::Register(l->self<srv::TileLayer>());
		l->makeInitialCommit();
		*layer = l;
		return TNOK;
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_update_tilelayer(TNLAYER layer, TNTILE* tiles, TNBOOL* toupdate)
{
	CHECK_NULL(tiles);
	CHECK_CAST(_layer, layer, srv::TileLayer);
	
	try {
		_layer->update(tiles, toupdate);

		return TNOK;
	} AUTO_CATCH(true);
}


TNAPI TNERROR tilenet_stdtile(const wchar_t* name, TNID* id )
{
	CHECK_NULL(name);
	CHECK_NULL(id);

	try {
		*id = srv::StdTileset::Inst().getTileId(name);
		return TNOK;
	} AUTO_CATCH(true);
}

TNAPI TNERROR tilenet_exit()
{
	srv::LocalAcceptor::WaitForClientExit();
	return TNOK;
}

TNAPI TNERROR tilenet_keycode( const wchar_t* name, TNKEYCODE* code )
{
	CHECK_NULL(name);
	if(!proto::curv::KeyMapper::Inst().toCode(name, code))
	{
		NOT_IMPLEMENTED();
	}
	return TNOK;
}

TNAPI TNERROR tilenet_keyname( TNKEYCODE code, const wchar_t** name )
{
	CHECK_NULL(code);
	auto& keyname = proto::curv::KeyMapper::Inst().toName(code);

	if(keyname.empty())
	{
		NOT_IMPLEMENTED();
	}else{
		*name = keyname.c_str();
	}

	return TNOK;
}


























/**
 * @}
 */
