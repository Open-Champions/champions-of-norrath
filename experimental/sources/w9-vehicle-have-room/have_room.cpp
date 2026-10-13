// Observed haveRoom__7Vehicle at 0x00277ce0, 52 bytes.
// Returns 1 when the word at object+980 is less than the word at object+976
// (signed) and the float at object+808 compares equal to 0.0; otherwise 0.
// A NaN at +808 fails the equality. Nothing is written. Nothing here was
// executed on the EE. The meaning of the three fields is not established.
typedef int AliasedInt __attribute__((may_alias));
typedef float AliasedFloat __attribute__((may_alias));

extern "C" int haveRoom__7Vehicle(unsigned char* object) {
    if (*reinterpret_cast<AliasedInt*>(object + 980) < *reinterpret_cast<AliasedInt*>(object + 976)) {
        if (*reinterpret_cast<AliasedFloat*>(object + 808) == 0.0f) return 1;
    }
    return 0;
}
