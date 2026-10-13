// Observed dropGold__4itemG6Point3i at 0x00100a28, 628 bytes (static member of item). Arguments: a
// 12-byte point passed by value (the callee receives a pointer to a copy) and an int amount. The point
// is copied to a local first. When amount is positive: up to ten attempts, each drawing a point from
// the unit ball by rejection with the shared generator syncRandState (state = state * 0x19660d +
// 0x3c6ef35f; each component is (float)(state >> 16) * 2^-16, doubled, minus 1; three steps per draw;
// redraw while 1.0 < x*x + y*y + z*z; syncRandState receives the third state of the accepted draw).
// The offset is (x * 12 * 3, y * 12 * 3, 0). worldFindHeight(world, candidate, 12.0f) is called with
// the candidate (point.x + offset.x, point.y + offset.y, point.z). When |height - point.z| < 6.0 the
// local point becomes (candidate x, candidate y, height) and the attempts end. Either way blockAlloc(192)
// follows, and when it returns nonzero Gold::Gold(block, local point, amount) is called. A
// nonpositive amount does nothing.
// Inferred: the 192-byte block is a gold pickup and the loop scatters it onto nearby ground. Unknown:
// the meaning of 12.0 and 6.0, and the layout of the gold object.
extern "C" {
extern unsigned syncRandState;
extern void* world;
void* blockAlloc__Fi(int size);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float step);
void __4GoldG6Point3i(void* self, const float* point, int amount);

void dropGold__4itemG6Point3i(const float* point, int amount) {
    float position[3] = {point[0], point[1], point[2]};
    if (amount <= 0) return;
    const float unit = 1.52587890625e-05f;
    int attempts = 0;
    unsigned state = syncRandState;
    for (;;) {
        float x, y, z;
        do {
            state = state * 0x19660dU + 0x3c6ef35fU;
            float a = static_cast<float>(state >> 16) * unit;
            state = state * 0x19660dU + 0x3c6ef35fU;
            float b = static_cast<float>(state >> 16) * unit;
            state = state * 0x19660dU + 0x3c6ef35fU;
            float c = static_cast<float>(state >> 16) * unit;
            x = (a + a) - 1.0f;
            y = (b + b) - 1.0f;
            z = (c + c) - 1.0f;
            syncRandState = state;
        } while (1.0f < x * x + y * y + z * z);
        const float offsetX = x * 12.0f * 3.0f;
        const float offsetY = y * 12.0f * 3.0f;
        float candidate[3] = {offsetX + position[0], offsetY + position[1], position[2]};
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, candidate, 12.0f);
        float delta = height - position[2];
        if (delta < 0.0f) delta = -delta;
        if (delta < 6.0f) {
            position[0] += offsetX;
            position[1] += offsetY;
            position[2] = height;
            break;
        }
        if (++attempts >= 10) break;
        state = syncRandState;
    }
    void* block = blockAlloc__Fi(192);
    if (block != nullptr) __4GoldG6Point3i(block, position, amount);
}
}
