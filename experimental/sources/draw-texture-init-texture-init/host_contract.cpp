#include <cstdio>

extern "C" unsigned textureBufferStartAddress;
extern "C" unsigned textureBufferSize;
extern "C" int textureMaxBlockSize;
extern "C" void textureInit__Fii(int address, int size);

static unsigned seen_address;
static unsigned seen_size;
static int calls;
static unsigned replacement;

extern "C" void textureInternalInit__Fv() {
    ++calls;
    seen_address = textureBufferStartAddress;
    seen_size = textureBufferSize;
    if (replacement != 0xffffffffu) textureBufferSize = replacement;
}

int main() {
    replacement = 0xffffffffu;
    calls = 0;
    textureBufferStartAddress = 0;
    textureBufferSize = 0;
    textureMaxBlockSize = 0;
    textureInit__Fii(0x00359000, 0x000a7000);
    if (calls != 1) return 1;
    if (seen_address != 0x3590u || seen_size != 0xa70u) return 2;
    if (textureBufferStartAddress != 0x3590u || textureBufferSize != 0xa70u) return 3;
    if (textureMaxBlockSize != 0xa6f) return 4;

    calls = 0;
    textureInit__Fii(0x101, 0x200);
    if (calls != 1 || seen_address != 2u || seen_size != 1u) return 5;
    if (textureMaxBlockSize != 0) return 6;

    calls = 0;
    replacement = 10u;
    textureInit__Fii(0x101, 0x200);
    if (seen_size != 1u || textureBufferSize != 10u || textureMaxBlockSize != 9) return 7;

    calls = 0;
    replacement = 0xffffffffu;
    textureInit__Fii(0, 0);
    if (seen_address != 0u || seen_size != 0u || textureMaxBlockSize != -1) return 8;

    calls = 0;
    textureInit__Fii(-200, -50);
    if (calls != 1 || seen_address != 0u || seen_size != 0xffffffffu) return 9;
    if (textureMaxBlockSize != -2) return 10;

    textureInit__Fii(-1, 255);
    if (textureBufferStartAddress != 0u || textureBufferSize != 0u || textureMaxBlockSize != -1) return 11;

    std::puts("textureInit contract passed");
    return 0;
}
