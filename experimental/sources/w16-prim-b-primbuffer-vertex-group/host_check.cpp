#include <initializer_list>
#include <string.h>
unsigned char PrimVertexGroupDef[8] __asm__("_12VIPrimBuffer$PrimVertexGroup");
#include "vertex_group.cpp"

int main() {
    for (int g : {0, 1, 32767, -1, -32768, 0x1234}) {
        memset(PrimVertexGroupDef, 0x5a, sizeof PrimVertexGroupDef);
        if (VertexGroup__12VIPrimBuffers(0, static_cast<short>(g)) != 0) return 1;
        short got;
        memcpy(&got, PrimVertexGroupDef, 2);
        if (got != static_cast<short>(g)) return 2;
        for (int i = 2; i < 8; ++i)
            if (PrimVertexGroupDef[i] != 0x5a) return 3;
    }
    return 0;
}
