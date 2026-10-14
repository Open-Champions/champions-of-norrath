// Host check for the local pad state storage. Not an EE plan input.
#include "pad_state.cpp"
int main() {
    if (sizeof netLocalPadState != 80) return 1;
    for (unsigned char b : netLocalPadState) if (b) return 2;
    return 0;
}
