// Observed: Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene at 0x010a47e8 is 384
// bytes (frame 672). It builds a VIObjFile local with __9VIObjFile, clears a
// local segment pointer, and calls Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm
// (this, file, &count, &segments). A negative result destroys the local with
// _$_9VIObjFile flags 2 and returns -1. Otherwise, if the first word of this is
// nonzero it is called as a function with the second word of this as its only
// argument. The segment count is read after that call. With count > 0 the total is
// the sum of the 64-bit values at offset 0 of each 16-byte segment record, else 0.
// Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode(local, count, segments,
// total, 0) is called; a negative result destroys the local with flags 2 and
// returns -1. Then Parse__10VIESFParseR9VIObjFileR7VIScene(parse, local, scene):
// negative destroys the local and returns -1. On success, for each of the count
// records the word at record+8 is passed to __builtin_delete (the count is read
// again each iteration), the segment array is passed to __builtin_vec_delete if
// nonzero, the segment pointer is cleared, the local is destroyed with flags 2
// and 0 is returned. On the failure paths after Decompress the segment array and
// records are not released.
// Inferred: the 16-byte records are VIFile::MemSegm; the first word of this is a
// progress/callback pointer. Unknown: sizeof(VIObjFile) (the frame bounds it at
// 592 bytes or less), the meaning of the 64-bit total, and the mode value 0.
extern "C" {
void __9VIObjFile(void* file);
void _$_9VIObjFile(void* file, int flags);
int Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(void* object, void* file, int* count, unsigned char** segments);
int Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode(void* file, int count, unsigned char* segments, long long total, int mode);
int Parse__10VIESFParseR9VIObjFileR7VIScene(void* parse, void* file, void* scene);
void __builtin_delete(void* pointer);
void __builtin_vec_delete(void* pointer);

int Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(void* object, void* file, void* parse, void* scene) {
    alignas(8) unsigned char objfile[592];
    int count;
    unsigned char* segments = 0;
    __9VIObjFile(objfile);
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(object, file, &count, &segments) < 0) {
        _$_9VIObjFile(objfile, 2);
        return -1;
    }
    unsigned int* words = static_cast<unsigned int*>(object);
    if (words[0] != 0) {
        typedef void (*Callback)(unsigned int);
        reinterpret_cast<Callback>(static_cast<__UINTPTR_TYPE__>(words[0]))(words[1]);
    }
    long long total = 0;
    if (0 < count) {
        unsigned char* record = segments;
        for (int remaining = count; remaining != 0; --remaining) {
            total += *reinterpret_cast<long long*>(record);
            record += 16;
        }
    }
    if (Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode(objfile, count, segments, total, 0) < 0 ||
        Parse__10VIESFParseR9VIObjFileR7VIScene(parse, objfile, scene) < 0) {
        _$_9VIObjFile(objfile, 2);
        return -1;
    }
    for (int index = 0; index < count; ++index) {
        __builtin_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(
            *reinterpret_cast<unsigned int*>(segments + index * 16 + 8))));
    }
    if (segments != 0) __builtin_vec_delete(segments);
    segments = 0;
    _$_9VIObjFile(objfile, 2);
    return 0;
}
}
