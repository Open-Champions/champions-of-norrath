// Host comparison only. Not an EE build input.
extern "C" const char norrath_actors_a_view_477758[];
int main() {
    const char expected[] = "frequency";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_actors_a_view_477758[i] != expected[i]) return 1;
    }
    return 0;
}
