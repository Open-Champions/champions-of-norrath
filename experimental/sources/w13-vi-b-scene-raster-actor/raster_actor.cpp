// Observed: RasterActor__7VISceneR8VICameraiR11VIClipStack at 0x011420c0, 492 bytes. The int
// argument is used as the address of an actor-like record (as in GetActorTransform__7VISceneiR10VIMatrix44).
// In order, with A = that record and S = the scene:
//  1. if A word +128 equals S word +1348 return 0 (already handled in this pass);
//  2. if A word +64 has bit 0 or bit 2 set return 0;
//  3. d2 = squared distance between A's three floats at +68 and the camera's at +28; the
//     record's object pointer is the first word of entry A word +96 (12-byte entries) of the
//     table pointed to by S word +1004; if d2 is not below the square of that object's float
//     at +48, return 0;
//  4. if CullByTop__11VIClipStackRC8VISphere(clipstack, A + 136) is nonzero return 0;
//  5. store S word +1348 into A word +128; Mul__10VIMatrix44RC10VIMatrix44T1(S + 1232, A,
//     camera + 48); SetModelView__8VIRasterRC10VIMatrix44(S word +19356, S + 1232);
//  6. if the object's word +52 is 5 and d2 is below the scene float at +6268 and the scene
//     count at +6792 is below 128, append A to the pointer list at S + 6280 and bump the count;
//  7. store A word +108 into S word +1092 and -1 into S word +1096, then
//     SetStaticLighting__8VIRasterRC10VIColor32F(S word +19356, A + 112);
//  8. call the object's table entry (object word +60, entry +24: a 16-bit adjustment and a
//     function pointer at entry + 4) as fn(object + adjustment, camera, scene, raster, 1);
//  9. if that returned zero set S words +1092 and +1096 to -1 and return 0; otherwise append
//     {-1, A, A word +96} (12 bytes) with Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem
//     to the vector at S + 1112, return -1 if that fails, else set S words +1092 and +1096 to
//     -1 and return 0 (the image's branch delay slot loads -1 in both outcomes).
// Inferred: per-actor culling and display-list submission for one frame: step 1 a visit
// stamp, step 3 a distance cull, step 4 a clip-stack cull, step 6 a nearby-candidate list,
// and step 8 the actor's own raster method.
// Unknown: the member names, the types behind the bit masks, the callee return types, and the
// meaning of S words +1092 / +1096 (they hold a lighting/display index while the object draws).
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host an address argument is an offset into an arena and a pointer field is a
// 32-bit arena offset.
extern "C" unsigned char* norrath_host_address(int value);
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value);
#define NORRATH_ADDRESS(value) norrath_host_address(value)
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_STORE_POINTER(field, value) norrath_host_store_pointer(field, value)
#else
#define NORRATH_ADDRESS(value) reinterpret_cast<unsigned char*>(static_cast<long>(value))
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_STORE_POINTER(field, value) (*reinterpret_cast<unsigned char**>(field) = (value))
#endif

struct VITableEntry {
    short adjust;
    short pad;
    int (*function)(void* self, const void* camera, void* scene, void* raster, int flag);
}
#if defined(NORRATH_HOST_CONTRACT)
__attribute__((packed))
#endif
;

struct VISceneDisplayElem {
    int first;
    void* actor;
    int second;
};

extern "C" {
int CullByTop__11VIClipStackRC8VISphere(void* clipStack, const void* sphere);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* left, const void* right);
void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
void SetStaticLighting__8VIRasterRC10VIColor32F(void* raster, const void* color);
int Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(void* vector, const VISceneDisplayElem* element);

int RasterActor__7VISceneR8VICameraiR11VIClipStack(void* scene, void* camera, int actorAddress, void* clipStack) {
    unsigned char* s = static_cast<unsigned char*>(scene);
    unsigned char* a = NORRATH_ADDRESS(actorAddress);
    const float* cam = reinterpret_cast<const float*>(static_cast<unsigned char*>(camera) + 28);
    if (*reinterpret_cast<int*>(a + 128) == *reinterpret_cast<int*>(s + 1348)) return 0;
    if (*reinterpret_cast<int*>(a + 64) & 5) return 0;
    const float* position = reinterpret_cast<const float*>(a + 68);
    float dx = position[0] - cam[0];
    float dy = position[1] - cam[1];
    float dz = position[2] - cam[2];
    unsigned char* object = NORRATH_LOAD_POINTER(NORRATH_LOAD_POINTER(s + 1004) + *reinterpret_cast<int*>(a + 96) * 12);
    float range = *reinterpret_cast<float*>(object + 48);
    float distance = dx * dx + dy * dy + dz * dz;
    if (!(distance < range * range)) return 0;
    if (CullByTop__11VIClipStackRC8VISphere(clipStack, a + 136) != 0) return 0;
    *reinterpret_cast<int*>(a + 128) = *reinterpret_cast<int*>(s + 1348);
    Mul__10VIMatrix44RC10VIMatrix44T1(s + 1232, a, static_cast<unsigned char*>(camera) + 48);
    SetModelView__8VIRasterRC10VIMatrix44(NORRATH_LOAD_POINTER(s + 19356), s + 1232);
    if (*reinterpret_cast<int*>(object + 52) == 5) {
        if (distance < *reinterpret_cast<float*>(s + 6268)) {
            if (*reinterpret_cast<int*>(s + 6792) < 128) {
                int count = *reinterpret_cast<int*>(s + 6792);
                NORRATH_STORE_POINTER(s + 6280 + count * 4, a);
                *reinterpret_cast<int*>(s + 6792) = count + 1;
            }
        }
    }
    int lighting = *reinterpret_cast<int*>(a + 108);
    *reinterpret_cast<int*>(s + 1096) = -1;
    *reinterpret_cast<int*>(s + 1092) = lighting;
    SetStaticLighting__8VIRasterRC10VIColor32F(NORRATH_LOAD_POINTER(s + 19356), a + 112);
    VITableEntry* entry = reinterpret_cast<VITableEntry*>(NORRATH_LOAD_POINTER(object + 60) + 24);
    if (entry->function(object + entry->adjust, camera, scene, NORRATH_LOAD_POINTER(s + 19356), 1) != 0) {
        VISceneDisplayElem element;
        element.first = -1;
        element.actor = a;
        element.second = *reinterpret_cast<int*>(a + 96);
        if (Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(s + 1112, &element) < 0) return -1;
    }
    *reinterpret_cast<int*>(s + 1096) = -1;
    *reinterpret_cast<int*>(s + 1092) = -1;
    return 0;
}
}
