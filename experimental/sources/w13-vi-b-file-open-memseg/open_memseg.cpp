// Observed: Open__6VIFileiPQ26VIFile7MemSegmil at 0x010caed0, 20 bytes. It forwards
// all four incoming argument registers unchanged to MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil
// (0x01002350) as a tail call, so that function's result is the result seen by the
// caller. The image saves and restores the return address around the tail call.
// Inferred: the integer arguments are a mode, a segment descriptor pointer and a
// length/size, matching the mangled parameter list (this, int, MemSegm*, long).
// Unknown: the meaning of the integer and the long, and the return type (an int is
// assumed; the image does not touch the register after the call).
extern "C" {
int MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil(void* file, int mode, void* segment, long size);

int Open__6VIFileiPQ26VIFile7MemSegmil(void* file, int mode, void* segment, long size) {
    return MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil(file, mode, segment, size);
}
}
