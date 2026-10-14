// Observed: RenderSplashes__12VIAtmosphereR7VISceneR8VICamera at 0x0106ab38 is 416 bytes. The camera
// argument is not read. Words are 32-bit; "->" marks a pointer word. The atmosphere (this) holds
// ->(this+20), the first node of a list of 36-byte splash nodes (the walk visits a node only while
// its own next word at +0 is nonzero, so the last node is a terminator), a signed count at +32, a
// list head at +36 (the free list), and an int material at +56. The scene's word at +19356 is the
// raster. A node holds floats at +16, +20, +24 and +28 and a signed age at +32.
// Steps: cursor = 0. SetMaterial__8VIRasteri(raster, this[56]). remaining = this[32].
// For each visited node: if cursor is 0, n = BeginBillboards__8VIRasterPP11VIBillboardi(raster,
// &cursor, remaining) (cursor is a pointer word the callee fills in). Then n and remaining are
// decremented, and the 48-byte billboard at cursor receives: float node[16] at +0, float node[20] at +4,
// float node[24] at +8, word (node[32] + 1) * 4 at +12, the float 0x3da3d70a (0.08 to float precision) at +16
// and +20, and the float node[28] four times at +32, +36, +40, +44 (+24 and +28 are not written).
// The cursor then advances by 48; if n is now zero EndBillboards__8VIRaster(raster) runs and the
// cursor becomes 0. The node's age at +32 is incremented, and when it reaches 4 the node is
// removed with Remove__7VILNode and appended to the list at this+36 with AddTail__7VILListP7VILNode,
// and this[32] is decremented. After the loop a nonzero cursor ends the batch the same way.
// Inferred: spent splash sprites are billboards that fade over four ticks and then return to the
// free list. Unknown: the billboard field meanings, the unit of n (a count the callee returns, and
// with n of 0 or less the decrement goes negative and the loop keeps writing, as in the original),
// and member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void SetMaterial__8VIRasteri(unsigned char* raster, int material);
int BeginBillboards__8VIRasterPP11VIBillboardi(unsigned char* raster, unsigned* cursor, int count);
void EndBillboards__8VIRaster(unsigned char* raster);
void Remove__7VILNode(unsigned char* node);
void AddTail__7VILListP7VILNode(unsigned char* list, unsigned char* node);

static inline unsigned& word(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }

void RenderSplashes__12VIAtmosphereR7VISceneR8VICamera(unsigned char* self, unsigned char* scene, unsigned char* camera) {
    (void)camera;
    unsigned cursor = 0;
    unsigned char* raster = NORRATH_WORD_TO_POINTER(word(scene, 19356));
    SetMaterial__8VIRasteri(raster, static_cast<int>(word(self, 56)));
    int remaining = static_cast<int>(word(self, 32));
    unsigned char* node = NORRATH_WORD_TO_POINTER(word(self, 20));
    unsigned next = word(node, 0);
    if (next != 0) {
        int available = 0;
        do {
            if (cursor == 0) available = BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, remaining);
            --available;
            --remaining;
            unsigned char* billboard = NORRATH_WORD_TO_POINTER(cursor);
            const float shade = *reinterpret_cast<const float*>(node + 28);
            *reinterpret_cast<float*>(billboard + 0) = *reinterpret_cast<const float*>(node + 16);
            word(billboard, 12) = (word(node, 32) + 1) << 2;
            *reinterpret_cast<float*>(billboard + 4) = *reinterpret_cast<const float*>(node + 20);
            *reinterpret_cast<unsigned*>(billboard + 16) = 0x3da3d70au;
            *reinterpret_cast<float*>(billboard + 8) = *reinterpret_cast<const float*>(node + 24);
            *reinterpret_cast<unsigned*>(billboard + 20) = 0x3da3d70au;
            *reinterpret_cast<float*>(billboard + 32) = shade;
            *reinterpret_cast<float*>(billboard + 36) = shade;
            *reinterpret_cast<float*>(billboard + 40) = shade;
            *reinterpret_cast<float*>(billboard + 44) = shade;
            cursor = cursor + 48;
            if (available == 0) {
                EndBillboards__8VIRaster(raster);
                cursor = 0;
            }
            const unsigned age = word(node, 32) + 1;
            word(node, 32) = age;
            if (age == 4) {
                Remove__7VILNode(node);
                AddTail__7VILListP7VILNode(self + 36, node);
                word(self, 32) = word(self, 32) - 1;
            }
            node = NORRATH_WORD_TO_POINTER(next);
            next = word(node, 0);
        } while (next != 0);
    }
    if (cursor != 0) {
        EndBillboards__8VIRaster(raster);
        cursor = 0;
    }
}
}
