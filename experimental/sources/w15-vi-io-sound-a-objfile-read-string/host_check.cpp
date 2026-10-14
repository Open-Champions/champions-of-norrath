// Host check only; not a build input.
static int lengthResult, bodyResult, bodyCalls;
static unsigned lastLength, lastLimit;
extern "C" int ReadStringLength__9VIObjFileRUs(unsigned char*, unsigned short* length) { *length = 0x4321; return lengthResult; }
extern "C" int ReadStringBody__9VIObjFilePcUsUs(unsigned char*, char*, unsigned short length, unsigned short limit) { ++bodyCalls; lastLength = length; lastLimit = limit; return bodyResult; }
#include "read_string.cpp"
int main() {
    unsigned char f[8] = {};
    char dest[8];
    lengthResult = -1;
    if (ReadString__9VIObjFilePcUs(f, dest, 5) != -1 || bodyCalls != 0) return 1;
    lengthResult = 0;
    bodyResult = 7;
    if (ReadString__9VIObjFilePcUs(f, dest, 0x12345) != 0 || bodyCalls != 1 || lastLength != 0x4321 || lastLimit != 0x2345) return 2;
    bodyResult = -1;
    if (ReadString__9VIObjFilePcUs(f, dest, 5) != -1 || bodyCalls != 2) return 3;
    bodyResult = 0;
    if (ReadString__9VIObjFilePcUs(f, dest, 5) != 0) return 4;
    return 0;
}
