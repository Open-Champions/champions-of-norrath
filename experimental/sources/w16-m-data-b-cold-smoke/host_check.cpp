// Host check for coldSmoke. Not an EE plan input; it includes the definition file.
#include "coldSmoke.cpp"
int main() {
    if (sizeof coldSmoke != 80u) return 1;
    for (unsigned char byte : coldSmoke) if (byte != 0) return 2;
    return 0;
}
