// Shared layouts for the w15-libs-eh-a decompression sources. Nothing here is an EE input by itself.
// Observed from inflateInit2_ (0x010a2d60), inflate_blocks_reset (0x01097f60) and inflate_blocks_free (0x01098010) in
// SLUS_205.65. All words are 32 bit (pointers included). Names are mine, chosen from how each field is used.
//
// Stream record (56 bytes; inflateInit2_ checks the caller's size argument against 56):
//    0 next input   4 input left   8 input total   12 next output   16 output room   20 output total
//   24 message pointer (cleared on init)   28 decoder state   32 allocator   36 releaser   40 allocator context
//   44 data type   48 running checksum   52 reserved
// Decoder state (24 bytes): 0 mode, 4 and 8 mode payload, 12 raw flag ("negative window bits"), 16 window bits,
//   20 block-decoder pointer. Mode 0 is the first mode, mode 7 is entered instead when the raw flag is set.
// Block-decoder record: 0 mode, 4 pointer used by mode 6 (codes) and 12 pointer used by modes 4 and 5 (a scratch
//   table), 24 last-block flag, 28 bit count, 32 bit buffer, 36 table pool, 40 window start, 44 window end,
//   48 read position, 52 write position, 56 checksum function (null for raw), 60 checksum value.
// Block-decoder modes seen here: 4 and 5 own a scratch table; 6 owns a codes record.
#ifndef W15_LIBS_EH_A_INFLATE_HPP
#define W15_LIBS_EH_A_INFLATE_HPP

namespace w15z {

struct ZStream;
typedef void* (*Allocator)(void* context, unsigned items, unsigned size);
typedef void (*Releaser)(void* context, void* block);
typedef unsigned (*ChecksumFunction)(unsigned seed, const unsigned char* data, unsigned length);

struct ZStream {
    unsigned char* next_in;
    unsigned avail_in;
    unsigned total_in;
    unsigned char* next_out;
    unsigned avail_out;
    unsigned total_out;
    const char* msg;
    void* state;
    Allocator zalloc;
    Releaser zfree;
    void* opaque;
    int data_type;
    unsigned adler;
    unsigned reserved;
};
static_assert(sizeof(ZStream) == 56, "stream record size");

struct BlockState {
    int mode;
    void* codes;        // mode 6 (and the first word of the mode payload)
    unsigned payload8;
    void* scratch;      // modes 4 and 5, at byte 12
    unsigned payload16;
    unsigned payload20;
    unsigned last;
    unsigned bitk;
    unsigned bitb;
    void* hufts;
    unsigned char* window;
    unsigned char* end;
    unsigned char* read;
    unsigned char* write;
    ChecksumFunction checkfn;
    unsigned check;
};
static_assert(sizeof(BlockState) == 64, "block-decoder record size");
static_assert(__builtin_offsetof(BlockState, scratch) == 12 && __builtin_offsetof(BlockState, last) == 24, "layout");
static_assert(__builtin_offsetof(BlockState, bitb) == 32 && __builtin_offsetof(BlockState, window) == 40, "layout");
static_assert(__builtin_offsetof(BlockState, checkfn) == 56 && __builtin_offsetof(BlockState, check) == 60, "layout");

struct InflateState {
    int mode;
    unsigned payload4;
    unsigned payload8;
    int raw;
    unsigned wbits;
    BlockState* blocks;
};
static_assert(sizeof(InflateState) == 24, "decoder state size");

}  // namespace w15z

#endif
