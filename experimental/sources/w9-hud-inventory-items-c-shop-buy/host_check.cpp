// Host check for shopBuy: affordability test, double price query, call order and arguments.
#include <vector>
#include <string>
extern "C" void shopBuy__FcPQ24item11description(char, const void*);
extern "C" unsigned char player[3 * 23984];
unsigned char player[3 * 23984];
static std::vector<std::string> calls;
static int price, added_owner, added_count; static const void* added_item;
extern "C" int getValue__4itemPQ24item11description(const void*) { calls.push_back("value"); return price; }
extern "C" void addItemToInventory__FiPQ24item11descriptioni(int o, const void* i, int c) {
    calls.push_back("add"); added_owner = o; added_item = i; added_count = c;
}
static int& gold(int owner) { return *reinterpret_cast<int*>(player + owner * 23984 + 232); }
int main() {
    int item;
    gold(0) = 77; gold(1) = 100; gold(2) = 5;
    price = 100; shopBuy__FcPQ24item11description(1, &item);                 // exactly enough
    if (gold(1) != 0 || calls != std::vector<std::string>{"value", "value", "add"}) return 1;
    if (added_owner != 1 || added_item != &item || added_count != 1) return 2;
    calls.clear(); price = 6; shopBuy__FcPQ24item11description(2, &item);    // too little
    if (gold(2) != 5 || calls != std::vector<std::string>{"value"}) return 3;
    calls.clear(); price = 0; shopBuy__FcPQ24item11description(0, &item);
    if (gold(0) != 77 || calls.size() != 3) return 4;
    calls.clear(); price = -10; shopBuy__FcPQ24item11description(0, &item);  // negative price still compares signed
    if (gold(0) != 87) return 5;
    if (gold(1) != 0 || gold(2) != 5) return 6;
    return 0;
}
