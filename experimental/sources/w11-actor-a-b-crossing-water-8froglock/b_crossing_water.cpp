// Observed bCrossingWater__8Froglock at 0x002011b8, 60 bytes. Reads four signed bytes of the object:
// A at +1104, B at +1105, C at +1106 and D at +568. Returns 1 when A is zero and D is non-zero. Otherwise
// returns 1 exactly when C is non-zero and B is zero (0 when C is zero, or when both are non-zero).
// The meanings of the four fields are unknown; the name suggests water but that is not tested here.
// Nothing here ran on the EE.
extern "C" int bCrossingWater__8Froglock(void* self) {
    const signed char* o = static_cast<const signed char*>(self);
    if (o[1104] == 0 && o[568] != 0) return 1;
    if (o[1106] == 0) return 0;
    return o[1105] == 0 ? 1 : 0;
}
