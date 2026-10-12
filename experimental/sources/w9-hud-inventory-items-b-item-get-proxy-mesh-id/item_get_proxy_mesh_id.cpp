// Observed getProxyMeshId__4itemPQ24item11description at 0x00108498, 72 bytes.
// Despite the mangled parameter type (never read), the argument is the item
// record, as in getWeight. Layout read:
//   +0  pointer to a description whose signed halfword at +52 is a base mesh id
//   +8  64-bit flags word
// When bit 36 of the flags is set, the result is the base id plus the 5-bit
// field at bits 11..15; otherwise it is the base id. The meaning of bit 36 and
// of the field is not established ("proxy" comes from the symbol only).
struct ItemProxyDescription {
    unsigned char before[52];
    short base_mesh_id;
};

struct ItemProxyRecord {
    const ItemProxyDescription* description;
    unsigned char gap[8 - sizeof(void*)];
    long long flags;
};

static_assert(sizeof(void*) != 4 || sizeof(ItemProxyRecord) == 16, "record layout");

extern "C" int getProxyMeshId__4itemPQ24item11description(const ItemProxyRecord* item) {
    long long flags = item->flags;
    int variant = static_cast<int>(flags >> 11) & 0x1f;
    if (flags & (1LL << 36)) {
        return item->description->base_mesh_id + variant;
    }
    return item->description->base_mesh_id;
}
