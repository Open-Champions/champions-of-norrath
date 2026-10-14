// Observed worldRunWater__FP12_worldHeader at 0x00178348, 1272 bytes. The argument is not read. The water
// patches are the 4784-byte records of waterPatch (count nmWaterPatches), as waterInit lays them out. The
// fields used, in decimal bytes of a patch: 80 + 80 * k (k below 32) a column of halfwords, 2704 + 2 * (32 * y
// + x) a 32 by 32 grid of signed halfwords (heights), 4752 a world header address, 4756 a frame number,
// 4768 a link address (zero when idle). The link names a block whose first word is an owner record; the
// owner has a word at +8 and the signed halfwords at +0 and +4, which index the 60-byte records at world +36
// of the patch's world. Everything below happens only when gameIsPaused__Fv returns zero.
// Pass 1, for every patch with a nonzero link:
//   - if frameNm - (frame at +4756) is at least 200 the patch is retired: the link is cleared and the word
//     at +8 of the owner is cleared;
//   - otherwise worldWaterRainTimer is decremented, and if its old value was negative a ripple is added:
//     eRandState steps three times as state = state * 0x19660d + 0x3c6ef35f, taking the top 16 bits of
//     each result r1, r2, r3; the centre is (r1 % 20 + 6, r2 % 20 + 6) and worldWaterRainTimer is set to
//     r3 % worldWaterBumpTimer (signed). With radius = worldWaterBumpRadius and amount = worldWaterBumpAmount,
//     the box from (centre - radius) truncated up to (centre + radius + 1) truncated is limited to 0..31 in
//     both axes, and for each cell in it whose squared distance d2 from the centre is not above radius
//     squared, the height grows by the truncated (amount - d2 * (amount / radius^2)), limited to 32767;
//   - then waterStep is called with the patch's +80, its +2704, and the owner address plus 16.
// Pass 2, for every patch with a nonzero link: each of the owner's two halfword indices that is not -1
//   names a record whose first word is a block; if that block's word at +8 (a neighbouring patch) is nonzero,
//   border strips are exchanged between the two patches. For the first index: the 32 halfwords at +80 + 80 k
//   take the neighbour's halfwords at +140 + 80 k, the neighbour's at +142 + 80 k take ours at +82 + 80 k,
//   our heights at +2704 + 64 k take the neighbour's at +2764 + 64 k, and the neighbour's at +2766 + 64 k
//   take ours at +2706 + 64 k. For the second index: our +80 + 2 k take the neighbour's +2480 + 2 k, the
//   neighbour's +2560 + 2 k take our +160 + 2 k, our +2704 + 2 k take the neighbour's +4624 + 2 k, and the
//   neighbour's +4688 + 2 k take our +2768 + 2 k. All k run 0 to 31.
// Nothing is returned. Inferred only: that the patches are water surfaces and the strips are shared edges.
// Unknown: the layout of the owner and the meaning of the pair of indices.
#if defined(NORRATH_HOST_WATERRUN)
extern unsigned char* norrath_water_arena;
#endif

extern "C" {
int gameIsPaused__Fv();
void waterStep(void* a, void* b, void* c);
extern int nmWaterPatches;
extern unsigned char* waterPatch;
extern int frameNm;
extern int worldWaterRainTimer;
extern int worldWaterBumpTimer;
extern float worldWaterBumpRadius;
extern float worldWaterBumpAmount;
extern int eRandState;
}

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_WATERRUN)
    return norrath_water_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned short& half(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned short*>(base + offset); }
inline short& shalf(unsigned char* base, unsigned offset) { return *reinterpret_cast<short*>(base + offset); }

unsigned nextRandom() {
    unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(state);
    return state >> 16;
}

void ripple(unsigned char* patch) {
    float radius = worldWaterBumpRadius;
    float amount = worldWaterBumpAmount;
    int timer = worldWaterBumpTimer;
    float radiusSquared = radius * radius;
    float scale = amount / radiusSquared;
    unsigned first = nextRandom();
    unsigned second = nextRandom();
    unsigned third = nextRandom();
    int centreX = static_cast<int>(first % 20u) + 6;
    int centreY = static_cast<int>(second % 20u) + 6;
    worldWaterRainTimer = static_cast<int>(third) % timer;
    float fx = static_cast<float>(centreX);
    float fy = static_cast<float>(centreY);
    int x0 = static_cast<int>(fx - radius);
    int x1 = static_cast<int>(fx + radius + 1.0f);
    int y0 = static_cast<int>(fy - radius);
    int y1 = static_cast<int>(fy + radius + 1.0f);
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= 32) x1 = 31;
    if (y1 >= 32) y1 = 31;
    if (y1 < y0) return;
    for (int y = y0; y <= y1; ++y) {
        if (x1 < x0) continue;
        int dy = y - centreY;
        for (int x = x0; x <= x1; ++x) {
            int dx = x - centreX;
            float distance = static_cast<float>(dy * dy + dx * dx);
            if (radiusSquared < distance) continue;
            int sum = static_cast<int>(amount - distance * scale) + shalf(patch, 2704 + 2 * (32 * y + x));
            if (sum > 32767) sum = 32767;
            shalf(patch, 2704 + 2 * (32 * y + x)) = static_cast<short>(sum);
        }
    }
}

void exchange(unsigned char* patch, unsigned char* other, int index) {
    if (index == 0) {
        for (int k = 0; k < 32; ++k) half(patch, 80 + 80 * k) = half(other, 140 + 80 * k);
        for (int k = 0; k < 32; ++k) half(other, 142 + 80 * k) = half(patch, 82 + 80 * k);
        for (int k = 0; k < 32; ++k) half(patch, 2704 + 64 * k) = half(other, 2764 + 64 * k);
        for (int k = 0; k < 32; ++k) half(other, 2766 + 64 * k) = half(patch, 2706 + 64 * k);
    } else {
        for (int k = 0; k < 32; ++k) half(patch, 80 + 2 * k) = half(other, 2480 + 2 * k);
        for (int k = 0; k < 32; ++k) half(other, 2560 + 2 * k) = half(patch, 160 + 2 * k);
        for (int k = 0; k < 32; ++k) half(patch, 2704 + 2 * k) = half(other, 4624 + 2 * k);
        for (int k = 0; k < 32; ++k) half(other, 4688 + 2 * k) = half(patch, 2768 + 2 * k);
    }
}
}

extern "C" void worldRunWater__FP12_worldHeader(void* world) {
    (void)world;
    if (gameIsPaused__Fv() != 0) return;
    for (int i = 0; i < nmWaterPatches; ++i) {
        unsigned char* patch = waterPatch + i * 4784;
        unsigned link = word(patch, 4768);
        if (link == 0) continue;
        unsigned char* owner = address(word(address(link), 0));
        if (frameNm - static_cast<int>(word(patch, 4756)) >= 200) {
            *reinterpret_cast<unsigned*>(patch + 4768) = 0;
            *reinterpret_cast<unsigned*>(owner + 8) = 0;
            continue;
        }
        int rain = worldWaterRainTimer;
        worldWaterRainTimer = rain - 1;
        if (rain < 0) ripple(patch);
        waterStep(patch + 80, patch + 2704, owner + 16);
    }
    for (int i = 0; i < nmWaterPatches; ++i) {
        unsigned char* patch = waterPatch + i * 4784;
        unsigned link = word(patch, 4768);
        if (link == 0) continue;
        unsigned char* owner = address(word(address(link), 0));
        for (int which = 0; which < 2; ++which) {
            int index = shalf(owner, which * 4);
            if (index == -1) continue;
            unsigned char* records = address(word(address(word(patch, 4752)), 36));
            unsigned char* block = address(word(records + index * 60, 0));
            unsigned neighbour = word(block, 8);
            if (neighbour != 0) exchange(patch, address(neighbour), which);
        }
    }
}
