// Observed shopBuy__FcPQ24item11description at 0x001bbe58, 136 bytes.
// Arguments: a buyer number (a char, sign-extended) and an item record. Asks
// getValue for the item's price; when the signed word at record+232 of the
// buyer's 23984-byte player record (the gold the shop code spends) is not below
// the price, it asks getValue again, subtracts that price from the word and
// calls addItemToInventory(buyer, item, 1). Otherwise it does nothing more.
// Unknown: the name of the +232 word beyond its use as the purse here.
extern "C" unsigned char player[];
extern "C" int getValue__4itemPQ24item11description(const void* item);
extern "C" void addItemToInventory__FiPQ24item11descriptioni(int owner, const void* item, int count);

extern "C" void shopBuy__FcPQ24item11description(char buyer, const void* item) {
    // The image extends the char itself (shift left 24, arithmetic shift right 24), so this keeps the
    // value opaque to the compiler instead of trusting the caller's extension.
    int widened = static_cast<unsigned char>(buyer);
    asm("" : "+r"(widened));
    const int owner = static_cast<signed char>(widened);
    int& gold = *reinterpret_cast<int*>(player + owner * 23984 + 232);
    if (gold >= getValue__4itemPQ24item11description(item)) {
        gold -= getValue__4itemPQ24item11description(item);
        addItemToInventory__FiPQ24item11descriptioni(owner, item, 1);
    }
}
