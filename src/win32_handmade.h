#if !defined(WIN32_HANDMADE_H)

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

struct win32_sound_output
{
    int SamplesPerSecond;
    int BytesPerSample;
    int SecondaryBufferSize;
    u32 RunningSampleIndex;
    int LatencySampleCount;
};

#define WIN32_HANDMADE_H
#endif