#include "impure_state.hpp"
#include <cstdio>
#include <cstring>

extern "C" unsigned char* norrath_factory_impure_pointer_word;

int main() {
    auto* state = reinterpret_cast<NorrathImpureState*>(norrath_factory_impure_pointer_word);
    if (state->errno_word != 0) return 1;
    if (state->standard_in != &state->files[0]) return 2;
    if (state->standard_out != &state->files[1]) return 3;
    if (state->standard_error != &state->files[2]) return 4;
    if (state->locale == nullptr || std::strcmp(state->locale, "C") != 0) return 5;
    if (state->random_next != 1) return 6;
    for (const auto& file : state->files) {
        for (unsigned char byte : file.bytes) {
            if (byte != 0) return 7;
        }
    }
    if (state->tail[0] != 0 || state->tail[3] != 0) return 8;
    std::printf("impure pointer cases passed\n");
    return 0;
}
