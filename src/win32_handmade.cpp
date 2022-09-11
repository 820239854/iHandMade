#include <windows.h>
#include <stdint.h>

// unsigned integers
typedef uint8_t u8;     // 1-byte long unsigned integer
typedef uint16_t u16;   // 2-byte long unsigned integer
typedef uint32_t u32;   // 4-byte long unsigned integer
typedef uint64_t u64;   // 8-byte long unsigned integer
// signed integers
typedef int8_t s8;      // 1-byte long signed integer
typedef int16_t s16;    // 2-byte long signed integer
typedef int32_t s32;    // 4-byte long signed integer
typedef int64_t s64;    // 8-byte long signed integer

#define internal static 
#define local_persist static 
#define global_variable static 

struct win32_window_dimension
{
	int Width;
	int Height;
};
global_variable bool Running;
struct win32_offscreen_buffer
{
	// NOTE(casey): Pixels are always 32-bits wide, 
	// Memory Order  0x BB GG RR xx
	// Little Endian 0x xx RR GG BB
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
};
global_variable win32_offscreen_buffer GlobalBackbuffer;

internal win32_window_dimension Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result; 
    
	RECT ClientRect;
	GetClientRect(Window, &ClientRect);    
	Result.Width = ClientRect.right - ClientRect.left;
	Result.Height = ClientRect.bottom - ClientRect.top;
    
	return(Result);
}

internal void RenderWeirdGradient(win32_offscreen_buffer *Buffer, int XOffset, int YOffset)
{
	int Pitch = Buffer->Width * Buffer->BytesPerPixel;
	u8 *Row = (u8 *)Buffer->Memory;
	for (int Y = 0;Y < Buffer->Height;++Y)
	{
		u32* Pixel = (u32*)Row;
		for(int X = 0;X < Buffer->Width;++X)
		{
			u8 Red = 0;
			u8 Green = (u8)(Y + YOffset);
			u8 Blue = (u8)(X + XOffset);
			*Pixel++ = Red << 16 | Green << 8 | Blue; // << 0
		}
		Row += Buffer->Pitch;
	}
}

internal void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
	// NOTE(yakvi): Remember to VirtualFree the memory if we ever
	// call this function more than once on the same buffer!
	/*	if(Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}*/
	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->BytesPerPixel = 4;
	Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;
    
	// NOTE(casey): When the biHeight field is negative, this is the clue 
	// to Windows to treat this bitmap as top-down, not bottom-up, meaning
	// that the first bytes of the image are the color for the top left 
	// pixel in the bitmap, not the bottom left!
	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = Buffer->Width;
	Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // negative value: top-down
	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32;
	Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
	int BitmapMemorySize = Buffer->BytesPerPixel * (Buffer->Width * Buffer->Height);
    
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

internal void Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,HDC DeviceContext, int WindowWidth, int WindowHeight)
{
	StretchDIBits(DeviceContext, // 003
	              /* // 004 001
	              X, Y, Width, Height, // 003 002
	              X, Y, Width, Height, // 003 002
	              */ // 004 001
	              0, 0, WindowWidth, WindowHeight,
	              0, 0, Buffer->Width, Buffer->Height,
	              Buffer->Memory,
	              &Buffer->Info,
	              DIB_RGB_COLORS, SRCCOPY); // 003
}


LRESULT CALLBACK Win32MainWindowCallback(HWND Window,
                                    UINT Message,
                                    WPARAM WParam,
                                    LPARAM LParam)
{
	LRESULT Result = 0;
	switch (Message)
	{
		case WM_DESTROY:
		{
			Running = false;
		} break;
    
		case WM_CLOSE:
		{
			Running = false;
		} break;
    
		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
			EndPaint(Window, &Paint);
		} break;

		default:
		{
			Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}
	return (Result);
}


int CALLBACK WinMain(HINSTANCE Instance,
                     HINSTANCE PrevInstance,
                     LPSTR     CommandeLine,
                     int       ShowCode)
{
	WNDCLASSA WindowClass = {};
	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = "HandmadeHeroWindowClass";
	if (RegisterClassA(&WindowClass))
	{
		HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "Handmade Hero",
		                              WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		                              CW_USEDEFAULT, CW_USEDEFAULT,
		                              CW_USEDEFAULT, CW_USEDEFAULT,
		                              0, 0, Instance, 0);
		if (Window)
		{
			Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);
			// NOTE(casey): Since we specified CS_OWNDC, we can just
			// get one device context and use it forever because we
			// are not sharing it with anyone.
			HDC DeviceContext = GetDC(Window);
			int XOffset = 0; 
			int YOffset = 0; 
			Running = true;
			while (Running) // Replaces "for" loop
			{
				MSG Message;
				while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
				{
					if (Message.message == WM_QUIT)
					{
						Running = false;
					}
					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				}
				RenderWeirdGradient(&GlobalBackbuffer, XOffset, YOffset);
				++XOffset;

				win32_window_dimension Dimension = Win32GetWindowDimension(Window);
				Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
			}
		}
		else
		{
			// Window Creation failed! 
			// TODO(casey): Logging
		}
	}
	return (0);
}

