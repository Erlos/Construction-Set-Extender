#pragma once

#ifdef CSE
	#include "CSAS\ScriptCommandInfo.h"
	#include "CSAS\Array.h"
#endif

using namespace CSAutomationScript;

/********** CSE Interface API ****************************************************
*	Interface object passed through the OBSE messaging system. A pointer to the 
*	object will be dispatched to plugins that pass an arbitrary message of type 
*	'CSEI' post post-plugin load (reply will be of the same type).
*	
*	All other interfaces need to be initialized by calling InitializeInterface().
*********************************************************************************/

struct CSEInterface
{
	enum 
	{
		kCSEInterface_Console = 0,
		kCSEInterface_IntelliSense,
		kCSEInterface_Renderer,
		kCSEInterface_Script
	};

	// Used to initialize CSE's interface objects. Similar to OBSE's QueryInterface.
	void*			(* InitializeInterface)(UInt8 InterfaceType);
};

struct CSEIntelliSenseInterface
{
	// Registers an arbitrary URL to a script command. Registered URLs will be displayed in the 
	// script editor's context menu when the corresponding command is selected.
	void			(* RegisterCommandURL)(const char* CommandName, const char* URL);
};

struct CSEConsoleInterface
{
	typedef void	(* ConsolePrintCallback)(const char* Message, const char* Prefix);

	// Prints a message to the console. Prefix can be an abbreviated string of any kind.
	// Printed messages will be of the following format: [<Prefix>]\t\t<Message>
	// Mustn't be called inside a callback.
	//
	// Reserved Prefixes: CMD, CSE, SE, CS, BSAV, USE, BE, TAG
	void			(* PrintToConsole)(const char*	Prefix, const char* FormatString, ...);
	// Registers a handler that gets called whenever a message is printed to the console.
	void			(* RegisterCallback)(ConsolePrintCallback Handler);
};

struct CSERendererInterface
{
	// Queues a text message for display in the render window
	void			(* PrintToRenderWindow)(const char* Message, long double DisplayDuration);
};

struct CSEScriptInterface
{
	// Registers a CSAS script command. Returns false if registration fails (in case of command name/alias conflicts)
	bool			(* RegisterCommand)(CSASCommandInfo* CommandInfo);

	// convenience function similar to OBSE's ExtractArgs/Ex
	// string data buffers passed to the function are expected to be large enough to hold their arguments
	bool			(* ExtractCommandArgs)(CSASDataElement* ArgArray, CSASParamInfo* Parameters, UInt32 NoOfParams, ...);

	// array helper functions
	// not to be used directly, other than for initializing the global instances of the fn ptrs
	Array*			(* ArrayInterface_AllocateNewArray)(CSASDataElement* Element, UInt32 Size);
	Array*			(* ArrayInterface_CopyNewArray)(Array* Source);
	void			(* ArrayInterface_ReleaseArray)(Array* Source);
};