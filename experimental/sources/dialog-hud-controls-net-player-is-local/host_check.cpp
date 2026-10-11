extern "C" {
int nmPartners;
unsigned char partners[0x12000];
int netPlayerIsLocal__Fi(int index);
}

static int fail(int code) { return code; }

int main() {
    nmPartners = -1;
    partners[0x44d4u] = 0x7f;
    if (netPlayerIsLocal__Fi(0) != 1) return fail(1);

    nmPartners = 5;
    if (netPlayerIsLocal__Fi(5) != 1) return fail(2);
    if (netPlayerIsLocal__Fi(6) != 1) return fail(3);

    nmPartners = 2;
    partners[0x44d4u] = 0;
    partners[0x44d4u + 17632u] = 0x80;
    partners[0x44d4u + 1u] = 0x5a;
    if (netPlayerIsLocal__Fi(0) != 0) return fail(4);
    if (netPlayerIsLocal__Fi(1) != -128) return fail(5);

    partners[0x44d4u] = 0xff;
    if (netPlayerIsLocal__Fi(0) != -1) return fail(6);
    partners[0x44d4u] = 0x7f;
    if (netPlayerIsLocal__Fi(0) != 127) return fail(7);
    if (partners[0x44d4u + 1u] != 0x5a) return fail(8);
    return 0;
}
