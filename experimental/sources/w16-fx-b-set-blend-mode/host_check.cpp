// Host check only; not a build input.
#include "set_blend_mode.cpp"
#include <cstring>
static int ma, mb, calls, lay, md; static char mat;
extern "C" void* Material__8VIRasteri(int a, int b) { ma = a; mb = b; return &mat; }
extern "C" void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* m, int l, int mode) { if (m != &mat) return; lay = l; md = mode; calls++; }
int main() {
    alignas(8) char obj[800]; std::memset(obj, 0, sizeof obj);
    *(int*)(obj + 772) = 5; *(int*)(obj + 776) = 6;
    for (unsigned v = 0; v < 8; v++) {
        calls = 0; SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(obj, v);
        if (*(unsigned*)(obj + 748) != v || ma != 5 || mb != 6) return 1;
        if (v < 5 ? (calls != 1 || lay != 0 || md != (int)v) : calls != 0) return 2;
    }
    return 0;
}
