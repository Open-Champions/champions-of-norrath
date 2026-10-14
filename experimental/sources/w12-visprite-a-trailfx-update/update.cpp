// Observed: Update__9VITrailFxl at 0x01027550, 196 bytes. The second argument
// is a 64-bit time (the image keeps it whole: it subtracts and stores it as a
// doubleword). With the object as this and first = the word at this + 28 (the
// first-link of an embedded list; a node whose first link is zero is the end
// marker):
//   dt = (float)(int)(time - the doubleword at this + 16) * 0.001f, where the
//   subtraction is 64-bit and only the low 32 bits of the difference are
//   converted. The doubleword at this + 16 is replaced by time.
//   Walking from first while the node's first link is non-zero: the float at
//   node + 72 gets dt added and stored. When the float at this + 24 is not
//   greater than that sum: with p the word at node + 4, if the word at p + 4
//   is zero, or the float at p + 72 is also not below the float at this + 24,
//   ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(this, node) is called
//   and the walk ends; otherwise the walk goes on to the next node. A node
//   whose sum is below this + 24 simply goes on to the next node.
//   Afterwards the word at this + 28 is read again; the result is 1 when the
//   first-link of that node is non-zero and 0 otherwise.
// Inferred: the list at this + 28 holds trail segments each with an age at
// +72 and a lifetime at this + 24; the second test keeps one expired segment
// before the walk clears the rest. Unknown: the unit of time (the 0.001 factor
// suggests milliseconds to seconds) and the member names.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(void* trail, void* segment);

int Update__9VITrailFxl(void* self, long long time) {
    char* base = static_cast<char*>(self);
    long long previous = *reinterpret_cast<long long*>(base + 16);
    unsigned first = *reinterpret_cast<unsigned*>(base + 28);
    *reinterpret_cast<long long*>(base + 16) = time;
    float dt = static_cast<float>(static_cast<int>(time - previous)) * 0.001f;
    char* node = NORRATH_TRANSLATE(first);
    for (;;) {
        unsigned next = *reinterpret_cast<unsigned*>(node);
        if (next == 0) break;
        float age = *reinterpret_cast<float*>(node + 72) + dt;
        *reinterpret_cast<float*>(node + 72) = age;
        float lifetime = *reinterpret_cast<float*>(base + 24);
        if (lifetime <= age) {
            char* prev = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(node + 4));
            if (*reinterpret_cast<unsigned*>(prev + 4) == 0 ||
                lifetime <= *reinterpret_cast<float*>(prev + 72)) {
                ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(self, node);
                break;
            }
        }
        node = NORRATH_TRANSLATE(next);
    }
    first = *reinterpret_cast<unsigned*>(base + 28);
    return *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(first)) != 0;
}
}
