#include "set_num_layers.cpp"
#include <string.h>
#include <string>

static std::string tr; static unsigned char* mat;
extern "C" void Identity__10VIMatrix33(void* m) { tr += "I" + std::to_string((unsigned char*)m - mat) + ";"; }
extern "C" void ReleaseSurface__8VIRasteriR12VIDictionary(void* r, int s, void* d) { (void)r; (void)d; tr += "R" + std::to_string(s) + ";"; }

int main() {
    alignas(4) static unsigned char m[48 + 144 * 4 + 16]; mat = m; int rast, dict;
    auto fresh = [&](int count) { memset(m, 0x6b, sizeof m); tr.clear(); *(int*)(m + 8) = count; };
    fresh(1);
    if (SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(m, 4, &rast, &dict) != -1 || tr != "" || *(int*)(m + 8) != 1 || *(int*)(m + 32) != 0x6b6b6b6b) return 1;
    if (SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(m, 9, &rast, &dict) != -1) return 2;
    // grow from 1 to 3: layers 1 and 2 are initialised
    fresh(1);
    if (SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(m, 3, &rast, &dict) != 0) return 3;
    if (tr != "I208;I352;") return 4;                // 48 + 144 + 16 and 48 + 288 + 16 relative to the material
    if (*(int*)(m + 8) != 3 || *(int*)(m + 32) != 1) return 5;
    for (int i = 1; i < 3; ++i) {
        unsigned char* l = m + 48 + 144 * i;
        if (*(int*)l != -1 || *(int*)(l + 4) != 1 || l[8] != 0xff || l[9] != 0xff || l[10] != 0xff || l[11] != 0xff || *(int*)(l + 12) != 0) return 6;
        if (*(float*)(l + 64) != 0.0f || *(float*)(l + 68) != 0.0f || *(int*)(l + 16) != 0x6b6b6b6b || *(int*)(l + 72) != 0x6b6b6b6b) return 7;
    }
    if (*(int*)(m + 48) != 0x6b6b6b6b || *(int*)(m + 48 + 144 * 3) != 0x6b6b6b6b) return 8;   // layers 0 and 3 untouched
    // shrink from 3 to 1: layer 1 holds surface 5, layer 2 none
    fresh(3); *(int*)(m + 48 + 144) = 5; *(int*)(m + 48 + 288) = -1;
    if (SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(m, 1, &rast, &dict) != 0 || tr != "R5;") return 9;
    if (*(int*)(m + 8) != 1 || *(int*)(m + 32) != 1 || *(int*)(m + 48 + 144) != 5) return 10;
    // shrink releases both when both are set
    fresh(3); *(int*)(m + 48 + 144) = 5; *(int*)(m + 48 + 288) = 0;
    SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(m, 1, &rast, &dict);
    if (tr != "R5;R0;") return 11;
    // same count: only the count and the flag are stored
    fresh(2);
    if (SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(m, 2, &rast, &dict) != 0 || tr != "" || *(int*)(m + 32) != 1) return 12;
    return 0;
}
