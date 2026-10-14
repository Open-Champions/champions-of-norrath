#include "is_iop_streaming_enabled.cpp"
#include <initializer_list>

int norrath_file_iop_stream __asm__("_6VIFile$sIopStream") = 0;

int main() {
    for (int v : {0, 1, 2, -1, 0x7fffffff}) {
        norrath_file_iop_stream = v;
        if (IsIopStreamingEnabled__6VIFile() != v) return 1;
    }
    return 0;
}
