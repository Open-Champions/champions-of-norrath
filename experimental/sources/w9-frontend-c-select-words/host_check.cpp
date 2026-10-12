#include "select_words.cpp"
int main() {
    static_assert(sizeof(newSel) == 4 && sizeof(nmActions) == 4 && sizeof(nmDialogNodes) == 4, "words");
    static_assert(sizeof(playerSelectInfo) == 4 * 41, "four 41-byte records");
    if (newSel || nmActions || nmDialogNodes) return 1;
    for (unsigned char b : playerSelectInfo) if (b != 0) return 2;
    return 0;
}
