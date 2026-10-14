// Observed hudGivePlayerGold__FiG6Point3i at 0x001c4190, 868 bytes.
// Arguments: a player index, a world position (three floats, passed by value) and
// an amount. `goldNumber` (0x007d0588) is an array of eight 36-byte entries:
//   +0..+8   three floats: screen position (set here)
//   +12..+20 three floats: a drift vector (set here)
//   +24      word: frameNm when the entry was filled
//   +28      word: the amount; zero marks a free entry
//   +32      byte: the player index
//   +33      byte: a flag set here (see below)
// (the field roles are inferred from how this function and its callers use them).
// The first entry whose +28 word is zero is taken. If all eight are in use the
// amount is added to the word at (player record + 232) instead (player records
// are 23984 bytes apart at `player`, 0x00408600) and nothing else happens.
// Otherwise the entry is filled: amount, player, frameNm, flag 0. The position is
// transformed by matTransform__FP6Point3P8Matrix44T0(&out, worldViewMatrix, &position)
// which returns a float w. The flag is set to 1 if w < 20.0, or w * 700.0 < |out.x|,
// or w * 300.0 < |out.y|. With inv = 1 / w, the screen position is
// (out.x * inv * 0.5 + 320.0, out.y * inv + 240.0, 0.0) (z is zeroed after the copy).
// Then d = (playerGoldx[player], playerGoldy[player]) - (entry x, entry y), both as
// floats, and dz = -entry z; s = 1/sqrt(dx*dx + dy*dy + dz*dz) (rsqrt.s with
// numerator 1.0). One draw is taken from eRandState (state * 1664525 + 1013904223,
// high 16 bits) and r = (draw * 2^-16 - 0.5) * 9.0. The drift vector is
// (dy * s * r, -(dx * s) * r, 0.0): the direction to the target turned by a
// quarter turn, scaled by a random amount in [-4.5, 4.5). The meaning of the screen
// target arrays, the flag and the drift is inferred; they are not named in the
// original beyond playerGoldx and playerGoldy. The callee and the objects are not
// defined here.
struct GoldPoint3 { float x, y, z; };
struct GoldNumberEntry {
    float x, y, z;
    float driftX, driftY, driftZ;
    int frame;
    int amount;
    unsigned char player;
    unsigned char flag;
    unsigned char pad[2];
};
static_assert(sizeof(GoldNumberEntry) == 36, "goldNumber entry size");

extern "C" {
extern GoldNumberEntry goldNumber[];
extern unsigned char player[];
extern unsigned char worldViewMatrix[];
extern int frameNm;
extern int eRandState;
extern int playerGoldx[];
extern int playerGoldy[];
float matTransform__FP6Point3P8Matrix44T0(GoldPoint3* out, void* matrix, GoldPoint3* in);
}

static inline float reciprocalSqrt(float lengthSquared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(lengthSquared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(lengthSquared);
#endif
}

extern "C" void hudGivePlayerGold__FiG6Point3i(int playerIndex, GoldPoint3 position, int amount) {
    int i = 0;
    while (i < 8 && goldNumber[i].amount != 0) ++i;
    if (i == 8) {
        *reinterpret_cast<int*>(player + playerIndex * 23984 + 232) += amount;
        return;
    }
    GoldNumberEntry& entry = goldNumber[i];
    entry.amount = amount;
    entry.player = static_cast<unsigned char>(playerIndex);
    entry.frame = frameNm;
    entry.flag = 0;
    GoldPoint3 screen;
    float w = matTransform__FP6Point3P8Matrix44T0(&screen, worldViewMatrix, &position);
    if (w < 20.0f || w * 700.0f < __builtin_fabsf(screen.x) || w * 300.0f < __builtin_fabsf(screen.y)) {
        entry.flag = 1;
    }
    float inv = 1.0f / w;
    screen.x = screen.x * inv;
    screen.y = screen.y * inv;
    screen.z = screen.z * inv;
    screen.x = screen.x * 0.5f;
    entry.x = screen.x + 320.0f;
    entry.y = screen.y + 240.0f;
    entry.z = screen.z;
    float targetX = static_cast<float>(playerGoldx[playerIndex]);
    float targetY = static_cast<float>(playerGoldy[playerIndex]);
    entry.z = 0.0f;
    float dx = targetX - entry.x;
    float dy = targetY - entry.y;
    float dz = -entry.z;
    float lengthSquared = dx * dx + dy * dy + dz * dz;
    unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    float scale = reciprocalSqrt(lengthSquared);
    float randomValue = (static_cast<float>(state >> 16) * (1.0f / 65536.0f) - 0.5f) * 9.0f;
    eRandState = static_cast<int>(state);
    float ndx = dx * scale;
    float ndy = dy * scale;
    entry.driftX = ndy * randomValue;
    entry.driftY = -ndx * randomValue;
    entry.driftZ = 0.0f;
}
