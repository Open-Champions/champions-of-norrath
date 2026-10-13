// Host check for the inventory texture words. Not an EE plan input.
#include "inventory_texture_words.cpp"
int main() {
    if (sizeof(moneyTex) != 7 * sizeof(void*)) return 1;
    for (void* p : moneyTex) if (p != nullptr) return 2;
    if (lcursor != nullptr || iconLump != nullptr || hudLump != nullptr) return 3;
    return 0;
}
