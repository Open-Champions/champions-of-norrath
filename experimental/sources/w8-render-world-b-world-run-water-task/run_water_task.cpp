// Observed worldRunWaterTask__Fv at 0x0017dc50, 40 bytes.
// If the world pointer at 0x004844a0 (world) is nonnull, calls
// worldRunWater__FP12_worldHeader with it. Nothing is returned. The world
// pointer is defined by another source of this package; worldRunWater is
// outside this package and stays an unresolved import.
extern "C" {
extern void* world;
void worldRunWater__FP12_worldHeader(void* header);
void worldRunWaterTask__Fv() {
    if (world != nullptr) {
        worldRunWater__FP12_worldHeader(world);
    }
}
}
