// Host check for the network flag storage. Not an EE plan input.
#include "net_flags.cpp"
int main() {
    if (netTrouble || netKickedOut || totallyDisconnected) return 1;
    if (sizeof netTrouble != 4 || sizeof netKickedOut != 1 || sizeof totallyDisconnected != 1) return 2;
    return 0;
}
