// Observed: Vertex__8VIRasterRC7VIVect3 at 0x0110d5e0, 240 bytes. It appends four 16-byte
// entries at the packet cursor (the pointer word at raster + 52), advancing the cursor by
// 16 after each, and then adds 1 to the vertex counter word at raster + 48. It returns 0.
//   entry 0: the three floats of the vector argument, then 1.0f
//   entry 1: raster floats +12 and +16 (the pair UV__8VIRasterRC7VIVect2 sets), 1.0f, integer 0
//   entry 2: raster floats +20, +24, +28, then integer 0
//   entry 3: raster floats +32, +36, +40, +44
// Inferred: the entries are position, texture coordinate, a second three-float attribute and
// a four-float color for one vertex of the open fan; the counter is the vertex count later
// written by EndTriFan__8VIRaster.
// Unknown: the member names and the meaning of the raster floats at +20..+28 and +32..+44.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_STORE_POINTER(field, value) norrath_host_store_pointer(field, value)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_STORE_POINTER(field, value) (*reinterpret_cast<unsigned char**>(field) = (value))
#endif

extern "C" {
int Vertex__8VIRasterRC7VIVect3(void* raster, const void* position) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    const float* p = static_cast<const float*>(position);
    const float* f = static_cast<const float*>(raster);
    float* entry = reinterpret_cast<float*>(NORRATH_LOAD_POINTER(r + 52));
    entry[0] = p[0];
    entry[1] = p[1];
    entry[2] = p[2];
    entry[3] = 1.0f;
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(entry + 4));
    entry = reinterpret_cast<float*>(NORRATH_LOAD_POINTER(r + 52));
    entry[0] = f[3];
    entry[1] = f[4];
    entry[2] = 1.0f;
    reinterpret_cast<int*>(entry)[3] = 0;
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(entry + 4));
    entry = reinterpret_cast<float*>(NORRATH_LOAD_POINTER(r + 52));
    entry[0] = f[5];
    entry[1] = f[6];
    entry[2] = f[7];
    reinterpret_cast<int*>(entry)[3] = 0;
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(entry + 4));
    entry = reinterpret_cast<float*>(NORRATH_LOAD_POINTER(r + 52));
    entry[0] = f[8];
    entry[1] = f[9];
    entry[2] = f[10];
    entry[3] = f[11];
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(entry + 4));
    *reinterpret_cast<int*>(r + 48) += 1;
    return 0;
}
}
