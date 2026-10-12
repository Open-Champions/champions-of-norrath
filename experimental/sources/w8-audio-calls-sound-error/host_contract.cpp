#include "sound_error.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static std::string printed, a_file, a_cond;
static unsigned a_line = 0;
static int order = 0, print_at = 0, assert_at = 0;
extern "C" void scePrintf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    const char* t = __builtin_va_arg(ap, const char*);
    printed = std::string(f) + "|" + t; print_at = ++order;
}
extern "C" void norrath_sound_assert(const char* f, unsigned l, const char* c, int) {
    a_file = f; a_line = l; a_cond = c; assert_at = ++order;
}
int main() {
    char text[] = "boom";
    soundError__FPc(text);
    if (printed != "SOUND ERROR: %s\n|boom") return 1;
    if (a_file != "sound.cc" || a_line != 1904 || a_cond != "0") return 2;
    if (!(print_at == 1 && assert_at == 2)) return 3;
    std::puts("sound_error passed");
    return 0;
}
