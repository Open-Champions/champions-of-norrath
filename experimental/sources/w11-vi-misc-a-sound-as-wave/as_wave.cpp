// Observed: AsWave__7VISound at 0x0117b7c8 is 16 bytes. It returns the object
// pointer when the word at offset 8 is zero and a null pointer otherwise.
extern "C" {
void* AsWave__7VISound(void* object) {
    return *reinterpret_cast<int*>(static_cast<char*>(object) + 8) == 0 ? object : nullptr;
}
}
