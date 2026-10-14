/* animGetVolumeScale__Fii at 0x00163cb0 is 168 bytes. Observed:
 *
 *   - `spread` (second argument) above zero advances eRandState
 *     (0x00483e58) once by state * 0x19660d + 0x3c6ef35f and takes
 *     (state >> 16) % (2 * spread + 1) minus 2 * spread as an offset (so the
 *     offset lies between -2 * spread and 0). Otherwise the offset is zero
 *     and eRandState is not touched.
 *   - The index is 64 plus the offset, limited below to 0 and above to 127.
 *   - The result is pitchTab[index] (a local float table at 0x006cb078,
 *     128 floats up to the next symbol; its writers are not in this file)
 *     times groupVolumeScale[group] (0x0034ba08, 300 floats, no bounds check
 *     on `group`), then limited to 0.0 from below and 1.0 from above.
 *
 * The first argument is used only as the index into groupVolumeScale. What
 * the group number means, and what pitchTab holds beyond being the first
 * factor of a volume, are not established here.
 */
extern "C" {
extern unsigned eRandState;
extern float pitchTab[];
extern float groupVolumeScale[];

float animGetVolumeScale__Fii(int group, int spread) {
    int offset = 0;
    if (spread > 0) {
        const unsigned state = eRandState * 0x19660du + 0x3c6ef35fu;
        eRandState = state;
        offset = static_cast<int>(state >> 16) % (2 * spread + 1) - 2 * spread;
    }
    int index = 64 + offset;
    if (index < 0) index = 0;
    if (index > 127) index = 127;
    float scale = pitchTab[index] * groupVolumeScale[group];
    if (scale < 0.0f) scale = 0.0f;
    if (1.0f < scale) scale = 1.0f;
    return scale;
}
}
