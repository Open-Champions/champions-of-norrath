// Host contract for _$_19__pointer_type_info. Not an EE plan input. The base destructor and the
// delete are stubs that record order and arguments.
#include <cstdio>
#include <cstring>
#include <string>
#include "dtor.cpp"
extern "C" const unsigned char norrath_legacy_pointer_type_info_vtable_view[24] = {};
static std::string trace; static void* seen_obj; static int seen_flags = -1;
extern "C" void _$_9type_info(void* o, int flags) { trace += "base;"; seen_obj = o; seen_flags = flags; }
extern "C" void __builtin_delete(void* o) { trace += o == seen_obj ? "delete;" : "delete?;"; }
static unsigned word(const unsigned char* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<unsigned>(p[3]) << 24); }
int main() {
    alignas(8) unsigned char node[16];
    const unsigned expect = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_legacy_pointer_type_info_vtable_view));
    for (int flags : {0, 1, 2, 3, -1, 0x100}) {
        std::memset(node, 0xaa, sizeof node); trace.clear(); seen_flags = -1;
        _$_19__pointer_type_info(node, flags);
        if (word(node + 4) != expect) return 1;
        for (int i : {0, 1, 2, 3, 8, 9, 15}) if (node[i] != 0xaa) return 2;             // only the word at 4 is written
        if (seen_obj != node || seen_flags != 0) return 3;
        const bool del = (flags & 1) != 0;
        if (trace != (del ? "base;delete;" : "base;")) return 4;
    }
    std::puts("dtor 19__pointer_type_info ok");
    return 0;
}
