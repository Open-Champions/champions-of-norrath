// Observed: Collide__9VICollideR12VICollSphereR12VICollBuffer at 0x01097110 is 104 bytes. It reads the buffer type word at offset 8 of the
// third argument and tail-calls, with the three arguments unchanged: type 0 ->
// CollideBufferV__9VICollideR12VICollSphereR12VICollBuffer, type 1 -> CollideBufferPackV__9VICollideR12VICollSphereR12VICollBuffer, type 3 ->
// CollideBufferPackVGF__9VICollideR12VICollSphereR12VICollBuffer; any other type returns -1 without calling.
// Inferred: the three callees walk differently packed buffer layouts (plain, packed vectors, packed
// with an extra per-group flag, from their names only). Unknown: their results beyond being
// passed through, and what type 2 means (Init rewrites it to 3).
extern "C" {
int CollideBufferV__9VICollideR12VICollSphereR12VICollBuffer(void* self, void* shape, unsigned char* buffer);
int CollideBufferPackV__9VICollideR12VICollSphereR12VICollBuffer(void* self, void* shape, unsigned char* buffer);
int CollideBufferPackVGF__9VICollideR12VICollSphereR12VICollBuffer(void* self, void* shape, unsigned char* buffer);

int Collide__9VICollideR12VICollSphereR12VICollBuffer(void* self, void* shape, unsigned char* buffer) {
    switch (*reinterpret_cast<int*>(buffer + 8)) {
    case 0:
        return CollideBufferV__9VICollideR12VICollSphereR12VICollBuffer(self, shape, buffer);
    case 1:
        return CollideBufferPackV__9VICollideR12VICollSphereR12VICollBuffer(self, shape, buffer);
    case 3:
        return CollideBufferPackVGF__9VICollideR12VICollSphereR12VICollBuffer(self, shape, buffer);
    default:
        return -1;
    }
}
}
