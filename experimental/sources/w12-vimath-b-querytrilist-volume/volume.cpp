// Observed: Volume__14VIQueryTriListRC6VIBBox at 0x010d11d0 is 32 bytes. Arguments: this, box.
// It tail calls StoreOutward__C6VIBBoxP7VIPlane(box, this + 44) and returns its result.
// Inferred: the 44-byte offset addresses an embedded plane array of the query object.
// Unknown: the plane count, the query object size and the callee's return type.
extern "C" {
int StoreOutward__C6VIBBoxP7VIPlane(const void* box, void* planes);

int Volume__14VIQueryTriListRC6VIBBox(void* self, const void* box) {
    return StoreOutward__C6VIBBoxP7VIPlane(box, static_cast<unsigned char*>(self) + 44);
}
}
