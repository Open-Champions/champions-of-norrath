// Host check for nameGridSrcY. Not an EE plan input; it includes the definition file.
#include "nameGridSrcY.cpp"
int main() {
    if (sizeof nameGridSrcY != 4u) return 1;
    if (nameGridSrcY != 0.0f) return 2;
    return 0;
}
