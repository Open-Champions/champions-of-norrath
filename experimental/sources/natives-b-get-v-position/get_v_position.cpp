// Observed amxGetVPosition at 0x0013ad90, 84 bytes. It finds the
// object named by the first AMX cell. A missing object returns 0.
// Otherwise it returns the float at object+2316 multiplied by 128
// and rounded to an integer, ties to even.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void* objectFindObject__FPc(char* name);

static int round_nearest_even(float value) {
    const int negative = value < 0.f;
    const float magnitude = negative ? -value : value;
    const int whole = static_cast<int>(magnitude);
    const float fraction = magnitude - static_cast<float>(whole);
    int rounded = whole;
    if (fraction > 0.5f || (fraction == 0.5f && (whole & 1) != 0))
        rounded = whole + 1;
    return negative ? -rounded : rounded;
}

extern "C" int amxGetVPosition__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    unsigned char* object = static_cast<unsigned char*>(objectFindObject__FPc(text));
    if (object == nullptr) return 0;
    union Bits { unsigned word; float value; } bits;
    bits.word = static_cast<unsigned>(object[2316])
        | (static_cast<unsigned>(object[2317]) << 8)
        | (static_cast<unsigned>(object[2318]) << 16)
        | (static_cast<unsigned>(object[2319]) << 24);
    return round_nearest_even(bits.value * 128.f);
}
