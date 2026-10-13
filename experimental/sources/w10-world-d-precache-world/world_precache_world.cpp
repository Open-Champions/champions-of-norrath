// Observed worldPrecacheWorld__FP12_worldHeaderP8Matrix34 at 0x0017b080, 1120 bytes.
// Arguments: the world header and a Matrix34. A null header returns at once. Otherwise
// worldTransformFrustum__FP8Matrix34 is called with the matrix, the half-word stream tree
// at header+4 is walked, and worldTransformFrustum is called again with worldCameraMatrix
// (0x0037ef00). The walk and tree layout are the same as in worldDrawWorld, with its
// explicit stack of 32-byte entries {node address, clip mask, six floats [min x, max x,
// min y, max y, min z, max z]}. The root has mask 0x8000, x and y bounds of -100000.0f and
// 100000.0f (floats at gp-31108/-31104) and z bounds -60.0f and 780.0f. Interior nodes
// (half-word bit 0x8000; axis = h & 0xfff) and clipping with
// worldClipCubeToFrustum__FPA1_fi are handled as in that routine: mask 0x803f pushes both
// children with the same mask, otherwise the left (max = key1 * 4) and right (min = key2 * 4)
// masks decide which children survive. Each key is a 32-bit value from two half-words,
// sign-extended low half ORed with the high half shifted left 16.
// Leaves list record indices (60-byte records at header+36). Unlike the draw routine there is
// no drawing: for each record, when the mask is not 0x803f its six-float box
// (floats at +8, +20, +12, +24, +16, +28) must clip to a nonzero mask; records with bits
// 0xc880 of the word at +36 set or a zero half-word at +52 are skipped; a page byte of -1 in
// texturePageTable[half] clears worldLastDrawSucceded; otherwise the texture is
// texturePageLoc[page] + word(+32), and when it is nonzero, has bit 0x100 of the half-word at
// +8 set and a zero word at +20, texCacheData__FP8_texturei is called with the texture and
// the half-word at +52. Nothing is returned.
// Inferred: this requests texture data for the parts of the world visible from a camera.
// Unknown: record field names; the stack holds 16 entries in the original frame and no bound
// is checked.
#if defined(NORRATH_HOST_WORLD)
extern unsigned char* norrath_world_arena;
#endif
struct Matrix34;
extern "C" {
extern unsigned char worldLastDrawSucceded;
extern signed char texturePageTable[];
extern int texturePageLoc[];
extern float worldCameraMatrix[];
void worldTransformFrustum__FP8Matrix34(Matrix34*);
int worldClipCubeToFrustum__FPA1_fi(float* bounds, int mask);
void texCacheData__FP8_texturei(void* texture, int page);
}

namespace {
struct Entry {
    const short* node;
    int mask;
    float box[6];
};
inline int half(const short* p) { return *p; }
inline int combine(const short* p) {
    return static_cast<int>(static_cast<unsigned>(static_cast<int>(p[0])) |
                            (static_cast<unsigned>(static_cast<int>(p[1])) << 16));
}
inline float scaled(int key) { return static_cast<float>(key * 4); }
inline unsigned char* link(const unsigned char* base, unsigned offset) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(base + offset);
#if defined(NORRATH_HOST_WORLD)
    return norrath_world_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" void worldPrecacheWorld__FP12_worldHeaderP8Matrix34(void* world, Matrix34* matrix) {
    unsigned char* header = static_cast<unsigned char*>(world);
    if (header == nullptr) return;
    worldTransformFrustum__FP8Matrix34(matrix);

    Entry stack[16];
    int depth = 1;
    stack[0].node = reinterpret_cast<const short*>(link(header, 4));
    stack[0].mask = 0x8000;
    stack[0].box[0] = -100000.0f;
    stack[0].box[1] = 100000.0f;
    stack[0].box[2] = -100000.0f;
    stack[0].box[3] = 100000.0f;
    stack[0].box[4] = -60.0f;
    stack[0].box[5] = 780.0f;

    do {
        --depth;
        Entry& entry = stack[depth];
        const short* node = entry.node;
        const int h = half(node);
        if (h & 0x8000) {
            const unsigned axis = static_cast<unsigned>(h) & 0xfffu;
            const int key1 = combine(node + 1);
            const int key2 = combine(node + 3);
            const int mask = entry.mask;
            const short* left = node + 10;
            const short* right = node + 9 + half(node + 9);
            if (mask == 0x803f) {
                stack[depth + 1].node = right;
                stack[depth + 1].mask = 0x803f;
                entry.node = left;
                depth += 2;
            } else {
                float* low = &entry.box[axis * 2];
                float* high = &entry.box[axis * 2 + 1];
                entry.box[4] = scaled(combine(node + 5));
                entry.box[5] = scaled(combine(node + 7));
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
            const unsigned short* index = reinterpret_cast<const unsigned short*>(node + 1);
            for (int remaining = h; remaining != 0; --remaining) {
                unsigned char* record = link(header, 36) + *index++ * 60u;
                if (mask != 0x803f) {
                    float bounds[6];
                    bounds[0] = *reinterpret_cast<const float*>(record + 8);
                    bounds[1] = *reinterpret_cast<const float*>(record + 20);
                    bounds[2] = *reinterpret_cast<const float*>(record + 12);
                    bounds[3] = *reinterpret_cast<const float*>(record + 24);
                    bounds[4] = *reinterpret_cast<const float*>(record + 16);
                    bounds[5] = *reinterpret_cast<const float*>(record + 28);
                    if (worldClipCubeToFrustum__FPA1_fi(bounds, mask) == 0) continue;
                }
                if (*reinterpret_cast<const unsigned*>(record + 36) & 0xc880u) continue;
                const int page = *reinterpret_cast<const short*>(record + 52);
                if (page == 0) continue;
                const int slot = texturePageTable[page];
                if (slot == -1) {
                    worldLastDrawSucceded = 0;
                    continue;
                }
                const unsigned bits = static_cast<unsigned>(texturePageLoc[slot]) +
                                      *reinterpret_cast<const unsigned*>(record + 32);
                if (bits == 0u) continue;
#if defined(NORRATH_HOST_WORLD)
                unsigned char* texture = norrath_world_arena + bits;
#else
                unsigned char* texture = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
                if (!(*reinterpret_cast<const unsigned short*>(texture + 8) & 0x100u)) continue;
                if (*reinterpret_cast<const unsigned*>(texture + 20) != 0u) continue;
                texCacheData__FP8_texturei(texture, page);
            }
        }
    } while (depth > 0);
    worldTransformFrustum__FP8Matrix34(reinterpret_cast<Matrix34*>(worldCameraMatrix));
}
