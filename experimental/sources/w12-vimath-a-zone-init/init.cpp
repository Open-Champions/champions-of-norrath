// Observed: Init__6VIZone at 0x01115318 is 172 bytes. It sets bit 0 of the word at +0,
// then calls, in order and stopping with -1 at the first negative result:
//   Init__10VIZoneTreeii(this + 68, 0, 1);
//   Init__t7VIArray1Z10VIZoneRoomi(this + 92, 1);
// After the second call it reads the pointer at +80, stores zero into that record's
// words at +4 and +0 (in that order), reads the pointer at +96 and calls
//   Init__10VIZoneRoomiiUiUiR6VIBBox(pointer at +96, 0, 0, 0, 0, this + 44);
// A negative result gives -1. Otherwise it re-reads the pointer at +96, sets bit 0 of the
// word at its offset 0 and returns 0. The +80 and +96 pointers are not checked for null.
// Inferred: this+68 is a VIZoneTree, this+92 is a VIArray<VIZoneRoom> whose data pointer
// is at +96 (array base + 4), the record at +80 is a tree-owned structure with two
// counters, and this+44 is a VIBBox. Bit 0 looks like an "initialized" flag on both the
// zone and its first room. Unknown: names of the flag and of the members.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Init__10VIZoneTreeii(void* tree, int a, int b);
int Init__t7VIArray1Z10VIZoneRoomi(void* array, int count);
int Init__10VIZoneRoomiiUiUiR6VIBBox(void* room, int a, int b, unsigned c, unsigned d, void* box);

int Init__6VIZone(unsigned char* self) {
    *reinterpret_cast<unsigned*>(self) |= 1;
    if (Init__10VIZoneTreeii(self + 68, 0, 1) < 0) return -1;
    if (Init__t7VIArray1Z10VIZoneRoomi(self + 92, 1) < 0) return -1;
    unsigned* record = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 80)));
    unsigned char* rooms = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 96));
    record[1] = 0;
    record[0] = 0;
    if (Init__10VIZoneRoomiiUiUiR6VIBBox(rooms, 0, 0, 0, 0, self + 44) < 0) return -1;
    unsigned* first_room = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 96)));
    *first_room |= 1;
    return 0;
}
}
