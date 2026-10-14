// Host check only; not an EE input. Compares the authored object with the four words read from the image
// (0, 0, 1, 0) and its 16-byte extent. Build together with nil_rep.cpp.
#include <cstdio>
#include <cstring>
struct Rep { unsigned length, capacity; int references, shared; };
extern "C" Rep w15_nil_rep_char __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
int main() {
    static const unsigned expected[4] = {0, 0, 1, 0};
    if (sizeof(Rep) != 16 || std::memcmp(&w15_nil_rep_char, expected, 16) != 0) { std::puts("nilRep mismatch"); return 1; }
    std::puts("nil_rep ok");
    return 0;
}
