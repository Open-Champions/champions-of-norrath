// Host check for cb_poff_gp_span.cpp: the observed 64-byte span and zero start state.
#include <cstdio>
#include "cb_poff_gp_span.cpp"

static_assert(sizeof(sceCbPoff_Gp) == 64, "observed span to the next label");

int main() {
    int bad = 0;
    for (unsigned int w : sceCbPoff_Gp) if (w != 0) bad = 1;
    std::puts(bad ? "FAIL" : "PASS");
    return bad;
}
