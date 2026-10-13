// Observed: RenderRoom__6VIZoneiR15VISceneRendVars at 0x011164d8 is 520 bytes. Arguments: the zone
// (this), a room index and the render-variables record V. Words are 32-bit; "->" below means the word
// is a pointer. The zone holds room records of 120 bytes at ->(zone+96), re-read whenever it is used.
// V holds: scene ->(V+4), camera ->(V+12), raster ->(V+16), clip stack ->(V+20), a word at +48,
// and a float at +52. The scene's word at +1348 is a frame counter. In a record R: flags word at +0,
// bounding box at +4, sprite id word at +28, last-frame word at +36, actor list head word at +104.
//  1. CullByTop__11VIClipStackRC6VIBBox(clip stack, R+4) nonzero: return 0.
//  2. RenderThroughPortals__6VIZoneiR15VISceneRendVars(zone, room, V) negative: return -1.
//  3. If R[36] differs from the frame counter and R[0] has bit 2 clear: let sprite = R[28]; store the
//     frame counter in R[36] (even when sprite is -1); and when sprite is not -1: copy the 64-byte
//     matrix at camera+48 into zone+144 with sceVu0CopyMatrix, then store 0 at zone+192, 0 at +196,
//     0 at +200 and the float 1.0 at +204, call SetModelView__8VIRasterRC10VIMatrix44(raster, zone+144),
//     call SetStaticLighting__8VIRasterRC10VIColor32F(raster, c) with c four floats (0, 0, 0, 1.0), and
//     call RasterSprite__7VISceneR8VICameraiUi(scene, camera, sprite, 1). A nonzero result adds a
//     12-byte element {V[48], -1, sprite} with Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem
//     (scene+1112, &element); a negative add result returns -1.
//  4. d = DistanceSquared__C6VIBBoxRC7VIVect3(R+4, camera+28) with R re-read; unless d is below the
//     square of V's float at +52 return 0.
//  5. Walk the actor list from R[104] (R re-read after step 4) until the link -1: for each node call
//     RasterActor__7VISceneR8VICameraiR11VIClipStack(scene, camera, node[4], clip stack); a negative
//     result returns -1; the next node is node[16]. Return 0.
// Inferred: renders one room's sprite, then its actors if the room is near enough; the stored
// matrix tail (translation row cleared) is for sprite drawing. Unknown: member names, the flag at
// bit 1 of R[0], and the meaning of V[48].
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CullByTop__11VIClipStackRC6VIBBox(unsigned char* clip, const unsigned char* box);
int RenderThroughPortals__6VIZoneiR15VISceneRendVars(unsigned char* zone, int room, unsigned char* vars);
void sceVu0CopyMatrix(unsigned char* dst, const unsigned char* src);
int SetModelView__8VIRasterRC10VIMatrix44(unsigned char* raster, const unsigned char* matrix);
int SetStaticLighting__8VIRasterRC10VIColor32F(unsigned char* raster, const float* color);
int RasterSprite__7VISceneR8VICameraiUi(unsigned char* scene, unsigned char* camera, int sprite, unsigned flag);
int Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(unsigned char* vector, const unsigned* element);
float DistanceSquared__C6VIBBoxRC7VIVect3(const unsigned char* box, const unsigned char* point);
int RasterActor__7VISceneR8VICameraiR11VIClipStack(unsigned char* scene, unsigned char* camera, int actor, unsigned char* clip);

static inline unsigned& word(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }
static inline unsigned char* at(unsigned char* base, unsigned offset) { return NORRATH_WORD_TO_POINTER(word(base, offset)); }

int RenderRoom__6VIZoneiR15VISceneRendVars(unsigned char* self, int room, unsigned char* vars) {
    if (CullByTop__11VIClipStackRC6VIBBox(at(vars, 20), at(self, 96) + 120 * room + 4) != 0) return 0;
    if (RenderThroughPortals__6VIZoneiR15VISceneRendVars(self, room, vars) < 0) return -1;
    {
        unsigned char* record = at(self, 96) + 120 * room;
        const unsigned frame = word(at(vars, 4), 1348);
        if (word(record, 36) != frame && (word(record, 0) & 2) == 0) {
            const int sprite = static_cast<int>(word(record, 28));
            word(record, 36) = frame;
            if (sprite != -1) {
                sceVu0CopyMatrix(self + 144, at(vars, 12) + 48);
                word(self, 192) = 0;
                *reinterpret_cast<float*>(self + 204) = 1.0f;
                word(self, 196) = 0;
                word(self, 200) = 0;
                SetModelView__8VIRasterRC10VIMatrix44(at(vars, 16), self + 144);
                const float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                SetStaticLighting__8VIRasterRC10VIColor32F(at(vars, 16), color);
                if (RasterSprite__7VISceneR8VICameraiUi(at(vars, 4), at(vars, 12), sprite, 1) != 0) {
                    const unsigned element[3] = {word(vars, 48), 0xffffffffu, static_cast<unsigned>(sprite)};
                    if (Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(at(vars, 4) + 1112, element) < 0) return -1;
                }
            }
        }
    }
    const float distance = DistanceSquared__C6VIBBoxRC7VIVect3(at(self, 96) + 120 * room + 4, at(vars, 12) + 28);
    const float limit = *reinterpret_cast<const float*>(vars + 52);
    if (!(distance < limit * limit)) return 0;
    unsigned node = word(at(self, 96) + 120 * room, 104);
    while (node != 0xffffffffu) {
        unsigned char* n = NORRATH_WORD_TO_POINTER(node);
        if (RasterActor__7VISceneR8VICameraiR11VIClipStack(at(vars, 4), at(vars, 12), static_cast<int>(word(n, 4)), at(vars, 20)) < 0) return -1;
        node = word(n, 16);
    }
    return 0;
}
}
