// Host check for osdsrc. Not an EE plan input; it includes the definition file.
#include "osdsrc.cpp"
int main() {
    if (sizeof osdsrc != 1960u) return 1;
    for (unsigned i = 0; i < sizeof osdsrc; ++i)
        if (osdsrc[i] != 0) return 2;
    return 0;
}
