// Object check for __throw. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <unit.o compiled with authored_cpp flags>
// The behaviour needs compiler exception-return builtins and cannot run on a host, so the behavioural
// evidence is a private paired run (see docs/agents/w12-startup-rt-a.md). This check guards the object
// shape: __throw is defined in .text, imports exactly get_eh_context, __terminate, __frame_state_for
// and throw_helper, and reaches each of them through a relocation (the pointer load, two calls to
// __terminate, one each to __frame_state_for and throw_helper).
#include "../w12-startup-rt-a-check/elf_object.hpp"
#include <algorithm>
#include <map>
int main(int argc, char** argv) {
    if (argc < 2) return 9;
    ElfObject o(argv[1]);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol("__throw");
    if (!s.found || s.section != ".text" || s.size < 300 || s.size > 480) { std::puts("shape"); return 1; }
    std::vector<std::string> undef = s.undefined;
    undef.erase(std::remove(undef.begin(), undef.end(), std::string()), undef.end());
    std::sort(undef.begin(), undef.end());
    undef.erase(std::unique(undef.begin(), undef.end()), undef.end());
    const std::vector<std::string> want = {"__frame_state_for", "__terminate", "get_eh_context", "throw_helper"};
    if (undef != want) { std::puts("imports"); return 2; }
    std::map<std::string, int> count;
    for (auto& r : s.relocs) ++count[r.symbol];
    if (count["get_eh_context"] != 2 || count["__terminate"] != 2 || count["__frame_state_for"] != 1 || count["throw_helper"] != 1) { std::puts("relocs"); return 3; }
    std::puts("__throw object ok");
    return 0;
}
