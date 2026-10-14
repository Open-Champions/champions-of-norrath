// Host check for nameGridSrcX. Not an EE plan input; it includes the definition file.
#include "nameGridSrcX.cpp"
int main() {
    if (sizeof nameGridSrcX != 4u) return 1;
    if (nameGridSrcX != 0.0f) return 2;
    return 0;
}
