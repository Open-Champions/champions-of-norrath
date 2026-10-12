struct ItemWeightEntry {
    short tag;
    short value;
    unsigned char unread[4];
};
struct ItemWeightDescription {
    unsigned char before[8];
    float unit_weight;
};
struct ItemWeightRecord {
    const ItemWeightDescription* description;
    short count;
    unsigned char gap[36 - sizeof(void*) - sizeof(short)];
    ItemWeightEntry entries[4];
};

extern "C" float getWeight__4itemPQ24item11description(const ItemWeightRecord* item);

static ItemWeightDescription description;
static ItemWeightRecord item;

static void build(float unit, short count) {
    description = ItemWeightDescription{};
    description.unit_weight = unit;
    item = ItemWeightRecord{};
    item.description = &description;
    item.count = count;
}

static bool near(float a, float b) {
    float d = a - b;
    return d < 0.0005f && d > -0.0005f;
}

static float weight() { return getWeight__4itemPQ24item11description(&item); }

int main() {
    build(10.0f, 3);
    if (weight() != 30.0f) return 1;                 // no tag 36
    item.entries[2].tag = 36;
    item.entries[2].value = 50;
    if (!near(weight(), 45.0f)) return 2;
    build(25.0f, 2);
    if (!near(weight(), 34.615384f)) return 3;       // (25 - 10 * 0.7692308) * 2
    item.entries[3].tag = 36;
    item.entries[3].value = -50;
    if (!near(weight(), 17.307692f)) return 4;
    build(15.0f, 1);                                 // exactly 15 is not reduced
    if (weight() != 15.0f) return 5;
    build(40.0f, 1);
    item.entries[1].tag = 35;
    item.entries[1].value = 100;
    item.entries[0].tag = 36;                        // first match wins
    item.entries[0].value = 100;
    item.entries[2].tag = 36;
    item.entries[2].value = 7;
    if (!near(weight(), 2.0f * (40.0f - 25.0f * 0.7692308f))) return 6;
    build(4.0f, 0);
    item.entries[0].tag = 36;
    item.entries[0].value = 10;
    if (weight() != 0.0f) return 7;
    return 0;
}
