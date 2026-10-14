// Observed inflate_blocks_free at 0x01098010, 212 bytes, pinned SLUS_205.65 (layouts in
// ../w15-libs-eh-a-zlib-common/inflate.hpp). Arguments: the block-decoder record and the stream. It performs the
// reset sequence of inflate_blocks_reset (0x01097f60) inline, without the optional checksum output, and then releases,
// in this order, the window (byte 40), the table pool (byte 36) and the record itself through the stream's releaser
// (context = stream byte 40). It returns 0.
// Unknown: the body of inflate_codes_free (0x0109bf60); nothing was executed on the EE.
#include "../w15-libs-eh-a-zlib-common/inflate.hpp"

extern "C" {
void inflate_blocks_reset(w15z::BlockState* blocks, w15z::ZStream* stream, unsigned* check);

int inflate_blocks_free(w15z::BlockState* blocks, w15z::ZStream* stream) {
    inflate_blocks_reset(blocks, stream, nullptr);
    stream->zfree(stream->opaque, blocks->window);
    stream->zfree(stream->opaque, blocks->hufts);
    stream->zfree(stream->opaque, blocks);
    return 0;
}
}
