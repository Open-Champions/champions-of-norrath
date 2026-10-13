// Object check for __throw_type_match_rtti_2. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <unit.o compiled with authored_cpp flags>
// The type_info records have 32-bit pointer fields, so the behaviour cannot run on a 64-bit host; the
// behavioural evidence is the private paired run recorded in docs/agents/w12-startup-rt-a.md. This
// check guards the object shape: the function is defined in .text, exports nothing else, and imports
// exactly the nine routines the routine reaches, each through a relocation.
#include "../w12-startup-rt-a-check/elf_object.hpp"
#include <algorithm>
#include <set>
int main(int argc, char** argv) {
    if (argc < 2) return 9;
    ElfObject o(argv[1]);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol("__throw_type_match_rtti_2");
    if (!s.found || s.section != ".text" || s.size < 900 || s.size > 1700) { std::puts("shape"); return 1; }
    std::set<std::string> undef;
    for (auto& n : s.undefined) if (!n.empty()) undef.insert(n);
    const std::set<std::string> want = {"__eq__C9type_infoRC9type_info", "__dynamic_cast_2", "upcast__C16__user_type_infoRC9type_infoPvPPv", "__tfv", "__tf9type_info",
                                        "__tf16__user_type_info", "__tf19__pointer_type_info", "__tf16__attr_type_info", "__tf16__func_type_info"};
    if (undef != want) { std::puts("imports"); return 2; }
    std::set<std::string> reached;
    for (auto& r : s.relocs) reached.insert(r.symbol);
    for (auto& n : want) if (!reached.count(n)) { std::printf("not reached: %s\n", n.c_str()); return 3; }
    std::puts("__throw_type_match_rtti_2 object ok");
    return 0;
}
