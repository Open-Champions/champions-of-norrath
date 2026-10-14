// Observed inflate_blocks at 0x01097360, 3068 bytes, pinned SLUS_205.65: the block-level decoder of the decompressor
// (layouts in ../w15-libs-eh-a-zlib-common/inflate.hpp). Arguments: the block-decoder record, the stream and the
// running result code. It works on local copies of the input cursor (stream byte 0 and 4), a 32-bit bit buffer and
// bit count (record bytes 32 and 28) and the window write position (record byte 52), with the room to the read
// position or window end computed as: read - write - 1 when write < read, else end - write. The mode word
// (record byte 0) selects a step; ten modes exist, any other mode value ends with result -2.
//   0 header: three bits; the low bit is the last-block flag, the rest the type: 0 drops the bits that complete the
//     byte and goes to 1; 1 obtains the fixed tables with inflate_trees_fixed (0x010a4340) and a codes record with
//     inflate_codes_new (0x0109bee8) (-4 if none, before any bit is dropped) and goes to 6; 2 goes to 3; 3 sets
//     the message "invalid block type", result -3, mode 9 and ends;
//   1 stored lengths: 32 bits whose upper half must be the complement of the lower; else message "invalid stored
//     block lengths", result -3, mode 9. The length is kept, the bit buffer emptied, and mode 2 follows (or 7 when
//     the length is zero and this was the last block, or 0 when it was not);
//   2 stored data: with no input it ends; when the window has no room it wraps the write position if it sits at the
//     window end and the read position is not at the window start, otherwise flushes with inflate_flush
//     (0x010a85d8), re-reads the write position and wraps again, ending if there is still no room. Then the result is
//     set to 0 and min(length left, input left, room) bytes are copied with memcpy; when the length reaches zero the
//     mode becomes 7 if last, else 0;
//   3 dynamic header: 14 bits: low five (more than 29 is an error), next five (more than 29 is an error), top four;
//     the error is message "too many length or distance symbols", result -3, mode 9. A table of
//     258 + low + next words is requested from the stream allocator (-4 if none); 14 bits are dropped, the index is
//     cleared and mode 4 follows;
//   4 code-length code lengths: for index below 4 + (header >> 10) three bits are read into the table at the position
//     named by the 19-entry order table (16 17 18 0 8 7 9 6 10 5 11 4 12 3 13 2 14 1 15), the rest are zeroed;
//     inflate_trees_bits (0x010a4048) is called with width 7; a failure becomes the result, and when it is -3
//     the table is released and mode 9 set; success clears the index and goes to 5;
//   5 code lengths: while the index is below 258 + low + next, a code is looked up in the table (entry of the width
//     bits of the buffer, masked with inflate_mask); a value below 16 is stored; 16, 17, 18 read 2, 3 or 7 more bits
//     and repeat the previous length (16) or zero for 3 + bits, 3 + bits or 11 + bits entries; overrunning the count
//     or repeating at index 0 releases the table, sets message "invalid bit length repeat", result -3, mode 9 and
//     ends. Then the table pointer is cleared, inflate_trees_dynamic (0x010a4148) is called with widths 9 and 6 and
//     257 + low / 1 + next symbol counts; a failure becomes the result (releasing the table and setting mode 9
//     when -3); inflate_codes_new failing gives -4 without releasing; otherwise the length table is released and
//     mode 6 follows;
//   6 codes: inflate_codes (0x0109b6d8) runs with the cursor stored back; any result but 1 is flushed and
//     returned; on 1 the result is reset to 0, inflate_codes_free (0x0109bf60) is called, the cursor is reloaded, and the
//     mode becomes 0 or, for the last block, 7;
//   7 drain: flush; if the read and write positions differ the routine ends, else mode 8;
//   8 done: result 1, ends;   9 bad: result -3, ends.
// "Ends" means: the cursor, bit buffer and write position are stored back, inflate_flush is called with the current
// result and its answer is returned. Running out of input in a bit read ends with the result unchanged (the result
// is set to 0 only when a byte is taken).
// Inferred: the field and mode names, and that this is the usual block-level inflate state machine. Unknown: the
// callee bodies; nothing was executed on the EE.
#include "../w15-libs-eh-a-zlib-common/inflate.hpp"

extern "C" {
extern const unsigned inflate_mask[17];
int inflate_flush(w15z::BlockState* blocks, w15z::ZStream* stream, int result);
int inflate_trees_fixed(unsigned* bl, unsigned* bd, const w15z::TableEntry** tl, const w15z::TableEntry** td, w15z::ZStream* stream);
void* inflate_codes_new(unsigned bl, unsigned bd, const w15z::TableEntry* tl, const w15z::TableEntry* td, w15z::ZStream* stream);
int inflate_trees_bits(unsigned* lengths, unsigned* bits, const w15z::TableEntry** table, void* pool, w15z::ZStream* stream);
int inflate_trees_dynamic(unsigned nl, unsigned nd, unsigned* lengths, unsigned* bl, unsigned* bd, const w15z::TableEntry** tl,
                          const w15z::TableEntry** td, void* pool, w15z::ZStream* stream);
int inflate_codes(w15z::BlockState* blocks, w15z::ZStream* stream, int result);
void inflate_codes_free(void* codes, w15z::ZStream* stream);
void* memcpy(void* to, const void* from, unsigned length);
}

namespace {

const unsigned kOrder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

struct Cursor {
    unsigned b;
    unsigned k;
    const unsigned char* p;
    unsigned n;
    unsigned char* q;
    unsigned m;
};

unsigned room(const w15z::BlockState* s, const unsigned char* q) {
    return q < s->read ? static_cast<unsigned>(s->read - q) - 1 : static_cast<unsigned>(s->end - q);
}

int leave(w15z::BlockState* s, w15z::ZStream* z, const Cursor& c, int r) {
    s->bitb = c.b;
    s->bitk = c.k;
    z->avail_in = c.n;
    z->total_in += static_cast<unsigned>(c.p - z->next_in);
    z->next_in = const_cast<unsigned char*>(c.p);
    s->write = c.q;
    return inflate_flush(s, z, r);
}

int fail(w15z::BlockState* s, w15z::ZStream* z, const Cursor& c, const char* message) {
    s->mode = 9;
    z->msg = message;
    return leave(s, z, c, -3);
}

bool need(Cursor& c, unsigned bits, int& r) {
    while (c.k < bits) {
        if (c.n == 0) return false;
        r = 0;
        --c.n;
        c.b |= static_cast<unsigned>(*c.p++) << c.k;
        c.k += 8;
    }
    return true;
}

void drop(Cursor& c, unsigned bits) {
    c.b >>= bits;
    c.k -= bits;
}

}  // namespace

extern "C" int inflate_blocks(w15z::BlockState* s, w15z::ZStream* z, int r) {
    Cursor c;
    c.p = z->next_in;
    c.n = z->avail_in;
    c.b = s->bitb;
    c.k = s->bitk;
    c.q = s->write;
    c.m = room(s, c.q);

    for (;;) {
        switch (s->mode) {
        case 0: {
            if (!need(c, 3, r)) return leave(s, z, c, r);
            const unsigned t = c.b & 7;
            s->last = t & 1;
            switch (t >> 1) {
            case 0:
                drop(c, 3);
                drop(c, c.k & 7);
                s->mode = 1;
                break;
            case 1: {
                unsigned bl, bd;
                const w15z::TableEntry *tl, *td;
                inflate_trees_fixed(&bl, &bd, &tl, &td, z);
                s->codes = inflate_codes_new(bl, bd, tl, td, z);
                if (s->codes == nullptr) return leave(s, z, c, -4);
                drop(c, 3);
                s->mode = 6;
                break;
            }
            case 2:
                drop(c, 3);
                s->mode = 3;
                break;
            default:
                drop(c, 3);
                return fail(s, z, c, "invalid block type");
            }
            break;
        }
        case 1:
            if (!need(c, 32, r)) return leave(s, z, c, r);
            if (((~c.b >> 16) & 0xffff) != (c.b & 0xffff)) return fail(s, z, c, "invalid stored block lengths");
            s->left = c.b & 0xffff;
            c.b = c.k = 0;
            s->mode = s->left != 0 ? 2 : (s->last != 0 ? 7 : 0);
            break;
        case 2: {
            if (c.n == 0) return leave(s, z, c, r);
            if (c.m == 0) {
                if (c.q == s->end && s->read != s->window) {
                    c.q = s->window;
                    c.m = room(s, c.q);
                }
                if (c.m == 0) {
                    s->write = c.q;
                    r = inflate_flush(s, z, r);
                    c.q = s->write;
                    c.m = room(s, c.q);
                    if (c.q == s->end && s->read != s->window) {
                        c.q = s->window;
                        c.m = room(s, c.q);
                    }
                    if (c.m == 0) return leave(s, z, c, r);
                }
            }
            r = 0;
            unsigned t = s->left;
            if (t > c.n) t = c.n;
            if (t > c.m) t = c.m;
            memcpy(c.q, c.p, t);
            c.p += t;
            c.n -= t;
            c.q += t;
            c.m -= t;
            s->left -= t;
            if (s->left == 0) s->mode = s->last != 0 ? 7 : 0;
            break;
        }
        case 3: {
            if (!need(c, 14, r)) return leave(s, z, c, r);
            const unsigned t = c.b & 0x3fff;
            s->table = t;
            if ((t & 0x1f) > 29 || ((t >> 5) & 0x1f) > 29) return fail(s, z, c, "too many length or distance symbols");
            s->blens = static_cast<unsigned*>(z->zalloc(z->opaque, 258 + (t & 0x1f) + ((t >> 5) & 0x1f), 4));
            if (s->blens == nullptr) return leave(s, z, c, -4);
            drop(c, 14);
            s->index = 0;
            s->mode = 4;
            break;
        }
        case 4: {
            while (s->index < 4 + (s->table >> 10)) {
                if (!need(c, 3, r)) return leave(s, z, c, r);
                s->blens[kOrder[s->index++]] = c.b & 7;
                drop(c, 3);
            }
            while (s->index < 19) s->blens[kOrder[s->index++]] = 0;
            s->bb = 7;
            const int t = inflate_trees_bits(s->blens, &s->bb, &s->tb, s->hufts, z);
            if (t != 0) {
                r = t;
                if (r == -3) {
                    z->zfree(z->opaque, s->blens);
                    s->mode = 9;
                }
                return leave(s, z, c, r);
            }
            s->index = 0;
            s->mode = 5;
            break;
        }
        case 5: {
            for (;;) {
                unsigned t = s->table;
                if (s->index >= 258 + (t & 0x1f) + ((t >> 5) & 0x1f)) break;
                t = s->bb;
                if (!need(c, t, r)) return leave(s, z, c, r);
                const w15z::TableEntry* entry = s->tb + (c.b & inflate_mask[t]);
                t = entry->bits;
                const unsigned code = entry->base;
                if (code < 16) {
                    drop(c, t);
                    s->blens[s->index++] = code;
                } else {
                    const unsigned extra = code == 18 ? 7 : code - 14;
                    unsigned repeat = code == 18 ? 11 : 3;
                    if (!need(c, t + extra, r)) return leave(s, z, c, r);
                    drop(c, t);
                    repeat += c.b & inflate_mask[extra];
                    drop(c, extra);
                    unsigned i = s->index;
                    const unsigned header = s->table;
                    if (i + repeat > 258 + (header & 0x1f) + ((header >> 5) & 0x1f) || (code == 16 && i < 1)) {
                        z->zfree(z->opaque, s->blens);
                        return fail(s, z, c, "invalid bit length repeat");
                    }
                    const unsigned value = code == 16 ? s->blens[i - 1] : 0;
                    do {
                        s->blens[i++] = value;
                    } while (--repeat);
                    s->index = i;
                }
            }
            s->tb = nullptr;
            unsigned bl = 9, bd = 6;
            const w15z::TableEntry *tl, *td;
            const unsigned header = s->table;
            const int t = inflate_trees_dynamic(257 + (header & 0x1f), 1 + ((header >> 5) & 0x1f), s->blens, &bl, &bd, &tl, &td,
                                                s->hufts, z);
            if (t != 0) {
                if (t == -3) {
                    z->zfree(z->opaque, s->blens);
                    s->mode = 9;
                }
                return leave(s, z, c, t);
            }
            void* codes = inflate_codes_new(bl, bd, tl, td, z);
            if (codes == nullptr) return leave(s, z, c, -4);
            s->codes = codes;
            z->zfree(z->opaque, s->blens);
            s->mode = 6;
            break;
        }
        case 6: {
            s->bitb = c.b;
            s->bitk = c.k;
            z->avail_in = c.n;
            z->total_in += static_cast<unsigned>(c.p - z->next_in);
            z->next_in = const_cast<unsigned char*>(c.p);
            s->write = c.q;
            r = inflate_codes(s, z, r);
            if (r != 1) return inflate_flush(s, z, r);
            r = 0;
            inflate_codes_free(s->codes, z);
            c.p = z->next_in;
            c.n = z->avail_in;
            c.b = s->bitb;
            c.k = s->bitk;
            c.q = s->write;
            c.m = room(s, c.q);
            if (s->last == 0) {
                s->mode = 0;
                break;
            }
            s->mode = 7;
            // fall through
        }
        case 7:
            s->write = c.q;
            r = inflate_flush(s, z, r);
            c.q = s->write;
            if (s->read != s->write) return leave(s, z, c, r);
            s->mode = 8;
            // fall through
        case 8:
            return leave(s, z, c, 1);
        case 9:
            return leave(s, z, c, -3);
        default:
            return leave(s, z, c, -2);
        }
    }
}
