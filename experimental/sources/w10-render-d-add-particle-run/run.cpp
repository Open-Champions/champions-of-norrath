// Observed run__11AddParticleG6Point3 at 0x00225150, 36 bytes. The Point3 argument arrives by address
// (second argument). The original copies its twelve bytes to a local on its own stack frame and returns;
// the copy is never read, so the function has no effect on memory outside its frame and returns nothing.
// The first argument (the object) is not touched. Unknown: why the copy exists; the by-value parameter
// is the only evidence of what a fuller implementation took.
extern "C" void run__11AddParticleG6Point3(void* self, const float* point) {
    (void)self;
    (void)point;
}
