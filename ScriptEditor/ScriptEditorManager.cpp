#include "ScriptEditorManager.h"
#include "Common\NativeWrapper.h"
#include "Globals.h"
#include "Common\HandShakeStructs.h"
#include "ScriptListDialog.h"
#include "IntelliSense.h"

ScriptEditorManager::ScriptEditorManager()
{
	WorkspaceAllocationMap = gcnew LinkedList<ScriptEditor::Workspace^>();
	TabContainerAllocationMap = gcnew LinkedList<ScriptEditor::TabContainer^>();
}

ScriptEditorManager^% ScriptEditorManager::GetSingleton()
{
	if (Singleton == nullptr) {
		Singleton = gcnew ScriptEditorManager();
		DebugPrint("Initialized ScriptEditorManager");
	}
	return Singleton;
}

void ScriptEditorManager::PerformOperation(ScriptEditorManager::OperationType Op, ScriptEditorManager::OperationParams^ Parameters)
{
	try {
		UInt16 NoOfParams = Parameters->ParameterList->Count, ParamsRequired = ParamCount[(int)Op];

		if (NoOfParams < ParamsRequired)
			throw gcnew CSEGeneralException("Incorrect number of parameters passed (" + NoOfParams + ")");
		if (GetAllocatedWorkspace(Parameters->VanillaHandleIndex) == ScriptEditor::Workspace::NullSE && Parameters->VanillaHandleIndex)
			throw gcnew CSEGeneralException("Invalid editor index '" + Parameters->VanillaHandleIndex + "' passed");

		switch (Op)
		{
		case OperationType::e_AllocateTabContainer:
			AllocateNewTabContainer((UInt32)Parameters->ParameterList[0],
								(UInt32)Parameters->ParameterList[1],
								(UInt32)Parameters->ParameterList[2],
								(UInt32)Parameters->ParameterList[3]);
			break;
		case OperationType::e_InitializeScript:
			InitializeScript(Parameters->VanillaHandleIndex,
						dynamic_cast<String^>(Parameters->ParameterList[0]),
						(UInt16)Parameters->ParameterList[1],
						dynamic_cast<String^>(Parameters->ParameterList[2]),
						(UInt32)Parameters->ParameterList[3],
						(UInt32)Parameters->ParameterList[4],
						(UInt32)Parameters->ParameterList[5]);
			break;

		case OperationType::e_SendMessage:
			switch ((SendReceiveMessageType)Parameters->ParameterList[0])
			{
				case SendReceiveMessageType::e_New:
					MessageHandler_SendNew(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Open:
					MessageHandler_SendOpen(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Previous:
					MessageHandler_SendPrevious(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Next:
					MessageHandler_SendNext(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Save:
					MessageHandler_SendSave(Parameters->VanillaHandleIndex, (SaveWorkspaceOpType)Parameters->ParameterList[1]);
					break;
				case SendReceiveMessageType::e_Recompile:
					MessageHandler_SendRecompile(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Delete:
					MessageHandler_SendDelete(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Close:
					MessageHandler_SendClose(Parameters->VanillaHandleIndex);
					break;
			}
			break;
		case OperationType::e_ReceiveMessage:
			switch ((SendReceiveMessageType)((UInt16)Parameters->ParameterList[0]))
			{
				case SendReceiveMessageType::e_New:
					MessageHandler_ReceiveNew(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Open:
					MessageHandler_ReceiveOpen(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Previous:
					MessageHandler_ReceivePrevious(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Next:
					MessageHandler_ReceiveNext(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Save:
					MessageHandler_ReceiveSave(Parameters->VanillaHandleIndex);
					break;
				case SendReceiveMessageType::e_Close:
					MessageHandler_ReceiveClose(Parameters->VanillaHandleIndex);
					break;

				case SendReceiveMessageType::e_LoadRelease:
					MessageHandler_ReceiveLoadRelease();
					break;
				case SendReceiveMessageType::e_PostPluginLoad:
					MessageHandler_ReceivePostPluginLoad();
					break;
				case SendReceiveMessageType::e_PostPluginSave:
					MessageHandler_ReceivePostPluginSave();
					break;
			}
			break;
		case OperationType::e_SetScriptSelectItemData:
			SetScriptSelectItemData(Parameters->VanillaHandleIndex,
						dynamic_cast<String^>(Parameters->ParameterList[0]),
						(UInt32)Parameters->ParameterList[1],
						(UInt16)Parameters->ParameterList[2],
						(UInt32)Parameters->ParameterList[3]);
			break;
		case OperationType::e_SetVariableListItemData:
			SetVariableListItemData(Parameters->VanillaHandleIndex,
						dynamic_cast<String^>(Parameters->ParameterList[0]),
						(UInt8)Parameters->ParameterList[1],
						(UInt32)Parameters->ParameterList[2]);
			break;
		case OperationType::e_AllocateWorkspace:
			AllocateNewWorkspace((UInt32)Parameters->ParameterList[0],
						dynamic_cast<ScriptEditor::TabContainer^>(Parameters->ParameterList[1]));
			break;
		case OperationType::e_DestroyTabContainer:
			DestroyTabContainer(dynamic_cast<ScriptEditor::TabContainer^>(Parameters->ParameterList[0]));
			break;
		case OperationType::e_TabTearOp:
			TabTearOpHandler((TabTearOpType)Parameters->ParameterList[0],
						dynamic_cast<ScriptEditor::Workspace^>(Parameters->ParameterList[1]),
						dynamic_cast<ScriptEditor::TabContainer^>(Parameters->ParameterList[2]),
						(Point)(Parameters->ParameterList[3]));
			break;
		case OperationType::e_AddToCompileErrorPool:
			AddToCompileErrorPool(Parameters->VanillaHandleIndex,
						(UInt32)Parameters->ParameterList[0],
						dynamic_cast<String^>(Parameters->ParameterList[1]));
			break;
		}
	}
	catch (Exception^ E) {
		DebugPrint("ScriptEditorManager couldn't complete operation '" + TypeIdentifier[(int)Op] + "'\n\tException: " + E->Message, true);
	}
}


ScriptEditor::Workspace^ ScriptEditorManager::GetAllocatedWorkspace(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^ Result = ScriptEditor::Workspace::NullSE;
	for each (ScriptEditor::Workspace^% Itr in WorkspaceAllocationMap) {
		if (Itr->GetAllocatedIndex() == AllocatedIndex) {
			Result = Itr;
			break;
		}
	}
	return Result;
}

void ScriptEditorManager::MoveScriptDataToVanillaEditor(ScriptEditor::Workspace^% CSEEditor, SaveWorkspaceOpType SaveOperation)
{
	CStringWrapper^ CScriptText = gcnew CStringWrapper(const_cast<String^>(CSEEditor->PreProcessScriptText(PreProcessor::PreProcessOp::e_Expand, nullptr, (SaveOperation == SaveWorkspaceOpType::e_SaveButDontCompile))));
	g_ScriptDataPackage->Text = CScriptText->String();
	g_ScriptDataPackage->Type = CSEEditor->GetScriptType();
	g_ScriptDataPackage->ModifiedFlag = CSEEditor->GetModifiedStatus();
	NativeWrapper::ScriptEditor_SetScriptData(CSEEditor->GetAllocatedIndex(), g_ScriptDataPackage);
}

void ScriptEditorManager::MoveScriptDataFromVanillaEditor(ScriptEditor::Workspace^% CSEEditor)
{
	g_ScriptDataPackage = NativeWrapper::ScriptEditor_GetScriptData();

	switch (g_ScriptDataPackage->Type)
	{
	case 9:									// Function script
		g_ScriptDataPackage->Type = 0;
		break;
	case 99:
		DebugPrint("Couldn't fetch script data from the vanilla editor!", true);
		return;
	}

	InitializeScript(CSEEditor->GetAllocatedIndex(), 
						gcnew String((const char*)g_ScriptDataPackage->Text), 
						g_ScriptDataPackage->Type, 
						gcnew String((const char*)g_ScriptDataPackage->EditorID), 
						(UInt32)g_ScriptDataPackage->ByteCode, 
						g_ScriptDataPackage->Length,
						g_ScriptDataPackage->FormID);
}

void ScriptEditorManager::AllocateNewTabContainer(UInt32 PosX, UInt32 PosY, UInt32 Width, UInt32 Height)
{
	DebugPrint(String::Format("Allocated a new tab container"));
	TabContainerAllocationMap->AddLast(gcnew ScriptEditor::TabContainer(PosX, PosY, Width, Height));
	ScriptEditor::TabContainer::LastUsedBounds.X = PosX; 
	ScriptEditor::TabContainer::LastUsedBounds.Y = PosY; 
	ScriptEditor::TabContainer::LastUsedBounds.Width = Width; 
	ScriptEditor::TabContainer::LastUsedBounds.Height = Height; 
}

void ScriptEditorManager::InitializeScript(UInt32 AllocatedIndex, String^ ScriptText, UInt16 ScriptType, String^ ScriptName, UInt32 Data, UInt32 DataLength, UInt32 FormID)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	Itr->InitializeScript(ScriptText, ScriptType, ScriptName, Data, DataLength, FormID);
}



void ScriptEditorManager::MessageHandler_SendNew(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
	
	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_New);

}
void ScriptEditorManager::MessageHandler_SendOpen(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
		
	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Open);
}
void ScriptEditorManager::MessageHandler_SendPrevious(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
		
	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Previous);
}
void ScriptEditorManager::MessageHandler_SendNext(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
		
	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Next);
}
void ScriptEditorManager::MessageHandler_SendSave(UInt32 AllocatedIndex, SaveWorkspaceOpType Operation)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
	
	MoveScriptDataToVanillaEditor(Itr, Operation);

	switch (Operation)
	{
	case SaveWorkspaceOpType::e_SaveAndCompile:
		Itr->ValidateScript();
		NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Save);
		break;
	case SaveWorkspaceOpType::e_SaveButDontCompile:
		NativeWrapper::ScriptEditor_ToggleScriptCompiling(false);
		NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Save);
		NativeWrapper::ScriptEditor_ToggleScriptCompiling(true);
		break;
	case SaveWorkspaceOpType::e_SaveActivePluginToo:
		Itr->ValidateScript();
		NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Save);
		NativeWrapper::ScriptEditor_SaveActivePlugin();
		break;
	}
}
void ScriptEditorManager::MessageHandler_SendRecompile(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
		
	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Recompile);
}
void ScriptEditorManager::MessageHandler_SendDelete(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);
		
	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Delete);
}
void ScriptEditorManager::MessageHandler_SendClose(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	MoveScriptDataToVanillaEditor(Itr, SaveWorkspaceOpType::e_SaveAndCompile);
	NativeWrapper::ScriptEditor_SetWindowParameters(AllocatedIndex, 
													Itr->GetParentContainer()->GetEditorFormRect().Top, 
													Itr->GetParentContainer()->GetEditorFormRect().Left, 
													Itr->GetParentContainer()->GetEditorFormRect().Width, 
													Itr->GetParentContainer()->GetEditorFormRect().Height);
	NativeWrapper::ScriptEditor_MessagingInterface(AllocatedIndex, (UInt16)SendReceiveMessageType::e_Close);
}



void ScriptEditorManager::MessageHandler_ReceiveNew(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	InitializeScript(AllocatedIndex, "", (UInt16)ScriptParser::ScriptType::e_Object, "New Script", 0, 0, 0);
	Itr->SetModifiedStatus(true);
}
void ScriptEditorManager::MessageHandler_ReceiveOpen(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	MoveScriptDataFromVanillaEditor(Itr);
}
void ScriptEditorManager::MessageHandler_ReceivePrevious(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	MoveScriptDataFromVanillaEditor(Itr);
}
void ScriptEditorManager::MessageHandler_ReceiveNext(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	MoveScriptDataFromVanillaEditor(Itr);
}

void ScriptEditorManager::MessageHandler_ReceiveSave(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	g_ScriptDataPackage = NativeWrapper::ScriptEditor_GetScriptData();
	Itr->UpdateScriptFromDataPackage(g_ScriptDataPackage);
}
void ScriptEditorManager::MessageHandler_ReceiveClose(UInt32 AllocatedIndex)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	DebugPrint(String::Format("Released allocated workspace at index {0}", AllocatedIndex));
	Itr->Destroy();
	WorkspaceAllocationMap->Remove(Itr);
}

void ScriptEditorManager::MessageHandler_ReceiveLoadRelease()
{
	for each (ScriptEditor::TabContainer^% Itr in TabContainerAllocationMap) {
		Itr->FlagDestruction(true);
	}
	for each (ScriptEditor::Workspace^% Itr in WorkspaceAllocationMap) {
		Itr->Destroy();
	}

	WorkspaceAllocationMap->Clear();
	TabContainerAllocationMap->Clear();

	DebugPrint("Released all allocated editors");
}
void ScriptEditorManager::MessageHandler_ReceivePostPluginLoad()
{
	ISDB->ForceUpdateDatabase();
}
void ScriptEditorManager::MessageHandler_ReceivePostPluginSave()
{
	ISDB->ForceUpdateDatabase();
}



void ScriptEditorManager::SetScriptSelectItemData(UInt32 AllocatedIndex, String^% ScriptName, UInt32 FormID, UInt16 Type, UInt32 Flags)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	Itr->AddItemToScriptListBox(ScriptName, FormID, Type, Flags);
}


void ScriptEditorManager::SetVariableListItemData(UInt32 AllocatedIndex, String^% Name, UInt32 Type, UInt32 Index)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	Itr->AddItemToVariableBox(Name, Type, Index);
}

void ScriptEditorManager::AllocateNewWorkspace(UInt32 AllocatedIndex, ScriptEditor::TabContainer^% Parent)
{
	WorkspaceAllocationMap->AddLast(gcnew ScriptEditor::Workspace(AllocatedIndex, Parent));
	DebugPrint(String::Format("Allocated a new workspace at index {0}", AllocatedIndex));
}

void ScriptEditorManager::DestroyTabContainer(ScriptEditor::TabContainer^ Container)
{
	DebugPrint("Released an allocated container");
	Container->Destroy();
	TabContainerAllocationMap->Remove(Container);
}

void ScriptEditorManager::TabTearOpHandler(TabTearOpType Operation, ScriptEditor::Workspace^ Workspace, ScriptEditor::TabContainer^ Container, Point MousePos)
{
	switch (Operation)
	{
	case TabTearOpType::e_NewContainer:
		{
			AllocateNewTabContainer(MousePos.X, MousePos.Y, ScriptEditor::TabContainer::LastUsedBounds.Width, ScriptEditor::TabContainer::LastUsedBounds.Height);
			Container = TabContainerAllocationMap->Last->Value;
			TabTearOpHandler(TabTearOpType::e_RelocateToContainer, Workspace, Container, MousePos);
			MessageHandler_ReceiveClose((dynamic_cast<ScriptEditor::Workspace^>(Container->LookupWorkspaceByTab(1)))->GetAllocatedIndex());	
			DebugPrint("Moved workspace " + Workspace->GetAllocatedIndex().ToString() + " to a new tab container");
			break;
		}
	case TabTearOpType::e_RelocateToContainer:
		{
			ScriptEditor::TabContainer^ Parent = Workspace->GetParentContainer();
			Workspace->Relocate(Container);
			DebugPrint("Moved workspace " + Workspace->GetAllocatedIndex().ToString() + " to another tab container");
			break;
		}
	}
	Container->RedrawContainer();
	Workspace->MakeActiveInParentContainer();
}

void ScriptEditorManager::AddToCompileErrorPool(UInt32 AllocatedIndex, UInt32 Line, String^% Message)
{
	ScriptEditor::Workspace^% Itr = GetAllocatedWorkspace(AllocatedIndex);

	if (Itr != ScriptEditor::Workspace::NullSE)
	{
		Itr->AddMessageToPool(ScriptEditor::Workspace::MessageType::e_Error, Line, Message);
	}
}