// Observed playerRecomputeWeight__Fi at 0x001a5358, 292 bytes (player index).
// The player record is player + index * 23984. The float at record+308 is set
// to 0, then three item lists are walked. List b (0..2) has a count word at
// record + 4936 + b * 4616 and its items start at record + 328 + b * 4616, 72
// bytes each. For every item, the first word is a pointer; if the signed byte
// at pointer+28 equals 8 and playerGetFeatRank__Fix(index, 0x10000) is nonzero
// the item's weight is scaled by 0.5, otherwise by 1.0. The scaled result of
// item::getWeight (called with the item's address) is added to the float at
// record+308, in list order. The count word is read again after every item.
// The pointer is a 32-bit word on the target.
// No range check is made on the index or on the counts. What the byte value 8
// or the feat 0x10000 stand for is not established here.
extern "C" {
extern unsigned char player[];
int playerGetFeatRank__Fix(int index, long long feat);
float getWeight__4itemPQ24item11description(void* item);

void playerRecomputeWeight__Fi(int index) {
    unsigned char* record = player + index * 23984;
    float* total = (float*)(record + 308);
    *total = 0.0f;
    for (int list = 0; list < 3; ++list) {
        const int* count = (const int*)(record + 4936 + list * 4616);
        unsigned char* item = record + 328 + list * 4616;
        for (int i = 0; i < *count; ++i, item += 72) {
            float scale = 1.0f;
            const signed char* kind = (const signed char*)(unsigned long)*(const unsigned*)item;
            if (kind[28] == 8 && playerGetFeatRank__Fix(index, 0x10000LL) != 0) scale = 0.5f;
            *total = *total + getWeight__4itemPQ24item11description(item) * scale;
        }
    }
}
}
