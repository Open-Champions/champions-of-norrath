// Observed revealMap__3mapG6Point3 at 0x0015dd58, 688 bytes. The Point3 argument
// is passed by address (a 12-byte structure copied by the caller).
// Takes the x and y floats of the point, converts them with
// worldToMap__3mapP6Point2PfT2 to map coordinates (mx, my), and then raises the
// reveal nibble (the high four bits of byte 1) of every tile within a square
// of about eight tiles. Rounding below is to nearest with halves away from
// zero (add 0.5 when the value is not negative, else subtract 0.5, then
// truncate); the window ends use the two float forms noted at roundLow and
// roundHigh, which equal that rule up to float rounding of the intermediate. The column range is round(mx - 8) limited to at least 0, up to
// round(mx + 8) limited to at most _3map$mapwidth - 1 (0x00483ec0); the row
// range is the same for my with _3map$mapheight - 1 (0x00483ec4). Each range
// is empty when its start is above its end. For each tile (column xx, row yy)
// at _3map$maptiles (0x003347c0) + 432 * yy + 2 * xx, let d2 = (mx - xx)^2 +
// (my - yy)^2, with the row term evaluated once per row. When d2 <= 64.0, let
// v = round((1.0 - sqrt(d2) * 0.125) * 254.0), limited to at most 255 (only
// values of 256 and above are replaced, by 255), and v >>= 4 (arithmetic). The
// tile's byte 1 keeps its low nibble and its high nibble becomes the larger of
// its old value and v. Nothing is returned and _3map$revealed is not touched.
// The z component is unused. Nothing here was executed on the EE.
extern "C" {
void worldToMap__3mapP6Point2PfT2(const float* point, float* out_x, float* out_y);
extern int _3map$mapwidth;
extern int _3map$mapheight;
extern unsigned char _3map$maptiles[];
}

static inline int roundAway(float value) {
    return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f);
}

// The window ends are rounded without going through a rounded intermediate on
// one side: below, a negative mc - 8 becomes mc + -8.5; above, a non-negative
// mc + 8 becomes mc + 8.5.
static inline int roundLow(float mc) {
    const float t = mc - 8.0f;
    return static_cast<int>(t >= 0.0f ? t + 0.5f : mc + -8.5f);
}

static inline int roundHigh(float mc) {
    const float t = mc + 8.0f;
    return static_cast<int>(t >= 0.0f ? mc + 8.5f : t - 0.5f);
}

extern "C" void revealMap__3mapG6Point3(const float* point) {
    float xy[2] = {point[0], point[1]};
    float mx;
    float my;
    worldToMap__3mapP6Point2PfT2(xy, &mx, &my);

    int x_first = roundLow(mx);
    if (x_first < 0) x_first = 0;
    int x_last = roundHigh(mx);
    if (x_last > _3map$mapwidth - 1) x_last = _3map$mapwidth - 1;
    int y_first = roundLow(my);
    if (y_first < 0) y_first = 0;
    int y_last = roundHigh(my);
    if (y_last > _3map$mapheight - 1) y_last = _3map$mapheight - 1;

    for (int yy = y_first; yy <= y_last; ++yy) {
        const float dy = my - static_cast<float>(yy);
        const float dy2 = dy * dy;
        for (int xx = x_first; xx <= x_last; ++xx) {
            const float dx = mx - static_cast<float>(xx);
            const float d2 = dx * dx + dy2;
            if (d2 <= 64.0f) {
                int v = roundAway((1.0f - __builtin_sqrtf(d2) * 0.125f) * 254.0f);
                if (v >= 256) v = 255;
                v >>= 4;
                unsigned char* tile = _3map$maptiles + 432 * yy + 2 * xx + 1;
                int high = *tile >> 4;
                if (high < v) high = v;
                *tile = static_cast<unsigned char>((*tile & 0xf) | (high << 4));
            }
        }
    }
}
