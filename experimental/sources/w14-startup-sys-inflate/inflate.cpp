// Observed inflate at 0x010a2f30, 1284 bytes, in the zlib code loaded above the main image (zlib 1.1.x;
// uncompress passes the version text "1.1.4"). Arguments: the stream record and a flush request.
// The routine is the zlib wrapper-level decoder: a state machine over the state word at state offset 0,
// reading single header and trailer bytes itself and handing the compressed blocks to inflate_blocks.
//
// Entry: a null stream, null state pointer (stream offset 28) or null input pointer (offset 0) returns -2.
// The result variable starts at -5 (buffer error) and the flush request is reduced to "-5 if the request
// is 4 (finish), else 0". Each state then does, in the image's numbering (METHOD 0, FLAG 1, DICT4 2,
// DICT3 3, DICT2 4, DICT1 5, DICT0 6, BLOCKS 7, CHECK4 8, CHECK3 9, CHECK2 10, CHECK1 11, DONE 12, BAD 13):
//  * Taking a byte: with no input left (stream offset 4) the routine returns the current result; otherwise
//    the result becomes the reduced flush request, the input count falls by one, the input total (offset 8)
//    rises by one and the next input byte is consumed.
//  * METHOD: the byte is stored at state offset 4. Its low four bits must be 8 (deflate); otherwise the
//    message "unknown compression method" is set (stream offset 24), the marker (state offset 4) becomes 5
//    and the state BAD. Its high four bits plus 8 must not exceed the window size (state offset 16);
//    otherwise "invalid window size", marker 5, BAD. Then FLAG.
//  * FLAG: the byte b; when (method * 256 + b) modulo 31 is not zero: "incorrect header check", marker 5,
//    BAD. Without the preset-dictionary bit (0x20) the next state is BLOCKS, otherwise DICT4.
//  * DICT4..DICT1: four bytes are accumulated, most significant first, in the word at state offset 8
//    (assigned by the first, added by the others). After the last the word is copied to the stream's
//    adler word (offset 48), the state becomes DICT0 and the routine returns 2 (need dictionary).
//  * DICT0: "need dictionary", marker 0, BAD, and the routine returns -2.
//  * BLOCKS: r = inflate_blocks(state->blocks (offset 20), stream, r). A result of -3 sets marker 0 and BAD.
//    A result of 0 becomes the reduced flush request. A result other than 1 is returned. On 1 the result
//    becomes the reduced flush request again, inflate_blocks_reset(blocks, stream, &state offset 4) runs
//    (it stores the running check there), and the state becomes DONE when the no-wrapper word (state
//    offset 12) is non-zero, else CHECK4.
//  * CHECK4..CHECK1: four bytes accumulate at state offset 8 as above; after the last, a value different
//    from the word at state offset 4 gives "incorrect data check", marker 5, BAD; otherwise DONE.
//  * DONE: returns 1. BAD: returns -3. Any other state value: returns -2.
// Inferred: the field roles follow the public zlib record layout; the two helpers are the block decoder
// and its reset. Not authored here: inflate_blocks, inflate_blocks_reset.
// Unknown: nothing beyond the helpers' contents.
extern "C" {
struct NorrathZState {
    unsigned mode;
    unsigned sub[2];
    int nowrap;
    unsigned wbits;
    void* blocks;
};
struct NorrathZStream {
    const unsigned char* next_in;
    unsigned avail_in;
    unsigned total_in;
    void* next_out;
    unsigned avail_out;
    unsigned total_out;
    const char* message;
    NorrathZState* state;
    void* allocate;
    void* release;
    void* opaque;
    int data_type;
    unsigned adler;
    unsigned reserved;
};

int inflate_blocks(void* blocks, NorrathZStream* stream, int result);
void inflate_blocks_reset(void* blocks, NorrathZStream* stream, unsigned* check);

enum {
    kMethod, kFlag, kDict4, kDict3, kDict2, kDict1, kDict0, kBlocks,
    kCheck4, kCheck3, kCheck2, kCheck1, kDone, kBad
};

int inflate(NorrathZStream* z, int flush) {
    if (z == nullptr || z->state == nullptr || z->next_in == nullptr) return -2;
    const int reduced = flush == 4 ? -5 : 0;
    int r = -5;
    for (;;) {
        switch (z->state->mode) {
        case kMethod:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[0] = *z->next_in++;
            if ((z->state->sub[0] & 0xf) != 8) {
                z->message = "unknown compression method";
                z->state->mode = kBad;
                z->state->sub[0] = 5;
                break;
            }
            if ((z->state->sub[0] >> 4) + 8 > z->state->wbits) {
                z->message = "invalid window size";
                z->state->mode = kBad;
                z->state->sub[0] = 5;
                break;
            }
            z->state->mode = kFlag;
            // fall through
        case kFlag: {
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            const unsigned b = *z->next_in++;
            if (((z->state->sub[0] << 8) + b) % 31 != 0) {
                z->message = "incorrect header check";
                z->state->mode = kBad;
                z->state->sub[0] = 5;
                break;
            }
            if ((b & 0x20) == 0) {
                z->state->mode = kBlocks;
                break;
            }
            z->state->mode = kDict4;
        }
            // fall through
        case kDict4:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] = static_cast<unsigned>(*z->next_in++) << 24;
            z->state->mode = kDict3;
            // fall through
        case kDict3:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] += static_cast<unsigned>(*z->next_in++) << 16;
            z->state->mode = kDict2;
            // fall through
        case kDict2:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] += static_cast<unsigned>(*z->next_in++) << 8;
            z->state->mode = kDict1;
            // fall through
        case kDict1:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] += *z->next_in++;
            z->adler = z->state->sub[1];
            z->state->mode = kDict0;
            return 2;
        case kDict0:
            z->message = "need dictionary";
            z->state->mode = kBad;
            z->state->sub[0] = 0;
            return -2;
        case kBlocks:
            r = inflate_blocks(z->state->blocks, z, r);
            if (r == -3) {
                z->state->mode = kBad;
                z->state->sub[0] = 0;
                break;
            }
            if (r == 0) r = reduced;
            if (r != 1) return r;
            r = reduced;
            inflate_blocks_reset(z->state->blocks, z, &z->state->sub[0]);
            if (z->state->nowrap != 0) {
                z->state->mode = kDone;
                break;
            }
            z->state->mode = kCheck4;
            // fall through
        case kCheck4:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] = static_cast<unsigned>(*z->next_in++) << 24;
            z->state->mode = kCheck3;
            // fall through
        case kCheck3:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] += static_cast<unsigned>(*z->next_in++) << 16;
            z->state->mode = kCheck2;
            // fall through
        case kCheck2:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] += static_cast<unsigned>(*z->next_in++) << 8;
            z->state->mode = kCheck1;
            // fall through
        case kCheck1:
            if (z->avail_in == 0) return r;
            r = reduced;
            z->avail_in--; z->total_in++;
            z->state->sub[1] += *z->next_in++;
            if (z->state->sub[0] != z->state->sub[1]) {
                z->message = "incorrect data check";
                z->state->mode = kBad;
                z->state->sub[0] = 5;
                break;
            }
            z->state->mode = kDone;
            // fall through
        case kDone:
            return 1;
        case kBad:
            return -3;
        default:
            return -2;
        }
    }
}
}
