// Observed: __5VIWnd at 0x010c3350 is 64 bytes. With the object as this it stores the address of
// _vt$5VIWnd (0x0120ab60) at this+32, calls __t5VISet1ZP5VIWnd(this), then stores 0 at
// this+20, 0 at this+24 and 1 at this+28. Nothing is returned.
// Inferred: a set of child window pointers is embedded at offset 0 (the destructor and the
// child routines pass this as the set). Offsets 20 and 24 are the parent word and the
// application word (SetAppPtr and AddChild write them in other routines).
// Unknown: the size of the class, and what the word at +28 (initial value 1) means.
extern "C" {
void __t5VISet1ZP5VIWnd(void* self);
extern unsigned char _vt$5VIWnd[];

void __5VIWnd(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$5VIWnd));
    __t5VISet1ZP5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 20) = 0;
    *reinterpret_cast<unsigned*>(bytes + 24) = 0;
    *reinterpret_cast<unsigned*>(bytes + 28) = 1;
}
}
