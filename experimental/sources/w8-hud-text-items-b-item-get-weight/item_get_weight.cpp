// Observed getWeight__4itemPQ24item11description at 0x001080d8, 160 bytes.
// The mangled name is the member item::getWeight taking a pointer to
// item::description. The second argument is never read, and it is not declared
// here. Layout read from the first argument (the item record):
//   +0   pointer to a description whose float at +8 is the unit weight
//   +4   signed halfword count
//   +36 + 8*k for k = 0..3: two halfwords, a tag and a value (4 more bytes
//        per entry are not read)
// A unit weight above 15.0 is reduced by (weight - 15.0) * 0.7692308. The
// result is multiplied by the count. The four entries are scanned in order for
// the first tag equal to 36; its value is the percentage v (0 when no entry
// matches). The return value is evaluated as (float)v * 0.01 * w + w, with w
// the multiplied weight. Only the shape is established; the meaning of tag 36
// and of the 15.0 threshold is not. Floats are single precision.
// The gap array keeps the entries at offset 36 for any pointer width.
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

static_assert(sizeof(void*) != 4 || sizeof(ItemWeightRecord) == 68, "record layout");

extern "C" float getWeight__4itemPQ24item11description(const ItemWeightRecord* item) {
    float weight = item->description->unit_weight;
    if (15.0f < weight) {
        weight = weight - (weight - 15.0f) * 0.7692308f;
    }
    weight = weight * static_cast<float>(item->count);
    int percent = 0;
    for (int k = 0; k < 4; ++k) {
        if (item->entries[k].tag == 36) {
            percent = item->entries[k].value;
            break;
        }
    }
    return static_cast<float>(percent) * 0.01f * weight + weight;
}
