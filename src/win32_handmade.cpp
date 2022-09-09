#include <windows.h>

int CALLBACK
WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	MessageBox(0, "This is Handmade Hero.", "Handmade Hero", MB_ICONINFORMATION | MB_OK);
	return (0);
}