// Object check for _ctype_. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <unit.o compiled with authored_cpp flags>
// Builds the expected 257 bytes from the host's C-locale classification (slot 0 and characters 128..255
// are zero) and requires the object in .rodata with exactly those bytes, size 257, no relocations.
#include <cctype>
#include <clocale>
#include <cstdio>
#include "../w12-startup-rt-a-check/data_object_check.hpp"
int main(int argc, char** argv) {
    if (argc < 2) return 9;
    std::setlocale(LC_ALL, "C");
    std::string hex = "00";
    char buf[3];
    for (int c = 0; c < 256; ++c) {
        unsigned want = 0;
        if (c < 128) {
            if (std::isupper(c)) want |= 0x01;
            if (std::islower(c)) want |= 0x02;
            if (std::isdigit(c)) want |= 0x04;
            if (std::isspace(c)) want |= 0x08;
            if (std::ispunct(c)) want |= 0x10;
            if (std::iscntrl(c)) want |= 0x20;
            if (std::isxdigit(c) && !std::isdigit(c)) want |= 0x40;
            if (c == ' ') want |= 0x80;
        }
        std::snprintf(buf, sizeof buf, "%02x", want);
        hex += buf;
    }
    return data_object_check(argv[1], "_ctype_", ".rodata", 257, hex);
}
