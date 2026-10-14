// Host check for nameGridDstW. Not an EE plan input; it includes the definition file.
#include "nameGridDstW.cpp"
int main() {
    if (sizeof nameGridDstW != 4u) return 1;
    if (nameGridDstW != 0.0f) return 2;
    return 0;
}
