// Observed drawCharacterCallBack__FiP6Point3PsPiN23 at 0x001caa40, 400 bytes. Arguments:
// a0 = index, a1 = Point3 output, a2 = short output, a3/t0/t1 = three int outputs.
//  1. object = mapObjList[index] (mapObjList, 0x00484f9c, points at an array of object
//     pointers). A null object returns -1 with no output written.
//  2. The first virtual-table slot function is called: vtable = word at object+72, function
//     = word at vtable+4. The original leaves A0 holding the address of the list slot
//     (not the object) at that call; the result is compared with __tf6Player() through
//     __eq__C9type_infoRC9type_info(result, playerType).
//  3. Player type (equal): the word at object+960 selects the three int outputs
//     (first, second, third): 0 -> 182, 47, 58;  1 -> 0, 191, 0;  2 -> 180, 178, 0;
//     3 -> 29, 32, 161; any other value leaves all three untouched.
//  4. Other type: if gameShowEnemiesInMap (0x00484f54) is zero, return 0 with no output
//     written (the equality result, 0). Otherwise the bit 0x01000000 of the word at
//     object+12 gives (first, second) = (0, 64) when set, (64, 0) when clear, and the
//     third is 0.
//  5. In every non-early path the 12 bytes at object+16 are copied to the Point3 output,
//     the unsigned halfword at object+56 is stored to the short output, and 1 is returned.
// Unknown: what the outputs mean (inferred from the name only: icon/colour selectors for a
// map draw callback). Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};

struct MapObject {
    unsigned char unknown_0[12];
    unsigned flags;
    Point3 position;
    unsigned char unknown_28[28];
    unsigned short short_value;
    unsigned char unknown_58[14];
    const unsigned char* vtable;
    unsigned char unknown_76[884];
    int player_state;
};
static_assert(__builtin_offsetof(MapObject, flags) == 12, "flags offset");
static_assert(__builtin_offsetof(MapObject, position) == 16, "position offset");
static_assert(__builtin_offsetof(MapObject, short_value) == 56, "halfword offset");
static_assert(__builtin_offsetof(MapObject, vtable) == 72, "vtable offset");
static_assert(__builtin_offsetof(MapObject, player_state) == 960, "state offset");

extern "C" {
extern MapObject** mapObjList;
extern int gameShowEnemiesInMap;
void* __tf6Player();
int __eq__C9type_infoRC9type_info(void* left, void* right);

int drawCharacterCallBack__FiP6Point3PsPiN23(int index, Point3* position_out, short* short_out, int* first,
                                             int* second, int* third) {
    MapObject** slot = mapObjList + index;
    MapObject* object = *slot;
    if (object == 0) return -1;
    typedef void* (*TypeFunction)(void*);
    const TypeFunction type_function = *reinterpret_cast<const TypeFunction*>(object->vtable + 4);
    void* object_type = type_function(slot);
    void* player_type = __tf6Player();
    const int is_player = __eq__C9type_infoRC9type_info(object_type, player_type);
    const int show_enemies = gameShowEnemiesInMap;
    if (is_player) {
        switch (object->player_state) {
        case 0:
            *first = 182;
            *second = 47;
            *third = 58;
            break;
        case 1:
            *first = 0;
            *second = 191;
            *third = 0;
            break;
        case 2:
            *first = 180;
            *second = 178;
            *third = 0;
            break;
        case 3:
            *first = 29;
            *second = 32;
            *third = 161;
            break;
        default:
            break;
        }
    } else {
        if (show_enemies == 0) return is_player;
        if ((object->flags & 0x01000000u) != 0) {
            *first = 0;
            *second = 64;
        } else {
            *first = 64;
            *second = 0;
        }
        *third = 0;
    }
    *position_out = object->position;
    *short_out = static_cast<short>(object->short_value);
    return 1;
}
}
