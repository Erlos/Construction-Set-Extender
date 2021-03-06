#pragma once
#include "IncludesCLR.h"
#include "ComponentDLLInterface.h"

namespace cse
{
	namespace nativeWrapper
	{
		[DllImport("Construction Set Extender.dll")]
		void*											QueryInterface(void);

		[DllImport("user32.dll")]
		bool											LockWindowUpdate(IntPtr hWndLock);
		[DllImport("user32.dll")]
		IntPtr											WindowFromPoint(Point Point);
		[DllImport("user32.dll")]
		IntPtr											GetParent(IntPtr Handle);
		[DllImport("user32.dll")]
		bool											SetWindowPos(IntPtr hWnd, int hWndInsertAfter, int X, int Y, int cx, int cy, UInt32 uFlags);
		[DllImport("user32.dll")]
		bool											ShowWindow(IntPtr hWnd, int nCmdShow);
		[DllImport("user32.dll")]
		IntPtr											SendMessageA(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);
		[DllImport("user32.dll")]
		IntPtr											SetActiveWindow(IntPtr handle);
		[DllImport("user32.dll")]
		IntPtr											GetActiveWindow();
		[DllImport("user32.dll")]
		IntPtr											GetForegroundWindow();
		[DllImport("user32.dll")]
		IntPtr											GetFocus();
		[DllImport("gdi32.dll")]
		int												DeleteObject(IntPtr hObject);

		void											WriteToMainWindowStatusBar(int PanelIndex, String^ Message);
		void											ShowNonActivatingWindow(Control^ Window, IntPtr ParentHandle);

		extern componentDLLInterface::CSEInterfaceTable*		g_CSEInterfaceTable;
	};
}