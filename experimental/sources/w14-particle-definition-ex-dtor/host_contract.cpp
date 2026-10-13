#include "definition_ex_dtor.cpp"
#include <string>

static std::string tr;
static unsigned char def[1024];
extern "C" void _$_t6VIPool1ZP15VIParticleMotif(void* p, int f) { tr += "P" + std::to_string((unsigned char*)p - def) + "," + std::to_string(f) + ";"; }
extern "C" void __builtin_delete(void* n) { tr += "D" + std::to_string((unsigned char*)n - def) + ";"; }

int main() {
    struct Case { int flags; const char* expect; } cases[] = {
        {0, "P724,2;"}, {1, "P724,2;D0;"}, {2, "P724,2;"}, {3, "P724,2;D0;"}, {-1, "P724,2;D0;"},
    };
    for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        tr.clear();
        _$_22VIParticleDefinitionEx(def, cases[i].flags);
        if (tr != cases[i].expect) return 1 + i;
    }
    return 0;
}
