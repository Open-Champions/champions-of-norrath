// Host comparison only. Not an EE build input.
extern "C" const char norrath_ctors_ant_lantern_view_470df8[];
int main() {
    const char expected[] = "Breakable";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_ctors_ant_lantern_view_470df8[i] != expected[i]) return 1;
    }
    return 0;
}
