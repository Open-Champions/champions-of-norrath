// Observed __11ResourceMapPcT1 at 0x00166f30, 20 bytes.
// Word 0 becomes -1. Word 1 becomes the first character pointer. Word 2
// becomes the second character pointer. Word 3 is not written.
extern "C" void __11ResourceMapPcT1(void* map, char* group, char* name) {
    auto words = static_cast<unsigned*>(map);
    words[0] = 0xffffffffu;
    words[1] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(group));
    words[2] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(name));
}
