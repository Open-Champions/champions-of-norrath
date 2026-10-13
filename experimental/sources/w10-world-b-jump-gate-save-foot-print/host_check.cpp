// Host contract for jumpGateSaveFootPrint__Fv. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
#include <cstdio>
static char* seen; static int calls; static int result = 484;
extern "C" int jumpGateSave__FPc(char* buffer) { seen = buffer; ++calls; buffer[4095] = 1; return result; }
#include "jump_gate_save_foot_print.cpp"
int main() {
    if (jumpGateSaveFootPrint__Fv() != 484 || calls != 1 || seen == nullptr) return 1;
    result = 17;
    if (jumpGateSaveFootPrint__Fv() != 17 || calls != 2) return 2;  // result passes straight through
    std::puts("jumpGateSaveFootPrint check passed");
    return 0;
}
