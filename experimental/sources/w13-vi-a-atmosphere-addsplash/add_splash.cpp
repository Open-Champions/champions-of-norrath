// Observed: AddSplash__12VIAtmosphereR7VIVect3 at 0x0106b7e0 is 208 bytes. Arguments: atmosphere
// (a0) and a position of three floats (a1). It calls RemHead__7VILList(atmosphere + 36); a null
// result ends the call with no other effect. Otherwise, with node the result: it copies the
// three floats to node+16, node+20 and node+24, increments the int at atmosphere+32, stores zero
// at node+32, calls rand(), stores ((131 * r) & 0xff) * 0.0039215684f * 0.19999999f +
// 0.28199998f at node+28 and calls AddTail__7VILListP7VILNode(atmosphere + 20, node). Nothing
// is returned. The three scale floats are the truncated single-precision values 0x3b808080,
// 0x3e4ccccc and 0x3e90624d.
// Inferred: a pool of splash nodes (free list at +36, active list at +20, active count at +32);
// +16 is the splash position, +28 a random size/duration in 0.282..0.482 and +32 an age or
// phase counter reset to zero.
// Unknown: what the fields at node+28 and node+32 are used for and the node size.
extern "C" {
void* RemHead__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
int rand(void);

void AddSplash__12VIAtmosphereR7VIVect3(unsigned char* atmosphere, const float* position) {
    unsigned char* node = static_cast<unsigned char*>(RemHead__7VILList(atmosphere + 36));
    if (node == 0) {
        return;
    }
    float* at = reinterpret_cast<float*>(node + 16);
    at[0] = position[0];
    *reinterpret_cast<int*>(atmosphere + 32) += 1;
    at[1] = position[1];
    *reinterpret_cast<int*>(node + 32) = 0;
    at[2] = position[2];
    float u = static_cast<float>((131 * rand()) & 0xff) * 0.0039215684f;
    *reinterpret_cast<float*>(node + 28) = u * 0.19999999f + 0.28199998f;
    AddTail__7VILListP7VILNode(atmosphere + 20, node);
}
}
