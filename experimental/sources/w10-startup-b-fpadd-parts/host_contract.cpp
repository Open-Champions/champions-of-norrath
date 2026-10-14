#include <cstdio>
static const unsigned char *seen_l, *seen_r;
static unsigned char* seen_o;
static int calls;
static unsigned char sentinel;
extern "C" const unsigned char* norrath_player_double_add_parts(const unsigned char* l, const unsigned char* r, unsigned char* o) {
    ++calls; seen_l = l; seen_r = r; seen_o = o; return &sentinel;
}
#include "fpadd_parts.cpp"
int main() {
    unsigned char a[32], b[32], c[32];
    const unsigned char* got = _fpadd_parts(a, b, c);
    if (calls != 1 || seen_l != a || seen_r != b || seen_o != c || got != &sentinel) { std::puts("_fpadd_parts wrong"); return 1; }
    got = _fpadd_parts(b, a, c);
    if (calls != 2 || seen_l != b || seen_r != a) { std::puts("_fpadd_parts order wrong"); return 1; }
    std::puts("_fpadd_parts forwards its three pointers and its result");
    return 0;
}
