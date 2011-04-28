#include "Misc.h"
#include "ScriptEditor.h"
#include "..\CSDialogs.h"
#include "..\ToolManager.h"
#include "..\WorkspaceManager.h"
#include "..\RenderWindowTextPainter.h"
#include "..\RenderSelectionGroupManager.h"
#include "..\GMSTMap.h"
#include "..\ArchiveManager.h"
#include "..\TESFormReferenceData.h"
#include "..\RenderTimeManager.h"

char g_NumericIDWarningBuffer[0x10] = {0};

_DefineHookHdlr(CSExit, 0x0041936E);
_DefineHookHdlrWithBuffer(CSInit, 0x00419260)(5, 0xE8, 0xEB, 0xC5, 0x2C, 0), 5);
_DefineNopHdlr(MissingTextureWarning, 0x0044F3AF, 14);
_DefineHookHdlr(AssertOverride, 0x004B5670);
_DefinePatchHdlr(TextureMipMapCheck, 0x0044F49B);
_DefinePatchHdlr(UnnecessaryDialogEdits, 0x004EDFF7);
_DefinePatchHdlr(UnnecessaryCellEdits, 0x005349A5);
_DefineHookHdlr(PluginSave, 0x0041BBCD);
_DefineHookHdlr(PluginLoad, 0x0041BEFA);
_DefineHookHdlr(DataHandlerClearData, 0x0047AE76);
_DefineJumpHdlr(TopicInfoCopyProlog, 0x004F0738, 0x004F07C4);
_DefineHookHdlr(TopicInfoCopyEpilog, 0x004F1280);
_DefineHookHdlr(NumericEditorID, 0x00497670);
_DefineHookHdlr(DataHandlerConstructSpecialForms, 0x00481049);
_DefineHookHdlr(ResultScriptSaveForm, 0x004FD258);
_DefineHookHdlr(TESObjectREFRDoCopyFrom, 0x0054763D);
_DefineHookHdlr(TESFormAddReference, 0x0049644B);
_DefineHookHdlr(TESFormRemoveReference, 0x00496494);
_DefineHookHdlr(TESFormClearReferenceList, 0x0049641E);
_DefineHookHdlr(TESFormPopulateUseInfoList, 0x004964F2);
_DefineHookHdlr(TESFormDelete, 0x00498712);
_DefinePatchHdlr(TextureSizeCheck, 0x0044F444);
_DefineHookHdlr(AboutDialog, 0x00441CC5);

void PatchMiscHooks(void)
{
	_MemoryHandler(CSExit).WriteJump();
	_MemoryHandler(CSInit).WriteJump();
	_MemoryHandler(PluginSave).WriteJump();
	_MemoryHandler(PluginLoad).WriteJump();
	_MemoryHandler(TextureMipMapCheck).WriteUInt8(0xEB);
	_MemoryHandler(UnnecessaryCellEdits).WriteUInt8(0xEB);
	_MemoryHandler(UnnecessaryDialogEdits).WriteUInt8(0xEB);
	_MemoryHandler(DataHandlerClearData).WriteJump();
	_MemoryHandler(TopicInfoCopyProlog).WriteJump();
	_MemoryHandler(TopicInfoCopyEpilog).WriteJump();
	_MemoryHandler(NumericEditorID).WriteJump();
	_MemoryHandler(DataHandlerConstructSpecialForms).WriteJump();
	_MemoryHandler(ResultScriptSaveForm).WriteJump();
	_MemoryHandler(TESObjectREFRDoCopyFrom).WriteJump();
	_MemoryHandler(TESFormAddReference).WriteJump();
	_MemoryHandler(TESFormRemoveReference).WriteJump();
	_MemoryHandler(TESFormClearReferenceList).WriteJump();
	_MemoryHandler(TESFormPopulateUseInfoList).WriteJump();
	_MemoryHandler(TESFormDelete).WriteJump();
	_MemoryHandler(TextureSizeCheck).WriteUInt8(0xEB);
	_MemoryHandler(AboutDialog).WriteJump();

	if (g_INIManager->FetchSetting("LogCSWarnings")->GetValueAsInteger())
		PatchMessageHandler();
	if (g_INIManager->FetchSetting("LogAssertions")->GetValueAsInteger())
		_MemoryHandler(AssertOverride).WriteJump();

	if (CreateDirectory(std::string(g_AppPath + "Data\\Backup").c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		DebugPrint("Couldn't create the Backup folder in Data directory");

	OSVERSIONINFO OSInfo;
	GetVersionEx(&OSInfo);
	if (OSInfo.dwMajorVersion >= 6)		// if running on Windows Vista/7, fix the listview selection sound
		RegDeleteKey(HKEY_CURRENT_USER , "AppEvents\\Schemes\\Apps\\.Default\\CCSelect\\.Current");
}

void PatchMessageHandler(void)
{
	SafeWrite32(kVTBL_MessageHandler + 0, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x4, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x8, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x10, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x14, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x18, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x1C, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x20, (UInt32)&MessageHandlerOverride);
	SafeWrite32(kVTBL_MessageHandler + 0x24, (UInt32)&MessageHandlerOverride);

													// patch spammy subroutines
	NopHdlr kDataHandlerAutoSave(0x0043083B, 5);
	NopHdlr	kAnimGroupNote(0x004CA21D, 5);
	NopHdlr kTangentSpaceCreation(0x0076989C, 5);
	NopHdlr	kHeightMapGenA(0x005E0D9D, 5), kHeightMapGenB(0x005E0DB6, 5);
	NopHdlr kModelLoadError(0x0046C215, 5);
	NopHdlr	kLoadTerrainLODQuad(0x005583F1, 5);

	SafeWrite8(0x00468597, 0xEB);					//		FileFinder::LogMessage
	kDataHandlerAutoSave.WriteNop();
	kAnimGroupNote.WriteNop();
	kTangentSpaceCreation.WriteNop();
	kHeightMapGenA.WriteNop();
	kHeightMapGenB.WriteNop();
	kModelLoadError.WriteNop();
	kLoadTerrainLODQuad.WriteNop();
}

void __stdcall MessageHandlerOverride(const char* Message)
{
	DebugPrint(Console::e_CS, "%s", Message);
}

void __stdcall DoCSExitHook(HWND MainWindow)
{
	WritePositionToINI(MainWindow, NULL);
	WritePositionToINI(*g_HWND_CellView, "Cell View");
	WritePositionToINI(*g_HWND_ObjectWindow, "Object Window");
	WritePositionToINI(*g_HWND_RenderWindow, "Render Window");
	DebugPrint("CS INI Settings Flushed");

	RENDERTEXT->Release();
	DebugPrint("Render Window Text Painter Released");
	CSIOM->Deinitialize();
	DebugPrint("CSInterop Manager Deinitialized");
	g_ToolManager.WriteToINI(g_INIPath.c_str());
	DebugPrint("Tool Manager Deinitialized");

	DebugPrint("Deinitializing Console, Flushing CSE INI Settings and Closing the CS!");
	CONSOLE->Deinitialize();
	g_INIManager->SaveSettingsToINI();

	ExitProcess(0);
}

_BeginHookHdlrFn(CSExit)
{
	__asm
	{
		push    ebx
		call    DoCSExitHook
	}
}

void __stdcall DoCSInitHook()
{
	if (!g_PluginPostLoad)
		return;
											// prevents the hook from being called before the full init
											// perform deferred patching
											// remove hook rightaway to keep it from hindering the subclassing that follows
	kCSInit.WriteBuffer();

	DebugPrint("Initializing Tools");
	CONSOLE->Indent();
	g_ToolManager.InitializeToolsMenu();
	g_ToolManager.ReadFromINI(g_INIPath.c_str());
	g_ToolManager.ReloadToolsMenu();
	CONSOLE->Exdent();

	DebugPrint("Initializing Window Manager");
	CONSOLE->Indent();
	InitializeWindowManager();
	CONSOLE->Exdent();

	DebugPrint("Initializing Console");
	CONSOLE->Indent();
	CONSOLE->InitializeConsole();
	CONSOLE->LoadINISettings();
	CONSOLE->Exdent();

	DebugPrint("Initializing GMST Map");
	CONSOLE->Indent();
	InitializeDefaultGMSTMap();
	CONSOLE->Exdent();

	DebugPrint("Initializing Intellisense Database Update Thread");
	CLIWrapper::ScriptEditor::InitializeDatabaseUpdateTimer();

	DebugPrint("Initializing Hall of Fame");
	CONSOLE->Indent();
	HallOfFame::Initialize(true);
	CONSOLE->Exdent();

	DebugPrint("Initializing Render Window Text Painter");
	g_RenderTimeManager.Update();
	RENDERTEXT->Initialize();
	RENDERTEXT->QueueDrawTask(RenderWindowTextPainter::kRenderChannel_2, "Construction Set Extender", 5);

	DebugPrint("Initializing CS Startup Manager");
	CONSOLE->Indent();
	if (g_INIManager->GetINIInt("LoadPluginOnStartup"))
		LoadStartupPlugin();

	if (g_INIManager->GetINIInt("OpenScriptWindowOnStartup"))
	{
		const char* ScriptID = g_INIManager->GetINIStr("StartupScriptEditorID");
		if (strcmp(ScriptID, "") && GetFormByID(ScriptID))
			SpawnCustomScriptEditor(ScriptID);
		else
			SendMessage(*g_HWND_CSParent, WM_COMMAND, 0x9CE1, 0);
	}

	DebugPrint("Initializing Workspace Manager");
	CONSOLE->Indent();
	g_WorkspaceManager.Initialize(g_AppPath.c_str());
	if (g_INIManager->GetINIInt("SetWorkspaceOnStartup"))
		g_WorkspaceManager.SelectWorkspace(g_INIManager->GetINIStr("DefaultWorkspacePath"));
	CONSOLE->Exdent();
	CONSOLE->Exdent();

	DebugPrint("Initializing Archives");
	CONSOLE->Indent();
	InitializeArchives();
	CONSOLE->Exdent();

	CONSOLE->ExdentAll();
	DebugPrint("Construction Set Extender Initialized!\r\n");
}

_BeginHookHdlrFn(CSInit)
{
	_DeclareHookHdlrFnVariable(CSInit, Retn, 0x00419265);
	_DeclareHookHdlrFnVariable(CSInit, Call, 0x006E5850);
	__asm
	{
		call	[_HookHdlrFnVariable(CSInit, Call)]
		call	DoCSInitHook
		jmp		[_HookHdlrFnVariable(CSInit, Retn)]
	}
}

void __stdcall DoAssertOverrideHook(UInt32 EIP)
{
	DebugPrint("\tAssertion handled at 0x%08X", EIP);
	MessageBeep(MB_ICONHAND);
}

_BeginHookHdlrFn(AssertOverride)
{
	_DeclareHookHdlrFnVariable(AssertOverride, Retn, 0x004B575E);
	__asm
	{
		mov		eax, [esp]
		sub		eax, 5
		pushad
		push	eax
		call	DoAssertOverrideHook
		popad
		xor		eax, eax

		jmp		[_HookHdlrFnVariable(AssertOverride, Retn)]
	}
}

_BeginHookHdlrFn(PluginSave)
{
	_DeclareHookHdlrFnVariable(PluginSave, Retn, 0x0041BBD3);
    __asm
    {
		call	SetWindowTextAddress
		call	[g_WindowHandleCallAddr]				// SetWindowTextA
		pushad
		push	10
		call	SendPingBack
		popad
		jmp		[_HookHdlrFnVariable(PluginSave, Retn)]
    }
}

_BeginHookHdlrFn(PluginLoad)
{
	_DeclareHookHdlrFnVariable(PluginLoad, Retn, 0x0041BEFF);
    __asm
    {
		call	InitializeCSWindows

		pushad
		push	9
		call	SendPingBack
		call	FormEnumerationWrapper::ResetFormVisibility
		popad

		jmp		[_HookHdlrFnVariable(PluginLoad, Retn)]
    }
}

void __stdcall DestroyShadeMeRef(void)
{
	TESForm* Ref = GetFormByID("TheShadeMeRef");
	if (Ref)
		thisVirtualCall(kVTBL_TESObjectREFR, 0x34, Ref);
}
void __stdcall ClearRenderSelectionGroupMap(void)
{
	g_RenderSelectionGroupManager.Clear();
}
void __stdcall ClearGMSTCollection(void)
{
	void* Unk01 = (void*)thisCall(0x0051F920, (void*)g_GMSTMap);
	while (Unk01)
	{
		const char*	 Name = NULL;
		SettingData* Data = NULL;

		thisCall(0x005E0F90, (void*)g_GMSTMap, &Unk01, &Name, &Data);
		if (Data)
		{
			GameSetting* SettingForm = (GameSetting*)((UInt32)Data - 0x24);
			thisVirtualCall(*(UInt32*)SettingForm, 0x94, SettingForm, 0);
			_DefaultGMSTMap::iterator Match = g_DefaultGMSTMap.find(Name);

			if (Match != g_DefaultGMSTMap.end())
			{
				GameSetting* DefaultGMST = Match->second;
				thisVirtualCall(*(UInt32*)SettingForm, 0xB8, SettingForm, DefaultGMST);
			}
		}
	}
}

_BeginHookHdlrFn(DataHandlerClearData)
{
	_DeclareHookHdlrFnVariable(DataHandlerClearDataShadeMeRefDtor, Retn, 0x0047AE7B);
	__asm
	{
		lea     edi, [ebx+44h]
		mov     ecx, edi
		pushad
		call	DestroyShadeMeRef
		call	ClearRenderSelectionGroupMap
		call	ClearGMSTCollection
		popad

		jmp		[_HookHdlrFnVariable(DataHandlerClearDataShadeMeRefDtor, Retn)]
	}
}

_BeginHookHdlrFn(TopicInfoCopyEpilog)
{
	_DeclareHookHdlrFnVariable(TopicInfoCopyEpilog, Retn, 0x004F1286);
	__asm
	{
		pushad
		mov		eax, [esi]
		mov		eax, [eax + 0x94]		// SetFromActiveFile
		push	1
		mov		ecx, esi
		call	eax
		popad

		mov     [esi + 0x30], bx
		mov     eax, [edi]
		jmp		[_HookHdlrFnVariable(TopicInfoCopyEpilog, Retn)]
	}
}

void __stdcall DoNumericEditorIDHook(const char* EditorID)
{
	if (g_INIManager->FetchSetting("ShowNumericEditorIDWarning")->GetValueAsInteger() &&
		g_PluginPostLoad &&
		strlen(EditorID) > 0 &&
		isdigit((int)*EditorID))
	{
		sprintf_s(g_Buffer, sizeof(g_Buffer), "The editorID '%s' begins with an integer.\n\nWhile this is generally accepted by the engine, scripts referring this form might fail to run or compile as the script compiler can attempt to parse it as an integer.\n\nConsider starting the editorID with an alphabet.", EditorID);
		MessageBox(*g_HWND_CSParent, g_Buffer, "CSE", MB_OK|MB_ICONWARNING);
	}
}

_BeginHookHdlrFn(NumericEditorID)
{
	_DeclareHookHdlrFnVariable(NumericEditorID, Retn, 0x00497676);
	__asm
	{
		mov		eax, [esp + 0x4]
		pushad
		push	eax
		call	DoNumericEditorIDHook
		popad

		xor		eax, eax
		push	ebp
		mov		ebp, esp
		sub		esp, 0x10
		jmp		[_HookHdlrFnVariable(NumericEditorID, Retn)]
	}
}

_BeginHookHdlrFn(DataHandlerConstructSpecialForms)
{
	_DeclareHookHdlrFnVariable(DataHandlerConstructSpecialForms, Retn, 0x0048104E);
	_DeclareHookHdlrFnVariable(DataHandlerConstructSpecialForms, Call, 0x00505070);
	__asm
	{
		pushad
		push	0
		call	HallOfFame::Initialize
		popad

		call	[_HookHdlrFnVariable(DataHandlerConstructSpecialForms, Call)]
		jmp		[_HookHdlrFnVariable(DataHandlerConstructSpecialForms, Retn)]
	}
}

_BeginHookHdlrFn(ResultScriptSaveForm)
{
	_DeclareHookHdlrFnVariable(ResultScriptSaveForm, Retn, 0x004FD260);
	__asm
	{
		mov		eax, [ecx]
		mov		edx, [eax + 0x8]
		test	edx, edx
		jz		FAIL

		jmp		[_HookHdlrFnVariable(ResultScriptSaveForm, Retn)]
	FAIL:
		mov		eax, 0x004FD271
		jmp		eax
	}
}

_BeginHookHdlrFn(TESObjectREFRDoCopyFrom)
{
	_DeclareHookHdlrFnVariable(TESObjectREFRDoCopyFrom, Retn, 0x00547668);
	__asm
	{
		pushad
		push	ebx
		mov		ecx, ebp
		call	kExtraDataList_CopyList
		popad

		pushad
		mov		ecx, edi
		call	[kTESObjectREFR_RemoveExtraTeleport]
		popad

		jmp		[_HookHdlrFnVariable(TESObjectREFRDoCopyFrom, Retn)]
	}
}

void __stdcall DoTESFormAddReferenceHook(GenericNode<TESFormReferenceData>* ReferenceList, TESForm* Form)
{
	TESFormReferenceData* Data = TESFormReferenceData::FindDataInRefList(ReferenceList, Form);
	if (Data)
		Data->IncrementRefCount();
	else
	{
		TESFormReferenceData* NewNode = (TESFormReferenceData*)FormHeap_Allocate(sizeof(TESFormReferenceData));		// nasty, but ought to work
		NewNode->Initialize(Form);
		NewNode->IncrementRefCount();

		thisCall(kLinkedListNode_NewNode, ReferenceList, NewNode);
	}
}

_BeginHookHdlrFn(TESFormAddReference)
{
	_DeclareHookHdlrFnVariable(TESFormAddReference, Retn, 0x00496461);
	__asm
	{
		push	edi
		push	eax
		call	DoTESFormAddReferenceHook
		jmp		[_HookHdlrFnVariable(TESFormAddReference, Retn)]
	}
}

void __stdcall DoTESFormRemoveReferenceHook(TESForm* Parent, GenericNode<TESFormReferenceData>* ReferenceList, TESForm* Form)
{
	TESFormReferenceData* Data = TESFormReferenceData::FindDataInRefList(ReferenceList, Form);
	if (Data)
	{
		if (Data->DecrementRefCount() == 0)
		{
			thisCall(kLinkedListNode_RemoveNode, ReferenceList, Data);
			FormHeap_Free(Data);
		}

		if (thisCall(kLinkedListNode_GetIsDangling, ReferenceList))
			thisCall(kTESForm_CleanupFormReferenceList, Parent);
	}
}

_BeginHookHdlrFn(TESFormRemoveReference)
{
	_DeclareHookHdlrFnVariable(TESFormRemoveReference, Retn, 0x004964AE);
	__asm
	{
		push	ebx
		push	esi
		push	edi
		call	DoTESFormRemoveReferenceHook
		jmp		[_HookHdlrFnVariable(TESFormRemoveReference, Retn)]
	}
}

void __stdcall DoTESFormClearReferenceListHook(GenericNode<TESFormReferenceData>* ReferenceList)
{
	for (GenericNode<TESFormReferenceData>* Itr = ReferenceList; Itr && Itr->data; Itr = Itr->next)
	{
		TESFormReferenceData* Data = Itr->data;
		FormHeap_Free(Data);
	}
	thisCall(kLinkedListNode_Cleanup, ReferenceList);
}

_BeginHookHdlrFn(TESFormClearReferenceList)
{
	_DeclareHookHdlrFnVariable(TESFormClearReferenceList, Retn, 0x00496423);
	__asm
	{
		pushad
		push	ecx
		call	DoTESFormClearReferenceListHook
		popad

		jmp		[_HookHdlrFnVariable(TESFormClearReferenceList, Retn)]
	}
}

_BeginHookHdlrFn(TESFormPopulateUseInfoList)
{
	_DeclareHookHdlrFnVariable(TESFormPopulateUseInfoList, Retn, 0x004964FB);
	_DeclareHookHdlrFnVariable(TESFormPopulateUseInfoList, Jump, 0x00496509);
	__asm
	{
		call	[kLinkedListNode_GetData]
		test	eax, eax
		jz		FAIL

		mov		eax, [eax]
		test	eax, eax
		jz		FAIL

		jmp		[_HookHdlrFnVariable(TESFormPopulateUseInfoList, Retn)]
	FAIL:
		jmp		[_HookHdlrFnVariable(TESFormPopulateUseInfoList, Jump)]
	}
}

_BeginHookHdlrFn(TESFormDelete)
{
	_DeclareHookHdlrFnVariable(TESFormDelete, Retn, 0x00498717);
	_DeclareHookHdlrFnVariable(TESFormDelete, Jump, 0x0049872D);
	__asm
	{
		call	[kLinkedListNode_GetData]
		cmp		eax, ebx
		jz		FAIL

		mov		eax, [eax]
		jmp		[_HookHdlrFnVariable(TESFormDelete, Retn)]
	FAIL:
		jmp		[_HookHdlrFnVariable(TESFormDelete, Jump)]
	}
}

void __stdcall DoAboutDialogHook(HWND Dialog)
{
	HWND PictureControl = GetDlgItem(Dialog, 1963);
	HANDLE Splash = LoadImage(g_DLLInstance, MAKEINTRESOURCE(BITMAP_SPLASH), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	SendMessage(PictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Splash);
	Static_SetText(GetDlgItem(Dialog, -1), "The Elder Scrolls Construction Set && Construction Set Extender");
}

_BeginHookHdlrFn(AboutDialog)
{
	_DeclareHookHdlrFnVariable(AboutDialog, Retn, 0x00441CCB);
	__asm
	{
		pushad
		push	esi
		call	DoAboutDialogHook
		popad

		call	ShowWindowAddress
		call	g_WindowHandleCallAddr
		jmp		[_HookHdlrFnVariable(AboutDialog, Retn)]
	}
}