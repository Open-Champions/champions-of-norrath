// Observed: skirmishInit__Fv at 0x002bfbc0 is 8 bytes: a return with an empty delay slot. It
// takes no arguments and touches nothing. Its observed caller (game load of a level whose name
// contains "skirmish") ignores any result.
// Unknown: whether the body was removed from the shipped build.

extern "C" void skirmishInit__Fv() {}
