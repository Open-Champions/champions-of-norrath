// Object check for get_eh_context. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <unit.o compiled with authored_cpp flags>
// Requires a four-byte object in .data whose only content is one relocation against
// eh_context_initialize (the stored word is zero in the unlinked object).
#include "../w12-startup-rt-a-check/data_object_check.hpp"
int main(int argc, char** argv) {
    return argc < 2 ? 9 : data_object_check(argv[1], "get_eh_context", ".data", 4, "00000000", {"eh_context_initialize@0"});
}
