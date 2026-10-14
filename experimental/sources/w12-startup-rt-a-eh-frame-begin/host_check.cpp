// Object check for __EH_FRAME_BEGIN__. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <unit.o compiled with authored_cpp flags>
// Requires a symbol of size 0 in the section .norrath_eh_frame_begin (not .eh_frame, see the unit's note)
// with no bytes and no relocations. The linking claim itself was checked by the reviewed ld -r against
// the full partial object; see docs/agents/w12-startup-rt-a.md.
#include "../w12-startup-rt-a-check/data_object_check.hpp"
int main(int argc, char** argv) {
    return argc < 2 ? 9 : data_object_check(argv[1], "__EH_FRAME_BEGIN__", ".norrath_eh_frame_begin", 0, "");
}
