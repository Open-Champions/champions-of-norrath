// Host comparison only. Not an EE build input.
extern "C" unsigned char norrath_actors_b_view_4844a0[4];

int main() {
    for (unsigned i = 0; i < 4; ++i) {
        if (norrath_actors_b_view_4844a0[i] != 0) {
            return 1;
        }
    }
    norrath_actors_b_view_4844a0[0] = 1;
    if (norrath_actors_b_view_4844a0[0] != 1) {
        return 1;
    }
    norrath_actors_b_view_4844a0[0] = 0;
    return 0;
}
