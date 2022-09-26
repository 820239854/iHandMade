// TODO(casey): Implement sine ourselves
#include <math.h>
#include <stdint.h>
// unsigned integers
typedef uint8_t u8;     // 1-byte long unsigned integer
typedef uint16_t u16;   // 2-byte long unsigned integer
typedef uint32_t u32;   // 4-byte long unsigned integer
typedef uint64_t u64;   // 8-byte long unsigned integer
typedef float f32;
typedef double f64;
// signed integers
typedef int8_t s8;      // 1-byte long signed integer
typedef int16_t s16;    // 2-byte long signed integer
typedef int32_t s32;    // 4-byte long signed integer
typedef int64_t s64;    // 8-byte long signed integer
typedef s32 b32;

#define internal static 
#define local_persist static 
#define global_variable static

#define Pi32 3.14159265359f

#include "handmade.cpp"

#include <windows.h>
#include <stdio.h>
#include <xinput.h>
#include <dsound.h>
#include "win32_handmade.h"

global_variable bool GlobalRunning;

global_variable win32_offscreen_buffer GlobalBackbuffer;
global_variable IDirectSoundBuffer *GlobalSecondaryBuffer;

// Define a function macro
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
// Define a type of a function
typedef X_INPUT_GET_STATE(x_input_get_state);
typedef X_INPUT_SET_STATE(x_input_set_state);

X_INPUT_GET_STATE(XInputGetStateStub) 
{ 
	return (ERROR_DEVICE_NOT_CONNECTED); 
}
X_INPUT_SET_STATE(XInputSetStateStub) 
{ 
	return (ERROR_DEVICE_NOT_CONNECTED); 
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub; 
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
// Create an "alias" to be able to call it with its old name
#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_
// NOTE(yakvi): DirectSoundCreate
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

internal void DEBUGPlatformFreeFileMemory(void *Memory)
{
	if (Memory)
	{
		VirtualFree(Memory, 0, MEM_RELEASE);
	}
}

internal debug_read_file_result DEBUGPlatformReadEntireFile(const char *Filename)
{
	debug_read_file_result Result = {};
	HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER FileSize;
		if(GetFileSizeEx(FileHandle, &FileSize))
		{
			u32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
			Result.Contents = VirtualAlloc(0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
			if (Result.Contents)
			{
				DWORD BytesRead;
				if (ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) &&
				    (FileSize32 == BytesRead))
				{
					// NOTE(casey): File read successfully
					Result.ContentsSize = BytesRead;
				}
				else
				{
					// Error: Read failed
					DEBUGPlatformFreeFileMemory(Result.Contents);
					Result.Contents = 0;
					// TODO(casey): Logging
				}
			}
			else
			{
				// Error: Memory allocation failed
				// TODO(casey): Logging
			}
			CloseHandle(FileHandle);
		}
		else
		{
			// Error: File size evaluation failed
			// TODO(casey): Logging
		}
	}
	else
	{
		// Error: handle creation failed
		// TODO(casey): Logging
	}
	return (Result);
}

internal b32 DEBUGPlatformWriteEntireFile(const char *Filename, u32 MemorySize, void *Memory)
{
	b32 Result = false;
    
	HANDLE FileHandle = CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if(FileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD BytesWritten;
		if(WriteFile(FileHandle, Memory, MemorySize, &BytesWritten, 0))
		{
			// NOTE(casey): File written successfully
			Result = (BytesWritten == MemorySize);
		}
		else
		{
			// Error: Write failed
			// TODO(casey): Logging
		}
		CloseHandle(FileHandle);
	}
	else
	{
		// Error: Handle creation failed
		// TODO(casey): Logging
	}
    
	return(Result);
}


internal void Win32LoadXInput()
{
	HMODULE XInputLibrary = LoadLibraryA("Xinput1_4.dll");
	if (!XInputLibrary)
	{
		XInputLibrary = LoadLibraryA("Xinput1_3.dll");
	}
	if (!XInputLibrary)
	{
		XInputLibrary = LoadLibraryA("Xinput9_1_0.dll");
	}
	if (XInputLibrary)
	{
		XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
		if (!XInputGetState) { XInputGetState = XInputGetStateStub; }
		XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
		if (!XInputSetState) { XInputSetState = XInputSetStateStub; }
	}
	else
	{
		// We still don't have any XInputLibrary
		XInputGetState = XInputGetStateStub;
		XInputSetState = XInputSetStateStub;
		// TODO(casey): Diagnostic
	}
}

internal void Win32ProcessXInputDigitalButton(DWORD XInputButtonState,
	                                game_button_state *OldState, DWORD ButtonBit,
	                                game_button_state *NewState)
{
	NewState->EndedDown = ((XInputButtonState & ButtonBit) == ButtonBit);
	NewState->HalfTransitionCount = (OldState->EndedDown != NewState->EndedDown) ? 1 : 0;
}

internal void Win32InitDSound(HWND Window, s32 SamplesPerSecond, s32 BufferSize)
{
	// NOTE(casey): Load the library 
	HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
	if(DSoundLibrary)
	{
		// NOTE(casey): Get a DirectSound object 
		direct_sound_create *DirectSoundCreate = (direct_sound_create*)GetProcAddress(DSoundLibrary, "DirectSoundCreate");
		IDirectSound *DirectSound;
		if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
		{
			WAVEFORMATEX WaveFormat = {};
			WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
			WaveFormat.nChannels = 2;
			WaveFormat.nSamplesPerSec = SamplesPerSecond;
			WaveFormat.wBitsPerSample = 16;
			WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8; // 4 under current settings
			WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

			if (SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
			{
				// NOTE(casey): "Create" a primary buffer
				DSBUFFERDESC BufferDescription = {};
				BufferDescription.dwSize = sizeof(BufferDescription);
				BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
                
				IDirectSoundBuffer *PrimaryBuffer;
				if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
				{
					if (SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat)))
					{
						OutputDebugStringA("Primary buffer format was set.\n");
					}
				}
			}
			// NOTE(casey): "Create" a secondary buffer
			// NOTE(casey): "Create" a secondary buffer
			DSBUFFERDESC BufferDescription = {};
			BufferDescription.dwSize = sizeof(BufferDescription);      
			BufferDescription.dwBufferBytes = BufferSize;
			BufferDescription.lpwfxFormat = &WaveFormat;

			if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0)))
			{
				OutputDebugStringA("Secondary buffer created Successfully.\n");
			}
		}
	} 
}

internal void Win32ClearBuffer(win32_sound_output *SoundOutput)
{
	VOID *Region1;
	DWORD Region1Size;
	VOID *Region2;
	DWORD Region2Size;
	if(SUCCEEDED(GlobalSecondaryBuffer->Lock(0, SoundOutput->SecondaryBufferSize,
	                                         &Region1, &Region1Size,
	                                         &Region2, &Region2Size,
	                                         0)))
	{
		u8 *DestSample = (u8 *)Region1;
		for (DWORD ByteIndex = 0;
			ByteIndex < Region1Size;
			++ByteIndex)
		{
			*DestSample++ = 0;
		}
		DestSample = (u8 *)Region2;
		for (DWORD ByteIndex = 0;
			ByteIndex < Region2Size;
			++ByteIndex)
		{
			*DestSample++ = 0;
		}
		GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
	}
}

internal void Win32FillSoundBuffer(win32_sound_output *SoundOutput, DWORD ByteToLock, DWORD BytesToWrite,
                                   game_sound_output_buffer *SourceBuffer)
{
	VOID *Region1;
	DWORD Region1Size;
	VOID *Region2;
	DWORD Region2Size;
	if(SUCCEEDED(GlobalSecondaryBuffer->Lock(ByteToLock, BytesToWrite,
	                                         &Region1, &Region1Size,
	                                         &Region2, &Region2Size,
	                                         0)))
	{
		// TODO(casey): assert that Region1Size/Region2Size are valid
		DWORD Region1SampleCount = Region1Size / SoundOutput->BytesPerSample;
		s16 *SourceSample = SourceBuffer->Samples;
		s16 *DestSample = (s16 *)Region1;
		for (DWORD SampleIndex = 0;
			SampleIndex < Region1SampleCount;
			++SampleIndex)
		{
			*DestSample++ = *SourceSample++;
			*DestSample++ = *SourceSample++;
            
			++SoundOutput->RunningSampleIndex;
		}
		DestSample = (s16 *)Region2;
		DWORD Region2SampleCount = Region2Size / SoundOutput->BytesPerSample;
		for (DWORD SampleIndex = 0;
			SampleIndex < Region2SampleCount;
			++SampleIndex)
		{
			*DestSample++ = *SourceSample++;
			*DestSample++ = *SourceSample++;
            
			++SoundOutput->RunningSampleIndex;
		}
        
		GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
	}
}

internal win32_window_dimension Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result; 
    
	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Width = ClientRect.right - ClientRect.left;
	Result.Height = ClientRect.bottom - ClientRect.top;
    
	return(Result);
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
	int BytesPerPixel = 4;
    
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
    
	int BitmapMemorySize = BytesPerPixel * (Buffer->Width * Buffer->Height);
    
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	Buffer->Pitch = Buffer->Width*BytesPerPixel;
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
			GlobalRunning = false;
		} break;
    
		case WM_CLOSE:
		{
			GlobalRunning = false;
		} break;
    
		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			bool IsDown = ((LParam & (1 << 31)) == 0);
			bool WasDown = ((LParam & (1 << 30)) != 0);
			b32 AltKeyWasDown = (LParam & (1 << 29));
			u32 VKCode = WParam;
			if (IsDown != WasDown)
			{
				if (VKCode == 'W')
				{
				}
				else if (VKCode == 'A')
				{
				} 
				else if (VKCode == 'S')
				{
				}
				else if (VKCode == 'D')
				{
				}
				else if (VKCode == 'Q')
				{
				} 
				else if (VKCode == 'E')
				{
				} 
				else if (VKCode == VK_UP)
				{
				} 
				else if (VKCode == VK_DOWN)
				{
				} 
				else if (VKCode == VK_LEFT)
				{
				} 
				else if (VKCode == VK_RIGHT)
				{
				} 
				else if (VKCode == VK_ESCAPE)
				{
					OutputDebugStringA("ESCAPE: ");
					if (IsDown)
					{
						OutputDebugStringA("IsDown ");
					}
					if (WasDown)
					{
						OutputDebugStringA("WasDown");
					}
					OutputDebugStringA("\n");
				} 
				else if (VKCode == VK_SPACE)
				{
				}
				if((VKCode == VK_F4) && AltKeyWasDown)
				{
					GlobalRunning = false;
				}
			}
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
	LARGE_INTEGER PerfCountFrequencyResult;
	QueryPerformanceFrequency(&PerfCountFrequencyResult);
	s64 PerfCountFrequency = PerfCountFrequencyResult.QuadPart;

	Win32LoadXInput();
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
			// NOTE(casey): Since we specified CS_OWNDC, we can just
			// get one device context and use it forever because we
			// are not sharing it with anyone.
			HDC DeviceContext = GetDC(Window);
			Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);

			win32_sound_output SoundOutput = {};
			SoundOutput.SamplesPerSecond = 48000;
			SoundOutput.BytesPerSample = sizeof(s16) * 2;
			SoundOutput.SecondaryBufferSize = 2 * SoundOutput.SamplesPerSecond * SoundOutput.BytesPerSample;
			SoundOutput.RunningSampleIndex = 0;
			SoundOutput.LatencySampleCount = SoundOutput.SamplesPerSecond / 15;
			Win32InitDSound(Window, SoundOutput.SamplesPerSecond, SoundOutput.SecondaryBufferSize);
			Win32ClearBuffer(&SoundOutput);
			GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
			s16 *Samples = (s16 *)VirtualAlloc(0, SoundOutput.SecondaryBufferSize,
			                                   MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			
#if HANDMADE_INTERNAL
			LPVOID BaseAddress = (LPVOID)Terabytes(2);
#else
			LPVOID BaseAddress = 0;
#endif
			game_memory GameMemory = {};
			GameMemory.PermanentStorageSize = Megabytes(64);
			GameMemory.TransientStorageSize = Gigabytes(2);
			u64 TotalStorageSize = GameMemory.PermanentStorageSize + GameMemory.TransientStorageSize;
			GameMemory.PermanentStorage = VirtualAlloc(BaseAddress, TotalStorageSize,
			                                           MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			GameMemory.TransientStorage = ((u8 *)GameMemory.PermanentStorage + 
			                              GameMemory.PermanentStorageSize);
			
			if (Samples && 
			    GameMemory.PermanentStorage && 
			    GameMemory.TransientStorage)
			{
				game_input Input[2] = {};
				game_input* OldInput = &Input[0];
				game_input* NewInput = &Input[1];
				LARGE_INTEGER LastCounter;
				QueryPerformanceCounter(&LastCounter);
				u64 LastCycleCount = __rdtsc();
				GlobalRunning = true;
				while (GlobalRunning) // Replaces "for" loop
				{
					MSG Message;
					while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
					{
						if (Message.message == WM_QUIT)
						{
							GlobalRunning = false;
						}
						TranslateMessage(&Message);
						DispatchMessageA(&Message);
					}
					// TODO(casey): Should we poll this more frequently? 
					int MaxControllerCount = XUSER_MAX_COUNT; 
					if(MaxControllerCount > ArrayCount(NewInput->Controllers))
					{
						MaxControllerCount = ArrayCount(NewInput->Controllers);
					}
					for (DWORD ControllerIndex = 0; ControllerIndex < MaxControllerCount; ++ControllerIndex)
					{
						game_controller_input *OldController = &OldInput->Controllers[ControllerIndex];
						game_controller_input *NewController = &NewInput->Controllers[ControllerIndex];         

						XINPUT_STATE ControllerState;
						if (XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
						{
							// TODO(casey): See if ControllerState.dwPacketNumber increments too rapidly
							XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;
                        
							bool Up            = Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
							bool Down          = Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
							bool Left          = Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
							bool Right         = Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
							//bool Start         = Pad->wButtons & XINPUT_GAMEPAD_START;
							//bool Back          = Pad->wButtons & XINPUT_GAMEPAD_BACK;
							f32 X;
							if(Pad->sThumbLX < 0)
							{
								X = (f32)Pad->sThumbLX / 32768.0f;
							}
							else
							{
								X = (f32)Pad->sThumbLX / 32767.0f;
							}

							f32 Y;
							if(Pad->sThumbLY < 0)
							{
								Y = (f32)Pad->sThumbLY / 32768.0f;
							}
							else
							{
								Y = (f32)Pad->sThumbLY / 32767.0f;
							}
							NewController->StartX = OldController->EndX;
							NewController->StartY = OldController->EndY;

							// TODO(casey): Min/Max macros!!!
							NewController->MinX = NewController->MaxX = NewController->EndX = X;
							NewController->MinY = NewController->MaxY = NewController->EndY = Y;

							NewController->IsAnalog = true;

							// bool Start         = Pad->wButtons & XINPUT_GAMEPAD_START;
							// bool Back          = Pad->wButtons & XINPUT_GAMEPAD_BACK;
							Win32ProcessXInputDigitalButton(Pad->wButtons,
							                                &OldController->Down,XINPUT_GAMEPAD_A,
							                                &NewController->Down);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
							                                &OldController->Right,XINPUT_GAMEPAD_B,
							                                &NewController->Right);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
							                                &OldController->Left,XINPUT_GAMEPAD_X,
							                                &NewController->Left);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
							                                &OldController->Up,XINPUT_GAMEPAD_Y,
							                                &NewController->Up);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
							                                &OldController->LeftShoulder,XINPUT_GAMEPAD_LEFT_SHOULDER,
							                                &NewController->LeftShoulder);
							Win32ProcessXInputDigitalButton(Pad->wButtons,
							                                &OldController->RightShoulder,XINPUT_GAMEPAD_RIGHT_SHOULDER,
							                                &NewController->RightShoulder);
						}
						else
						{
							// NOTE(casey): This controller is not available.
						}
					}
					XINPUT_VIBRATION Vibration;
					Vibration.wLeftMotorSpeed = 60000;
					Vibration.wRightMotorSpeed = 60000;
					XInputSetState(0, &Vibration);

					DWORD ByteToLock;
					DWORD TargetCursor;
					DWORD BytesToWrite;
					DWORD PlayCursor;
					DWORD WriteCursor;
					b32 SoundIsValid = false;
					// TODO(casey): Tighten up sound logic so that we know where we should be 
					// writing to and can anticipate the time spent in the game updates.
					if (SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
					{
						ByteToLock = ((SoundOutput.RunningSampleIndex * SoundOutput.BytesPerSample)
						             % SoundOutput.SecondaryBufferSize);
						TargetCursor = ((PlayCursor +
						               (SoundOutput.LatencySampleCount * SoundOutput.BytesPerSample))
						               % SoundOutput.SecondaryBufferSize);

						if(ByteToLock > TargetCursor)
						{
							BytesToWrite = SoundOutput.SecondaryBufferSize - ByteToLock;
							BytesToWrite += TargetCursor;
						}
						else
						{
							BytesToWrite = TargetCursor - ByteToLock;
						}
    
						SoundIsValid = true;
					}
					game_sound_output_buffer SoundBuffer = {}; // clear to zero!
					SoundBuffer.SamplesPerSecond = SoundOutput.SamplesPerSecond;
					SoundBuffer.SampleCount = BytesToWrite / SoundOutput.BytesPerSample;
					SoundBuffer.Samples = Samples;

					game_offscreen_buffer Buffer = {}; // clear to zero!
					Buffer.Memory = GlobalBackbuffer.Memory;
					Buffer.Width = GlobalBackbuffer.Width;
					Buffer.Height = GlobalBackbuffer.Height;
					Buffer.Pitch = GlobalBackbuffer.Pitch;
					GameUpdateAndRender(&GameMemory, NewInput, &Buffer, &SoundBuffer);

					if (SoundIsValid)
					{
						Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
					}
					win32_window_dimension Dimension = Win32GetWindowDimension(Window);
					Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
					LARGE_INTEGER EndCounter;
					QueryPerformanceCounter(&EndCounter);
					u64 EndCycleCount = __rdtsc();
					// TODO(casey): Display the value here
					s64 CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
					s64 CyclesElapsed = EndCycleCount - LastCycleCount;
					f64 MSPerFrame = 1000.0*(f64)CounterElapsed / (f64)PerfCountFrequency;
					f64 FPS = (f64)PerfCountFrequency / (f64)CounterElapsed;
					f64 MegaCyclesPerFrame = (f64)CyclesElapsed / (1000.0 * 1000.0);
#if 0
					char Buffer[256];
					sprintf(Buffer, "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
					OutputDebugStringA(Buffer);
#endif
					LastCounter = EndCounter;
					LastCycleCount = EndCycleCount;

					game_input *Temp = NewInput;
					NewInput = OldInput;
					OldInput = Temp;
				}
			}
			else
			{
				// Memory allocation failed
				// TODO(casey): Logging
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