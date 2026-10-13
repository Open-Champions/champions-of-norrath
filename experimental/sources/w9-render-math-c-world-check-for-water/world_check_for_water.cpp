// Observed worldCheckForWater__FP12_worldHeaderG6Point3fPf at 0x0017db78, 216 bytes.
// Arguments: the world header, a Point3 passed by address (a copy of it is made first),
// a float radius, and an optional float output pointer. A box is formed around the point:
//   low  = (x - radius - 1, y - radius - 1, z - 1)
//   high = (x + radius + 1, y + radius + 1, z + 60)
// (x and y subtract/add the radius first, then 1.0; z has no radius) and
// worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi is called with
// (world, &low, &high, patches, 2). The result is 1 when the first patch pointer it left
// is nonzero, else 0. When that pointer is nonzero and the output pointer is not null,
// the float at offset 16 of the first patch is stored through the output pointer.
// Inferred: offset 16 of a patch header holds a water surface height; the box asks
// for patches within radius horizontally and from 1 below to 60 above the point.
// Unknown: the patch header's other fields and the meaning of the limit 2 (the array
// has room for two pointers; only the first is read).
extern "C" void worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
    void* world, const float* low, const float* high, unsigned char** out, int limit);

extern "C" int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, const float* point, float radius, float* height) {
    const float x = point[0], y = point[1], z = point[2];
    float low[3];
    float high[3];
    unsigned char* patches[2];
    low[0] = x - radius - 1.0f;
    low[1] = y - radius - 1.0f;
    low[2] = z - 1.0f;
    high[0] = x + radius + 1.0f;
    high[1] = y + radius + 1.0f;
    high[2] = z + 60.0f;
    worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(world, low, high, patches, 2);
    unsigned char* first = patches[0];
    if (height != nullptr && first != nullptr) *height = *reinterpret_cast<float*>(first + 16);
    return first != nullptr;
}
