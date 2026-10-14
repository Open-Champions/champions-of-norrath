// Host check for nameGridSrcW. Not an EE plan input; it includes the definition file.
#include "nameGridSrcW.cpp"
int main() {
    if (sizeof nameGridSrcW != 4u) return 1;
    if (nameGridSrcW != 0.0f) return 2;
    return 0;
}
