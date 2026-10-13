// Host contract for getVifAndTex__FPcT0PP12_modelHeaderPP8_texture. Not an EE plan input.
// Uses the host libc sprintf; the unit declares its own prototype.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
extern "C" {
char* npcLumpName = const_cast<char*>("lump");
static std::vector<std::string> looked;
static void* result[2];
void* lumpFindResource__FPcT0(char* lump, char* name) {
    looked.push_back(std::string(lump) + ":" + name);
    return result[looked.size() - 1];
}
int getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(char*, char*, void**, void**);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    char a[] = "alpha", b[] = "beta";
    void *m = nullptr, *t = nullptr;
    result[0] = &a; result[1] = nullptr;
    int r = getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(a, b, &m, &t);
    expect(r == 1 && m == &a && t == nullptr, "model found, texture missing still returns 1");
    expect(looked.size() == 2 && looked[0] == "lump:alpha_beta.vif" && looked[1] == "lump:alpha_beta.tex", "texts and order");
    looked.clear(); result[0] = nullptr; result[1] = &b;
    r = getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(a, b, &m, &t);
    expect(r == 0 && m == nullptr && t == &b, "model missing returns 0, texture still stored");
    if (failures) return 1;
    std::printf("getVifAndTex contract passed\n");
    return 0;
}
