// Host check for goblinWeaponResourceMap. Not an EE plan input; it includes the definition file.
#include "goblinWeaponResourceMap.cpp"
int main() {
    if (sizeof goblinWeaponResourceMap != 128u) return 1;
    for (unsigned char byte : goblinWeaponResourceMap) if (byte != 0) return 2;
    return 0;
}
