#include "[Common]\ComponentDLLInterface.h"
#include "TagBrowser.h"

using namespace ComponentDLLInterface;
using namespace ConstructionSetExtender;

extern ComponentDLLInterface::TagBrowserInterface g_InteropInterface;

extern "C"
{
	__declspec(dllexport) void* QueryInterface(void)
	{
		return &g_InteropInterface;
	}
}

void ShowTagBrowserDialog(HWND Parent)
{
	TAGBRWR->Show(IntPtr(Parent));
}

void HideTagBrowserDialog(void)
{
	TAGBRWR->Hide();
}

bool AddFormToActiveTag(ComponentDLLInterface::FormData* Data)
{
	return TAGBRWR->AddItemToActiveTag(Data);
}

HWND GetFormDropWindowHandle()
{
	return (HWND)TAGBRWR->GetFormListHandle();
}

HWND GetFormDropParentHandle()
{
	return (HWND)TAGBRWR->GetWindowHandle();
}

ComponentDLLInterface::TagBrowserInterface g_InteropInterface =
{
	DeleteManagedHeapPointer,
	ShowTagBrowserDialog,
	HideTagBrowserDialog,
	AddFormToActiveTag,
	GetFormDropWindowHandle,
	GetFormDropParentHandle
};