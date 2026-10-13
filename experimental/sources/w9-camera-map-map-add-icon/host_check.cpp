// Host contract for addIcon__3mapP6Point3iPciT3N33. Not an EE plan input.
#include "add_icon.cpp"

#include <cstdio>
#include <cstring>
#include <string>

int _3map$iconListCount;
unsigned char _3map$iconList[28 * 8];

static int lump_calls;
static std::string seen_lump, seen_name;
extern "C" unsigned lumpFindResource__FPcT0(const char* lump, const char* name) {
    ++lump_calls;
    seen_lump = lump;
    seen_name = name;
    return 0xA0000000u + (unsigned)lump_calls;
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s (name %s)\n", message, seen_name.c_str());
        ++failures;
    }
}

static float getf(int rec, int off) { float v; std::memcpy(&v, _3map$iconList + 28 * rec + off, 4); return v; }
static unsigned getu(int rec, int off) { unsigned v; std::memcpy(&v, _3map$iconList + 28 * rec + off, 4); return v; }

int main() {
    const float point[3] = {10.5f, -20.25f, 99.0f};
    // Direction names rotate by type / 16384.
    const char* dirs[4] = {"flatup", "flatrght", "flatdown", "flatleft"};
    const char* texs[4] = {"flatup.tex", "flatrght.tex", "flatdown.tex", "flatleft.tex"};
    int count = 0;
    for (int k = 0; k < 4; ++k) {
        for (int quarter = -3; quarter <= 3; ++quarter) {
            for (int extra : {0, 5000, -5000, 16383, -16383}) {
                const int type = quarter * 16384 + (quarter < 0 ? -(extra < 0 ? -extra : extra) : (extra < 0 ? -extra : extra));
                if (type / 16384 != quarter) continue;
                std::memset(_3map$iconList, 0xcc, sizeof _3map$iconList);
                _3map$iconListCount = 2;
                lump_calls = 0;
                char name[16];
                std::strcpy(name, dirs[k]);
                addIcon__3mapP6Point3iPciT3N33(point, 7, name, type, nullptr, nullptr, 0x111u, 0x222u);
                int r = (k - quarter + 4) % 4;
                expect(r >= 0, "test stays in the defined range");
                expect(seen_name == texs[r], "rotated texture name");
                expect(seen_lump == "mapicons.lmp", "lump name");
                expect(_3map$iconListCount == 3, "count advances");
                expect(getf(2, 0) == 10.5f && getf(2, 4) == -20.25f, "position without offset");
                expect(getu(2, 8) == 0xA0000001u, "lump result stored");
                expect(getu(2, 12) == 7, "kind stored");
                expect(getf(2, 16) == 1.0f, "default scale");
                expect(getu(2, 20) == 0x111u && getu(2, 24) == 0x222u, "extras stored");
                expect(getu(1, 20) == 0xcccccccc && getu(3, 0) == 0xcccccccc, "other records untouched");
                ++count;
            }
        }
    }
    expect(count > 100, "enough cases ran");
    // Non-flat names go through "%s.tex".
    {
        _3map$iconListCount = 0;
        char name[] = "flame";
        addIcon__3mapP6Point3iPciT3N33(point, 1, name, 0, nullptr, nullptr, 0, 0);
        expect(seen_name == "flame.tex", "plain name");
        char name2[] = "flat";
        addIcon__3mapP6Point3iPciT3N33(point, 1, name2, 0, nullptr, nullptr, 0, 0);
        expect(seen_name == "flatleft.tex", "unknown flat name has k -1 and rotates to flatleft");
    }
    // Offset text and flag text.
    {
        _3map$iconListCount = 0;
        char name[] = "torch";
        char off[] = "2.5,-4";
        char flag[] = "1";
        addIcon__3mapP6Point3iPciT3N33(point, 3, name, 0, off, flag, 9, 8);
        expect(getf(0, 0) == 10.5f + 2.5f && getf(0, 4) == -20.25f + -4.0f, "offset added");
        expect(getf(0, 16) == 0.65f, "flag nonzero gives 0.65");
        char flag0[] = "0";
        addIcon__3mapP6Point3iPciT3N33(point, 3, name, 0, nullptr, flag0, 9, 8);
        expect(getf(1, 16) == 1.0f && getf(1, 0) == 10.5f, "flag zero gives 1.0, no offset");
        char flagn[] = "-7";
        addIcon__3mapP6Point3iPciT3N33(point, 3, name, 0, nullptr, flagn, 9, 8);
        expect(getf(2, 16) == 0.65f, "any nonzero flag");
        expect(_3map$iconListCount == 3, "three records");
    }
    // Null name: nothing at all.
    {
        _3map$iconListCount = 4;
        lump_calls = 0;
        addIcon__3mapP6Point3iPciT3N33(point, 3, nullptr, 0, nullptr, nullptr, 1, 2);
        expect(_3map$iconListCount == 4 && lump_calls == 0, "null name ignored");
    }
    return failures ? 1 : 0;
}
