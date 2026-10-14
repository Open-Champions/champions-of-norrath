extern "C" int shopRunning;
int main() {
    static_assert(sizeof shopRunning == 4, "one word");
    if (shopRunning != 0) return 1;
    shopRunning = 1;
    if (shopRunning != 1) return 2;
    return 0;
}
