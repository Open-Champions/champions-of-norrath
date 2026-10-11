#include <cstring>
extern "C" unsigned char* _4item$picLumpHeader;
extern "C" int _4item$nmPicGroups;
extern "C" unsigned char* _fbss = 0;
extern "C" void picInit__4itemv();

static int lookup_size;
static unsigned char storage[4096];
static int lookup_calls;
static int alloc_bytes;
static int load_bytes;
static int load_flag;
static int lookup_flag;
static int* lookup_first;
static void* load_dest;
static const char* load_name;
static const char* alloc_label;

extern "C" void cdDirectoryLookup__FPcPiT1i(char* name, int* first, int* second, int flag) {
    ++lookup_calls;
    lookup_first = first;
    lookup_flag = flag;
    if (std::strcmp(name, "piccache.hdr") != 0) lookup_flag = 99;
    if (second != 0) *second = lookup_size;
}
extern "C" void* memAlloc__FiPc(int bytes, char* label) {
    alloc_bytes = bytes;
    alloc_label = label;
    return storage;
}
extern "C" void cdLoad__FPcPvii(char* name, void* dest, int bytes, int flag) {
    load_name = name;
    load_dest = dest;
    load_bytes = bytes;
    load_flag = flag;
    unsigned* words = static_cast<unsigned*>(dest);
    words[0] = lookup_size == 0 ? 0u : 2u;
    words[1] = 16;
    words[2] = 32;
    words[3] = 99;
    words[4] = 40;
    words[5] = 48;
    words[6] = 77;
}

static int expect_relocated(unsigned word, unsigned addend) {
    unsigned base = static_cast<unsigned>(reinterpret_cast<unsigned long>(storage));
    return word == addend + base;
}

int main() {
    lookup_size = 1000;
    picInit__4itemv();
    if (lookup_calls != 1 || lookup_first != 0 || lookup_flag != 0) return 1;
    if (alloc_bytes != 2048) return 2;
    if (alloc_label == 0 || std::strcmp(alloc_label, "picCache header") != 0) return 3;
    if (load_dest != storage || load_bytes != 2048 || load_flag != 0) return 4;
    if (load_name == 0 || std::strcmp(load_name, "piccache.hdr") != 0) return 5;
    if (_4item$picLumpHeader != storage) return 6;
    if (_fbss != storage + 4) return 7;
    if (_4item$nmPicGroups != 2) return 8;
    unsigned* words = reinterpret_cast<unsigned*>(storage);
    if (!expect_relocated(words[1], 16) || !expect_relocated(words[2], 32)) return 9;
    if (words[3] != 99) return 10;
    if (!expect_relocated(words[4], 40) || !expect_relocated(words[5], 48)) return 11;
    if (words[6] != 77) return 12;

    lookup_size = 0;
    picInit__4itemv();
    if (alloc_bytes != 0 || _4item$nmPicGroups != 0) return 13;
    if (_fbss != storage + 4) return 14;
    if (words[1] != 16 || words[3] != 99) return 15;
    return 0;
}
