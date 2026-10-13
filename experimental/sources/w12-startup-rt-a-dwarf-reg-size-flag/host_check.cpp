// Object check for dwarf_reg_size_table_initialized. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <unit.o compiled with authored_cpp flags>
#include "../w12-startup-rt-a-check/data_object_check.hpp"
int main(int argc, char** argv) {
    return argc < 2 ? 9 : data_object_check(argv[1], "dwarf_reg_size_table_initialized", ".data", 4, "00000000");
}
