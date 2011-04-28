#include "ScriptEditorManager.h"
#include "Exports.h"
#include "IntelliSense.h"
#include "Globals.h"

extern "C"
{
[STAThread]
__declspec(dllexport) void InitializeComponents(CommandTableData* Data)
{
	AppDomain^ CurrentDomain = AppDomain::CurrentDomain;
	CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(&ScriptEditor::ResolveMissingAssemblies);

//	Globals^ Dummy = gcnew Globals();
	ScriptEditorManager::GetSingleton();
	ISDB->ParseCommandTable(Data);
}

__declspec(dllexport) void AddToURLMap(const char* CmdName, const char* URL)
{
	IntelliSenseDatabase::GetSingleton()->AddToURLMap(gcnew String(CmdName), gcnew String(URL));
}

__declspec(dllexport) void AllocateNewEditor(UInt32 PosX, UInt32 PosY, UInt32 Width, UInt32 Height)
{
	ScriptEditorManager::OperationParams^ Parameters = gcnew ScriptEditorManager::OperationParams();
	Parameters->VanillaHandleIndex = 0;
	Parameters->ParameterList->Add(PosX);
	Parameters->ParameterList->Add(PosY);
	Parameters->ParameterList->Add(Width);
	Parameters->ParameterList->Add(Height);

	SEMGR->PerformOperation(ScriptEditorManager::OperationType::e_AllocateTabContainer, Parameters);
}

__declspec(dllexport) void InitializeScript(UInt32 VanillaHandleIndex, ScriptData* Data)
{
	ScriptEditorManager::OperationParams^ Parameters = gcnew ScriptEditorManager::OperationParams();
	Parameters->VanillaHandleIndex = VanillaHandleIndex;
	Parameters->ParameterList->Add(gcnew String(Data->Text));
	Parameters->ParameterList->Add(Data->Type);
	Parameters->ParameterList->Add(gcnew String(Data->EditorID));
	Parameters->ParameterList->Add((UInt32)Data->ByteCode);
	Parameters->ParameterList->Add(Data->Length);
	Parameters->ParameterList->Add(Data->FormID);

	SEMGR->PerformOperation(ScriptEditorManager::OperationType::e_InitializeScript, Parameters);
}

__declspec(dllexport) void SendMessagePingback(UInt32 VanillaHandleIndex, UInt16 Message)
{
	ScriptEditorManager::OperationParams^ Parameters = gcnew ScriptEditorManager::OperationParams();
	Parameters->VanillaHandleIndex = VanillaHandleIndex;
	Parameters->ParameterList->Add(Message);

	SEMGR->PerformOperation(ScriptEditorManager::OperationType::e_ReceiveMessage, Parameters);
}

__declspec(dllexport) void SetScriptListItemData(UInt32 VanillaHandleIndex, ScriptData* Data)
{
	ScriptEditorManager::OperationParams^ Parameters = gcnew ScriptEditorManager::OperationParams();
	Parameters->VanillaHandleIndex = VanillaHandleIndex;
	Parameters->ParameterList->Add(gcnew String(Data->EditorID));
	Parameters->ParameterList->Add(Data->FormID);
	Parameters->ParameterList->Add(Data->Type);
	Parameters->ParameterList->Add(Data->Flags);

	SEMGR->PerformOperation(ScriptEditorManager::OperationType::e_SetScriptSelectItemData, Parameters);
}

__declspec(dllexport) void SetVariableListItemData(UInt32 VanillaHandleIndex, ScriptVarIndexData::ScriptVarInfo* Data)
{
	ScriptEditorManager::OperationParams^ Parameters = gcnew ScriptEditorManager::OperationParams();
	Parameters->VanillaHandleIndex = VanillaHandleIndex;
	Parameters->ParameterList->Add(gcnew String(Data->Name));
	Parameters->ParameterList->Add(Data->Type);
	Parameters->ParameterList->Add(Data->Index);

	SEMGR->PerformOperation(ScriptEditorManager::OperationType::e_SetVariableListItemData, Parameters);
}

__declspec(dllexport) void InitializeDatabaseUpdateTimer()
{
	ISDB->InitializeDatabaseUpdateTimer();
}

__declspec(dllexport) void PassScriptError(UInt32 LineNumber, const char* Message, UInt32 EditorIndex)
{
	ScriptEditorManager::OperationParams^ Parameters = gcnew ScriptEditorManager::OperationParams();
	Parameters->VanillaHandleIndex = EditorIndex;
	Parameters->ParameterList->Add(LineNumber);
	Parameters->ParameterList->Add(gcnew String(Message));

	SEMGR->PerformOperation(ScriptEditorManager::OperationType::e_AddToCompileErrorPool, Parameters);
}
}