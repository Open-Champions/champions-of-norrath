// Observed: Find__Ct5VISet1ZP5VIWndRCP5VIWnd at 0x0120bab0 is 124 bytes. It reads the root index at
// set+12; if zero it tail-calls End__Ct5VISet1ZP5VIWnd(set) and returns that. Otherwise it reads
// the key word *key and walks the 20-byte elements from the base at set+8: if the element's key
// word (element+16) is above the key it goes to the link at element+8, if below to the link at
// element+12, and if equal returns that index. A zero link ends the walk with the End result.
// Inferred: an unsigned-key search of a binary search tree held in an array (see Begin/Next).
// Unknown: the template parameters beyond the mangled name.
extern "C" {
int End__Ct5VISet1ZP5VIWnd(const void* set);

int Find__Ct5VISet1ZP5VIWndRCP5VIWnd(const void* set, const unsigned* key) {
    const unsigned char* bytes = static_cast<const unsigned char*>(set);
    int index = *reinterpret_cast<const int*>(bytes + 12);
    if (index != 0) {
        const unsigned wanted = *key;
        const unsigned char* elements = reinterpret_cast<const unsigned char*>(
            static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
        for (;;) {
            const unsigned char* element = elements + index * 20;
            const unsigned found = *reinterpret_cast<const unsigned*>(element + 16);
            if (wanted < found) index = *reinterpret_cast<const int*>(element + 8);
            else if (found < wanted) index = *reinterpret_cast<const int*>(element + 12);
            else return index;
            if (index == 0) break;
        }
    }
    return End__Ct5VISet1ZP5VIWnd(set);
}
}
