// Host check: the shop storage has the observed sizes and starts zeroed.
#include "shop_data.cpp"
static_assert(sizeof(shop) == 464 && sizeof(sellMode) == 16 && sizeof(playerGettinIt) == 16, "observed gaps");
static_assert(sizeof(shopDisableMouse) == 8 && sizeof(shopMouseDecay) == 8, "observed sizes");
int main() {
    for (unsigned char b : shop) if (b) return 1;
    for (int w : sellMode) if (w) return 2;
    for (int w : playerGettinIt) if (w) return 3;
    if (shopIntro || shopTick || shopGrid || shopDisableMouse[0] || shopDisableMouse[1] || shopMouseDecay[0] || shopMouseDecay[1]) return 4;
    return 0;
}
