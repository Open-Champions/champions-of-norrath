// Host check for nameGridDstY. Not an EE plan input; it includes the definition file.
#include "nameGridDstY.cpp"
int main() {
    if (sizeof nameGridDstY != 4u) return 1;
    if (nameGridDstY != 0.0f) return 2;
    return 0;
}
