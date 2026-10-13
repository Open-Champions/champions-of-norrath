#include "copy_layer.cpp"
#include <string.h>
#include <string>

static std::string tr; static unsigned char* mat;
extern "C" void* __as__10VIMatrix33RC10VIMatrix33(void* d, const void* s) { tr += "M" + std::to_string((unsigned char*)d - mat) + "," + std::to_string((const unsigned char*)s - mat) + ";"; return d; }
extern "C" void ReleaseSurface__8VIRasteriR12VIDictionary(void*, int s, void*) { tr += "R" + std::to_string(s) + ";"; }
extern "C" void ShareSurface__8VIRasteri(void*, int s) { tr += "S" + std::to_string(s) + ";"; }

int main() {
    alignas(4) static unsigned char m[48 + 144 * 4]; mat = m; int rast, dict;
    unsigned char before[sizeof m];
    auto fresh = [&] { for (unsigned i = 0; i < sizeof m; ++i) m[i] = (unsigned char)(i * 7 + 3); tr.clear(); };
    fresh(); memcpy(before, m, sizeof m);
    if (CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(m, 2, 2, &rast, &dict) != 0 || tr != "" || memcmp(m, before, sizeof m) != 0) return 1;
    // 1 -> 3, both surfaces set
    fresh(); memcpy(before, m, sizeof m);
    *(int*)(m + 48 + 144) = 11; *(int*)(m + 48 + 432) = 22; memcpy(before, m, sizeof m);
    if (CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(m, 1, 3, &rast, &dict) != 0) return 2;
    if (tr != "M496,208;R22;S11;") return 3;
    unsigned char e[sizeof m]; memcpy(e, before, sizeof m);
    unsigned char* F = before + 48 + 144; unsigned char* T = e + 48 + 432;
    memcpy(T + 4, F + 4, 4); memcpy(T + 8, F + 8, 4); memcpy(T + 12, F + 12, 4); memcpy(T + 52, F + 52, 20); memcpy(T + 0, F + 0, 4);
    *(int*)(e + 32) = 1;
    // the matrix stub does not copy: matrix bytes (16..51) stay as before
    if (memcmp(m, e, sizeof m) != 0) return 4;
    // old target surface -1, source -1: neither call
    fresh(); *(int*)(m + 48) = -1; *(int*)(m + 48 + 144) = -1;
    CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(m, 0, 1, &rast, &dict);
    if (tr != "M208,64;") return 5;                 // target layer 1 (48 + 144 + 16), source layer 0 (48 + 16)
    if (*(int*)(m + 48 + 144) != -1) return 6;
    // target set, source -1: release only
    fresh(); *(int*)(m + 48) = -1; *(int*)(m + 48 + 144) = 9;
    CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(m, 0, 1, &rast, &dict);
    if (tr.find("R9;") == std::string::npos || tr.find("S") != std::string::npos) return 7;
    // target -1, source set: share only
    fresh(); *(int*)(m + 48) = 4; *(int*)(m + 48 + 144) = -1;
    CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(m, 0, 1, &rast, &dict);
    if (tr.find("R") != std::string::npos || tr.find("S4;") == std::string::npos || *(int*)(m + 48 + 144) != 4) return 8;
    return 0;
}
