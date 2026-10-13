#include "open_memseg.cpp"

static void* seen[4];
static int calls;
extern "C" int MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil(void* file, int mode, void* segment, long size) {
    ++calls;
    seen[0] = file; seen[1] = (void*)(long)mode; seen[2] = segment; seen[3] = (void*)size;
    return mode * 3 + 1;
}

int main() {
    char a, b;
    int r = Open__6VIFileiPQ26VIFile7MemSegmil(&a, 5, &b, 77);
    if (r != 16 || calls != 1) return 1;
    if (seen[0] != &a || seen[1] != (void*)5 || seen[2] != &b || seen[3] != (void*)77) return 2;
    r = Open__6VIFileiPQ26VIFile7MemSegmil(&b, -2, &a, -9);
    if (r != -5 || calls != 2 || seen[3] != (void*)-9L) return 3;
    return 0;
}
