// Observed worldGetDoorHeight__FP12_worldHeaderi at 0x0017d3d0, 64 bytes. Arguments: a world header
// and a door index. A negative index gives 0. Otherwise the header's pointer at +84 is a table of
// 28-byte door records; the signed halfword at record offset 24 selects an entry of 36-byte records
// reached through the header's pointer at +32, and the word at offset 28 of that entry is returned.
// The word comes back in the integer result register (a plain 32-bit load; no float conversion).
// There is no upper bound check on the index or on the selector.
// Inferred: the selector names a door model/type and the returned word is its height (the callee
// name says so; the type of the word is not established). Unknown: both record layouts beyond the
// fields used here. Nothing here ran on the EE.
namespace {
inline const unsigned char* load_pointer(const unsigned char* at) {
    const unsigned char* value;
    __builtin_memcpy(&value, at, sizeof value);
    return value;
}
}  // namespace

extern "C" int worldGetDoorHeight__FP12_worldHeaderi(const void* world, int door) {
    if (door < 0) return 0;
    const unsigned char* header = static_cast<const unsigned char*>(world);
    const unsigned char* record = load_pointer(header + 84) + door * 28;
    short selector;
    __builtin_memcpy(&selector, record + 24, sizeof selector);
    int result;
    __builtin_memcpy(&result, load_pointer(header + 32) + selector * 36 + 28, sizeof result);
    return result;
}
