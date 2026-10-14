// Host check for tlbsrc. Not an EE plan input; it includes the definition file.
#include "tlbsrc.cpp"
int main() {
    if (sizeof tlbsrc != 816u) return 1;
    for (unsigned i = 0; i < sizeof tlbsrc; ++i)
        if (tlbsrc[i] != 0) return 2;
    return 0;
}
