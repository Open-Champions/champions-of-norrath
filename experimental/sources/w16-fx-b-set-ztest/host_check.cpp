// Host check only; not a build input.
#include "set_ztest.cpp"
#include <cstring>
static int ma, mb, got_flag, calls; static char mat;
extern "C" void* Material__8VIRasteri(int a, int b) { ma = a; mb = b; return &mat; }
extern "C" void SetZTest__10VIMateriali(void* m, int f) { if (m != &mat) got_flag = -99; else got_flag = f; calls++; }
int main() {
    alignas(8) char obj[800]; std::memset(obj, 0, sizeof obj);
    *(int*)(obj + 772) = 11; *(int*)(obj + 776) = 22;
    int in[] = {0, 1, 2, -1};
    for (int v : in) {
        calls = 0; SetZTest__20VIParticleDefinitionb(obj, v);
        if (calls != 1 || ma != 11 || mb != 22 || got_flag != (v != 0) || *(int*)(obj + 756) != v) return 1;
    }
    return 0;
}
