// Observed clone__15ShockProjectile at 0x00134ec0, 8 bytes: returns zero (`jr ra` with `move v0,zero` in the
// delay slot) without reading its argument, touching memory or calling anything. So no clone is made.
// The argument is the source object (inferred from the name). Nothing here ran on the EE.
extern "C" void* clone__15ShockProjectile(void*) { return nullptr; }
