// Observed worldDrawWorld__FP12_worldHeader at 0x0017ab38, 1348 bytes.
// Walks a half-word stream tree from the pointer at header+4 with an explicit
// stack of 32-byte entries {node address, clip mask, six floats}. The six
// floats are [min x, max x, min y, max y, min z, max z]. The root entry has
// mask 0x8000 and box x and y -1000000.0f..1000000.0f, z -60.0f..780.0f
// (the x/y constants are read-only floats at 0x00480064/0x00480068).
//
// Prologue: worldLastDrawSucceded (0x0048412d) is set to 1; a null header
// returns at once. If gameIsPaused__Fv returns zero, the 48-byte records at
// worldAnimation whose byte at +4 is nonzero (scan stops at the first zero
// byte or after 64) are each advanced with animAdvanceAnimationTime__F... by
// 1.0f, a zero Point3, and null Point2/short pointers.
//
// A popped node with half-word h:
//  * bit 0x8000 set, an interior node. axis = h & 0xfff. Keys k1..k4 are the
//    32-bit values (sign-extended low half | high half << 16) at +2, +6, +10,
//    +14, each shifted left by 2 and converted to float. The left child is
//    at +20 and the right child at +18 + 2*half(+18).
//    - Entry mask 0x803f (all six planes known inside): both children are pushed with mask 0x803f (the new
//      entry's box is not initialised; the mask makes it unused).
//    - Otherwise box z is set to k3,k4. The max of the axis is set to k1 and
//      worldClipCubeToFrustum gives the left mask; restored; the min of the
//      axis is set to k2 and the same call gives the right mask. Both nonzero:
//      the entry is copied, the left child keeps max=k1 with the original
//      min, the right child keeps min=k2 with the original max. Only left:
//      left child, max=k1. Only right: right child, min stays k2. Neither:
//      nothing is pushed.
//  * bit 0x8000 clear, a leaf: h is a count of unsigned half-word indices of
//    60-byte records at header+36. For a mask other than 0x803f the record
//    box (floats at +8,+20,+12,+24,+16,+28) is clipped first; a zero result
//    skips the record, otherwise the second clip function is applied to the
//    result and the flag is nonzero unless it equals 0x803f. When
//    worldDrawEnable (0x0048412c) is set, the display list is checked: if
//    GLISTSIZE - ((glistp - gstart) >> 3) < 3000 the function returns without
//    clearing modelClippingOn; otherwise drawWorldPolyset is called with
//    the header, the record and the flag. When it is clear, records with flag
//    word (+36) bits 0xc880 set, a zero half at +52, or an unloaded page
//    (-1 in texturePageTable[half]) are skipped, the latter also clearing
//    worldLastDrawSucceded; otherwise the texture pointer
//    texturePageLoc[page] + word(+32) is checked (nonzero, bit 0x100 of the
//    half at +8 set, word at +20 zero) and worldLastDrawSucceded is cleared
//    and texCacheData called with it and the half at +52.
// Finally modelClippingOn (0x0048407c) is cleared. The stack holds 18
// entries in the original frame; no bound is checked. The meaning of the
// record fields is not established beyond these offsets.
#if defined(NORRATH_HOST_WORLD)
extern unsigned char* norrath_world_arena;
#endif

extern "C" {
extern unsigned char worldLastDrawSucceded;
extern unsigned char worldDrawEnable;
extern unsigned char modelClippingOn;
extern unsigned glistp;
extern unsigned gstart;
extern int GLISTSIZE;
extern unsigned char worldAnimation[];
extern unsigned char texturePageTable[];
extern unsigned char texturePageLoc[];
int gameIsPaused__Fv();
void animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* state, float time, float* position, void* uv, short* extra);
int worldClipCubeToFrustum__FPA1_fi(float* bounds, int mask);
int worldClipCubeToClipFrustum__FPA1_fi(float* bounds, int mask);
void drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(void* world, void* polyset, int flag);
void texCacheData__FP8_texturei(void* texture, int page);
}

namespace {
struct Entry {
    unsigned char* node;
    int mask;
    float box[6];
};

inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline int half(const unsigned char* base) { return *reinterpret_cast<const short*>(base); }
inline int combine(const unsigned char* base) { return half(base) | (half(base + 2) << 16); }
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_WORLD)
    return norrath_world_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline float scaled(int key) { return static_cast<float>(key << 2); }
}

extern "C" void worldDrawWorld__FP12_worldHeader(void* world) {
    worldLastDrawSucceded = 1;
    unsigned char* header = static_cast<unsigned char*>(world);
    if (header == nullptr) return;

    if (gameIsPaused__Fv() == 0) {
        for (int i = 0; i < 64 && worldAnimation[i * 48 + 4] != 0; ++i) {
            float origin[3] = {0.0f, 0.0f, 0.0f};
            animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(worldAnimation + i * 48, 1.0f, origin, nullptr, nullptr);
        }
    }

    Entry stack[18];
    int depth = 1;
    stack[0].node = address(word(header, 4));
    stack[0].mask = 0x8000;
    stack[0].box[0] = -1000000.0f;
    stack[0].box[1] = 1000000.0f;
    stack[0].box[2] = -1000000.0f;
    stack[0].box[3] = 1000000.0f;
    stack[0].box[4] = -60.0f;
    stack[0].box[5] = 780.0f;

    do {
        --depth;
        Entry& entry = stack[depth];
        unsigned char* node = entry.node;
        const int h = half(node);
        if ((h & 0x8000) != 0) {
            const unsigned axis = static_cast<unsigned>(h) & 0xfffu;
            const int key1 = combine(node + 2);
            const int key2 = combine(node + 6);
            const int mask = entry.mask;
            unsigned char* children = node + 18;
            unsigned char* left = node + 20;
            unsigned char* right = children + half(children) * 2;
            if (mask == 0x803f) {
                stack[depth + 1].node = right;
                stack[depth + 1].mask = 0x803f;
                entry.node = left;
                depth += 2;
            } else {
                const float key3 = scaled(combine(node + 10));
                const float key4 = scaled(combine(node + 14));
                float* low = &entry.box[axis * 2];
                float* high = &entry.box[axis * 2 + 1];
                entry.box[4] = key3;
                entry.box[5] = key4;
                const float saved_high = *high;
                *high = scaled(key1);
                const int left_mask = worldClipCubeToFrustum__FPA1_fi(entry.box, mask);
                *high = saved_high;
                const float saved_low = *low;
                *low = scaled(key2);
                const int right_mask = worldClipCubeToFrustum__FPA1_fi(entry.box, mask);
                if (left_mask != 0) {
                    if (right_mask != 0) {
                        stack[depth + 1] = entry;
                        *low = saved_low;
                        entry.mask = left_mask;
                        entry.node = left;
                        *high = scaled(key1);
                        stack[depth + 1].mask = right_mask;
                        stack[depth + 1].node = right;
                        depth += 2;
                    } else {
                        *low = saved_low;
                        entry.mask = left_mask;
                        *high = scaled(key1);
                        entry.node = left;
                        depth += 1;
                    }
                } else if (right_mask != 0) {
                    entry.mask = right_mask;
                    entry.node = right;
                    depth += 1;
                }
            }
        } else {
            const int mask = entry.mask;
            const unsigned char* index = node + 2;
            for (int remaining = h; remaining != 0; --remaining) {
                const unsigned record_index = *reinterpret_cast<const unsigned short*>(index);
                index += 2;
                unsigned char* record = address(word(header, 36)) + record_index * 60u;
                int outside_clip = 0;
                if (mask != 0x803f) {
                    float bounds[6];
                    bounds[0] = *reinterpret_cast<const float*>(record + 8);
                    bounds[1] = *reinterpret_cast<const float*>(record + 20);
                    bounds[2] = *reinterpret_cast<const float*>(record + 12);
                    bounds[3] = *reinterpret_cast<const float*>(record + 24);
                    bounds[4] = *reinterpret_cast<const float*>(record + 16);
                    bounds[5] = *reinterpret_cast<const float*>(record + 28);
                    const int visible = worldClipCubeToFrustum__FPA1_fi(bounds, mask);
                    if (visible == 0) continue;
                    outside_clip = worldClipCubeToClipFrustum__FPA1_fi(bounds, visible) != 0x803f;
                }
                if (worldDrawEnable != 0) {
                    const int used = static_cast<int>(glistp - gstart) >> 3;
                    if (GLISTSIZE - used < 3000) return;
                    drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(header, record, outside_clip);
                } else {
                    if ((word(record, 36) & 0xc880u) != 0u) continue;
                    const int page = *reinterpret_cast<const short*>(record + 52);
                    if (page == 0) continue;
                    const signed char slot = static_cast<signed char>(texturePageTable[page]);
                    if (slot == -1) {
                        worldLastDrawSucceded = 0;
                        continue;
                    }
                    const unsigned base = *reinterpret_cast<const unsigned*>(texturePageLoc + slot * 4);
                    const unsigned bits = base + word(record, 32);
                    if (bits == 0u) continue;
                    unsigned char* texture = address(bits);
                    if ((*reinterpret_cast<const unsigned short*>(texture + 8) & 0x100u) == 0) continue;
                    if (word(texture, 20) != 0u) continue;
                    worldLastDrawSucceded = 0;
                    texCacheData__FP8_texturei(texture, page);
                }
            }
        }
    } while (depth > 0);
    modelClippingOn = 0;
}
