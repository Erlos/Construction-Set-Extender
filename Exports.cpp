#include "Exports.h"
#include "ExtenderInternals.h"
#include "[Common]\CLIWrapper.h"
#include "resource.h"
#include "WindowManager.h"
#include "Hooks\CompilerErrorDetours.h"
#include "Hooks\Misc.h"
#include "Hooks\ScriptEditor.h"

#include "ScriptEditorAllocator.h"
#include "CSDialogs.h"
#include "GMSTMap.h"
#include "TESFormReferenceData.h"

using namespace Hooks;

FormData*						g_FormDataInteropPackage = new FormData();
UseListCellItemData*			g_UseListCellItemDataInteropPackage = new UseListCellItemData();

template <typename tData>
void UseInfoList_SetFormListItemText_ParseFormNode(DataHandler::Node<tData>* ThisNode);

TESObjectREFR* TESForm_LoadIntoView_GetReference(TESObjectCELL* Cell, TESForm* Parent);

template <typename tData>
void BatchRefEditor_ParseFormNode(DataHandler::Node<tData>* ThisNode, UInt8 ListID);

UInt32 ScriptEditor_CompileDependencies_CheckConditions(ConditionEntry* Entry, TESForm* ToCompare);
void ScriptEditor_CompileDependencies_ParseObjectUseList(TESForm* Form);

extern "C"
{
	__declspec(dllexport) void __stdcall _D_PRINT(UInt8 Source, const char* Message)
	{
		DebugPrint(Source, Message);
	}

	__declspec(dllexport) const char* __stdcall GetINIString(const char* Section, const char* Key, const char* Default)
	{
		GetPrivateProfileString(Section, Key, Default, g_TextBuffer, sizeof(g_TextBuffer), g_INIPath.c_str());
		return (const char*)g_TextBuffer;
	}

	__declspec(dllexport) const char* __stdcall GetAppPath(void)
	{
		return g_AppPath.c_str();
	}

	__declspec(dllexport) void __stdcall WriteStatusBarText(int PanelIndex, const char* Message)
	{
		if (PanelIndex < 0 || PanelIndex > 2)	PanelIndex = 2;
		TESDialog_WriteToStatusBar(MAKEWPARAM(PanelIndex, 0), (LPARAM)Message);
	}

	__declspec(dllexport) HWND __stdcall GetCSMainWindowHandle(void)
	{
		return *g_HWND_CSParent;
	}

	__declspec(dllexport) HWND __stdcall GetRenderWindowHandle(void)
	{
		return *g_HWND_RenderWindow;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_MessagingInterface(UInt32 TrackedEditorIndex, UInt16 Message)
	{
		static UInt16 ReturnType[8] =
		{
			0x9CDB,		// New
			0x9CD9,		// Open
			0x9D38,		// Previous
			0x9D39,		// Next
			0x9CDA,		// Save
			0x9D12,		// Recompile
			0x9D13,		// Delete
			0x9D14		// Close
		};

		HWND ScriptEditor = EDAL->GetTrackedDialog(TrackedEditorIndex);
		switch (Message)
		{
		case 5:			// prevent error messages from accumulating when recompiling scripts
			EDAL->SetLastContactedEditor(0);
			if (g_RecompilingScripts)
				MessageBox(*g_HWND_CSParent, "A previous recompile operation is in progress. Please wait until it completes.", "CSE", MB_OK|MB_ICONEXCLAMATION);
			else
			{
				g_RecompilingScripts = true;
				SendMessage(ScriptEditor, WM_COMMAND, ReturnType[Message], NULL);
				g_RecompilingScripts = false;
			}
			return;
		default:
			EDAL->SetLastContactedEditor(TrackedEditorIndex);
			break;
		}

		SendMessage(ScriptEditor, WM_COMMAND, ReturnType[Message], NULL);
	}

	__declspec(dllexport) void __stdcall ScriptEditor_SetScriptData(UInt32 TrackedEditorIndex, ScriptData* Data)
	{
		HWND ScriptEditor = EDAL->GetTrackedDialog(TrackedEditorIndex);
		SetWindowText(EDAL->GetTrackedREC(ScriptEditor), (LPSTR)Data->Text);
		SendMessage(EDAL->GetTrackedLBC(ScriptEditor), CB_SETCURSEL, Data->Type, NULL);
		SendMessage(EDAL->GetTrackedREC(ScriptEditor), EM_SETMODIFY, Data->ModifiedFlag, NULL);
	}

	__declspec(dllexport) void __stdcall ScriptEditor_SetWindowParameters(UInt32 TrackedEditorIndex, UInt32 Top, UInt32 Left, UInt32 Width, UInt32 Height)
	{
		HWND ScriptEditor = EDAL->GetTrackedDialog(TrackedEditorIndex);
		SetWindowPos(ScriptEditor, HWND_NOTOPMOST, Left, Top, Width, Height, SWP_NOZORDER);
	}

	__declspec(dllexport) UInt32 __stdcall ScriptEditor_InstantiateCustomEditor(const char* ScriptID)
	{
		if (ScriptID)
		{
			TESForm* Form = TESForm_LookupByEditorID(ScriptID);
			if (!Form)									return 0;
			Script* ScriptForm = CS_CAST(Form, TESForm, Script);
			if (!ScriptForm)							return 0;

			g_EditorInitScript = ScriptForm;
		}
		else
			g_EditorInitScript = NULL;

		HWND CustomEditor = CreateDialogParamA(*g_TESCS_Instance, (LPCSTR)0xBC, NULL, g_ScriptEditor_DlgProc, NULL);
		g_EditorInitScript = NULL;
		UInt32 AllocatedIndex = EDAL->TrackNewEditor(CustomEditor);

		return AllocatedIndex;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_PostProcessEditorInit(UInt32 AllocatedIndex)
	{
		HWND ScriptEditorHandle = EDAL->GetTrackedDialog(AllocatedIndex);
		ShowWindow(ScriptEditorHandle, SW_HIDE);

		if (IsWindowEnabled(EDAL->GetTrackedREC(ScriptEditorHandle)))
		{
			FillScriptDataPackage(g_SetEditorTextCache);
			CLIWrapper::ScriptEditor::InitializeScript(AllocatedIndex, g_ScriptDataInteropPackage);
		}
	}

	__declspec(dllexport) ScriptData* __stdcall ScriptEditor_GetScriptData()			// retrieves the cached script data
	{
		ASSERT(g_SetEditorTextCache);

		FillScriptDataPackage(g_SetEditorTextCache);

		return g_ScriptDataInteropPackage;
	}

	__declspec(dllexport) const char* __stdcall ScriptEditor_GetAuxScriptName()
	{
		if (g_EditorAuxScript)
			return g_EditorAuxScript->editorData.editorID.m_data;
		else
			return NULL;
	}

	__declspec(dllexport) ScriptData* __stdcall FetchScriptFromForm(const char* EditorID)
	{
		g_ScriptDataInteropPackage->ParentID = NULL, g_ScriptDataInteropPackage->EditorID = NULL;

		TESForm* Form = TESForm_LookupByEditorID(EditorID);

		if (!Form)									return g_ScriptDataInteropPackage;

		if (Form->IsReference())
		{
			TESObjectREFR* Ref =  CS_CAST(Form, TESForm, TESObjectREFR);
			if (Ref)	Form = Ref->baseForm;
		}

		g_ScriptDataInteropPackage->ParentID = Form->editorData.editorID.m_data;				// EditorID of the script's parent form

		TESScriptableForm* ScriptableForm = CS_CAST(Form, TESForm, TESScriptableForm);
		if (!ScriptableForm)						return g_ScriptDataInteropPackage;
		Script* FormScript = ScriptableForm->script;
		if (!FormScript)							return g_ScriptDataInteropPackage;

		FillScriptDataPackage(FormScript);
		return g_ScriptDataInteropPackage;
	}

	__declspec(dllexport) bool __stdcall IsFormAnObjRefr(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return false;
		else							return Form->IsReference();
	}

	__declspec(dllexport) FormData* __stdcall LookupFormByEditorID(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (Form)
		{
			g_FormDataInteropPackage->FillFormData(Form);
			return g_FormDataInteropPackage;
		}
		else
			return NULL;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_GetScriptListData(UInt32 TrackedEditorIndex)
	{
		Script* ThisScript = NULL;

		for (DataHandler::Node<Script>* ThisNode = &(*g_dataHandler)->scripts; ThisNode && ThisNode->data; ThisNode = ThisNode->next)
		{
			ThisScript = ThisNode->data;
			if (!ThisScript->editorData.editorID.m_data || !FormEnumerationWrapper::GetShouldEnumerateForm(ThisScript))
				continue;

			FillScriptDataPackage(ThisScript);

			CLIWrapper::ScriptEditor::SetScriptListItemData(TrackedEditorIndex, g_ScriptDataInteropPackage);
		}
	}

	__declspec(dllexport) const char* __stdcall ScriptEditor_GetScriptListItemText(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)									return NULL;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)							return NULL;
		return ScriptForm->text;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_SetScriptListResult(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)									return;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)							return;

		g_ScriptListResult = ScriptForm;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_GetUseReportForForm(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return;

		CreateDialogParam(*g_TESCS_Instance, (LPCSTR)0xDC, NULL, g_FormUseReport_DlgProc, (LPARAM)Form);
	}

	__declspec(dllexport) void __stdcall ScriptEditor_GetScriptVariableIndices(UInt32 TrackedEditorIndex, const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)				return;

		Script::VarInfoEntry* ThisNode = &ScriptForm->varList;
		Script::VariableInfo* ThisVariable = NULL;

		ScriptVarIndexData::ScriptVarInfo Data;

		while (ThisNode) 
		{
			ThisVariable = ThisNode->data;
			if (!ThisVariable)		break;

			Data.Name = ThisVariable->name.m_data;
			Data.Type = ThisVariable->type;
			Data.Index = ThisVariable->idx;

			if (Data.Type == 0)
			{
				for (Script::RefListEntry* entry = &ScriptForm->refList; entry; entry = entry->next)
				{
					if (entry->var && entry->var->varIdx == Data.Index)
						Data.Type = 2;
				}
			}

			CLIWrapper::ScriptEditor::SetVariableListItemData(TrackedEditorIndex, &Data);

			ThisNode = ThisNode->next;
		}
	}

	__declspec(dllexport) bool __stdcall ScriptEditor_SetScriptVariableIndex(const char* EditorID, ScriptVarIndexData::ScriptVarInfo* Data)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return false;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)				return false;

	//	### FIX
	// 	Script::VariableInfo* ScriptVar = ScriptForm->LookupVariableInfoByName(Data->Name);
	// 	if (ScriptVar)
	// 	{
	// 		if (Data->Type == 2)
	// 		{
	// 			Script::RefVariable* RefVar = ScriptForm->LookupRefVariableByIndex(ScriptVar->idx);
	// 			if (RefVar)
	// 				RefVar->varIdx = Data->Index;
	// 			else
	// 				return false;
	// 		}
	// 		ScriptVar->idx = Data->Index;
	// 	}
	// 	else
	// 		return false;

		return true;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_CompileDependencies(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)				return;

		DebugPrint("Recompiling dependencies of script %s {%08X}...", ScriptForm->editorData.editorID.m_data, ScriptForm->refID);
		CONSOLE->Indent();

		DebugPrint("Resolving script parent...");
		CONSOLE->Indent();
		switch (ScriptForm->info.type)
		{
		case Script::eType_Object:
		{
			DebugPrint("Source script type = Object Script");
			for (GenericNode<TESFormReferenceData>* i = (GenericNode<TESFormReferenceData>*)thisCall(kTESForm_GetFormReferenceList, Form, 0); i != 0; i = i->next)
			{
				if (!i || !i->data) 
				{
					DebugPrint("No Parents found!");
					break;
				}

				TESForm* Parent = i->data->GetForm();
				TESScriptableForm* ValidParent = CS_CAST(Parent, TESForm, TESScriptableForm);

				if (ValidParent)
				{
					DebugPrint("Scriptable Form EDID = %s ; TYPE = %d", Parent->editorData.editorID.m_data, Parent->typeID);
					DebugPrint("Parsing cell use list...");
					CONSOLE->Indent();
					TESCellUseList* UseList = CS_CAST(Parent, TESForm, TESCellUseList);
					for (GenericNode<TESCellUseData>* CellList = (GenericNode<TESCellUseData>*)thisCall(kTESCellUseList_GetUseListRefHead, UseList); CellList; CellList = CellList->next)
					{
						TESCellUseData* Data = CellList->data;
						if (!Data)		break;

						for (TESObjectCELL::ObjectListEntry* RefList = &Data->Cell->objectList; RefList; RefList = RefList->Next()) {
							TESObjectREFR* ThisReference = RefList->Info();
							if (!ThisReference)		break;

							if (ThisReference->baseForm == Parent)
							{
								ScriptEditor_CompileDependencies_ParseObjectUseList((TESForm*)ThisReference);
							}
						}
					}
					CONSOLE->Exdent();
				}
			}
			break;
		}
		case Script::eType_Quest:
		{
			DebugPrint("Source script type = Quest Script");
			for (GenericNode<TESFormReferenceData>* i = (GenericNode<TESFormReferenceData>*)thisCall(kTESForm_GetFormReferenceList, Form, 0); i != 0; i = i->next)
			{
				if (!i || !i->data)
				{
					DebugPrint("No Parents found!");
					break;
				}

				TESForm* Parent = i->data->GetForm();
				if (Parent->typeID == kFormType_Quest)
				{
					DebugPrint("Quest EDID = %s", Parent->editorData.editorID.m_data);
					ScriptEditor_CompileDependencies_ParseObjectUseList(Parent);
				}
			}
			break;
		}
		}
		CONSOLE->Exdent();

		DebugPrint("Parsing direct dependencies...");
		ScriptEditor_CompileDependencies_ParseObjectUseList(Form);

		CONSOLE->Exdent();
		DebugPrint("Recompile operation completed!");
	}

	__declspec(dllexport) IntelliSenseUpdateData* __stdcall ScriptEditor_BeginIntelliSenseDatabaseUpdate()
	{
		IntelliSenseUpdateData* Data = new IntelliSenseUpdateData();

		UInt32 QuestCount = thisCall(kLinkedListNode_CountNodes, &(*g_dataHandler)->quests),
				ScriptCount = 0,
				GlobalCount = thisCall(kLinkedListNode_CountNodes, &(*g_dataHandler)->globals);

		ScriptData TestData;
		for (DataHandler::Node<Script>* Itr = &(*g_dataHandler)->scripts; Itr; Itr = Itr->next)
		{
			if (Itr->data)
			{
				TestData.FillScriptData(Itr->data);
				if (TestData.UDF)	ScriptCount++;
			}
		}


		Data->QuestListHead = new QuestData[QuestCount];
		Data->QuestCount = QuestCount;
		Data->ScriptListHead = new ScriptData[ScriptCount];
		Data->ScriptCount = ScriptCount;
		Data->GlobalListHead = new GlobalData[GlobalCount];
		Data->GlobalCount = GlobalCount;

		QuestCount = 0, ScriptCount = 0, GlobalCount = 0;
		for (DataHandler::Node<TESQuest>* Itr = &(*g_dataHandler)->quests; Itr; Itr = Itr->next)
		{
			if (Itr->data)
			{
				Data->QuestListHead[QuestCount].FillFormData(Itr->data);
				Data->QuestListHead[QuestCount].FullName = Itr->data->fullName.name.m_data;
				Data->QuestListHead[QuestCount].ScriptName = NULL;
				if (Itr->data->scriptable.script)
				{
					Data->QuestListHead[QuestCount].ScriptName = Itr->data->scriptable.script->editorData.editorID.m_data;
				}
			}
			QuestCount++;
		}

		for (DataHandler::Node<Script>* Itr = &(*g_dataHandler)->scripts; Itr; Itr = Itr->next)
		{
			if (Itr->data)
			{
				TestData.FillScriptData(Itr->data);
				if (TestData.UDF)
				{
					Data->ScriptListHead[ScriptCount].FillFormData(Itr->data);
					Data->ScriptListHead[ScriptCount].FillScriptData(Itr->data);
					ScriptCount++;
				}
			}
		}

		for (DataHandler::Node<TESGlobal>* Itr = &(*g_dataHandler)->globals; Itr; Itr = Itr->next)
		{
			if (Itr->data)
			{
				Data->GlobalListHead[GlobalCount].FillFormData(Itr->data);
				Data->GlobalListHead[GlobalCount].FillVariableData(Itr->data);
				GlobalCount++;
			}
		}

		return Data;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_EndIntelliSenseDatabaseUpdate(IntelliSenseUpdateData* Data)
	{
		delete [] Data->ScriptListHead;
		delete [] Data->QuestListHead;
		delete [] Data->GlobalListHead;
		delete Data;
	}

	__declspec(dllexport) void __stdcall ScriptEditor_ToggleScriptCompiling(bool Enable)
	{
		ToggleScriptCompiling(Enable);
	}

	__declspec(dllexport) void __stdcall ScriptEditor_SaveActivePlugin()
	{
		SendMessage(*g_HWND_CSParent, WM_COMMAND, 0x9CD2, NULL);
	}

	__declspec(dllexport) void __stdcall ScriptEditor_SetScriptText(const char* EditorID, const char* ScriptText)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)				return;

		thisCall(kScript_SetText, ScriptForm, ScriptText);
	}

	__declspec(dllexport) void __stdcall ScriptEditor_BindScript(const char* EditorID, HWND Parent)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)						return;
		Script* ScriptForm = CS_CAST(Form, TESForm, Script);
		if (!ScriptForm)				return;

		Form = (TESForm*)DialogBox(g_DLLInstance, MAKEINTRESOURCE(DLG_BINDSCRIPT), Parent, (DLGPROC)BindScriptDlgProc);
		if (Form)
		{
			TESQuest* Quest = CS_CAST(Form, TESForm, TESQuest);
			TESBoundObject* BoundObj = CS_CAST(Form, TESForm, TESBoundObject);
			TESScriptableForm* ScriptableForm = CS_CAST(Form, TESForm, TESScriptableForm);

			if ((Quest && ScriptForm->info.type != Script::eType_Quest) ||
				(BoundObj && ScriptForm->info.type != Script::eType_Object))
			{
				MessageBox(Parent, "Script type doesn't correspond to binding form.", "CSE", MB_OK|MB_ICONEXCLAMATION);
			}
			else if (ScriptableForm == NULL)
				MessageBox(Parent, "Binding form isn't scriptable.", "CSE", MB_OK|MB_ICONEXCLAMATION);
			else
			{
				ScriptableForm->script = ScriptForm;
				thisCall(kTESForm_AddReference, ScriptForm, Form);
				thisVirtualCall(*((UInt32*)Form), 0x94, Form, 1);		// SetFromActiveFile

				sprintf_s(g_TextBuffer, sizeof(g_TextBuffer), "Script '%s' bound to form '%s'", ScriptForm->editorData.editorID.m_data, Form->editorData.editorID.m_data);
				MessageBox(Parent, g_TextBuffer, "CSE", MB_OK|MB_ICONINFORMATION);
			}
		}
	}

	__declspec(dllexport) void __stdcall UseInfoList_SetFormListItemText()
	{
		// bound objects
		UInt32 Count = 0, Total = (*g_dataHandler)->boundObjects->boundObjectCount;
		WriteStatusBarText(0, "FormList += BoundObjects...");

		for (TESBoundObject* Itr = (*g_dataHandler)->boundObjects->last; Itr; Itr = Itr->next) {
			g_FormDataInteropPackage->EditorID = Itr->editorData.editorID.m_data;
			g_FormDataInteropPackage->FormID = Itr->refID;
			g_FormDataInteropPackage->TypeID = Itr->typeID;
			CLIWrapper::UseInfoList::SetFormListItemData(g_FormDataInteropPackage);
			Count++;

			sprintf_s(g_TextBuffer, sizeof(g_TextBuffer), "[%d/%d]", Count, Total);
			WriteStatusBarText(1, g_TextBuffer);
			WriteStatusBarText(2, g_FormDataInteropPackage->EditorID);
		}

		// everything else.
		WriteStatusBarText(0, "FormList += Packages...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->packages);
		WriteStatusBarText(0, "FormList += WorldSpaces...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->worldSpaces);
		WriteStatusBarText(0, "FormList += Climates...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->climates);
		WriteStatusBarText(0, "FormList += Weathers...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->weathers);
		WriteStatusBarText(0, "FormList += Enchantments...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->enchantmentItems);
		WriteStatusBarText(0, "FormList += Spells...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->spellitems);
		WriteStatusBarText(0, "FormList += Hair...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->hairs);
		WriteStatusBarText(0, "FormList += Eyes...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->eyes);
		WriteStatusBarText(0, "FormList += Races...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->races);
		WriteStatusBarText(0, "FormList += LandTextures...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->landTextures);
		WriteStatusBarText(0, "FormList += Classes...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->classes);
		WriteStatusBarText(0, "FormList += Factions...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->factions);
		WriteStatusBarText(0, "FormList += Scripts...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->scripts);
		WriteStatusBarText(0, "FormList += Sounds...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->sounds);
		WriteStatusBarText(0, "FormList += Globals...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->globals);
		WriteStatusBarText(0, "FormList += Topics...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->topics);
		WriteStatusBarText(0, "FormList += Quests...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->quests);
		WriteStatusBarText(0, "FormList += Birthsigns...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->birthsigns);
		WriteStatusBarText(0, "FormList += CombatStyles...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->combatStyles);
		WriteStatusBarText(0, "FormList += LoadScreens...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->loadScreens);
		WriteStatusBarText(0, "FormList += WaterForms...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->waterForms);
		WriteStatusBarText(0, "FormList += EffectShaders...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->effectShaders);
		WriteStatusBarText(0, "FormList += AnimObjects...");
		UseInfoList_SetFormListItemText_ParseFormNode(&(*g_dataHandler)->objectAnios);

		WriteStatusBarText(0, ""), WriteStatusBarText(1, "");
		WriteStatusBarText(2, "UseInfoList Populated");
	}

	__declspec(dllexport) void __stdcall UseInfoList_SetObjectListItemText(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)									return;

		for (GenericNode<TESFormReferenceData>* UseList = (GenericNode<TESFormReferenceData>*)thisCall(kTESForm_GetFormReferenceList, Form, 0); UseList; UseList = UseList->next) {
			TESForm* Reference = UseList->data->GetForm();

			g_FormDataInteropPackage->EditorID = Reference->editorData.editorID.m_data;
			g_FormDataInteropPackage->FormID = Reference->refID;
			g_FormDataInteropPackage->TypeID = Reference->typeID;

			CLIWrapper::UseInfoList::SetUseListObjectItemData(g_FormDataInteropPackage);
		}
	}

	__declspec(dllexport) void __stdcall UseInfoList_SetCellListItemText(const char* EditorID)
	{
		TESForm* Form = TESForm_LookupByEditorID(EditorID);
		if (!Form)									return;

		TESCellUseList* UseList = CS_CAST(Form, TESForm, TESCellUseList);
		for (GenericNode<TESCellUseData>* CellList = (GenericNode<TESCellUseData>*)thisCall(kTESCellUseList_GetUseListRefHead, UseList); CellList; CellList = CellList->next) {
			TESCellUseData* Data = CellList->data;
			if (!Data)		break;

			TESObjectREFR* FirstRef = TESForm_LoadIntoView_GetReference(Data->Cell, Form);
			TESWorldSpace* WorldSpace = (TESWorldSpace*)thisCall(kTESObjectCELL_GetParentWorldSpace, Data->Cell);

			g_UseListCellItemDataInteropPackage->EditorID = Data->Cell->editorData.editorID.m_data;
			g_UseListCellItemDataInteropPackage->FormID = Data->Cell->refID;
			g_UseListCellItemDataInteropPackage->Flags = Data->Cell->flags0 & TESObjectCELL::kFlags0_Interior;
			g_UseListCellItemDataInteropPackage->WorldEditorID = ((!WorldSpace)?"Interior":WorldSpace->editorData.editorID.m_data);
			g_UseListCellItemDataInteropPackage->RefEditorID = ((!FirstRef || !FirstRef->editorData.editorID.m_data)?"<Unnamed>":FirstRef->editorData.editorID.m_data);
			g_UseListCellItemDataInteropPackage->XCoord = Data->Cell->coords->x;
			g_UseListCellItemDataInteropPackage->YCoord = Data->Cell->coords->y;
			g_UseListCellItemDataInteropPackage->UseCount = Data->Count;

			CLIWrapper::UseInfoList::SetUseListCellItemData(g_UseListCellItemDataInteropPackage);
		}
	}

	__declspec(dllexport) void __stdcall TESForm_LoadIntoView(const char* EditorID, const char* FormType)
	{
		ShowFormEditDialog(EditorID, FormType);
	}

	__declspec(dllexport) void __stdcall BatchRefEditor_SetFormListItem(UInt8 ListID)
	{
		switch(ListID)
		{
		case BatchRefData::kListID_NPC:
			{
			FormData Data;
			Data.EditorID = "NONE";
			Data.ParentForm = NULL;
			CLIWrapper::BatchEditor::AddFormListItem(&Data, ListID);

			for (TESBoundObject* Itr = (*g_dataHandler)->boundObjects->last; Itr; Itr = Itr->next) {
				TESNPC* NPC = CS_CAST(Itr, TESBoundObject, TESNPC);

				if (NPC) {
					Data.EditorID = Itr->editorData.editorID.m_data;
					Data.FormID = Itr->refID;
					Data.TypeID = Itr->typeID;
					Data.ParentForm = Itr;
					CLIWrapper::BatchEditor::AddFormListItem(&Data, ListID);
				}
			}
			break;
			}
		case BatchRefData::kListID_Global:
			BatchRefEditor_ParseFormNode(&(*g_dataHandler)->globals, ListID);
			break;
		case BatchRefData::kListID_Faction:
			BatchRefEditor_ParseFormNode(&(*g_dataHandler)->factions, ListID);
			break;
		}
	}

	__declspec(dllexport) const char* __stdcall BatchRefEditor_ChooseParentReference(BatchRefData* Data, HWND Parent)
	{
		TESObjectREFR* Ref = NULL;
		while (true)
		{
			Ref = ShowReferencePickDialog(Parent);
			if (!Ref)	break;
			break;
		}

		Data->EnableParent.Parent = Ref;
		if (Ref)
			sprintf_s(g_TextBuffer, sizeof(g_TextBuffer), "%08X", Ref->refID);
		else
			sprintf_s(g_TextBuffer, sizeof(g_TextBuffer), "NONE");
		return (!Ref || !Ref->editorData.editorID.m_data)?g_TextBuffer:Ref->editorData.editorID.m_data;
	}

	__declspec(dllexport) void __stdcall TagBrowser_InstantiateObjects(TagBrowserInstantiationData* Data)
	{
		thisCall(kTESRenderSelection_ClearSelection, *g_TESRenderSelectionPrimary, 1);

		for (int i = 0; i < Data->FormCount; i++)
		{
			FormData* ThisData = &Data->FormListHead[i];
			UInt32 FormID = ThisData->FormID;

			TESForm* Form = TESForm_LookupByFormID(FormID);
			if (!Form)
			{
				DebugPrint(Console::e_TAG, "Couldn't find form '%08X'!", FormID);
				continue;
			}

			thisCall(kTESRenderSelection_AddFormToSelection, *g_TESRenderSelectionPrimary, Form, 0);
		}

		SendMessage(*g_HWND_RenderWindow, 0x407, NULL, (LPARAM)&Data->InsertionPoint);
	}
}

template <typename tData>
void UseInfoList_SetFormListItemText_ParseFormNode(DataHandler::Node<tData>* ThisNode)
{
	UInt32 Count = 0;
	while (ThisNode)
	{
		tData* ThisObject = ThisNode->data;
		if (!ThisObject)		break;

		g_FormDataInteropPackage->EditorID = ThisObject->editorData.editorID.m_data;
		g_FormDataInteropPackage->FormID = ThisObject->refID;
		g_FormDataInteropPackage->TypeID = ThisObject->typeID;

		CLIWrapper::UseInfoList::SetFormListItemData(g_FormDataInteropPackage);
		ThisNode = ThisNode->next;
		Count++;

		sprintf_s(g_TextBuffer, sizeof(g_TextBuffer), "[%d]", Count);
		WriteStatusBarText(1, g_TextBuffer);
	}
}

TESObjectREFR* TESForm_LoadIntoView_GetReference(TESObjectCELL* Cell, TESForm* Parent)
{
	TESObjectREFR* ThisReference = NULL;

	for (TESObjectCELL::ObjectListEntry* RefList = &Cell->objectList; RefList; RefList = RefList->Next()) 
	{
		ThisReference = RefList->Info();
		if (!ThisReference)		break;
		if (ThisReference->baseForm == Parent && ThisReference->editorData.editorID.m_data)
		{
			return ThisReference;
		}
	}
	return NULL;
}

template <typename tData>
void BatchRefEditor_ParseFormNode(DataHandler::Node<tData>* ThisNode, UInt8 ListID)
{
	FormData Data;
	Data.EditorID = "NONE";
	Data.ParentForm = NULL;
	CLIWrapper::BatchEditor::AddFormListItem(&Data, ListID);

	while (ThisNode)
	{
		tData* ThisObject = ThisNode->data;
		if (!ThisObject)		break;

		Data.EditorID = ThisObject->editorData.editorID.m_data;
		Data.FormID = ThisObject->refID;
		Data.TypeID = ThisObject->typeID;
		Data.ParentForm = ThisObject;

		CLIWrapper::BatchEditor::AddFormListItem(&Data, ListID);
		ThisNode = ThisNode->next;
	}
}

UInt32 ScriptEditor_CompileDependencies_CheckConditions(ConditionEntry* Entry, TESForm* ToCompare)
{
	UInt32 ScriptableConditions = 0;

	for (ConditionEntry* j = Entry; j != 0; j = j->next) {
		if (!j || !j->data)		break;

		ConditionEntry::Data* Data = j->data;
		if (Data->functionIndex == (53 & 0x0FFF) || j->data->functionIndex == (79 & 0x0FFF))			// GetScriptVariable || GetQuestVariable
		{
			if (Data->param1.form && Data->param1.form == ToCompare)
			{
				ScriptableConditions++;
			}
		}
	}
	return ScriptableConditions;
}

void ScriptEditor_CompileDependencies_ParseObjectUseList(TESForm* Form)
{
	DebugPrint("Parsing object use list of %08X...", Form->refID);
	CONSOLE->Indent();

	std::vector<Script*> ScriptDepends;		// updating usage info inside an use list loop invalidates the list.
	std::vector<TESTopicInfo*> InfoDepends; // so store the objects ptrs and parse them later
	std::vector<TESQuest*> QuestDepends;

	for (GenericNode<TESFormReferenceData>* i = (GenericNode<TESFormReferenceData>*)thisCall(kTESForm_GetFormReferenceList, Form, 0); i != 0; i = i->next)
	{
		if (!i || !i->data)
		{
			DebugPrint("No dependencies found!");
			break;
		}

		TESForm* Depends = i->data->GetForm();
		switch (Depends->typeID)
		{
		case kFormType_DialogInfo:
		{
			InfoDepends.push_back(CS_CAST(Depends, TESForm, TESTopicInfo));
			break;
		}
		case kFormType_Quest:
		{
			QuestDepends.push_back(CS_CAST(Depends, TESForm, TESQuest));
			break;
		}
		case kFormType_Script:
		{
			ScriptDepends.push_back(CS_CAST(Depends, TESForm, Script));
			break;
		}
		default:	// ### any other type that needs handling ?
	//		DebugPrint("Skipping form type %d", Depends->typeID);
			break;
		}
	}

	// scripts
	for (std::vector<Script*>::const_iterator Itr = ScriptDepends.begin(); Itr != ScriptDepends.end(); Itr++)
	{
		DebugPrint("Script %s {%08X}:", (*Itr)->editorData.editorID.m_data, (*Itr)->refID);
		CONSOLE->Indent();

		if ((*Itr)->info.dataLength > 0)
		{
			if (!thisCall(kScript_SaveScript, g_ScriptCompilerUnkObj, (*Itr), 0))
			{
				DebugPrint("Script failed to compile due to errors!");
			}
		}

		CONSOLE->Exdent();
	}
	// quests
	for (std::vector<TESQuest*>::const_iterator Itr = QuestDepends.begin(); Itr != QuestDepends.end(); Itr++)
	{
		DebugPrint("Quest %s {%08X}:", (*Itr)->editorData.editorID.m_data, (*Itr)->refID);
		CONSOLE->Indent();

		for (TESQuest::StageEntry* j = &(*Itr)->stageList; j != 0; j = j->next)
		{
			if (!j->data)		break;

			for (TESQuest::StageItemList* l = &j->data->itemList; l != 0; l = l->next)
			{
				if (!l->item)		break;

				QuestStageItem* QuestStage = l->item;
				if (QuestStage->resultScript.info.dataLength > 0)
				{
					if (!thisCall(kScript_SaveResultScript, g_ScriptCompilerUnkObj, &QuestStage->resultScript, 0, 0))
					{
						DebugPrint("Result script in stage %d-%d failed to compile due to errors!", j->data->index, QuestStage->index);
					}
				}

				DebugPrint("Found %d conditions in stage %d-%d that referenced source script", ScriptEditor_CompileDependencies_CheckConditions(&QuestStage->conditionList, Form), j->data->index, QuestStage->index);
			}
		}

		for (TESQuest::TargetEntry* j = &(*Itr)->targetList; j != 0; j = j->next)
		{
			if (!j->data)		break;

			DebugPrint("Found %d conditions in target entry {%08X} that referenced source script", ScriptEditor_CompileDependencies_CheckConditions(&j->data->conditionList, Form), j->data->target->refID);
		}

		thisVirtualCall(kVTBL_TESQuest, 0x104, (*Itr));	// UpdateUsageInfo.
		CONSOLE->Exdent();
	}
	// topic infos
	for (std::vector<TESTopicInfo*>::const_iterator Itr = InfoDepends.begin(); Itr != InfoDepends.end(); Itr++)
	{
		DebugPrint("Topic info %08X:", (*Itr)->refID);
		CONSOLE->Indent();

		if ((*Itr)->resultScript.info.dataLength > 0)
		{
			if (!thisCall(kScript_SaveResultScript, g_ScriptCompilerUnkObj, &(*Itr)->resultScript, 0, 0))
			{
				DebugPrint("Result script failed to compile due to errors!");
			}
		}

		DebugPrint("Found %d conditions that referenced source script", ScriptEditor_CompileDependencies_CheckConditions(&(*Itr)->conditions, Form));
		thisVirtualCall(kVTBL_TESTopicInfo, 0x104, (*Itr));	// UpdateUsageInfo
		CONSOLE->Exdent();
	}

	CONSOLE->Exdent();
	DebugPrint("Operation complete!");
}