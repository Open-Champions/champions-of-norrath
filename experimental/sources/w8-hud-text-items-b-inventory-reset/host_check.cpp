extern "C" {
int nmPlayers;
alignas(4) unsigned char inventory[464];
void inventoryReset__Fv();
}

static void fill() {
    for (int i = 0; i < 464; ++i) inventory[i] = 0xa5;
}

static int zero_word(int record, int offset) {
    const unsigned char* p = inventory + record * 116 + offset;
    return p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 0;
}

int main() {
    fill();
    nmPlayers = 2;
    inventoryReset__Fv();
    for (int i = 0; i < 464; ++i) {
        int rec = i / 116, off = i % 116;
        bool cleared = rec < 2 && ((off >= 24 && off < 28) || (off >= 36 && off < 40));
        if (cleared != (inventory[i] == 0)) return 1;
        if (!cleared && inventory[i] != 0xa5) return 2;
    }
    fill();
    nmPlayers = 4;
    inventoryReset__Fv();
    for (int r = 0; r < 4; ++r) {
        if (!zero_word(r, 24) || !zero_word(r, 36)) return 3;
    }
    fill();
    nmPlayers = 0;
    inventoryReset__Fv();
    for (int i = 0; i < 464; ++i) if (inventory[i] != 0xa5) return 4;
    nmPlayers = -1;
    inventoryReset__Fv();
    for (int i = 0; i < 464; ++i) if (inventory[i] != 0xa5) return 5;
    return 0;
}
