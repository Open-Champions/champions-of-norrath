// Host contract for the seven render-state data objects. Not an EE plan input.
// Checks the authored extents and zero initial contents against the observed gaps.
#include "body_alpha.cpp"
#include "cut_camera_pos.cpp"
#include "draw_bucket.cpp"
#include "dummy_list.cpp"
#include "fade_down_val.cpp"
#include "hvecs.cpp"
#include "light_mats.cpp"
static bool zero(const void* p, unsigned n) {
    const unsigned char* b = static_cast<const unsigned char*>(p);
    for (unsigned i = 0; i < n; ++i) if (b[i]) return false;
    return true;
}
int main() {
    if (sizeof(bodyAlpha) != 4 || !zero(&bodyAlpha, 4)) return 1;
    if (sizeof(cutCameraPos) != 16 || !zero(cutCameraPos, 16)) return 2;
    if (sizeof(drawBucket) != 4 || !zero(&drawBucket, 4)) return 3;
    if (sizeof(dummyList) != 48 || !zero(dummyList, 48)) return 4;
    if (sizeof(fadeDownVal) != 4 || fadeDownVal != 0) return 5;
    if (sizeof(hvecs) != 144 || !zero(hvecs, 144)) return 6;
    if (sizeof(lightMats) != 432 || !zero(lightMats, 432)) return 7;
    return 0;
}
