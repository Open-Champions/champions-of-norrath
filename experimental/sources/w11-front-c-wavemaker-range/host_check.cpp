// Host check for range. Not an EE plan input.
extern "C" unsigned char range[4000];
int main() {
    static_assert(sizeof(range) == 4000, "gap before splashInfoBits");
    for (unsigned char c : range) if (c) return 1;
    return 0;
}
