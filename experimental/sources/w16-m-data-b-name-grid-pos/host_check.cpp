// Host check for nameGridPos. Not an EE plan input; it includes the definition file.
#include "nameGridPos.cpp"
int main() {
    if (sizeof nameGridPos != 4u) return 1;
    if (nameGridPos != 0) return 2;
    return 0;
}
