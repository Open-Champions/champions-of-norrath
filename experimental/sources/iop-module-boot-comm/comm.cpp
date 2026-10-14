// Observed commSystemInit__Fv at 0x002ccec8, 164 bytes. It stores 1 in
// enableMallocWrapper at 0x00483b11 before testing audinitted. When
// audinitted at 0x0048661c is zero it calls lgAudInit(0, 0) and stores 1
// there. It always calls lgCodecInit, then lgCodecEnumerate from index 0
// until a zero result. A record whose name pointer at offset 4 compares
// equal to GSM keeps the word at offset 0. The last match is passed to
// lgCodecOpen with codec at 0x0048664c. No match passes -1. It then stores
// 0 in enableMallocWrapper. audinitted and codec are four zero .sdata bytes.
// GSM is the four bytes at 0x00486620 and has no symbol. The host check
// resolves the name through norrath_comm_name because a host pointer does
// not fit in the four-byte slot.
extern "C" unsigned char enableMallocWrapper;
extern "C" {
int audinitted = 0;
int codec = 0;
}
extern "C" void lgAudInit(int left, int right);
extern "C" void lgCodecInit();
extern "C" void* lgCodecEnumerate(int index);
extern "C" int strcmp(const char* left, const char* right);
extern "C" void lgCodecOpen(int identifier, int* codec_word);
#if defined(NORRATH_HOST_CONTRACT)
extern "C" const char* norrath_comm_name(const void* record);
#endif

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static const char* record_name(const unsigned char* record) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_comm_name(record);
#else
    const unsigned bits = load_word(record + 4);
    return reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}

extern "C" void commSystemInit__Fv() {
    enableMallocWrapper = 1;
    if (audinitted == 0) {
        lgAudInit(0, 0);
        audinitted = 1;
    }
    lgCodecInit();
    int chosen = -1;
    int index = 0;
    for (;;) {
        auto* record = static_cast<unsigned char*>(lgCodecEnumerate(index));
        index += 1;
        if (record == nullptr)
            break;
        if (strcmp(record_name(record), "GSM") == 0)
            chosen = static_cast<int>(load_word(record));
    }
    lgCodecOpen(chosen, &codec);
    enableMallocWrapper = 0;
}
