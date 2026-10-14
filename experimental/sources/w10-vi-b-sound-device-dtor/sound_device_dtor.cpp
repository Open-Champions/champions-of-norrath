// Observed _$_13VISoundDevice at 0x01126290, 148 bytes. It runs the VILList
// destructor (with flag 2) on the lists at offsets 3352, 3340, 3324, 3312,
// 3296 and 3284, in that order. If bit 0 of the second argument is set it
// then tail-calls __builtin_delete with the object. It returns nothing.
extern "C" {
struct VISoundDevice;
struct VILList;

void _$_7VILList(VILList* list, int flags);
void __builtin_delete(void* pointer);

void _$_13VISoundDevice(VISoundDevice* device, int flags) {
    char* base = reinterpret_cast<char*>(device);
    _$_7VILList(reinterpret_cast<VILList*>(base + 3352), 2);
    _$_7VILList(reinterpret_cast<VILList*>(base + 3340), 2);
    _$_7VILList(reinterpret_cast<VILList*>(base + 3324), 2);
    _$_7VILList(reinterpret_cast<VILList*>(base + 3312), 2);
    _$_7VILList(reinterpret_cast<VILList*>(base + 3296), 2);
    _$_7VILList(reinterpret_cast<VILList*>(base + 3284), 2);
    if (flags & 1) __builtin_delete(device);
}
}
