// Host check only; not an EE build input. Build the source with -Dprintf=w8_printf.
#include "../libc-format-impure/impure_state.hpp"
#include <cstdio>
#include <cstring>

extern "C" NorrathImpureState* norrath_factory_impure_pointer_word = 0;
extern "C" int w8_printf(const char* format, ...);

struct HostStream { unsigned char head[84]; NorrathImpureState* owner; unsigned char tail[16]; };
static unsigned char* seen_stream; static const char* seen_format; static const unsigned char* seen_cursor;
static int calls, next_result = 42;
static int first_int, second_int; static const char* third_text;
extern "C" int norrath_factory_vfprintf(unsigned char* stream, const char* format, const unsigned char* cursor) {
    ++calls; seen_stream = stream; seen_format = format; seen_cursor = cursor;
    // Read the arguments the way the formatter would (host cursor holds 8-byte slots).
    std::memcpy(&first_int, cursor, sizeof first_int);
    std::memcpy(&second_int, cursor + 8, sizeof second_int);
    std::memcpy(&third_text, cursor + 16, sizeof third_text);
    return next_result;
}

int main() {
    static NorrathImpureState state;
    static HostStream out, err;
    std::memset(&out, 0xee, sizeof out);
    state.standard_out = reinterpret_cast<NorrathImpureFile*>(&out);
    state.standard_error = reinterpret_cast<NorrathImpureFile*>(&err);
    norrath_factory_impure_pointer_word = &state;
    const int r = w8_printf("%d %d %s", 7, -9, "text");
    if (r != 42) return 1;
    if (calls != 1) return 2;
    if (seen_stream != reinterpret_cast<unsigned char*>(&out)) return 3;   // standard-out, not error
    if (out.owner != &state) return 4;                                       // owner word stored
    if (err.owner != nullptr) return 5;
    if (out.head[0] != 0xee || out.tail[0] != 0xee) return 6;                // nothing else touched
    if (std::strcmp(seen_format, "%d %d %s") != 0) return 7;
    if (first_int != 7 || second_int != -9 || std::strcmp(third_text, "text") != 0) return 8;
    next_result = -1;
    if (w8_printf("x") != -1) return 9;
    std::puts("printf ok");
    return 0;
}
