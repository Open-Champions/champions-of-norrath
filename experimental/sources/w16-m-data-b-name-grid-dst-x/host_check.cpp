// Host check for nameGridDstX. Not an EE plan input; it includes the definition file.
#include "nameGridDstX.cpp"
int main() {
    if (sizeof nameGridDstX != 4u) return 1;
    if (nameGridDstX != 0.0f) return 2;
    return 0;
}
