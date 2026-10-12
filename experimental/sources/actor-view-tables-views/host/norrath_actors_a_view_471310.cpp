// Host comparison only. Not an EE build input.
extern "C" const char norrath_actors_a_view_471310[];
int main() {
    const char expected[] = "anttrapa.anm";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_actors_a_view_471310[i] != expected[i]) return 1;
    }
    return 0;
}
