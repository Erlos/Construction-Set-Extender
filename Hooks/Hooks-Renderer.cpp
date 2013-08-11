#include "Hooks-Renderer.h"
#include "CSERenderSelectionGroupManager.h"
#include "CSERenderWindowPainter.h"
#include "CSEPathGridUndoManager.h"
#include "CSEAuxiliaryViewport.h"
#include "Hooks-LOD.h"

#pragma warning(push)
#pragma optimize("", off)
#pragma warning(disable: 4005 4748)
#pragma warning (disable: 4410)

namespace ConstructionSetExtender
{
	namespace Hooks
	{
		_DefineHookHdlr(DoorMarkerProperties, 0x00429EA1);
		_DefineHookHdlr(TESObjectREFRGet3DData, 0x00542950);
		_DefineHookHdlr(NiWindowRender, 0x00406442);
		_DefineHookHdlr(NiDX9RendererRecreateA, 0x006D79E8);
		_DefineHookHdlr(NiDX9RendererRecreateB, 0x006D7A0D);
		_DefineHookHdlr(NiDX9RendererRecreateC, 0x006D7CFA);
		_DefineHookHdlr(RenderWindowUpdateViewport, 0x0042CE70);
		_DefineHookHdlr(RenderWindowAddToSelection, 0x0042AE71);
		_DefineHookHdlr(TESRenderControlPerformMoveScale, 0x0042CAB0);
		_DefineHookHdlr(TESRenderControlPerformRotate, 0x0042B997);
		_DefineHookHdlr(TESRenderControlPerformFall, 0x0042886A);
		_DefineHookHdlr(TESRenderUndoStackRecordRef, 0x00432D40);
		_DefineHookHdlr(TESObjectREFRSetupDialog, 0x005499FB);
		_DefineHookHdlr(TESObjectREFRCleanDialog, 0x00549B52);
		_DefineHookHdlr(TESRenderControlPerformFallVoid, 0x004270C2);
		_DefineHookHdlr(ForceShowTESObjectREFRDialog, 0x00429EE3);
		_DefineHookHdlr(TESRenderControlAltSnapGrid, 0x00425A34);
		_DefineHookHdlr(TESRenderControlAltRefMovementSpeedA, 0x00425737);
		_DefineHookHdlr(TESRenderControlAltRefMovementSpeedB, 0x0042BE80);
		_DefineHookHdlr(TESRenderControlAltRefMovementSpeedC, 0x0042D0AD);
		_DefineHookHdlr(TESRenderControlAltRefRotationSpeed, 0x00425DBB);
		_DefineHookHdlr(TESRenderControlAltRefSnapAngle, 0x00425DC7);
		_DefineHookHdlr(TESRenderControlAltCamRotationSpeed, 0x0042CCAB);
		_DefineHookHdlr(TESRenderControlAltCamZoomSpeedA, 0x0042CCE0);
		_DefineHookHdlr(TESRenderControlAltCamZoomSpeedB, 0x0042CDAF);
		_DefineHookHdlr(TESRenderControlAltCamPanSpeedA, 0x0042CD26);
		_DefineHookHdlr(TESRenderControlAltCamPanSpeedB, 0x0042CD71);
		_DefineHookHdlr(TESRenderControlRedrawGrid, 0x004283F7);
		_DefineHookHdlr(TESPreviewControlCallWndProc, 0x0044D700);
		_DefineHookHdlr(ActivateRenderWindowPostLandTextureChange, 0x0042B4E5);
		_DefineHookHdlr(TESPathGridRecordOperationMoveA, 0x0042A62D);
		_DefineHookHdlr(TESPathGridRecordOperationMoveB, 0x0042BE6D);
		_DefineHookHdlr(TESPathGridRecordOperationLink, 0x0042A829);
		_DefineHookHdlr(TESPathGridRecordOperationFlag, 0x0042A714);
		_DefineHookHdlr(TESPathGridRecordOperationRef, 0x00428367);
		_DefineHookHdlr(TESPathGridDeletePoint, 0x004291C6);
		_DefineHookHdlr(TESPathGridPointDtor, 0x00556190);
		_DefineHookHdlr(TESPathGridToggleEditMode, 0x00550660);
		_DefineHookHdlr(TESPathGridCreateNewLinkedPoint, 0x0042B37B);
		_DefineHookHdlr(TESPathGridPerformFall, 0x00428605);
		_DefineHookHdlr(TESPathGridShowMultipleSelectionRing, 0x0042FC7C);
		_DefinePatchHdlr(TESPathGridDtor, 0x00550B81);
		_DefineHookHdlr(InitialCellLoadCameraPosition, 0x0040A8AE);
		_DefinePatchHdlr(LandscapeEditBrushRadius, 0x0041F2EE + 2);
		_DefineHookHdlrWithBuffer(ConvertNiRenderedTexToD3DBaseTex, 0x00411616, 5, 0x85, 0xC0, 0x75, 0x2E, 0x8B);
		_DefineHookHdlr(DuplicateReferences, 0x0042EC2E);
		_DefinePatchHdlrWithBuffer(NiDX9RendererPresent, 0x006D5C9D, 2, 0xFF, 0xD0);
		_DefineHookHdlr(RenderToAuxiliaryViewport, 0x0042D405);
		_DefineHookHdlr(TESRenderControlPerformRelativeScale, 0x00424700);
		_DefinePatchHdlr(DataHandlerClosePlugins, 0x0047B2FA);
		_DefineHookHdlr(TESPathGridRubberBandSelection, 0x0042FBE0);
		_DefineHookHdlr(CoplanarRefDrop, 0x0042DE2A);
		_DefineHookHdlr(InitPathGridNodeSelectionRing, 0x00419AFA);
		_DefineHookHdlr(TESLoadIntoViewPort, 0x00430F5A);
		_DefineHookHdlr(RenderWindowAxisHotkeysMoveReferences, 0x0042CB79);
		_DefineHookHdlr(RenderWindowAxisHotkeysMovePathGridPoints, 0x0042BF17);
		_DefinePatchHdlr(RenderWindowAxisHotkeysRotateReferences, 0x0042CBBD + 2);
		_DefineHookHdlr(BSFadeNodeDrawTransparency, 0x004BC527);
		_DefineHookHdlr(TESRubberBandSelectionSkipInvisibleRefs, 0x0042FB89);
		_DefineHookHdlr(RenderWindowCameraRotationPivot, 0x0042CBFD);
		_DefinePatchHdlrWithBuffer(CellViewSetCurrentCell, 0x00409170, 1, 0x53);
		_DefineJumpHdlr(RenderWindowCursorSwap, 0x0042CA34, 0x0042CAA3);

		void PatchRendererHooks(void)
		{
			_MemHdlr(DoorMarkerProperties).WriteJump();
			_MemHdlr(TESObjectREFRGet3DData).WriteJump();
			_MemHdlr(NiWindowRender).WriteJump();
			_MemHdlr(NiDX9RendererRecreateA).WriteJump();
			_MemHdlr(NiDX9RendererRecreateB).WriteJump();
			_MemHdlr(NiDX9RendererRecreateC).WriteJump();
			_MemHdlr(RenderWindowUpdateViewport).WriteJump();
			_MemHdlr(RenderWindowAddToSelection).WriteJump();
			_MemHdlr(TESRenderControlPerformMoveScale).WriteJump();
			_MemHdlr(TESRenderControlPerformRotate).WriteJump();
			_MemHdlr(TESRenderControlPerformFall).WriteJump();
			_MemHdlr(TESRenderUndoStackRecordRef).WriteJump();
			_MemHdlr(TESObjectREFRSetupDialog).WriteJump();
			_MemHdlr(TESObjectREFRCleanDialog).WriteJump();
			_MemHdlr(TESRenderControlPerformFallVoid).WriteJump();
			_MemHdlr(ForceShowTESObjectREFRDialog).WriteJump();
			_MemHdlr(TESRenderControlAltSnapGrid).WriteJump();
			_MemHdlr(TESRenderControlAltRefMovementSpeedA).WriteJump();
			_MemHdlr(TESRenderControlAltRefMovementSpeedB).WriteJump();
			_MemHdlr(TESRenderControlAltRefMovementSpeedC).WriteJump();
			_MemHdlr(TESRenderControlAltRefRotationSpeed).WriteJump();
			_MemHdlr(TESRenderControlAltRefSnapAngle).WriteJump();
			_MemHdlr(TESRenderControlAltCamRotationSpeed).WriteJump();
			_MemHdlr(TESRenderControlAltCamZoomSpeedA).WriteJump();
			_MemHdlr(TESRenderControlAltCamZoomSpeedB).WriteJump();
			_MemHdlr(TESRenderControlAltCamPanSpeedA).WriteJump();
			_MemHdlr(TESRenderControlAltCamPanSpeedB).WriteJump();
			_MemHdlr(TESRenderControlRedrawGrid).WriteJump();
			_MemHdlr(TESPreviewControlCallWndProc).WriteJump();
			_MemHdlr(ActivateRenderWindowPostLandTextureChange).WriteJump();
			_MemHdlr(TESPathGridRecordOperationMoveA).WriteJump();
			_MemHdlr(TESPathGridRecordOperationMoveB).WriteJump();
			_MemHdlr(TESPathGridRecordOperationLink).WriteJump();
			_MemHdlr(TESPathGridRecordOperationFlag).WriteJump();
			_MemHdlr(TESPathGridRecordOperationRef).WriteJump();
			_MemHdlr(TESPathGridDeletePoint).WriteJump();
			_MemHdlr(TESPathGridPointDtor).WriteJump();
			_MemHdlr(TESPathGridToggleEditMode).WriteJump();
			_MemHdlr(TESPathGridCreateNewLinkedPoint).WriteJump();
			_MemHdlr(TESPathGridPerformFall).WriteJump();
			_MemHdlr(TESPathGridShowMultipleSelectionRing).WriteJump();
			_MemHdlr(TESPathGridDtor).WriteUInt8(0xEB);
			_MemHdlr(InitialCellLoadCameraPosition).WriteJump();
			_MemHdlr(LandscapeEditBrushRadius).WriteUInt32((UInt32)&TESRenderWindow::MaxLandscapeEditBrushRadius);
			_MemHdlr(DuplicateReferences).WriteJump();
			_MemHdlr(RenderToAuxiliaryViewport).WriteJump();
			_MemHdlr(TESRenderControlPerformRelativeScale).WriteJump();
			_MemHdlr(DataHandlerClosePlugins).WriteUInt8(0xEB);
			_MemHdlr(TESPathGridRubberBandSelection).WriteJump();
			_MemHdlr(CoplanarRefDrop).WriteJump();
			_MemHdlr(InitPathGridNodeSelectionRing).WriteJump();
			_MemHdlr(TESLoadIntoViewPort).WriteJump();
			_MemHdlr(RenderWindowAxisHotkeysMoveReferences).WriteJump();
			_MemHdlr(RenderWindowAxisHotkeysMovePathGridPoints).WriteJump();
			_MemHdlr(RenderWindowAxisHotkeysRotateReferences).WriteUInt32(0x00A0BC1E);
			_MemHdlr(BSFadeNodeDrawTransparency).WriteJump();
			_MemHdlr(TESRubberBandSelectionSkipInvisibleRefs).WriteJump();
			_MemHdlr(RenderWindowCameraRotationPivot).WriteJump();
			_MemHdlr(RenderWindowCursorSwap).WriteJump();
		}

		#define _hhName		DoorMarkerProperties
		_hhBegin()
		{
			_hhSetVar(Properties, 0x00429EAB);
			_hhSetVar(Teleport, 0x00429EE8);
			__asm
			{
				mov		eax, [esi + 0x8]
				shr		eax, 0x0E
				test	al, 1
				jnz		DOORMARKER

				jmp		_hhGetVar(Properties)
			TELEPORT:
				popad
				jmp		_hhGetVar(Teleport)
			DOORMARKER:
				pushad
				call	IsControlKeyDown
				test	eax, eax
				jz		TELEPORT
				popad

				jmp		_hhGetVar(Properties)
			}
		}

		void __stdcall DoTESObjectREFRGet3DDataHook(TESObjectREFR* Object, NiNode* Node)
		{
			if ((Node->m_flags & TESObjectREFR::kNiNodeSpecialFlags_DontUncull))
				return;

			SME::MiscGunk::ToggleFlag(&Node->m_flags, NiNode::kFlag_AppCulled, false);

			if (TESRenderWindow::ShowInitiallyDisabledRefs == false && (Object->formFlags & TESForm::kFormFlags_Disabled))
				SME::MiscGunk::ToggleFlag(&Node->m_flags, NiNode::kFlag_AppCulled, true);

			BSExtraData* xData = Object->extraData.GetExtraDataByType(BSExtraData::kExtra_EnableStateParent);
			if (xData)
			{
				ExtraEnableStateParent* xParent = CS_CAST(xData, BSExtraData, ExtraEnableStateParent);
				if (xParent->parent->GetChildrenInvisible())
					SME::MiscGunk::ToggleFlag(&Node->m_flags, NiNode::kFlag_AppCulled, true);
			}

			if (Object->GetInvisible())
				SME::MiscGunk::ToggleFlag(&Node->m_flags, NiNode::kFlag_AppCulled, true);
		}

		#define _hhName		TESObjectREFRGet3DData
		_hhBegin()
		{
			_hhSetVar(Call, 0x0045B1B0);
			__asm
			{
				push	esi
				push	ecx		// store
				push	0x56
				add		ecx, 0x4C
				xor		esi, esi
				call	_hhGetVar(Call)
				test	eax, eax
				jz		NO3DDATA

				mov		eax, [eax + 0xC]
				pop		ecx		// restore
				push	ecx		// store again for epilog

				pushad
				push	eax
				push	ecx
				call	DoTESObjectREFRGet3DDataHook
				popad
				jmp		EXIT
			NO3DDATA:
				mov		eax, esi
			EXIT:
				pop		ecx
				pop		esi
				retn
			}
		}

		void __stdcall NiWindowRenderDrawHook(void)
		{
			BGSEERWPAINTER->Render();
		}

		#define _hhName		NiWindowRender
		_hhBegin()
		{
			_hhSetVar(Call, 0x0076A3B0);
			_hhSetVar(Retn, 0x00406447);
			__asm
			{
				call	_hhGetVar(Call)

				pushad
				call	NiWindowRenderDrawHook
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoNiDX9RendererRecreateHook(bool State)
		{
			if (State == false)
				BGSEERWPAINTER->HandleD3DDeviceReset(BGSEditorExtender::BGSEERenderWindowPainter::kDeviceReset_Release);
			else
				BGSEERWPAINTER->HandleD3DDeviceReset(BGSEditorExtender::BGSEERenderWindowPainter::kDeviceReset_Renew);
		}

		#define _hhName		NiDX9RendererRecreateA
		_hhBegin()
		{
			_hhSetVar(Retn, 0x006D79ED);
			_hhSetVar(Call, 0x006D7260);
			__asm
			{
				pushad
				push	0
				call	DoNiDX9RendererRecreateHook
				popad

				call	_hhGetVar(Call)

				pushad
				push	1
				call	DoNiDX9RendererRecreateHook
				popad
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		NiDX9RendererRecreateB
		_hhBegin()
		{
			_hhSetVar(Retn, 0x006D7A12);
			_hhSetVar(Call, 0x006D7260);
			__asm
			{
				pushad
				push	0
				call	DoNiDX9RendererRecreateHook
				popad

				call	_hhGetVar(Call)

				pushad
				push	1
				call	DoNiDX9RendererRecreateHook
				popad
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		NiDX9RendererRecreateC
		_hhBegin()
		{
			_hhSetVar(Retn, 0x006D7CFF);
			_hhSetVar(Call, 0x006D7260);
			__asm
			{
				pushad
				push	0
				call	DoNiDX9RendererRecreateHook
				popad

				call	_hhGetVar(Call)

				pushad
				push	1
				call	DoNiDX9RendererRecreateHook
				popad
				jmp		_hhGetVar(Retn)
			}
		}

		bool __stdcall DoRenderWindowUpdateViewportHook(void)
		{
			if (BGSEERWPAINTER->GetHasActiveTasks() || Settings::Renderer::kUpdateViewPortAsync.GetData().i)
				return true;
			else
				return false;
		}

		#define _hhName		RenderWindowUpdateViewport
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042EF86);
			_hhSetVar(Jump, 0x0042CE7D);
			__asm
			{
				mov		eax, [TESRenderWindow::RefreshFlag]
				mov		eax, [eax]
				cmp		al, 0
				jz		DONTUPDATE

				jmp		_hhGetVar(Jump)
			DONTUPDATE:
				pushad
				xor		eax, eax
				call	DoRenderWindowUpdateViewportHook
				test	al, al
				jz		EXIT

				popad
				jmp		_hhGetVar(Jump)
			EXIT:
				popad
				jmp		_hhGetVar(Retn)
			}
		}

		bool __stdcall DoRenderWindowSelectionHook(TESObjectREFR* Ref)
		{
			bool Result = false;

			TESObjectCELL* CurrentCell = _TES->currentInteriorCell;
			if (CurrentCell == NULL)
				CurrentCell = _TES->currentExteriorCell;

			if (CurrentCell)
			{
				TESRenderSelection* Selection = CSERenderSelectionGroupManager::Instance.GetRefSelectionGroup(Ref, CurrentCell);
				if (Selection)
				{
					for (TESRenderSelection::SelectedObjectsEntry* Itr = Selection->selectionList; Itr && Itr->Data; Itr = Itr->Next)
						_RENDERSEL->AddToSelection(Itr->Data, true);

					Result = true;
				}
			}

			return Result;
		}

		#define _hhName		RenderWindowAddToSelection
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042AE76);
			_hhSetVar(Jump, 0x0042AE84);
			_hhSetVar(Call, 0x00511C20);
			__asm
			{
				call	_hhGetVar(Call)
				xor		eax, eax

				pushad
				push	esi
				call	DoRenderWindowSelectionHook
				test	al, al
				jnz		GROUPED
				popad

				jmp		_hhGetVar(Retn)
			GROUPED:
				popad
				jmp		_hhGetVar(Jump)
			}
		}

		void __stdcall TESRenderControlProcessFrozenRefs(TESRenderSelection::SelectedObjectsEntry* Current)
		{
			std::list<TESForm*> FrozenRefs;
			if (Current == NULL)
				Current = _RENDERSEL->selectionList;

			for (TESRenderSelection::SelectedObjectsEntry* Itr = Current; Itr && Itr->Data; Itr = Itr->Next)
			{
				TESObjectREFR* Selection = CS_CAST(Itr->Data, TESForm, TESObjectREFR);
				SME_ASSERT(Selection);

				if (Selection->GetFrozen() || (Selection->IsActive() == false && TESRenderWindow::FreezeInactiveRefs))
				{
					FrozenRefs.push_back(Itr->Data);
				}
			}

			for (std::list<TESForm*>::const_iterator Itr = FrozenRefs.begin(); Itr != FrozenRefs.end(); Itr++)
				_RENDERSEL->RemoveFromSelection(*Itr, true);
		}

		#define _hhName		TESRenderControlPerformMoveScale
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CAB7);
			__asm
			{
				pushad
				push	0
				call	TESRenderControlProcessFrozenRefs
				popad

				mov		eax, 0x00A0BBDD
				cmp		byte ptr [eax], 0
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlPerformRotate
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042B99E);

			*TESRenderWindow::RefreshFlag = 1;
			__asm
			{
				pushad
				push	0
				call	TESRenderControlProcessFrozenRefs
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlPerformFall
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042886F);
			_hhSetVar(Call, 0x00512990);
			__asm
			{
				pushad
				push	0
				call	TESRenderControlProcessFrozenRefs
				popad

				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderUndoStackRecordRef
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00432D48);
			__asm
			{
				mov		eax, [esp + 0x8]

				pushad
				push	eax
				call	TESRenderControlProcessFrozenRefs
				popad

				push    ebx
				push    ebp
				xor     ebp, ebp
				cmp     [esp + 0x10], ebp

				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESObjectREFREditDialogHook(NiNode* Node, bool State)
		{
			SME::MiscGunk::ToggleFlag(&Node->m_flags, TESObjectREFR::kNiNodeSpecialFlags_DontUncull, State);
		}

		#define _hhName		TESObjectREFRSetupDialog
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00549A05);
			__asm
			{
				mov     eax, [edx + 0x180]
				mov     ecx, esi
				call    eax

				pushad
				push	1
				push	eax
				call	DoTESObjectREFREditDialogHook
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESObjectREFRCleanDialog
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00549B57);
			__asm
			{
				push    edi
				mov     ecx, ebx
				call    edx

				pushad
				push	0
				push	eax
				call	DoTESObjectREFREditDialogHook
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlPerformFallVoid
		_hhBegin()
		{
			_hhSetVar(Retn, 0x004270C9);
			_hhSetVar(Jump, 0x00427193);
			__asm
			{
				test	eax, eax
				jz		FIX

				mov		edx, [eax + 0x8]
				mov		[esp + 0x3C], edx

				jmp		_hhGetVar(Retn)
			FIX:
				jmp		_hhGetVar(Jump)
			}
		}

		void __stdcall DoForceShowTESObjectREFRDialogHook(HWND PropertiesDialog)
		{
			TESRenderWindow::Redraw();
			SetWindowPos(PropertiesDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}

		#define _hhName		ForceShowTESObjectREFRDialog
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042EF86);
			__asm
			{
				pushad
				push	eax
				call	DoForceShowTESObjectREFRDialogHook
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		static float s_MovementSettingBuffer = 0.0;

		enum
		{
			kMovementSetting_RefMovementSpeed = 0,
			kMovementSetting_RefSnapGrid,
			kMovementSetting_RefRotationSpeed,
			kMovementSetting_RefSnapAngle,
			kMovementSetting_CamRotationSpeed,
			kMovementSetting_CamZoomSpeed,
			kMovementSetting_CamPanSpeed,
		};

		void __stdcall InitializeCurrentRenderWindowMovementSetting(UInt8 Setting)
		{
			switch (Setting)
			{
			case kMovementSetting_RefMovementSpeed:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltRefMovementSpeed.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::RefMovementSpeed;

				break;
			case kMovementSetting_RefSnapGrid:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltRefSnapGrid.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::SnapGridDistance;

				break;
			case kMovementSetting_RefRotationSpeed:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltRefRotationSpeed.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::RefRotationSpeed;

				break;
			case kMovementSetting_RefSnapAngle:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltRefSnapAngle.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::SnapAngle;

				break;
			case kMovementSetting_CamRotationSpeed:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltCamRotationSpeed.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::CameraRotationSpeed;

				break;
			case kMovementSetting_CamZoomSpeed:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltCamZoomSpeed.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::CameraZoomSpeed;

				break;
			case kMovementSetting_CamPanSpeed:
				if (TESRenderWindow::UseAlternateMovementSettings)
					s_MovementSettingBuffer = Settings::Renderer::kAltCamPanSpeed.GetData().f;
				else
					s_MovementSettingBuffer = *TESRenderWindow::CameraPanSpeed;

				break;
			default:
				s_MovementSettingBuffer = 0.0;

				break;
			}
		}

		#define _hhName		TESRenderControlAltSnapGrid
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00425A3E);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_RefSnapGrid);
			__asm	popad
			__asm
			{
				fild	s_MovementSettingBuffer
				fstp	dword ptr [esp + 0x20]

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltRefMovementSpeedA
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00425741);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_RefMovementSpeed);
			__asm	popad
			__asm
			{
				fmul	s_MovementSettingBuffer
				lea		ecx, [esp + 0x28]

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltRefMovementSpeedB
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042BE85);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_RefMovementSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltRefMovementSpeedC
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042D0B2);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_RefMovementSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltRefRotationSpeed
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00425DC1);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_RefRotationSpeed);
			__asm	popad
			__asm
			{
				fmul	s_MovementSettingBuffer

				mov		eax, [TESRenderWindow::StateFlags]
				mov		eax, [eax]
				test	eax, 0x2

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltRefSnapAngle
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00425DCD);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_RefSnapAngle);
			__asm	popad
			__asm
			{
				fild	s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltCamRotationSpeed
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CCB0);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_CamRotationSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltCamZoomSpeedA
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CCE5);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_CamZoomSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltCamZoomSpeedB
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CDB4);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_CamZoomSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltCamPanSpeedA
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CD2B);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_CamPanSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlAltCamPanSpeedB
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CD76);
			__asm	pushad
			InitializeCurrentRenderWindowMovementSetting(kMovementSetting_CamPanSpeed);
			__asm	popad
			__asm
			{
				lea		ecx, s_MovementSettingBuffer

				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESRenderControlRedrawGrid
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042EF88);
			_asm	pushad
			TESRenderWindow::Redraw(true);
			SetActiveWindow(*TESCSMain::WindowHandle);
			SetActiveWindow(*TESRenderWindow::WindowHandle);
			SetForegroundWindow(*TESRenderWindow::WindowHandle);
			__asm
			{
				popad
				mov		eax, 1
				jmp		_hhGetVar(Retn)
			}
		}

		UInt32 __stdcall DoTESPreviewControlCallWndProcHook(void* OrgWindowProc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (OrgWindowProc)
				return CallWindowProc((WNDPROC)OrgWindowProc, hWnd, uMsg, wParam, lParam);
			else
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		#define _hhName		TESPreviewControlCallWndProc
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0044D70E);
			__asm
			{
				push    ebx
				push    ebp
				push    edi
				push    esi
				push	eax
				call	DoTESPreviewControlCallWndProcHook

				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall ActivateRenderWindow(void)
		{
			if (TESLODTextureGenerator::GeneratorState == TESLODTextureGenerator::kLODDiffuseMapGeneratorState_NotInUse)
				SetForegroundWindow(*TESRenderWindow::WindowHandle);
		}

		#define _hhName		ActivateRenderWindowPostLandTextureChange
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042B4EB);
			__asm
			{
				pushad
				call	IATCacheSendMessageAddress
				popad

				call	IATProcBuffer
				pushad
				call	ActivateRenderWindow
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		static bool s_PathGridMoveStart = false;

		#define _hhName		TESPathGridRecordOperationMoveA
		_hhBegin()
		{
			_hhSetVar(Call, 0x0054D600);
			_hhSetVar(Retn, 0x0042A632);
			__asm
			{
				mov		s_PathGridMoveStart, 1
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESPathGridRecordOperation(void)
		{
			CSEPathGridUndoManager::Instance.ResetRedoStack();

			if (TESRenderWindow::SelectedPathGridPoints->Count())
				CSEPathGridUndoManager::Instance.RecordOperation(CSEPathGridUndoManager::kOperation_DataChange, TESRenderWindow::SelectedPathGridPoints);
		}

		void __stdcall DoTESPathGridRecordOperationMoveBHook(void)
		{
			if (s_PathGridMoveStart)
			{
				s_PathGridMoveStart = false;
				DoTESPathGridRecordOperation();
			}
		}

		#define _hhName		TESPathGridRecordOperationMoveB
		_hhBegin()
		{
			_hhSetVar(Call, 0x004FC950);
			_hhSetVar(Retn, 0x0042BE72);
			__asm
			{
				pushad
				call	DoTESPathGridRecordOperationMoveBHook
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESPathGridRecordOperationLink
		_hhBegin()
		{
			_hhSetVar(Call, 0x00405DA0);
			_hhSetVar(Retn, 0x0042A82E);
			__asm
			{
				pushad
				call	DoTESPathGridRecordOperation
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESPathGridRecordOperationFlag
		_hhBegin()
		{
			_hhSetVar(Call, 0x005557A0);
			_hhSetVar(Retn, 0x0042A719);
			__asm
			{
				pushad
				call	DoTESPathGridRecordOperation
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESPathGridRecordOperationRef
		_hhBegin()
		{
			_hhSetVar(Call, 0x00405DA0);
			_hhSetVar(Retn, 0x0042836C);
			__asm
			{
				pushad
				call	DoTESPathGridRecordOperation
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESPathGridDeletePointHook(void)
		{
			CSEPathGridUndoManager::Instance.ResetRedoStack();
			CSEPathGridUndoManager::Instance.HandlePathGridPointDeletion(TESRenderWindow::SelectedPathGridPoints);

			if (TESRenderWindow::SelectedPathGridPoints->Count())
				CSEPathGridUndoManager::Instance.RecordOperation(CSEPathGridUndoManager::kOperation_PointDeletion, TESRenderWindow::SelectedPathGridPoints);
		}

		#define _hhName		TESPathGridDeletePoint
		_hhBegin()
		{
			_hhSetVar(Call, 0x0048E0E0);
			_hhSetVar(Retn, 0x004291CB);
			__asm
			{
				pushad
				call	DoTESPathGridDeletePointHook
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESPathGridPointDtorHook(TESPathGridPoint* Point)
		{
			PathGridPointListT* DeletionList = (PathGridPointListT*)PathGridPointListT::Create(&FormHeap_Allocate);
			DeletionList->AddAt(Point, eListEnd);
			CSEPathGridUndoManager::Instance.HandlePathGridPointDeletion(DeletionList);
			DeletionList->RemoveAll();
			FormHeap_Free(DeletionList);
		}

		#define _hhName		TESPathGridPointDtor
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00556197);
			__asm
			{
				mov		eax, [esp]
				sub		eax, 5
				cmp		eax, 0x0054E5A3
				jnz		CULL

				mov		eax, [esp + 0x18]
				sub		eax, 5
				cmp		eax, 0x00429200		// don't handle deletion if called from the render window wnd proc, as we already do that in the previous hook
				jz		SKIP
			CULL:
				pushad
				push	ecx
				call	DoTESPathGridPointDtorHook
				popad
			SKIP:
				push    ebx
				push    esi
				mov     esi, ecx
				lea     ecx, [esi + 0x10]
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESPathGridToggleEditModeHook(void)
		{
			CSEPathGridUndoManager::Instance.ResetRedoStack();
			CSEPathGridUndoManager::Instance.ResetUndoStack();
		}

		#define _hhName		TESPathGridToggleEditMode
		_hhBegin()
		{
			_hhSetVar(Call, 0x0054C560);
			_hhSetVar(Retn, 0x00550665);
			__asm
			{
				pushad
				call	DoTESPathGridToggleEditModeHook
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESPathGridCreateNewLinkedPointHook(void)
		{
			CSEPathGridUndoManager::Instance.ResetRedoStack();

			if (TESRenderWindow::SelectedPathGridPoints->Count())
				CSEPathGridUndoManager::Instance.RecordOperation(CSEPathGridUndoManager::kOperation_PointCreation, TESRenderWindow::SelectedPathGridPoints);
		}

		#define _hhName		TESPathGridCreateNewLinkedPoint
		_hhBegin()
		{
			_hhSetVar(Call, 0x004E3900);
			_hhSetVar(Retn, 0x0042B380);
			__asm
			{
				call	_hhGetVar(Call)
				pushad
				call	DoTESPathGridCreateNewLinkedPointHook
				popad
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESPathGridShowMultipleSelectionRingHook(TESPathGridPoint* Point)
		{
			Point->ShowSelectionRing();
		}

		#define _hhName		TESPathGridShowMultipleSelectionRing
		_hhBegin()
		{
			_hhSetVar(Call, 0x004E3900);
			_hhSetVar(Retn, 0x0042FC81);
			__asm
			{
				pushad
				push	esi
				call	DoTESPathGridShowMultipleSelectionRingHook
				popad
				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESPathGridPerformFall
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00428610);
			__asm
			{
				mov		ebx, TESRenderWindow::SelectedPathGridPoints
				lea		ebx, [ebx]

				pushad
				call	DoTESPathGridRecordOperation
				popad
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoInitialCellLoadCameraPositionHook(TESObjectCELL* Cell)
		{
			static const Vector3 kDepletedMan(0.0, 0.0, 0.0), kNotSoDepletedMan(0.0, 0.0, -8192.0);

			if (Cell->objectList.Count())
			{
				TESObjectREFR* FirstRef = NULL;

				for (TESObjectCELL::ObjectREFRList::Iterator Itr = Cell->objectList.Begin(); Itr.End() == false && Itr.Get(); ++Itr)
				{
					if (FirstRef == NULL)
						FirstRef = Itr.Get();
					else if ((FirstRef->formID & 0xFFFFFF) > (Itr->formID & 0xFFFFFF))
						FirstRef = Itr.Get();
				}

				if (FirstRef)
				{
					_TES->LoadCellIntoViewPort(&kDepletedMan, FirstRef);
					_RENDERSEL->ClearSelection(true);
					return;
				}
			}

			SendMessage(*TESRenderWindow::WindowHandle, 0x40D, NULL, (LPARAM)&kNotSoDepletedMan);
		}

		#define _hhName		InitialCellLoadCameraPosition
		_hhBegin()
		{
			_hhSetVar(Call, 0x00532240);
			_hhSetVar(Retn, 0x0040A8B7);
			_hhSetVar(Jump, 0x0040A8D8);
			__asm
			{
				call	_hhGetVar(Call)
				test	al, al
				jnz		FIX

				jmp		_hhGetVar(Retn)
			FIX:
				push	ecx
				call	DoInitialCellLoadCameraPositionHook
				jmp		_hhGetVar(Jump)
			}
		}

		#define _hhName		ConvertNiRenderedTexToD3DBaseTex
		_hhBegin()
		{
			_hhSetVar(Retn, 0x004116A5);
			__asm
			{
				push	esi		// store IDirect3DBaseTexture9*

				mov     esi, [esp + 0x30]
				lea     eax, [esi + 4]
				push    eax
				pushad
				call	IATCacheInterlockedDecrementAddress
				popad
				call	IATProcBuffer
				test    eax, eax
				jnz     EXIT
				mov     edx, [esi]
				mov     eax, [edx]
				push    1
				mov     ecx, esi
				call    eax
			EXIT:
				pop		esi		// restore
				mov		eax, esi
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoDuplicateReferencesHook(void)
		{
			for (TESRenderSelection::SelectedObjectsEntry* Itr = _RENDERSEL->selectionList; Itr && Itr->Data; Itr = Itr->Next)
			{
				TESObjectREFR* Reference = CS_CAST(Itr->Data, TESForm, TESObjectREFR);
				Reference->SetPosition(Reference->position.x, Reference->position.y, Reference->position.z + 10.0f);
			}
		}

		#define _hhName		DuplicateReferences
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042EC6C);
			__asm
			{
				pushad
				call	DoDuplicateReferencesHook
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoRenderToAuxiliaryViewportHook(void)
		{
			if (AUXVIEWPORT->GetVisible() == false)
				return;

			if (AUXVIEWPORT->GetFrozen() == false)
			{
				AUXVIEWPORT->SyncViewportCamera(_PRIMARYRENDERER->primaryCamera);
				AUXVIEWPORT->DrawBackBuffer();
			}
			else
				AUXVIEWPORT->Draw(NULL, NULL);
		}

		#define _hhName		RenderToAuxiliaryViewport
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042D415);
			__asm
			{
				pushad
				call	DoRenderToAuxiliaryViewportHook
				popad

				mov     ecx, [eax]
				mov     edx, [ecx]
				mov     eax, [eax+8]
				mov     edx, [edx]
				push    0
				push    0
				push	eax
				call	edx

				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESRenderControlPerformRelativeScaleHook(TESObjectREFR* Ref, float Scale, bool Relative)
		{
			if (Relative == false)
				Ref->SetScale(Scale);
			else
			{
				if (Scale > 10.0f)
					Scale = 10.0;
				else if (Scale < 0.01)
					Scale = 0.01;

				Vector3 PositionOffset(_RENDERSEL->selectionPositionVectorSum.x - Ref->position.x,
									_RENDERSEL->selectionPositionVectorSum.y - Ref->position.y,
									_RENDERSEL->selectionPositionVectorSum.z - Ref->position.z);

				float ScaleFactor = (Ref->scale - Scale)/Ref->scale;

				Ref->scale = Scale;
				Ref->position.x += PositionOffset.x * ScaleFactor;
				Ref->position.y += PositionOffset.y * ScaleFactor;
				Ref->position.z += PositionOffset.z * ScaleFactor;
				Ref->UpdateNiNode();
			}
		}

		#define _hhName		TESRenderControlPerformRelativeScale
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00424807);
			__asm
			{
				pop		ecx
				pushad
				movzx	eax, bl
				push	eax
				push	ecx
				push	esi
				call	DoTESRenderControlPerformRelativeScaleHook
				popad

				jmp		_hhGetVar(Retn)
			}
		}

		bool __stdcall DoTESPathGridRubberBandSelectionHook(void)
		{
			if (TESRenderWindow::CurrentMouseLBDragCoordDelta.x < 2 && TESRenderWindow::CurrentMouseLBDragCoordDelta.y < 2)
				return false;
			else
				return true;
		}

		#define _hhName		TESPathGridRubberBandSelection
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042FBE8);
			__asm
			{
				pushad
				call	DoTESPathGridRubberBandSelectionHook
				test	al, al
				jz		SKIP
				popad

				push	ecx
				mov		[esp], ecx
				mov		ecx, [esp + 0x8]

				jmp		_hhGetVar(Retn)
			SKIP:
				popad
				retn	8
			}
		}

		bool __stdcall DoCoplanarRefDropHook(NiCamera* Camera, int XCoord, int YCoord, Vector3* OutPosition, Vector3* OutRotation)
		{
			bool Result = true;

			thisCall<bool>(0x006FF1A0, Camera, XCoord, YCoord, OutPosition, OutRotation);
			Vector3 PosBuf(*OutPosition), RotBuf(*OutRotation);

			if (Settings::Renderer::kCoplanarRefDrops.GetData().i)
			{
				// perform the necessary (nose)picking nonsense
				thisCall<void>(0x00417C40, 0x00A0BC64, _TES->sceneGraphObjectRoot);
				if (thisCall<bool>(0x005E6030, 0x00A0BC64, OutPosition, OutRotation, 0))
				{
					TODO("Repent the sin I'm about to commit")

					float*** NewPosition = (float***)0x00A0BC80;
					
					OutPosition->x = *(float*)((UInt32)**NewPosition + 0x8);
					OutPosition->y = *(float*)((UInt32)**NewPosition + 0xC);
					OutPosition->z = *(float*)((UInt32)**NewPosition + 0x10);

					OutRotation->Scale(0.0f);

					Result = false;
				}

				if (_TES->currentInteriorCell == NULL)
				{
					TESWorldSpace* CurrentWorldspace = _TES->currentWorldSpace;
					if (CurrentWorldspace == NULL || _DATAHANDLER->GetExteriorCell(OutPosition->x, OutPosition->y, CurrentWorldspace) == NULL)
					{
						*OutPosition = PosBuf;
						*OutRotation = RotBuf;
					}
				}
			}

			return Result;
		}

		#define _hhName		CoplanarRefDrop
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042DE37);
			__asm
			{
				push	ecx
				xor		eax, eax
				call	DoCoplanarRefDropHook
				mov		ecx, 0x00A0E064
				mov		ecx, [ecx]
				push	eax
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoInitPathGridNodeSelectionRingHook(void)
		{
			for (tList<TESPathGridPoint>::Iterator Itr = TESRenderWindow::SelectedPathGridPoints->Begin(); !Itr.End() && Itr.Get(); ++Itr)
			{
				Itr.Get()->selected = 0;
			}
		}

		#define _hhName		InitPathGridNodeSelectionRing
		_hhBegin()
		{
			_hhSetVar(Call, 0x00405DC0);
			_hhSetVar(Retn, 0x00419AFF);
			__asm
			{
				pushad
				call	DoInitPathGridNodeSelectionRingHook
				popad

				call	_hhGetVar(Call)
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		TESLoadIntoViewPort
		_hhBegin()
		{
			_hhSetVar(Retn, 0x00430F61);
			__asm
			{
				push	ebx
				mov		ebx, [esp + 0xC]
				test	ebx, ebx
				jnz		TESTCELL
			ENDTEST:
				jmp		_hhGetVar(Retn)
			TESTCELL:
				mov		eax, [ebx + 0x48]
				test	eax, eax
				jnz		ENDTEST
				pop		ebx
				retn
			}
		}

		#define _hhName		RenderWindowAxisHotkeysMoveReferences
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CB81);
			__asm
			{
				push	edx
				push	eax			// swap x/y key states, stored in al and cl respectively
				push	ecx
				mov		eax, 0x00A0BACC
				mov		eax, [eax]
				jmp		_hhGetVar(Retn)
			}
		}

		#define _hhName		RenderWindowAxisHotkeysMovePathGridPoints
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042BF1E);
			__asm
			{
				push	eax			// swap x/y key states, stored in al and cl respectively
				push	ecx
				push	ebx
				push	edi
				push	ecx
				mov		ecx, esi
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoBSFadeNodeDrawTransparencyHook(BSFadeNode* FadeNode, float* OutAlpha)
		{
			if ((FadeNode->m_flags & TESObjectREFR::kNiNodeSpecialFlags_SpecialFade))
			{
				*OutAlpha = FadeNode->fCurrentAlpha;
			}
		}

		#define _hhName		BSFadeNodeDrawTransparency
		_hhBegin()
		{
			_hhSetVar(Retn, 0x004BC52E);
			__asm
			{
				mov		eax, esp
				add		eax, 0x2C

				pushad
				push	eax
				push	esi
				call	DoBSFadeNodeDrawTransparencyHook
				popad

				cmp		byte ptr [esi + 0xDC], 0
				jmp		_hhGetVar(Retn)
			}
		}

		void __stdcall DoTESRubberBandSelectionSkipInvisibleRefsHook(TESRenderSelection* Selection, TESObjectREFR* Ref, bool ShowSelectionRing)
		{
			NiNode* Node = Ref->GetNiNode();
			if (Node)
			{
				if ((Node->m_flags & NiAVObject::kFlag_AppCulled) == false)
					Selection->AddToSelection(Ref, ShowSelectionRing);
			}
		}

		#define _hhName		TESRubberBandSelectionSkipInvisibleRefs
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042FB8E);
			__asm
			{
				push	ecx
				call	DoTESRubberBandSelectionSkipInvisibleRefsHook
				jmp		_hhGetVar(Retn)
			}
		}

		bool __stdcall DoRenderWindowCameraRotationPivotHook(Vector3* OutPivot, UInt8* AlternatePivot)
		{
			bool Enabled = _RENDERSEL->selectionCount == 0 && Settings::Renderer::kFixedCameraPivot.GetData().i;

			if (Enabled && GetAsyncKeyState(VK_CONTROL) == FALSE ||
				(Enabled == false && GetAsyncKeyState(VK_CONTROL)))
			{
				Vector3* StaticPivot = (Vector3*)SendMessage(*TESRenderWindow::WindowHandle, WM_RENDERWINDOW_GETCAMERASTATICPIVOT, NULL, NULL);
				*OutPivot = *StaticPivot;

				return true;
			}

			return false;
		}

		#define _hhName		RenderWindowCameraRotationPivot
		_hhBegin()
		{
			_hhSetVar(Retn, 0x0042CC04);
			_hhSetVar(Jump, 0x0042CCA3);
			__asm
			{
				lea		eax, [esp + 0x1C]
				push	0x00A0BC21
				push	eax
				call	DoRenderWindowCameraRotationPivotHook
				test	al, al
				jnz		USECUSTOMPIVOT

				mov		eax, 0x00A0BC21
				cmp		byte ptr [eax], 0
				jmp		_hhGetVar(Retn)
			USECUSTOMPIVOT:
				jmp		_hhGetVar(Jump)
			}
		}
	}
}

#pragma warning(pop)
#pragma optimize("", on)