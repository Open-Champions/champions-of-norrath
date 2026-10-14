#include "pick_prim_buffer.cpp"

int main() {
    char a, b, c, d;
    if (PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(&a, &b, &c, &d) != 0) return 1;
    if (PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(0, 0, 0, 0) != 0) return 2;
    return 0;
}
