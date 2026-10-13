// Observed: Open__10VIUIWindowb at 0x0103f800 is 36 bytes. With (this, flag) it stores the single-float
// 1.0 (0x3f800000) at byte offset 40 only when flag is nonzero, then always stores 1.0 at offset 44.
// It returns nothing and calls nothing.
// Inferred: offsets 40 and 44 are float fields (the stores use the FPU register file).
// Unknown: what the two floats mean; whether a closing counterpart resets them.
extern "C" void Open__10VIUIWindowb(void* self, bool flag) {
    float* fields = reinterpret_cast<float*>(static_cast<unsigned char*>(self) + 40);
    if (flag) fields[0] = 1.0f;
    fields[1] = 1.0f;
}
