// Observed inventoryCheckWieldingStatus__FiPQ24item11description at 0x001b0c18,
// 1228 bytes. Arguments: a player index and an item record (the mangled second
// parameter type is the item description pointer, but the argument is the item
// record, as in getWeight). Returns a small status code; the codes are described
// below by the check that produces them. The names of the codes are not
// established ("wielding status" comes from the symbol only).
// Player record (23984 bytes apart at `player`, 0x00408600), fields read:
//   +80 word (called `mode` here), +88 signed byte, +144, +148, +152, +156 words.
// Item record: +0 description pointer, +8 64-bit flags, +36 + 8*k for k = 0..3 a
// tag halfword and a value halfword. A "value of tag T" is the value of the first
// entry whose tag is T, or 0 when none matches. Description fields read: +0 name
// pointer, +29 and +30 signed bytes (`kind`, `sub`), +32 word (`shape`).
// Step 1 sets the code from the item kind and the player's mode (default 0):
//   kind == 1: if flag bit 19 is set the code is (mode != 4); otherwise it is 1
//     when the name does not contain "circlet" (strstr) and mode == 4, else 0.
//   kind != 1, mode == 1: code = (sub is 3 or 4); then 1 if shape is 8 or 4.
//   kind != 1, mode == 3: code = (sub is 1, 2 or 3).
//   kind != 1, mode == 4: code = (sub is 1, 2 or 4); then 1 if shape is 8 or 4;
//     then 0 if flag bit 36 is set.
//   anything else: 0.
// Step 2 overrides the code, each test replacing the earlier result:
//   2 if the player byte +88 is below the value of tag 14;
//   3 if word +144 is below the value of tag 15; 4 if +148 < tag 16; 5 if +152 <
//   tag 17; 6 if +156 < tag 18;
//   7 if playerGetSkillRank__Fii(player, 60) is below the value of tag 39;
//   8 if playerGetSkillRank__Fii(player, 61) is below the value of tag 40;
//   1 if the value of tag 19 is nonzero and mode != that value - 1.
// What the tags, kinds, modes and code numbers mean is not established. The
// callees and objects are not defined here.
extern "C" {
extern unsigned char player[];
int playerGetSkillRank__Fii(int, int);
char* strstr(const char*, const char*);
}

struct ItemWieldDescription {
    const char* name;
    unsigned char gap[29 - sizeof(void*)];
    signed char kind;
    signed char sub;
    unsigned char gap2[1];
    int shape;
};

struct ItemWieldEntry {
    short tag;
    short value;
    unsigned char unread[4];
};

struct ItemWieldRecord {
    const ItemWieldDescription* description;
    unsigned char gap[8 - sizeof(void*)];
    long long flags;
    unsigned char gap2[20];
    ItemWieldEntry entries[4];
};

static_assert(sizeof(void*) != 4 || sizeof(ItemWieldDescription) == 36, "description layout");
static_assert(__builtin_offsetof(ItemWieldRecord, entries) == 36, "record layout");

static int tagValue(const ItemWieldRecord* item, short tag) {
    for (int i = 0; i < 4; ++i) {
        if (item->entries[i].tag == tag) return item->entries[i].value;
    }
    return 0;
}

extern "C" int inventoryCheckWieldingStatus__FiPQ24item11description(int playerIndex, const ItemWieldRecord* item) {
    const unsigned char* record = player + playerIndex * 23984;
    const ItemWieldDescription* description = item->description;
    int mode = *reinterpret_cast<const int*>(record + 80);
    int status = 0;
    if (description->kind == 1) {
        if (item->flags & 0x80000LL) {
            status = mode != 4;
        } else if (strstr(description->name, "circlet") == nullptr && mode == 4) {
            status = 1;
        }
    } else if (mode == 1) {
        status = description->sub == 3 || description->sub == 4;
        if (description->shape == 8 || description->shape == 4) status = 1;
    } else if (mode == 3) {
        status = description->sub == 1 || description->sub == 2 || description->sub == 3;
    } else if (mode == 4) {
        status = description->sub == 1 || description->sub == 2 || description->sub == 4;
        if (description->shape == 8 || description->shape == 4) status = 1;
        if (item->flags & (1LL << 36)) status = 0;
    }
    if (static_cast<signed char>(record[88]) < tagValue(item, 14)) status = 2;
    if (*reinterpret_cast<const int*>(record + 144) < tagValue(item, 15)) status = 3;
    if (*reinterpret_cast<const int*>(record + 148) < tagValue(item, 16)) status = 4;
    if (*reinterpret_cast<const int*>(record + 152) < tagValue(item, 17)) status = 5;
    if (*reinterpret_cast<const int*>(record + 156) < tagValue(item, 18)) status = 6;
    if (playerGetSkillRank__Fii(playerIndex, 60) < tagValue(item, 39)) status = 7;
    if (playerGetSkillRank__Fii(playerIndex, 61) < tagValue(item, 40)) status = 8;
    int required = tagValue(item, 19);
    if (required != 0 && mode != required - 1) status = 1;
    return status;
}
