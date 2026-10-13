// Observed draw__5Spell at 0x00314628, 8 bytes: returns at once (`jr ra`) without reading its argument,
// touching memory or calling anything. The argument is the object (inferred from the name).
// Nothing here ran on the EE.
extern "C" void draw__5Spell(void*) {}
