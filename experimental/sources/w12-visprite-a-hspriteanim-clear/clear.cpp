// Observed: Clear__13VIHSpriteAnim at 0x0105d008, 116 bytes. With the object as
// this: when the word at this + 52 is non-zero, __builtin_vec_delete is called
// with it (one argument) and the word is set to zero. Then, unconditionally,
// zero is stored at this + 8, 12, 16, 32, 36, 40, 44 and 48, the float 10.0
// at this + 24 and the float 1.0 at this + 28. The result is zero.
// Inferred: the word at 52 is an array allocated with the vector allocator;
// 24 and 28 hold a default rate/scale pair. Unknown: the member names.
extern "C" {
void __builtin_vec_delete(void* pointer);

int Clear__13VIHSpriteAnim(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    void* array = *reinterpret_cast<void**>(bytes + 52);
    if (array) {
        __builtin_vec_delete(array);
        *reinterpret_cast<void**>(bytes + 52) = 0;
    }
    *reinterpret_cast<int*>(bytes + 48) = 0;
    *reinterpret_cast<float*>(bytes + 24) = 10.0f;
    *reinterpret_cast<float*>(bytes + 28) = 1.0f;
    *reinterpret_cast<int*>(bytes + 8) = 0;
    *reinterpret_cast<int*>(bytes + 12) = 0;
    *reinterpret_cast<int*>(bytes + 16) = 0;
    *reinterpret_cast<int*>(bytes + 32) = 0;
    *reinterpret_cast<int*>(bytes + 36) = 0;
    *reinterpret_cast<int*>(bytes + 40) = 0;
    *reinterpret_cast<int*>(bytes + 44) = 0;
    return 0;
}
}
