// Observed: __9VIWndEdit at 0x010aa7c8 is 364 bytes. With the object as this it:
//  1. calls __5VIWnd(this);
//  2. stores zero words at this+56, +60, +64, +68, stores the address of _vt$9VIWndEdit
//     (0x01208040) at this+32, and stores zero words at +328, +332, +424, +336, +340, +432, +344,
//     +348;
//  3. calls SetMaxTextLen__9VIWndEditi(this, 64), then ClearText__9VIWndEdit(this);
//  4. stores zero at +36, +40, +356, then 16 at +56 and 40 at +60, then zero at +360, +364, +352,
//     +44, +48, +52, +416, +420, +328, +332, +336, +340, then 1 at +368, zero at +344, +348, +396,
//     +400, +404, +440, and 1 at +444;
//  5. calls SetDisplayRows__9VIWndEditi(this, 2), then EnableCr__9VIWndEditi(this, 0);
//  6. stores a zero halfword at +72 and zero words at +376 and +380.
// Nothing is returned. The original builds the (16, 40) pair and three (0, 0) pairs in stack
// temporaries before copying them into +56, +328, +336 and +344; only the copied values are
// reproduced here.
// Inferred: +56 holds an integer pair (16, 40) and +328, +336, +344 hold zeroed integer pairs;
// +424 and +432 are owned array blocks (the destructor frees them). Unknown: what any field means,
// and whether the setters read fields stored before or after them in a way that matters beyond
// the order kept here.
extern "C" {
void __5VIWnd(void* self);
void SetMaxTextLen__9VIWndEditi(void* self, int length);
void ClearText__9VIWndEdit(void* self);
void SetDisplayRows__9VIWndEditi(void* self, int rows);
void EnableCr__9VIWndEditi(void* self, int enable);
extern unsigned char _vt$9VIWndEdit[];

static inline void put(unsigned char* bytes, int offset, unsigned value) {
    *reinterpret_cast<unsigned*>(bytes + offset) = value;
}

void __9VIWndEdit(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    put(bytes, 56, 0);
    put(bytes, 60, 0);
    put(bytes, 64, 0);
    put(bytes, 68, 0);
    put(bytes, 32, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEdit)));
    put(bytes, 328, 0);
    put(bytes, 332, 0);
    put(bytes, 424, 0);
    put(bytes, 336, 0);
    put(bytes, 340, 0);
    put(bytes, 432, 0);
    put(bytes, 344, 0);
    put(bytes, 348, 0);
    SetMaxTextLen__9VIWndEditi(self, 64);
    ClearText__9VIWndEdit(self);
    put(bytes, 36, 0);
    put(bytes, 40, 0);
    put(bytes, 356, 0);
    put(bytes, 56, 16);
    put(bytes, 60, 40);
    put(bytes, 360, 0);
    put(bytes, 364, 0);
    put(bytes, 352, 0);
    put(bytes, 44, 0);
    put(bytes, 48, 0);
    put(bytes, 52, 0);
    put(bytes, 416, 0);
    put(bytes, 420, 0);
    put(bytes, 328, 0);
    put(bytes, 332, 0);
    put(bytes, 336, 0);
    put(bytes, 340, 0);
    put(bytes, 368, 1);
    put(bytes, 344, 0);
    put(bytes, 348, 0);
    put(bytes, 396, 0);
    put(bytes, 400, 0);
    put(bytes, 404, 0);
    put(bytes, 440, 0);
    put(bytes, 444, 1);
    SetDisplayRows__9VIWndEditi(self, 2);
    EnableCr__9VIWndEditi(self, 0);
    *reinterpret_cast<unsigned short*>(bytes + 72) = 0;
    put(bytes, 376, 0);
    put(bytes, 380, 0);
}
}
