#if !defined(HANDMADE_H)

/*
	NOTE(casey): 

	HANDMADE_INTERNAL: 
	0 - Build for public release
	1 - Build for developer only

	HANDMADE_SLOW: 
	0 - No slow code allowed!
	1 - Slow code welcome. 
*/

// NOTE(casey): Services that the platform layer provides to the game.
#if HANDMADE_INTERNAL
struct debug_read_file_result
{
	u32 ContentsSize;
	void *Contents;
};
internal debug_read_file_result DEBUGPlatformReadEntireFile(const char *Filename);
internal void DEBUGPlatformFreeFileMemory(void *Memory);
internal b32 DEBUGPlatformWriteEntireFile(const char *Filename, u32 MemorySize, void *Memory);
#endif

#if HANDMADE_SLOW
#define Assert(Expression) if (!(Expression)) { *(int *)0 = 0; }
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

inline u32 SafeTruncateUInt64(u64 Value)
{
	Assert(Value <= 0xFFFFFFFF);
	u32 Result = (u32)Value;
	return (Result);
}

struct game_offscreen_buffer
{
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

struct game_sound_output_buffer
{
	int SamplesPerSecond;
	int SampleCount;
	s16* Samples;
};

struct game_button_state
{
	s32 HalfTransitionCount;
	b32 EndedDown;
};

struct game_controller_input
{
	b32 IsAnalog; 

	f32 StartX;
	f32 StartY;
    
	f32 MinX;
	f32 MinY;
    
	f32 MaxX;
	f32 MaxY;
    
	f32 EndX;
	f32 EndY;
    
	union 
	{
		game_button_state Buttons[6];
		struct
		{
			game_button_state Up;
			game_button_state Down;
			game_button_state Left;
			game_button_state Right;
			game_button_state LeftShoulder;
			game_button_state RightShoulder;
		};
	};
};

struct game_input
{
	game_controller_input Controllers[4];
};

struct game_memory
{
	u64 PermanentStorageSize; 
	void *PermanentStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

	u64 TransientStorageSize;
	void *TransientStorage;

	b32 IsInitialized;
};


internal void GameUpdateAndRender(game_input *Input, game_offscreen_buffer* Buffer, 
                                  game_sound_output_buffer *SoundBuffer);

struct game_state
{
	int ToneHz;
	int XOffset;
	int YOffset;
};

#define HANDMADE_H
#endif