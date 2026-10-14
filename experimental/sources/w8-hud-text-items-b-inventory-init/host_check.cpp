extern "C" {
alignas(4) unsigned char inventory[464];
void inventoryInit__Fv();
}

static int word(int record, int offset) {
    int v;
    unsigned char* p = inventory + record * 116 + offset;
    __builtin_memcpy(&v, p, 4);
    return v;
}

int main() {
    for (int i = 0; i < 464; ++i) inventory[i] = 0x5a;
    inventoryInit__Fv();
    for (int i = 0; i < 464; ++i) {
        int rec = i / 116, off = i % 116;
        bool set = false;
        if (rec < 2) {
            set = (off < 16) || (off >= 52 && off < 56) || (off >= 76 && off < 80);
        }
        if (!set && inventory[i] != 0) return 1;
    }
    for (int r = 0; r < 2; ++r) {
        if (word(r, 0) != 0 || word(r, 4) != 0) return 2;
        if (word(r, 8) != 320 || word(r, 12) != 448) return 3;
        if (word(r, 52) != -1 || word(r, 76) != 2) return 4;
    }
    for (int r = 2; r < 4; ++r) {
        for (int o = 0; o < 116; ++o) if (inventory[r * 116 + o] != 0) return 5;
    }
    return 0;
}

extern "C" void* memset(void* dest, int value, unsigned size) {
    unsigned char* p = static_cast<unsigned char*>(dest);
    for (unsigned i = 0; i < size; ++i) p[i] = static_cast<unsigned char>(value);
    return dest;
}
