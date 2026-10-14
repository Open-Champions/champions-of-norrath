// Observed inflateInit2_ at 0x010a2d60, 456 bytes, pinned SLUS_205.65 (layouts in
// ../w15-libs-eh-a-zlib-common/inflate.hpp). Arguments: the stream, the window-bits value, a version string and the
// caller's size of the stream record. The result is 0 on success. In order:
//   * a null version string, a first character other than '1', or a size other than 56 returns -6; a null stream
//     returns -2;
//   * the message is cleared; a missing allocator is replaced by zcalloc (0x0116d6a8) and the context cleared, a
//     missing releaser by zcfree (0x0116d6c8);
//   * a 24-byte decoder state is allocated (one item of 24 bytes) and stored in the stream (-4 if none); its block
//     decoder pointer is cleared; the raw flag is set when the window bits are negative, which are then negated;
//   * window bits outside 8..15 release the state through the releaser (and the block decoder, with
//     inflate_blocks_free (0x01098010), if one is present), clear the stream's state and return -2;
//   * the window bits are stored; the block decoder is created by inflate_blocks_new (0x01097248) with (stream,
//     adler32 (0x01000000) unless raw, 1 << window bits) and stored; if it is null the state is released and the
//     stream's state cleared (-4);
//   * otherwise the stream is reset: totals and message cleared, the state's mode set to 7 if raw and 0 if not,
//     inflate_blocks_reset (0x01097f60) called as (decoder, stream, null).
// Inferred: the reset and release sequences are inlined copies of the usual stream reset and end routines. Unknown:
// bodies of the called routines; nothing was executed on the EE.
#include "../w15-libs-eh-a-zlib-common/inflate.hpp"

extern "C" {
void* zcalloc(void* context, unsigned items, unsigned size);
void zcfree(void* context, void* block);
unsigned adler32(unsigned seed, const unsigned char* data, unsigned length);
int inflate_blocks_free(w15z::BlockState* blocks, w15z::ZStream* stream);
w15z::BlockState* inflate_blocks_new(w15z::ZStream* stream, w15z::ChecksumFunction check, unsigned window_size);
void inflate_blocks_reset(w15z::BlockState* blocks, w15z::ZStream* stream, unsigned* check);

int inflateInit2_(w15z::ZStream* stream, int window_bits, const char* version, int stream_size) {
    if (version == nullptr || version[0] != '1' || stream_size != 56) return -6;
    if (stream == nullptr) return -2;
    stream->msg = nullptr;
    if (stream->zalloc == nullptr) {
        stream->zalloc = zcalloc;
        stream->opaque = nullptr;
    }
    if (stream->zfree == nullptr) stream->zfree = zcfree;

    w15z::InflateState* state = static_cast<w15z::InflateState*>(stream->zalloc(stream->opaque, 1, 24));
    stream->state = state;
    if (state == nullptr) return -4;
    state->blocks = nullptr;
    state->raw = 0;
    if (window_bits < 0) {
        state->raw = 1;
        window_bits = -window_bits;
    }
    if (window_bits < 8 || window_bits > 15) {
        if (state->blocks != nullptr) inflate_blocks_free(state->blocks, stream);
        stream->zfree(stream->opaque, state);
        stream->state = nullptr;
        return -2;
    }
    state->wbits = window_bits;
    state->blocks = inflate_blocks_new(stream, state->raw ? nullptr : adler32, 1u << window_bits);
    if (state->blocks == nullptr) {
        stream->zfree(stream->opaque, state);
        stream->state = nullptr;
        return -4;
    }
    stream->total_out = 0;
    stream->total_in = 0;
    stream->msg = nullptr;
    state->mode = state->raw ? 7 : 0;
    inflate_blocks_reset(state->blocks, stream, nullptr);
    return 0;
}
}
