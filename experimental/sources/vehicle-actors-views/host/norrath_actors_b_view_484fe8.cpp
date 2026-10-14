// Host comparison only. Not an EE build input.
extern "C" const char norrath_actors_b_view_484fe8[];

int main() {
    const char expected[] = "t";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_actors_b_view_484fe8[i] != expected[i]) return 1;
    }
    return 0;
}
