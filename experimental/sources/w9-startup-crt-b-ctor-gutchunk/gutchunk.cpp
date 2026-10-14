// Observed __8GutChunkG6Point3iif at 0x001e5048, 188 bytes: the GutChunk
// constructor; arguments are the object, a position (12 bytes, copied to a
// local), two integers and a float. In the order of the image:
//  - PhProp's constructor (0x00149068) runs on the object with the position copy,
//    three strings read from the table fxResourceMap (0x0042ad00): the words at
//    +0 and +80, then the word at +96 when bit 1 of the second integer is set and
//    +88 when it is clear; the first integer is passed on as the next argument,
//    then the constant 2625, a null pointer, and the float constant 1.0
//    (the float argument of this constructor is not used);
//  - the second integer goes to object+584, the vtable _vt$8GutChunk
//    (0x0044ad20) to object+72, the 12 bytes at object+16 are copied to
//    object+592, object+604 becomes 2 and byte object+608 becomes 0.
// The image leaves 2 in the result register at return; that is a leftover, not
// a result, so this version returns nothing.
// Inferred: the three strings are resource names; the callee names come from
// the image's symbols. Unknown: what the two integers mean.
struct Point3 { float x, y, z; };

extern "C" {
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* first, char* second,
                                                    char* third, long a, int b, void* description, float scale);
extern char* fxResourceMap[28];
extern unsigned char _vt$8GutChunk[];

void __8GutChunkG6Point3iif(void* self, Point3 position, int first, int selector, float) {
    unsigned char* object = static_cast<unsigned char*>(self);
    char* third = fxResourceMap[(selector & 2) ? 24 : 22];
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(self, position, fxResourceMap[0], fxResourceMap[20], third,
                                                   first, 2625, nullptr, 1.0f);
    *reinterpret_cast<int*>(object + 584) = selector;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$8GutChunk));
    __builtin_memcpy(object + 592, object + 16, 12);
    *reinterpret_cast<int*>(object + 604) = 2;
    object[608] = 0;
}
}
