// Observed: Clear__9VITrailFx at 0x010272e8, 72 bytes. With the trail effect as this:
// if the pointer at +52 is non-zero it is passed to __builtin_vec_delete and the
// pointer is then set to zero. Init__7VILList is called on this + 28 and then on
// this + 40. The result is zero.
// Inferred: +52 owns an array of segments; +28 and +40 are two segment lists (the
// second is the one ClearSegmentsForward appends to). Unknown: member names.
extern "C" {
void __builtin_vec_delete(void* pointer);
void Init__7VILList(void* list);

int Clear__9VITrailFx(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    void*& segments = *reinterpret_cast<void**>(bytes + 52);
    if (segments != 0) {
        __builtin_vec_delete(segments);
        segments = 0;
    }
    Init__7VILList(bytes + 28);
    Init__7VILList(bytes + 40);
    return 0;
}
}
