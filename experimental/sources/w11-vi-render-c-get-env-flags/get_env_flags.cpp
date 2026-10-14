// Observed GetEnvFlags__7VISceneRC7VIVect3 at 0x011451c8, 108 bytes. The world member at offset 4
// of the scene is asked for the leaf containing the position:
//   FindLeaf__C7VIWorldRC7VIVect3RiT2(scene + 4, position, &a, &b)
// A negative result is returned as -1. Otherwise, with the word at offset 80 of the scene as T,
// the result is the 32-bit word at offset 4 of the element at  *(T + 592 * a + 80) + 8 * b.
// Unknown: what a, b and the returned flags mean, and the element types behind the strides 592
// and 8; the word at scene + 80 is presumably the same member as the world's word at offset 76.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIScene;
struct VIVect3;
int FindLeaf__C7VIWorldRC7VIVect3RiT2(void* world, const VIVect3* position, int* a, int* b);

int GetEnvFlags__7VISceneRC7VIVect3(VIScene* scene, const VIVect3* position) {
    char* base = reinterpret_cast<char*>(scene);
    int a;
    int b;
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(base + 4, position, &a, &b) < 0) return -1;
    const char* table = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(base + 80));
    const char* leaf = table + 592 * a;
    const char* elements = NORRATH_TRANSLATE(*reinterpret_cast<const unsigned*>(leaf + 80));
    return *reinterpret_cast<const int*>(elements + 8 * b + 4);
}
}
