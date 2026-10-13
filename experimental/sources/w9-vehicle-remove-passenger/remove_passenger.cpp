// Observed removePassenger__7Vehiclei at 0x002716a8, 452 bytes. The argument is
// a player index; record = the pointer at player + 23984*i + 21872.
// The seat words at vehicle+960+4*k, k below the signed count at +976 (read
// once), are scanned in order for the argument. At the first match: that seat
// word becomes -1, the occupied count (+980) drops by one, the seat flag word
// at +984+4*k becomes -1, dismount__6Player runs on the record, and record+1328
// becomes 0. If the argument is also the driver index (+956): a Point3 is
// built from the three floats at vehicle+1116 scaled by 20.0f plus record+16,
// objectFindEmptySpot (48, 48, null) is called for a spot, the result is stored
// at record+16, and +1032 becomes 0. After a match, when the argument is the
// driver index, the Vehicle virtual in table slot 168 (eventDeActivate
// position, delta halfword then pointer) runs and +956 becomes -1; then, still
// only after a match, the float at +808 becomes 30.0f. With no match nothing is
// written. Nothing is returned. Nothing here was executed on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void dismount__6Player(unsigned char* record);
void objectFindEmptySpot__FG6Point3iiPc(Point3* result, const Point3* spot, int a, int b, char* c);
extern unsigned char player[];
}

typedef int AliasedInt __attribute__((may_alias));
typedef float AliasedFloat __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef void (*VirtualFunction)(unsigned char*);
typedef VirtualFunction AliasedFunction __attribute__((may_alias));

static inline int& word(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedInt*>(base + offset);
}

extern "C" void removePassenger__7Vehiclei(unsigned char* vehicle, int index) {
    const int count = word(vehicle, 976);
    bool found = false;
    for (int k = 0; k < count; ++k) {
        if (word(vehicle, 960 + k * 4) != index) continue;
        word(vehicle, 960 + k * 4) = -1;
        unsigned char* const* slot = reinterpret_cast<unsigned char* const*>(player + index * 23984 + 21872);
        found = true;
        word(vehicle, 980) = word(vehicle, 980) - 1;
        word(vehicle, 984 + k * 4) = -1;
        dismount__6Player(*slot);
        unsigned char* record = *slot;
        *reinterpret_cast<AliasedPointer*>(record + 1328) = 0;
        if (index == word(vehicle, 956)) {
            Point3 want;
            Point3 spot;
            Point3* const position = reinterpret_cast<Point3*>(record + 16);
            want.x = *reinterpret_cast<AliasedFloat*>(vehicle + 1116) * 20.0f;
            want.y = *reinterpret_cast<AliasedFloat*>(vehicle + 1120) * 20.0f;
            want.z = *reinterpret_cast<AliasedFloat*>(vehicle + 1124) * 20.0f;
            want.x = want.x + position->x;
            want.y = want.y + position->y;
            want.z = want.z + position->z;
            objectFindEmptySpot__FG6Point3iiPc(&spot, &want, 48, 48, 0);
            *reinterpret_cast<Point3*>(*slot + 16) = spot;
            word(vehicle, 1032) = 0;
        }
        break;
    }
    if (!found) return;
    if (index == word(vehicle, 956)) {
        unsigned char* entry = *reinterpret_cast<AliasedPointer*>(vehicle + 72) + 168;
        (*reinterpret_cast<AliasedFunction*>(entry + 4))(vehicle + *reinterpret_cast<AliasedShort*>(entry));
        word(vehicle, 956) = -1;
    }
    *reinterpret_cast<AliasedFloat*>(vehicle + 808) = 30.0f;
}
