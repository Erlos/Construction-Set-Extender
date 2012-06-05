#pragma once
#include <BGSEERenderWindowPainter.h>

namespace ConstructionSetExtender
{
	namespace INISettings
	{
		enum
		{
			kRenderWindowPainter_ShowSelectionStats = 0,
			kRenderWindowPainter_ShowRAMUsage,

			kRenderWindowPainter__MAX
		};
		extern const BGSEditorExtender::BGSEEINIManagerSettingFactory::SettingData		kRenderWindowPainterINISettings[kRenderWindowPainter__MAX];
		BGSEditorExtender::BGSEEINIManagerSettingFactory*								GetRenderWindowPainter(void);

		enum
		{
			kRenderer_UpdatePeriod = 0,
			kRenderer_UpdateViewPortAsync,
			kRenderer_AltRefMovementSpeed,
			kRenderer_AltRefSnapGrid,
			kRenderer_AltRefRotationSpeed,
			kRenderer_AltRefSnapAngle,
			kRenderer_AltCamRotationSpeed,
			kRenderer_AltCamZoomSpeed,
			kRenderer_AltCamPanSpeed,
			kRenderer_CoplanarRefDrops,

			kRenderer__MAX
		};
		extern const BGSEditorExtender::BGSEEINIManagerSettingFactory::SettingData		kRendererINISettings[kRenderer__MAX];
		BGSEditorExtender::BGSEEINIManagerSettingFactory*								GetRenderer(void);
	}

	namespace RenderWindowPainter
	{
		extern BGSEditorExtender::BGSEEStaticRenderChannel*						RenderChannelSelectionStats;
		extern BGSEditorExtender::BGSEEStaticRenderChannel*						RenderChannelRAMUsage;
		extern BGSEditorExtender::BGSEEDynamicRenderChannel*					RenderChannelNotifications;

		class CSERAMUsageRenderChannel : public BGSEditorExtender::BGSEEStaticRenderChannel
		{
		protected:
			static CSERAMUsageRenderChannel*		Singleton;

			static VOID CALLBACK					TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
			static bool								RenderChannelCallback(std::string& RenderedText);

			DWORD									TimerID;
			UInt32									RAMCounter;

			CSERAMUsageRenderChannel(INT FontHeight,
				INT FontWidth,
				UINT FontWeight,
				const char* FontFace,
				D3DCOLOR Color,
				RECT* DrawArea,
				DWORD DrawFormat,
				UInt32 DrawAreaFlags);
		public:
			virtual ~CSERAMUsageRenderChannel();

			static CSERAMUsageRenderChannel*		GetSingleton();
		};

		void																	Initialize(void);
	}
};