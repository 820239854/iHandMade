#if !defined(HANDMADE_H)

struct game_offscreen_buffer
{
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

// TODO(casey): Services that the platform layer provides to the game.

// NOTE(casey): Services that the game provides to the platform layer.
internal void GameUpdateAndRender(game_offscreen_buffer *Buffer);

#define HANDMADE_H
#endif