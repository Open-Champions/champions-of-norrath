// Observed clone__15ShockProjectile at 0x00134ec0, 8 bytes: `jr ra` with `move v0,zero` in the delay slot. It returns zero without
// reading its argument, touching memory or calling anything, so no copy of the object is made. The argument is the source object
// (inferred from the name). Unknown: why this class has no clone. Nothing here ran on the EE.
extern "C" void* clone__15ShockProjectile(void*) { return nullptr; }
