// Host contract for objectSaveAddToStringPool__FPCc. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" { char* poolStart; char* poolEnd; }
extern "C" int objectSaveAddToStringPool__FPCc(const char*);

static int failures;
static void expect(bool ok, const char* message) { if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; } }

int main() {
    static char storage[256];
    std::memset(storage, 0x7e, sizeof storage);
    poolStart = poolEnd = nullptr;
    expect(objectSaveAddToStringPool__FPCc("abc") == 0, "no pool gives zero");
    poolStart = poolEnd = storage;
    expect(objectSaveAddToStringPool__FPCc(nullptr) == 0 && poolEnd == storage, "null text gives zero and changes nothing");

    expect(objectSaveAddToStringPool__FPCc("abc") == 1, "first string's characters start at offset 1");
    expect(storage[0] == 3 && std::strcmp(storage + 1, "abc") == 0 && poolEnd == storage + 5, "entry is length, text, NUL");
    expect(objectSaveAddToStringPool__FPCc("abc") == 1 && poolEnd == storage + 5, "repeat finds the entry without growing");
    expect(objectSaveAddToStringPool__FPCc("abd") == 6, "same first character, different text appends");
    expect(poolEnd == storage + 10 && storage[5] == 3, "second entry follows the first");
    expect(objectSaveAddToStringPool__FPCc("ab") == 11 && poolEnd == storage + 14, "a prefix is a different string");
    expect(objectSaveAddToStringPool__FPCc("abd") == 6, "later entries are found");
    expect(objectSaveAddToStringPool__FPCc("") == 15 && storage[14] == 0 && storage[15] == 0 && poolEnd == storage + 16,
           "empty string occupies two bytes");
    expect(objectSaveAddToStringPool__FPCc("") == 15 && poolEnd == storage + 16, "empty string is found again");
    expect(objectSaveAddToStringPool__FPCc("\xe9t\xe9") == 17 && poolEnd == storage + 21, "high-bit characters compare as signed bytes");
    expect(objectSaveAddToStringPool__FPCc("\xe9t\xe9") == 17 && poolEnd == storage + 21, "and are found again");
    expect(storage[21] == 0x7e, "nothing is written past the new end");

    // The length byte keeps only the low 8 bits for long text.
    static char big[1024];
    poolStart = poolEnd = big;
    char text[301];
    std::memset(text, 'x', 300);
    text[300] = 0;
    expect(objectSaveAddToStringPool__FPCc(text) == 1 && static_cast<unsigned char>(big[0]) == (300 & 0xff) && poolEnd == big + 302,
           "length byte is truncated while poolEnd advances by strlen + 2");
    if (failures) return 1;
    std::printf("objectSaveAddToStringPool contract passed\n");
    return 0;
}
