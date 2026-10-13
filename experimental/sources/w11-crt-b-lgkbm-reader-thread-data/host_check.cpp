// Host comparison only. Not an EE build input. Links the definitions with the authored
// CreateReaderThread's declarations (int[] record, unsigned char[] stack) and checks the extents.
#include <cstdio>
#include "reader_thread_data.cpp"
extern "C" int norrath_lgkbm_reader_thread_record_view[];
extern "C" unsigned char norrath_lgkbm_reader_thread_stack_view[];
int main() {
    static_assert(sizeof(norrath_lgkbm_reader_thread_record_view) == 64, "record extent");
    static_assert(sizeof(norrath_lgkbm_reader_thread_stack_view) == 6144, "stack extent");
    for (int word : norrath_lgkbm_reader_thread_record_view) if (word != 0) { std::puts("record not zero"); return 2; }
    for (unsigned char byte : norrath_lgkbm_reader_thread_stack_view) if (byte != 0) { std::puts("stack not zero"); return 2; }
    std::puts("reader thread data ok");
    return 0;
}
