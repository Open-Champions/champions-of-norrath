// __extendsfdf2: float to double for code built with the reviewed (newer) toolchain.
// Observed: the executable has no function of this name. Its float-to-double helper is fptodp
// (0x002fb320, 60 bytes), and the original callers invoke that (the soft-double entry points take
// and return a double as one 64-bit value). The authored sources that convert a float to a double
// (for example the variadic argument of a printf call in async_change_players) are compiled by the
// newer compiler, which names its helper __extendsfdf2 instead; the build then imports that name.
// This unit supplies the name as a pass-through to fptodp and returns its result unchanged, so
// those callers use the same conversion path as the original. It is a naming bridge, not a
// reconstruction of an original function, and it adds no behaviour of its own.
// Unknown: whether fptodp is bit-exact with the toolchain helper for NaN payloads and denormals.
extern "C" {
unsigned long long fptodp(float value);

unsigned long long __extendsfdf2(float value) {
    return fptodp(value);
}
}
