// Observed get__11ResourceMap at 0x00166f48, 76 bytes. The receiver is a record
// whose word 0 caches the value of the global resourceFrame (0x00483f88), words
// 1 and 2 are the two string arguments handed to lumpFindResource__FPcT0, and
// word 3 caches its result. When the cached frame equals resourceFrame the
// cached result is returned; otherwise the lookup runs, the result and the
// current frame are stored, and the result is returned.
// Unknown: field names, the resource type and who advances resourceFrame.
extern "C" {
extern int resourceFrame;
void* lumpFindResource__FPcT0(char* first, char* second);

struct ResourceMap {
    int frame;
    char* first;
    char* second;
    void* cached;
};

void* get__11ResourceMap(ResourceMap* self) {
    if (self->frame != resourceFrame) {
        self->cached = lumpFindResource__FPcT0(self->first, self->second);
        self->frame = resourceFrame;
    }
    return self->cached;
}
}
