// Host check only; not a build input.
#include "set_texture.cpp"
#include <cstring>
static int ma, mb, calls, lay, tid; static void *r, *d; static char mat, raster, dict;
extern "C" void* Material__8VIRasteri(int a, int b) { ma = a; mb = b; return &mat; }
extern "C" void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* m, int l, int id, void* ra, void* di) {
    if (m == &mat) { lay = l; tid = id; r = ra; d = di; calls++; }
}
int main() {
    alignas(8) char obj[800]; std::memset(obj, 0, sizeof obj);
    *(int*)(obj + 772) = 5; *(int*)(obj + 776) = 6;
    SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary(obj, 42, &raster, &dict);
    if (calls != 1 || lay != 0 || tid != 42 || r != &raster || d != &dict || ma != 5 || mb != 6 || *(int*)(obj + 780) != 42) return 1;
    return 0;
}
