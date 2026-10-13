// Observed: __6VIFile at 0x010cadf8 is 100 bytes. On the object it stores -1 at
// offset 0; zero at 4 and 8; 64-bit zeros at 16, 24, 32, 40 and 64; zero words at
// 48, 52, 56, 60 and 112; then calls BufferClear__6VIFile(this); then stores zero
// at 184 and 188. The word at 112 is cleared before the call (it sits in the call's
// delay slot), the words at 184 and 188 after it. It returns nothing.
// Unknown: what the fields are. Inferred: word 0 is a handle that is -1 while no
// file is open.
extern "C" {
void BufferClear__6VIFile(void* file);

void __6VIFile(void* file) {
    char* bytes = static_cast<char*>(file);
    *reinterpret_cast<int*>(bytes) = -1;
    *reinterpret_cast<unsigned*>(bytes + 4) = 0;
    *reinterpret_cast<unsigned*>(bytes + 8) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 16) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 24) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 32) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 40) = 0;
    *reinterpret_cast<unsigned*>(bytes + 48) = 0;
    *reinterpret_cast<unsigned*>(bytes + 52) = 0;
    *reinterpret_cast<unsigned*>(bytes + 56) = 0;
    *reinterpret_cast<unsigned*>(bytes + 60) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 64) = 0;
    *reinterpret_cast<unsigned*>(bytes + 112) = 0;
    BufferClear__6VIFile(file);
    *reinterpret_cast<unsigned*>(bytes + 184) = 0;
    *reinterpret_cast<unsigned*>(bytes + 188) = 0;
}
}
