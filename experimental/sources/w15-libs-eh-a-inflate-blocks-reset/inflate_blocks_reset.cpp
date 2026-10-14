// Observed inflate_blocks_reset at 0x01097f60, 172 bytes, pinned SLUS_205.65 (layouts in
// ../w15-libs-eh-a-zlib-common/inflate.hpp). Arguments: the block-decoder record, the stream, and an optional
// pointer. In order:
//   * if the pointer is non-null, the current checksum value (byte 60) is stored through it;
//   * modes 4 and 5 release the scratch table (byte 12) through the stream's releaser (context = stream byte 40);
//   * mode 6 calls inflate_codes_free (0x0109bf60) on the codes pointer (byte 4) and the stream;
//   * the mode, bit count and bit buffer are cleared; the read and write positions both become the window start;
//   * if a checksum function is present it is called as (0, null, 0); the result becomes the record's checksum and the
//     stream's running checksum (byte 48).
// Unknown: the body of inflate_codes_free; nothing was executed on the EE.
#include "../w15-libs-eh-a-zlib-common/inflate.hpp"

extern "C" {
void inflate_codes_free(void* codes, w15z::ZStream* stream);

void inflate_blocks_reset(w15z::BlockState* blocks, w15z::ZStream* stream, unsigned* check) {
    if (check != nullptr) *check = blocks->check;
    if (blocks->mode == 4 || blocks->mode == 5) stream->zfree(stream->opaque, blocks->scratch);
    if (blocks->mode == 6) inflate_codes_free(blocks->codes, stream);
    blocks->mode = 0;
    blocks->bitk = 0;
    blocks->bitb = 0;
    blocks->read = blocks->window;
    blocks->write = blocks->window;
    if (blocks->checkfn != nullptr) {
        blocks->check = blocks->checkfn(0, nullptr, 0);
        stream->adler = blocks->check;
    }
}
}
