#include "handmade.h"

internal void RenderWeirdGradient(game_offscreen_buffer *Buffer, int XOffset, int YOffset)
{
    u8 *Row = (u8 *)Buffer->Memory;
    for (int Y = 0;
         Y < Buffer->Height;
         ++Y)
    {
        u32 *Pixel = (u32  *)Row;
        for(int X = 0;
            X < Buffer->Width;
            ++X)
        {
            u8 Blue = (u8)(X + XOffset);
            u8 Green = (u8)(Y + YOffset);
            u8 Red = 0;
            
            *Pixel++ = Red << 16 | Green << 8 | Blue; // << 0
        }
        Row += Buffer->Pitch;
    }
}

internal void GameOutputSound(game_sound_output_buffer *SoundBuffer, int ToneHz)
{
	local_persist f32 tSine;
	s16 ToneVolume = 3000;
	int WavePeriod = SoundBuffer->SamplesPerSecond / ToneHz;

	s16 *SampleOut = SoundBuffer->Samples;

	for (int SampleIndex = 0;
		SampleIndex < SoundBuffer->SampleCount;
		++SampleIndex)
	{
		f32 SineValue = sinf(tSine);
		s16 SampleValue = (s16)(SineValue * ToneVolume);

		*SampleOut++ = SampleValue;
		*SampleOut++ = SampleValue;
		tSine += 2.0f * Pi32 * 1.0f / (f32)WavePeriod;
	}
}

internal void GameUpdateAndRender(game_input *Input, game_offscreen_buffer* Buffer, 
                                  game_sound_output_buffer *SoundBuffer)
{
	local_persist int XOffset = 0;
	local_persist int YOffset = 0;
	local_persist int ToneHz = 256;

	game_controller_input *Input0 = &Input->Controllers[0];
	if (Input0->IsAnalog)
	{
		// Input.StartX;
		// Input.MinX;
		// Input.MaxX;
		// Input.EndX;

		// Input.StartY;
		// Input.MinY;
		// Input.MaxY;
		// Input.EndY;

		XOffset += (int)(4.0f * Input0->EndX);
		ToneHz = 256 + (int)(128.0f * (Input0->EndY));
	}
	else
	{
		// NOTE(casey): Use digital movement tuning
	}
	// Input.DownButtonEndedDown;
	// Input.DownButtonHalfTransitionCount;
	if(Input0->Down.EndedDown)
	{
		YOffset += 1;
	}

	GameOutputSound(SoundBuffer, ToneHz);
    RenderWeirdGradient(Buffer, XOffset, YOffset);
}