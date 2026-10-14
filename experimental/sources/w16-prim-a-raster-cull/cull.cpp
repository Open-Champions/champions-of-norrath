// Observed: Cull__8VIRasterR13VIPickSegmentR6VIBBox at 0x010f9e18 is 224 bytes, leaf code (no
// calls). The segment reads as six floats s[0..5] and the box as six floats b[0..5]; the raster
// argument is unused. It returns 1 as soon as, for some axis i in {0,1,2}, either
// b[3+i] < s[i] and b[3+i] < s[3+i] (box ends before both segment ends) -- all three axes are
// tested before any of the second kind -- or s[i] < b[i] and s[3+i] < b[i] (box starts after both
// segment ends); otherwise it returns 0. Comparisons are ordered less-than, so NaN never culls.
// Inferred: b[0..2] is the box minimum and b[3..5] the maximum (matching the use of box+12..20 as
// the upper bound); s[0..2] and s[3..5] are the two end points of the pick segment. Unknown: the
// real field names and whether the segment's later words are padding.
extern "C" {
int Cull__8VIRasterR13VIPickSegmentR6VIBBox(void*, const float* s, const float* b) {
    for (int i = 0; i < 3; ++i)
        if (b[3 + i] < s[i] && b[3 + i] < s[3 + i])
            return 1;
    for (int i = 0; i < 3; ++i)
        if (s[i] < b[i] && s[3 + i] < b[i])
            return 1;
    return 0;
}
}
