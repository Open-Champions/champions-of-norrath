// Observed msg_draw__7OniBall at 0x0021dd40, 8 bytes: returns at once (`jr ra`) without reading its
// argument, touching memory or calling anything. The argument is the object (inferred from the name).
// Nothing here ran on the EE.
extern "C" void msg_draw__7OniBall(void*) {}
