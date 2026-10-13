// Observed inflateInit_ at 0x010a3720, 36 bytes, in the zlib code loaded above the main image. Arguments: the
// stream record, a version text and the record size the caller was built with. The routine is the zlib macro
// form of the default initialiser: it passes the stream, the window size 15 (the default window bits), the
// version text and the size on to inflateInit2_ (0x010a2d60) and returns that call's result unchanged, as a
// tail call.
// Inferred: the library is zlib 1.1.4 (the caller uncompress passes "1.1.4" and 56, the record size).
// Not authored here: inflateInit2_.
extern "C" {
int inflateInit2_(void* stream, int window_bits, const char* version, int stream_size);

int inflateInit_(void* stream, const char* version, int stream_size) {
    return inflateInit2_(stream, 15, version, stream_size);
}
}
