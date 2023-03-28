typedef unsigned long long UPtr;
typedef unsigned long long Size;

inline Size Size_min(Size a, Size b) { return a < b ? a : b; }
inline Size Size_max(Size a, Size b) { return a > b ? a : b; }

typedef enum {      //   [-A-)
    IVLPt_before,   // X |   |
    IVLPt_start,    //   X   |
    IVLPt_inside,   //   | X |
    IVLPt_end,      //   |   X
    IVLPt_after,    //   |   | X
    IVLPt_Count,
} IVLPt;

inline IVLPt
interval_pt(UPtr s, UPtr e, UPtr p)
{   return (p >= s) + (p > s) + (p >= e) + (p > e);   }

// NOTE: the 0-length intervals seem to be different from Allen Interval Algebra, where intervals are closed (no 0-length intervals)
//       could be useful for being able to treat points as intervals
// NOTE: because these are a tight 16 values, they can be packed as flags in a U16b. This is useful for proper Allen Interval Algebra where you might keep multiple possible relationships
// NOTE: additionally, 16 values fit into a nibble, so you could fit 2 interval checks into a byte, e.g. for axis-aligned bounding-box comparisons
typedef enum {            //           [-----B-----)
    IVL_null       = 0x0, //           |           |
    IVL_before     = 0x1, //  [-A--)   |           |
    IVL_meets      = 0x2, //  [----A---)           |
    IVL_overlaps   = 0x3, //  [-------A|-------)   |
    IVL_ended      = 0x4, //  [--------|-A---------)
    IVL_contains   = 0x5, //  [--------|----A------|-------)
    IVL_at_start   = 0x6, //           A           |  (0-length array: start and end both at start) - special case of IVL_starts
    IVL_starts     = 0x7, //           [--A----)   |
    IVL_equals     = 0x8, //           [-----A-----)
    IVL_started    = 0x9, //           [--------A--|-------)
    IVL_during     = 0xa, //           |   [-A-)   |
    IVL_ends       = 0xb, //           |   [---A---)
    IVL_overlapped = 0xc, //           |   [-----A-|-------)
    IVL_at_end     = 0xd, //           |           A  (0-length array: start and end both at end) - special case of IVL_met
    IVL_met        = 0xe, //           |           [--A----)
    IVL_after      = 0xf, //           |           |   [-A-)
    IVL_Count,            //   _____________________________
} IVL;                    //   |   |   |   | bs<ae |   |   |
//                           bs>as | bs=as | be>as | be<as |
//                                 | bs=ae |     be=bs   be<ae
//                               bs>ae   be>as   be=ae
//                                       bs<ae

inline bool IVL_is_overlapping(IVL ivl) { return IVL_overlaps <= ivl && ivl <= IVL_overlapped; }
inline bool IVL_is_inside(IVL ivl) { return IVL_at_start <= ivl && ivl <= IVL_ends && ivl != IVL_started; }

IVL
overlap_type(UPtr as, UPtr ae, UPtr bs, UPtr be, UPtr *overlap_s, UPtr *overlap_e)
{
    *overlap_s = Size_max(as, bs);
    *overlap_e = Size_min(ae, be);

    if (as == bs && ae == be) return IVL_equals; // needed for 0-length intervals; otherwise they get IVL_during

    IVLPt ps = interval_pt(bs, be, as);
    IVLPt pe = interval_pt(bs, be, ae);

    // TODO: nibble per value? - use the low bit to determine if it's the high or low nibble
    //       becomes 15 bytes instead of 25 - could then fit in single SSE
    static char ivls_by_pts[IVLPt_Count][IVLPt_Count] = { // [start][end]
        //                 before,     start,        inside,       end,        after
        [IVLPt_before] = { IVL_before, IVL_meets,    IVL_overlaps, IVL_ended,  IVL_contains   },
        [IVLPt_start]  = { IVL_null,   IVL_at_start, IVL_starts,   IVL_equals, IVL_started    },
        [IVLPt_inside] = { IVL_null,   IVL_null,     IVL_during,   IVL_ends,   IVL_overlapped },
        [IVLPt_end]    = { IVL_null,   IVL_null,     IVL_null,     IVL_at_end, IVL_met        },
        [IVLPt_after]  = { IVL_null,   IVL_null,     IVL_null,     IVL_null,   IVL_after      },
    };
    // NOTE: could also just do `IVLPt_Count * start + end`, then subtract the triangular number (n*n+1)/2

    return ivls_by_pts[ps][pe];
}

IVL
ivl_overlap(UPtr as, UPtr ae, UPtr bs, UPtr be, UPtr *overlap_s, UPtr *overlap_e)
{
    // NOTE: effectively reading into this table, using the fact that the null values form a triangular number on the row index
    /* static char ivls_by_pts[IVLPt_Count][IVLPt_Count] = { // [start][end] */
    /*     //         before,     start,        inside,       end,        after */
    /*     before = { IVL_before, IVL_meets,    IVL_overlaps, IVL_ended,  IVL_contains   }, */
    /*     start  = { IVL_null,   IVL_at_start, IVL_starts,   IVL_equals, IVL_started    }, */
    /*     inside = { IVL_null,   IVL_null,     IVL_during,   IVL_ends,   IVL_overlapped }, */
    /*     end    = { IVL_null,   IVL_null,     IVL_null,     IVL_at_end, IVL_met        }, */
    /*     after  = { IVL_null,   IVL_null,     IVL_null,     IVL_null,   IVL_after      }, */
    /* }; */

    IVL result = IVL_null;
    if (as <= ae && bs <= be)
    {
        *overlap_s = Size_max(as, bs);
        *overlap_e = Size_min(ae, be);

        if (as == bs && ae == be) return IVL_equals; // needed for 0-length intervals; otherwise they get IVL_during

        unsigned char ps = (as >= bs)+(as > bs) + (as >= be)+(as > be); // interval_pt(bs, be, as);
        unsigned char pe = (ae >= bs)+(ae > bs) + (ae >= be)+(ae > be); // interval_pt(bs, be, ae);

        unsigned char triangle = ps*(ps + 1) / 2;
        result                 = (IVLPt_Count * ps + pe - triangle);
    }

    return result;
}


