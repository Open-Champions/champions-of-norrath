// Observed: Plane__C9VIFrustumi at 0x01017590 is 16 bytes. It returns this + 96 +
// index * 16 (index in a1, shifted left by 4) and reads no memory.
// Inferred: the frustum object holds an array of 16-byte plane records starting at
// offset 96; the function is a bounds-unchecked accessor. Unknown: the plane count and
// the layout of the first 96 bytes.
extern "C" {
void* Plane__C9VIFrustumi(const void* frustum, int index) {
    return const_cast<unsigned char*>(static_cast<const unsigned char*>(frustum)) + 96 + (index << 4);
}
}
