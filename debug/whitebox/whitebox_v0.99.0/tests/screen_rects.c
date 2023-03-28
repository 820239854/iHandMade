static inline float minf(float a, float b) { return a < b ? a : b; }
static inline float maxf(float a, float b) { return a > b ? a : b; }

typedef struct Rect { float min_x, min_y,    max_x, max_y; } Rect; // axis-aligned

Rect rect_group(Rect a, Rect b) { // result is a bounding rect for a and b
    return (Rect){ minf(a.min_x, b.min_x), minf(a.min_y, b.min_y),
                   maxf(a.max_x, b.max_x), maxf(a.max_y, b.max_y), };
}

int rect_overlap(Rect const *a, Rect const *b, Rect *overlap) { // intersection rect
    *overlap = (Rect){ maxf(a->min_x, b->min_x), maxf(a->min_y, b->min_y),
                       minf(a->max_x, b->max_x), minf(a->max_y, b->max_y), };
    return (overlap->min_x < overlap->max_x &&
            overlap->min_y < overlap->max_y); // true if there is an overlap
}

typedef struct RectStats {
    Rect largest_rect, bounding_rect;
    float largest_area;
    int  rects_on_screen_n;
} RectStats;

RectStats screen_rect_stats(Rect const *rects, int n, float screen_w, float screen_h) {
    RectStats result      = {0};
    Rect      screen_rect = { 0.f, 0.f, screen_w, screen_h };

    for (int i = 0; i < n; ++i) {
        Rect rect = {0}; // will clip to the portion that's on screen

        if (rect_overlap(&rects[i], &screen_rect, &rect)) {
            float w    = rect.max_x - rect.min_x;
            float h    = rect.max_x - rect.min_y;
            float area = w * h;

            if (area > result.largest_area) {
                result.largest_area = area;
                result.largest_rect = rect;
            }

            result.bounding_rect = rect_group(result.bounding_rect, rect);
            ++result.rects_on_screen_n;
        }
    }

    return result;
}

// 1/sqrt(a)
inline float
F4_rsqrt(float a)
{
	float a_2 = 0.5 * a;
	union { float f; unsigned i; } val = { a };

	val.i = 0x5f3759df - (val.i >> 1);
	val.f *= (1.5f - (a_2 * val.f * val.f));   // Newton Raphsom iteration (could do a second time for more precision)
	val.f *= (1.5f - (a_2 * val.f * val.f));   // (optional second time for more precision)

	return val.f;
}



#include <stdio.h>

int main()
{
    Rect const rects[] = {
        { 40, 60,   200, 300 },
        {-10, 30,    20,  40 },
        {-5, -10,  4000, 400 },
        {-10,-30,    -2,   1 },
    };

    RectStats stats = screen_rect_stats(rects, 4, 1920, 1080);

    // lots of other code...

    return 0;
}
