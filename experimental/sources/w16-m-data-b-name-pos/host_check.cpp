// Host check for namePos. Not an EE plan input; it includes the definition file.
#include "namePos.cpp"
int main() {
    if (sizeof namePos != 4u) return 1;
    if (namePos != 0) return 2;
    return 0;
}
