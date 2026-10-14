// Observed objectSaveGetFromStringPool__Fi at 0x143d10, extent 24. A negative
// index gives a null pointer. Otherwise the result is poolOffset (0x4868a4)
// plus the index. Nothing is validated against a pool end. It is the inverse of
// the offsets objectSaveAddToStringPool returns when poolOffset holds the base of
// the same pool; whether it does at run time is unknown.
extern "C" char* poolOffset;

extern "C" char* objectSaveGetFromStringPool__Fi(int index) {
    if (index < 0) return nullptr;
    return poolOffset + index;
}
