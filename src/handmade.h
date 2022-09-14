#if !defined(HANDMADE_H)

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

// TODO(casey): Services that the platform layer provides to the game.

// NOTE(casey): Services that the game provides to the platform layer.
internal void GameUpdateAndRender(game_offscreen_buffer* Buffer, int XOffset, int YOffset,
                                  game_sound_output_buffer *SoundBuffer, int ToneHz);

#define HANDMADE_H
#endif