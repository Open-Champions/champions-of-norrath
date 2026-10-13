// Observed isPassenger__7Vehiclei at 0x00277d18, 72 bytes.
// Scans the n words starting at object+960, where n is the signed word at
// object+976. Returns 1 at the first word equal to the argument, else 0.
// A count of zero or less reads nothing. Nothing is written. Nothing here was
// executed on the EE.
typedef int AliasedInt __attribute__((may_alias));

extern "C" int isPassenger__7Vehiclei(unsigned char* object, int value) {
    const int count = *reinterpret_cast<AliasedInt*>(object + 976);
    const AliasedInt* slot = reinterpret_cast<AliasedInt*>(object + 960);
    for (int i = 0; i < count; ++i) {
        if (slot[i] == value) return 1;
    }
    return 0;
}
