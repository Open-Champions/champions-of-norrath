// Observed inflateEnd at 0x010a3588, 112 bytes, in the zlib code loaded above the main image. Argument: the
// stream record (56 bytes in zlib 1.1.x). Returns -2 (stream error) when the record pointer is null, when its
// state pointer (offset 28) is null, or when its release function (offset 36) is null. Otherwise, when the
// state's block-decoder pointer (state offset 20) is non-null it calls inflate_blocks_free(blocks, stream);
// then it calls the stream's release function with its opaque word (offset 40) and the state pointer (the
// state pointer and the release function are read again after the block free), stores null in the stream's
// state pointer and returns 0.
// Not authored here: inflate_blocks_free. The names of the fields are those of the public zlib record layout.
extern "C" {
struct NorrathZState {
    int mode;
    unsigned sub[2];
    int nowrap;
    unsigned wbits;
    void* blocks;
};
struct NorrathZStream {
    const void* next_in;
    unsigned avail_in;
    unsigned total_in;
    void* next_out;
    unsigned avail_out;
    unsigned total_out;
    const char* message;
    NorrathZState* state;
    void* allocate;
    void (*release)(void* opaque, void* address);
    void* opaque;
    int data_type;
    unsigned adler;
    unsigned reserved;
};

int inflate_blocks_free(void* blocks, NorrathZStream* stream);

int inflateEnd(NorrathZStream* stream) {
    if (stream == nullptr || stream->state == nullptr || stream->release == nullptr) return -2;
    if (stream->state->blocks != nullptr) inflate_blocks_free(stream->state->blocks, stream);
    stream->release(stream->opaque, stream->state);
    stream->state = nullptr;
    return 0;
}
}
