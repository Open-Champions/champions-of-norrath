// Observed getPicName__4itemPciPQ24item11description at 0x001075f0, 1164 bytes.
// Arguments: an output buffer `out`, an integer `n`, and (despite the mangled
// type) the item record, as in getWeight. Item fields: +0 description pointer,
// +8 64-bit flags, +32 byte `variant` (255 means none). Description fields: +0 name
// pointer, +14 and +16 halfwords, +28, +29 and +30 signed bytes, +48 pointer to a
// base picture name (may be zero), +52 halfword index into _4item$globalItem
// (16-byte records whose word at +4 is a name string). `_4item$prefixTable` has
// 40-byte rows indexed by `variant`; bytes +6, +7 and +8 of a row are read.
// The result is a texture file name written to `out`. First the base name B is
// built in a local buffer: "<+48 name>.tex" when the description has a +48 name,
// else the globalItem[+52] name copied as is; `out` is set to "%s" of B. Then:
//  A. Description byte +29 zero and flags bit 34 clear: a digit s1 is chosen. It is
//     1 for variant 255, else row byte +6 of the variant; 1 when description byte
//     +30 is 4; row byte +8 instead when flags bit 23 is set and the variant is not
//     255; (halfword +14 + halfword +16) % 18 (signed remainder) when the name
//     contains "+"; a zero becomes 1. B and `out` are built again as above, then
//     "%d%d.tex" of (((flags >> 11) + 1) & 0x1f) and s1 is written at
//     out + strlen(out) - 5.
//  B. Else byte +29 equal to 1: nothing more when byte +28 is 3. Otherwise `n` is
//     adjusted unless the name contains "circlet": with flags bit 19 set an odd n
//     becomes 7 and an even n becomes 6; with it clear 6 and 7 become 0 and 1 and
//     other values stay. When the name contains "circlet" and n is 6 or 7, B is
//     replaced by "cloth1helmet6%d.tex", "cloth2helmet6%d.tex" or
//     "cloth3helmet6%d.tex" (the name containing "circlet1", "circlet2" or neither)
//     with the integer scanned from the name by sscanf with "circlet1%d",
//     "circlet2%d" or "circlet3%d". `out` becomes "%s_%s" of raceArmorPrefix[n] (ten
//     short strings, bm bf wem wef hem hef em ef dem def) and B. Last, s0 is row byte
//     +7 of the variant (1 when the variant is 255), or getValue(item) % 4 (signed
//     remainder) when the name contains "+"; "%d.tex" of s0 is written at
//     out + strlen(out) - 4 (s0 is forced to 1 when the variant is 255).
//  C. Any other case: if `out` contains "1.tex", "%d.tex" of
//     (((flags >> 11) + 1) & 0x1f) is written at out + strlen(out) - 5.
// What the flag bits, variants and n mean is not established ("circlet" and the
// race prefixes come from the strings). The libc routines, getValue, the two item
// tables and raceArmorPrefix's source are not defined here.
extern "C" {
extern unsigned char _4item$prefixTable[];
extern unsigned char _4item$globalItem[];
int sprintf(char*, const char*, ...);
int sscanf(const char*, const char*, ...);
char* strcpy(char*, const char*);
char* strstr(const char*, const char*);
unsigned strlen(const char*);
}

struct PicDescription {
    const char* name;
    unsigned char gap0[14 - sizeof(void*)];
    short half14;
    short half16;
    unsigned char gap1[28 - 18];
    signed char byte28;
    signed char byte29;
    signed char byte30;
    unsigned char gap2[48 - 31];
    const char* baseName;
    short globalIndex;
};

struct PicItem {
    const PicDescription* description;
    unsigned char gap[8 - sizeof(void*)];
    long long flags;
    unsigned char gap2[32 - 16];
    unsigned char variant;
};

static_assert(sizeof(void*) != 4 || (__builtin_offsetof(PicDescription, baseName) == 48 &&
                                     __builtin_offsetof(PicDescription, globalIndex) == 52 &&
                                     __builtin_offsetof(PicItem, flags) == 8 &&
                                     __builtin_offsetof(PicItem, variant) == 32),
              "record layout");

static const char* const raceArmorPrefix[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};

extern "C" int getValue__4itemPQ24item11description(const PicItem* item);

static inline signed char rowByte(unsigned variant, int offset) {
    return static_cast<signed char>(_4item$prefixTable[variant * 40 + offset]);
}

static void buildBase(char* base, char* out, const PicDescription* d) {
    if (d->baseName != nullptr) {
        sprintf(base, "%s.tex", d->baseName);
    } else {
        strcpy(base, *reinterpret_cast<const char* const*>(_4item$globalItem + d->globalIndex * 16 + 4));
    }
    sprintf(out, "%s", base);
}

extern "C" void getPicName__4itemPciPQ24item11description(char* out, int n, const PicItem* item) {
    char base[256];
    buildBase(base, out, item->description);
    const PicDescription* d = item->description;
    long long flags = item->flags;
    if (d->byte29 == 0 && (flags & (1LL << 34)) == 0) {
        unsigned variant = item->variant;
        int digit = 1;
        if (variant != 255) digit = rowByte(variant, 6);
        if (d->byte30 == 4) digit = 1;
        if (variant != 255 && (flags & 0x800000LL) != 0) digit = rowByte(variant, 8);
        if (strstr(d->name, "+") != nullptr) digit = (d->half14 + d->half16) % 18;
        if (digit == 0) digit = 1;
        buildBase(base, out, item->description);
        int tens = static_cast<int>(((item->flags >> 11) + 1) & 0x1f);
        sprintf(out + strlen(out) - 5, "%d%d.tex", tens, digit);
        return;
    }
    if (d->byte29 != 1) {
        if (strstr(out, "1.tex") != nullptr) {
            sprintf(out + strlen(out) - 5, "%d.tex", static_cast<int>(((item->flags >> 11) + 1) & 0x1f));
        }
        return;
    }
    if (d->byte28 == 3) return;
    bool odd = (n & 1) != 0;
    bool sixOrSeven = static_cast<unsigned>(n - 6) < 2u;
    if (strstr(d->name, "circlet") == nullptr) {
        if ((item->flags & 0x80000LL) != 0) {
            n = odd ? 7 : 6;
        } else if (sixOrSeven) {
            n = n - 6;
        }
    }
    if (strstr(item->description->name, "circlet") != nullptr && sixOrSeven) {
        int number;
        if (strstr(item->description->name, "circlet1") != nullptr) {
            sscanf(item->description->name, "circlet1%d", &number);
            sprintf(base, "cloth1helmet6%d.tex", number);
        } else if (strstr(item->description->name, "circlet2") != nullptr) {
            sscanf(item->description->name, "circlet2%d", &number);
            sprintf(base, "cloth2helmet6%d.tex", number);
        } else {
            sscanf(item->description->name, "circlet3%d", &number);
            sprintf(base, "cloth3helmet6%d.tex", number);
        }
    }
    sprintf(out, "%s_%s", raceArmorPrefix[n], base);
    unsigned variant = item->variant;
    int last = variant != 255 ? rowByte(variant, 7) : 1;
    if (strstr(item->description->name, "+") != nullptr) {
        int value = getValue__4itemPQ24item11description(item);
        last = value % 4;
    }
    if (variant == 255) last = 1;
    sprintf(out + strlen(out) - 4, "%d.tex", last);
}
