// Observed: timeLastTransition is one 32-bit word at 0x00484cc0 in initialized data (symbol size 4). The
// file bytes are 81 69 67 ff, which is -9999999 as a signed value. runMusic__Fv loads and stores it and
// gameLoadWorld__FPc stores it.
// Inferred: a time stamp taken when music last changed, with a very old initial value, from the name and
// the two users.
// Unknown: the clock it samples.

extern "C" int timeLastTransition = -9999999;
static_assert(sizeof(timeLastTransition) == 4, "timeLastTransition");
