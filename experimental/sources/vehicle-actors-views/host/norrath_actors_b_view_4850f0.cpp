// Host comparison only. Not an EE build input.
extern "C" const char norrath_actors_b_view_4850f0[];

int main() {
    const char expected[] = "%d,%d";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_actors_b_view_4850f0[i] != expected[i]) return 1;
    }
    return 0;
}
