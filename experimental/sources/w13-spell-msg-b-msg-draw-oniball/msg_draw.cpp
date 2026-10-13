// Observed msg_draw__7OniBall at 0x0021dd40, 8 bytes: the body is `jr ra` and its delay slot, so it returns at once without
// reading its argument, touching memory or calling anything. The argument is the object (inferred from the name). This is an
// independent unit for the same function as w12-actor-spell-c-msg-draw-7oniball, which was left out of the shared plan for lack of
// a committed check; this one carries a paired-run record and a host contract. Select one of the two, not both.
// Nothing here ran on the EE.
extern "C" void msg_draw__7OniBall(void*) {}
