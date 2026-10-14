// Host check only; not a build input.
#include "ctor.cpp"
static int order[4], n; static void *a_self, *p_self;
extern "C" void __20VIParticleAttributes(void* s) { a_self = s; order[n++] = 1; }
extern "C" void __t6VIPool1ZP15VIParticleMotif(void* p) { p_self = p; order[n++] = 2; }
int main() {
    alignas(8) char obj[800];
    for (char& c : obj) c = 0x7f;
    __22VIParticleDefinitionEx(obj);
    if (n != 2 || order[0] != 1 || order[1] != 2 || a_self != obj || p_self != obj + 724) return 1;
    if (*(int*)(obj + 748) != 0 || *(int*)(obj + 752) != 0 || *(int*)(obj + 756) != 1 || *(int*)(obj + 760) != 1) return 2;
    if (*(int*)(obj + 744) != 0x7f7f7f7f || *(int*)(obj + 764) != 0x7f7f7f7f) return 3;
    return 0;
}
