#include "motif_blender_dtor.cpp"
#include <string>

static std::string tr;
extern "C" void Remove__7VILNode(void* n) { tr += "R" + std::to_string((long)n & 0xff) + ";"; }
extern "C" void __builtin_delete(void* n) { tr += "D" + std::to_string((long)n & 0xff) + ";"; }

int main() {
    alignas(256) unsigned char blender[256] = {};
    struct Case { unsigned linked; int flags; const char* expect; } cases[] = {
        {0, 0, ""}, {0, 1, "D0;"}, {5, 0, "R0;"}, {5, 1, "R0;D0;"}, {1, 2, "R0;"}, {0, 3, "D0;"}, {0, -2, ""},
    };
    for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        tr.clear();
        *reinterpret_cast<unsigned*>(blender + 16) = cases[i].linked;
        _$_14VIMotifBlender(blender, cases[i].flags);
        if (tr != cases[i].expect) return 1 + i;
    }
    return 0;
}
