// Host check: the pointer must address the object, the embedded record pointers must address
// the three records in order, and the text, value and every remaining byte must match the
// observed image contents. The byte offsets are enforced by static_asserts when built for the EE.
#include "impure_data.cpp"
#include <cstring>

static bool all_zero(const unsigned char* p, unsigned n) {
    for (unsigned i = 0; i < n; ++i)
        if (p[i]) return false;
    return true;
}

int main() {
    ImpureData* d = _impure_ptr;
    if (!d) return 1;
    if (d->word0 != 0) return 2;
    for (int i = 0; i < 3; ++i)
        if (d->record_pointers[i] != &d->records[i]) return 3;
    if (!d->text_pointer || std::strcmp(d->text_pointer, "C") != 0) return 4;
    if (d->value_168 != 1ull) return 5;
    if (!all_zero(d->gap_a, sizeof d->gap_a) || !all_zero(d->gap_b, sizeof d->gap_b) ||
        !all_zero(d->gap_c, sizeof d->gap_c))
        return 6;
    for (int i = 0; i < 3; ++i)
        if (!all_zero(d->records[i].bytes, sizeof d->records[i].bytes)) return 7;
    if (d->tail != 0) return 8;
    return 0;
}
