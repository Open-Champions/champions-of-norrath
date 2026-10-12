#include "card_buffers.cpp"
int main() {
    static_assert(sizeof(quickLoadStamp) == 64 && sizeof(ramSlot) == 731592, "image sizes");
    if (reinterpret_cast<__UINTPTR_TYPE__>(ramSlot) % 64 || reinterpret_cast<__UINTPTR_TYPE__>(quickLoadStamp) % 64) return 1;
    for (unsigned char b : quickLoadStamp) if (b) return 2;
    return ramSlot[0] || ramSlot[731591] ? 3 : 0;
}
