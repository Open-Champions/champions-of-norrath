extern "C" {
extern void* world;
extern unsigned char worldDrawEnable;
extern float worldViewCenter[3];
}
int main() {
    if (world != nullptr) return 1;
    if (worldDrawEnable != 1) return 2;
    if (sizeof(worldViewCenter) != 12) return 3;
    for (float v : worldViewCenter) if (v != 0.0f) return 4;
    return 0;
}
