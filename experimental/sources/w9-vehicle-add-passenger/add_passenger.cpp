// Observed addPassenger__7Vehiclei at 0x00271488, 544 bytes. The argument is a
// player index; the same index selects the player record (player + 23984*i, the
// pointer at record+21872) and the word at vehicle+1004+4*i.
// Fields used: passenger count +976 (signed), seat words +960.. (-1 = free),
// seat-in-use flags +984.., occupied count +980, driver index +956 (-1 = none),
// word +1032, word +2148, position Point3 at +16 and seat positions via
// getSeatPosition.
// Argument -1: the occupied count becomes 0, the four seat words become -1 and
// 16 bytes at +1004 are cleared; nothing else happens.
// Otherwise a seat is chosen. If there is no driver (+956 == -1) and +2148 is
// zero, the seat is 0. In every other case the seat is the free seat s in
// 1..count-1 (count re-read each pass) whose getSeatPosition is nearest, in
// x and y only, to the position at record_pointer+16 with a strict less-than
// against a starting best of 100000.0; if there is none the seat is -1 and the
// stores below land at +956 and +980 (the code does not check this).
// Then: seat word = argument; +980 += 1; seat flag word (+984+4*seat) = 1.
// If there is still no driver at that point and +2148 is zero: +1032 = 1,
// +956 = argument, and the Vehicle virtual in table slot 160 (eventActivate
// position in the table, delta halfword then pointer) is called. Finally
// +1004+4*argument = frameNm, record_pointer+1400 = 16 and
// record_pointer+1328 = the vehicle. Nothing is returned. Nothing here was
// executed on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void* memset(void* destination, int value, unsigned long count);
void getSeatPosition__7VehicleiP6Point3(unsigned char* vehicle, int seat, Point3* out);
extern int frameNm;
extern unsigned char player[];
}

typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef void (*VirtualFunction)(unsigned char*);
typedef VirtualFunction AliasedFunction __attribute__((may_alias));

static inline int& word(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedInt*>(base + offset);
}

extern "C" void addPassenger__7Vehiclei(unsigned char* vehicle, int index) {
    if (index == -1) {
        word(vehicle, 980) = 0;
        for (int i = 3; i >= 0; --i) word(vehicle, 960 + i * 4) = -1;
        memset(vehicle + 1004, 0, 16);
        return;
    }
    int seat;
    if (word(vehicle, 956) == -1 && word(vehicle, 2148) == 0) {
        seat = 0;
    } else {
        float best = 100000.0f;
        seat = -1;
        const Point3* target = reinterpret_cast<const Point3*>(
            *reinterpret_cast<AliasedPointer*>(player + index * 23984 + 21872) + 16);
        for (int i = 1; i < word(vehicle, 976); ++i) {
            if (word(vehicle, 960 + i * 4) != -1) continue;
            Point3 at;
            getSeatPosition__7VehicleiP6Point3(vehicle, i, &at);
            const float dx = at.x - target->x;
            const float dy = at.y - target->y;
            const float distance = __builtin_sqrtf(dx * dx + dy * dy);
            if (distance < best) {
                best = distance;
                seat = i;
            }
        }
    }
    word(vehicle, 960 + seat * 4) = index;
    word(vehicle, 980) = word(vehicle, 980) + 1;
    word(vehicle, 984 + seat * 4) = 1;
    if (word(vehicle, 956) == -1 && word(vehicle, 2148) == 0) {
        unsigned char* entry = *reinterpret_cast<AliasedPointer*>(vehicle + 72) + 160;
        word(vehicle, 1032) = 1;
        word(vehicle, 956) = index;
        (*reinterpret_cast<AliasedFunction*>(entry + 4))(vehicle + *reinterpret_cast<AliasedShort*>(entry));
    }
    word(vehicle, 1004 + index * 4) = frameNm;
    unsigned char* record = *reinterpret_cast<AliasedPointer*>(player + index * 23984 + 21872);
    word(record, 1400) = 16;
    *reinterpret_cast<AliasedPointer*>(record + 1328) = vehicle;
}
