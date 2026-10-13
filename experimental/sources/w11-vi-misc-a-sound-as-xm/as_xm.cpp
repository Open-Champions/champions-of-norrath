// Observed: AsXm__7VISound at 0x0117b7d8 is 20 bytes. It returns the object
// pointer when the word at offset 8 equals one and a null pointer otherwise.
extern "C" {
void* AsXm__7VISound(void* object) {
    return *reinterpret_cast<int*>(static_cast<char*>(object) + 8) == 1 ? object : nullptr;
}
}
