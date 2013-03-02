#pragma once
#ifndef _TN_SETTINGS_H
#define _TN_SETTINGS_H

#include <string>

#ifdef TILENET_AS_LIB
#	define TNAPI
#else
#	ifdef WIN32
#		ifdef TILENET_EXPORTS
#			define TNAPI __declspec(dllexport)
#		else
#			define TNAPI __declspec(dllimport)
#		endif
#	else
#		define TNAPI __attribute__ ((__visibility__ ("default")))
#	endif
#endif

namespace tn{
typedef wchar_t	wchar;
using ::std::string;
using ::std::wstring;
typedef unsigned char uchar;
typedef unsigned int uint;


namespace helper {
template<typename T, char c, wchar wc>
struct CharTaker { static const char C = c; };
template<char c, wchar wc>
struct CharTaker<wchar, c, wc> { static const wchar C = wc; };

template<typename T>
struct StringTaker {const char* String; StringTaker(const char* str, const wchar* wstr):String(str){} };
template<>
struct StringTaker<wchar>{const wchar* String; StringTaker(const char* str, const wchar* wstr):String(wstr){} };

#define TN_CHAR_TAKE(ty, ch)	(tn::helper::CharTaker<ty, ch, L##ch>::C)
#define TN_STRING_TAKE(ty, str)	(tn::helper::StringTaker<ty>(str, L##str).String)

wstring TNAPI ToWideString(const string& str);
string TNAPI ToAnsiString(const wstring& str);
}


class TnException
{
public:
	inline TnException(const string& msg):mMsg(helper::ToWideString(msg)){}
	inline TnException(const wstring& msg):mMsg(msg){}

	const string msg() const { return helper::ToAnsiString(mMsg); }
	const wstring& wmsg() const { return mMsg; }
private:
	wstring mMsg;
};


}

#include "tilenet/vector.h"

namespace tn {
inline  Ratio StdRatio() { return Ratio(100, 100); }

class TilenetConfig
{
public:
	TNAPI TilenetConfig();

	string serverName;			// Der Name des Servers
	string serverInfo;			// Eine Kleine Info ueber den Server

	string tilePackageName;		// Der Name/Bezeichner eines Tilesets welches benutzt werden soll
	string tilePackageInterface;// Interface ueber den passende Packet gefunden werden koennen

	string			serverPassword;
	uint			maxConnections;		// Anzahl der maximalen Verbindungen (>0 !)
	unsigned short	portBind;			// Der Port der fuer den Server benutzt werden soll

	bool standaloneServer;		// Gibt an ob ein Clientfenster gestartet werden soll

	bool useServerNetwork;		// Gibt an ob ein lokales Servernetzwerk benutzt werden soll
	
	
	bool setAsActive;			// Setzt den zu erzeugenden Server als Activen
//	bool createStandartLayer;	// Bestimmt ob ein Standartlayer erzeugt werden soll
};

namespace key {
	enum SpecialKey {
		Num0,         ///< The 0 key
		Num1,         ///< The 1 key
		Num2,         ///< The 2 key
		Num3,         ///< The 3 key
		Num4,         ///< The 4 key
		Num5,         ///< The 5 key
		Num6,         ///< The 6 key
		Num7,         ///< The 7 key
		Num8,         ///< The 8 key
		Num9,         ///< The 9 key
		Escape,       ///< The Escape key
		LControl,     ///< The left Control key
		LShift,       ///< The left Shift key
		LAlt,         ///< The left Alt key
		LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
		RControl,     ///< The right Control key
		RShift,       ///< The right Shift key
		RAlt,         ///< The right Alt key
		RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
		Menu,         ///< The Menu key
		LBracket,     ///< The [ key
		RBracket,     ///< The ] key
		SemiColon,    ///< The ; key
		Comma,        ///< The , key
		Period,       ///< The . key
		Quote,        ///< The ' key
		Slash,        ///< The / key
		BackSlash,    ///< The \ key
		Tilde,        ///< The ~ key
		Equal,        ///< The = key
		Dash,         ///< The - key
		Space,        ///< The Space key
		Return,       ///< The Return key
		Back,         ///< The Backspace key
		Tab,          ///< The Tabulation key
		PageUp,       ///< The Page up key
		PageDown,     ///< The Page down key
		End,          ///< The End key
		Home,         ///< The Home key
		Insert,       ///< The Insert key
		Delete,       ///< The Delete key
		Add,          ///< +
		Subtract,     ///< -
		Multiply,     ///< *
		Divide,       ///< /
		Left,         ///< Left arrow
		Right,        ///< Right arrow
		Up,           ///< Up arrow
		Down,         ///< Down arrow
		Numpad0,      ///< The numpad 0 key
		Numpad1,      ///< The numpad 1 key
		Numpad2,      ///< The numpad 2 key
		Numpad3,      ///< The numpad 3 key
		Numpad4,      ///< The numpad 4 key
		Numpad5,      ///< The numpad 5 key
		Numpad6,      ///< The numpad 6 key
		Numpad7,      ///< The numpad 7 key
		Numpad8,      ///< The numpad 8 key
		Numpad9,      ///< The numpad 9 key
		F1,           ///< The F1 key
		F2,           ///< The F2 key
		F3,           ///< The F3 key
		F4,           ///< The F4 key
		F5,           ///< The F5 key
		F6,           ///< The F6 key
		F7,           ///< The F7 key
		F8,           ///< The F8 key
		F9,           ///< The F8 key
		F10,          ///< The F10 key
		F11,          ///< The F11 key
		F12,          ///< The F12 key
		F13,          ///< The F13 key
		F14,          ///< The F14 key
		F15,          ///< The F15 key
		Pause,        ///< The Pause key

		KeyCount      ///< Keep last -- the total number of keyboard keys
	};
}

class KeyEvent
{
public:
	enum KeyModifier
	{
		SpecialKey	= 0x01,
		Pressed		= 0x02,
		Repeated	= 0x04,
		SystemKey	= 0x08,
		AltKey		= 0x10,
		ControlKey	= 0x20,
		ShiftKey	= 0x40,
	};
public:
	inline KeyEvent(unsigned char modifier, wchar_t code)
		:	mModifier(modifier),
			mCode(code)
	{}

	inline key::SpecialKey	keyCode()		const { return key::SpecialKey(mCode); }
	inline wchar_t			charCode()		const { return mCode; }
	inline unsigned char	modifier()		const { return mModifier; }
	inline bool				isSpecialKey()	const { return !!(SpecialKey & modifier());}
	inline bool				isKeyPressed()	const { return !!(Pressed & modifier());}
	inline bool				isRepeatedKey() const { return !!(Repeated & modifier());}
	inline bool				withSystemKey() const { return !!(SystemKey & modifier());}
	inline bool				withAltKey()	const { return !!(AltKey & modifier());}
	inline bool				withShiftKey()	const { return !!(ShiftKey & modifier());}
	inline bool				withControlKey() const { return !!(ControlKey & modifier());}

private:
	const unsigned char	mModifier;
	const wchar_t		mCode;
};

class Layer;
class Display;
class Tilenet;


class Display
{
public:
	enum Level{
		TopMost,
		BottomMost
	};

public:

	virtual Layer* createLayer(const Point& position, const Rect& size, Level lvl = TopMost) = 0;
	virtual Layer* createLayer(const Point& position, const Ratio& positionRatio, const Rect& size, const Ratio& sizeRatio, Level lvl = TopMost) = 0;

	virtual Layer* topLayer() const = 0;
	virtual Layer* bottomLayer() const = 0;
	virtual Tilenet* tilenet() const = 0;

	virtual Layer* end() const = 0;
};


class Layer
{
public:
	enum Level
	{
		Above,
		Below
	};
public:
	virtual void setTile(const Point& pos, uint tile) = 0;
	virtual void put(const Point& pos, uint tile, uint color = 0xFFFFFFFF) = 0;
	virtual uint getTile(const Point& pos) const = 0;
	virtual uint getColor(const Point& pos) const = 0;

	virtual void setPosition(const Point& pos) = 0;
	virtual void setSize(const Rect& size) = 0;

	virtual Rect size() const = 0;
	virtual Point position() const = 0;
	virtual Ratio sizeRatio() const = 0;
	virtual Ratio positionRatio() const = 0;

	virtual Layer* above() const = 0;
	virtual Layer* below() const = 0;

	virtual Layer* createLayer(const Point& position, const Rect& size, Level lvl = Above) = 0;
	virtual Layer* createLayer(const Point& position, const Ratio& positionRatio, const Rect& size, const Ratio& sizeRatio, Level lvl = Above) = 0;
	virtual Display* display() const = 0;
	virtual Tilenet* tilenet() const = 0;

	virtual bool isEnd() const = 0;
};



class Participant
{
public:

	virtual Display* attachDisplay(Display* display) = 0;
	virtual Display* detachDisplay() = 0;
	virtual Display* getDisplay() = 0;

	virtual bool kick() = 0;

	inline static bool Release(Participant*& p)
	{
		bool erg = p->releaseMe();

		p = nullptr;
		return erg;
	}

protected:
	virtual bool releaseMe() = 0;
};


class Tilenet
{
public:
	class EventHandler;

public:
	virtual Participant* acquireParticipant(uint p) = 0;

	virtual uint tile(const string& name) = 0;
	virtual uint tile(const wstring& name) = 0;

	virtual Display*	createDisplay() = 0;
	virtual bool		destroyDisplay(Display* display) = 0;

	virtual void start() = 0;
	virtual unsigned short port() const = 0;

	virtual bool registerEventHandler(EventHandler* handler) = 0;
	virtual bool unregisterEventHandler(EventHandler* handler) = 0;
	virtual uint unregisterAllEventHandlers() = 0;
	virtual uint eventHandlerCount() = 0;

	virtual uint yield() = 0;

	static TNAPI Tilenet* Create(TilenetConfig& config);

	static TNAPI Tilenet* Create(string serverName, string serverInfo, string tilePackageName, uint   tilePackageVersion,
							string playerPassword, string observerPassword, uint waitForOnePlayerSec, uint maxPlayers,
							bool useAutoObservation, uint maxObserver, bool standaloneServer, bool useServerNetwork,
							unsigned short portBind, bool setAsActive, bool createStandartLayer);

	static TNAPI Tilenet* Create(wstring serverName, wstring serverInfo, wstring tilePackageName, uint tilePackageVersion,
							wstring playerPassword, wstring observerPassword, uint waitForOnePlayerSec, uint maxPlayers,
							bool useAutoObservation, uint maxObserver, bool standaloneServer, bool useServerNetwork,
							unsigned short portBind, bool setAsActive, bool createStandartLayer);


	static TNAPI void SetLoggingLevel(int lvl);
	//static void Wait(long milliseconds);
};

class Tilenet::EventHandler
{
public:
	struct Bitmask {
		enum {
			HandleNewParticipant		= 0x01,
			HandleParticipantDisconnect = 0x02,
			HandleKeyEvent				= 0x04
		};
	};
public:
	virtual uint eventBitmask() = 0;
	virtual bool handleNewParticipant(uint p) = 0;
	virtual bool handleParticipantDisconnect(uint p) = 0;
	virtual bool handleKeyEvent(uint p, const KeyEvent& e) = 0;

	virtual void released() = 0;
};

}




#endif
