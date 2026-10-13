#include "iop_stream.cpp"
int main() {
    static_assert(sizeof(norrath_file_iop_stream) == 4, "four-byte word");
    if (norrath_file_iop_stream != 0) return 1;
    norrath_file_iop_stream = 3;
    return norrath_file_iop_stream == 3 ? 0 : 2;
}
