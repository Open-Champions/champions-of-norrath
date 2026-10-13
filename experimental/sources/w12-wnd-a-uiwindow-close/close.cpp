// Observed: Close__10VIUIWindowb at 0x0103f880 is 24 bytes. With (this, flag) it stores zero at
// this+40 when flag is nonzero, and always stores the float -1.0 at this+44.
// Inferred: this+40 and this+44 belong to the window's open/close animation state (the class has
// IsOpen, IsOpening, IsClosing and IsClosed queries); the exact meaning of the two words is not read here.
// Unknown: what the zero word and the -1.0 mean to those queries.
extern "C" {
void Close__10VIUIWindowb(void* self, bool immediate) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (immediate) *reinterpret_cast<int*>(bytes + 40) = 0;
    *reinterpret_cast<float*>(bytes + 44) = -1.0f;
}
}
