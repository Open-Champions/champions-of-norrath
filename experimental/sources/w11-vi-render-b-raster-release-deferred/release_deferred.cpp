// Observed: ReleaseDeferred__8VIRaster at 0x01112880 is 308 bytes. It walks two lists in the
// raster: the surface list at offset 19404 and then the primitive-buffer list at 19344. For each
// list it starts at Begin and, while the position differs from End, takes Next(position) first,
// reads the element pointer stored at the position, and decides: if the raster word at 17880
// is less than (a word of the element) + 3 the element is kept; otherwise a non-null element has
// its destructor run with flag 3 and the list's Erase(position) is called, and a negative Erase
// result makes the whole function return -1. The compared element word is at offset 36 of a
// surface and at offset 16 of a primitive buffer. The result is zero when both walks finish.
// Inferred: 17880 is the counter BeginScene advances after each display buffer swap, and the
// element word is the counter value when the element was last used; elements unused for 3 swaps
// are freed.
// The element is dereferenced before the null test, as in the image; the empty asm keeps the
// compiler from removing that test.
// Unknown: the types' remaining layout. Next, End, Begin and Erase are unresolved list members.
extern "C" {
struct VIRaster;

int Begin__Ct6VIList1ZP9VISurface(void* list);
int End__Ct6VIList1ZP9VISurface(void* list);
int Next__Ct6VIList1ZP9VISurfacei(void* list, int position);
int Erase__t6VIList1ZP9VISurfacei(void* list, int position);
void _$_9VISurface(void* surface, int flags);
int Begin__Ct6VIList1ZP12VIPrimBuffer(void* list);
int End__Ct6VIList1ZP12VIPrimBuffer(void* list);
int Next__Ct6VIList1ZP12VIPrimBufferi(void* list, int position);
int Erase__t6VIList1ZP12VIPrimBufferi(void* list, int position);
void _$_12VIPrimBuffer(void* buffer, int flags);

int ReleaseDeferred__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    void* surfaces = base + 19404;
    int position = Begin__Ct6VIList1ZP9VISurface(surfaces);
    while (position != End__Ct6VIList1ZP9VISurface(surfaces)) {
        int next = Next__Ct6VIList1ZP9VISurfacei(surfaces, position);
        char* element = *reinterpret_cast<char**>(position);
        if (*reinterpret_cast<int*>(base + 17880) < *reinterpret_cast<int*>(element + 36) + 3) {
            position = next;
            continue;
        }
        __asm__("" : "+r"(element));
        if (element != 0) _$_9VISurface(element, 3);
        if (Erase__t6VIList1ZP9VISurfacei(surfaces, position) < 0) return -1;
        position = next;
    }
    void* buffers = base + 19344;
    position = Begin__Ct6VIList1ZP12VIPrimBuffer(buffers);
    while (position != End__Ct6VIList1ZP12VIPrimBuffer(buffers)) {
        int next = Next__Ct6VIList1ZP12VIPrimBufferi(buffers, position);
        char* element = *reinterpret_cast<char**>(position);
        if (*reinterpret_cast<int*>(base + 17880) < *reinterpret_cast<int*>(element + 16) + 3) {
            position = next;
            continue;
        }
        __asm__("" : "+r"(element));
        if (element != 0) _$_12VIPrimBuffer(element, 3);
        if (Erase__t6VIList1ZP12VIPrimBufferi(buffers, position) < 0) return -1;
        position = next;
    }
    return 0;
}
}
