#include <cstdio>

struct PackedProgram {
    unsigned char start[816];
    unsigned char end[64];
};

extern "C" unsigned char norrath_floor_program_start_view[816];
extern "C" unsigned char norrath_floor_program_end_view[64];

int main() {
    PackedProgram packed{};
    if (&packed.end[0] - &packed.start[0] != 816) return 1;
    const auto gap = norrath_floor_program_end_view - norrath_floor_program_start_view;
    if (gap != 816) {
        std::fprintf(stderr, "program span %td\n", gap);
        return 2;
    }
    for (unsigned char byte : norrath_floor_program_start_view) {
        if (byte != 0) return 3;
    }
    for (unsigned char byte : norrath_floor_program_end_view) {
        if (byte != 0) return 4;
    }
    return 0;
}
