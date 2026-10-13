// Observed uncompress at 0x0101b128, 160 bytes, in the zlib code of the image. The one-call inflate:
// uncompress(dest, destLen, source, sourceLen), with destLen a pointer to the capacity on entry and the produced
// length on success. It fills a 56-byte zlib stream record on its stack (input pointer and length, output pointer,
// output capacity read from *destLen, both allocator words zero; nothing else is initialised), calls
// inflateInit_(stream, "1.1.4", 56), and returns that result when it is nonzero. Then it calls inflate(stream, 4)
// (4 is the finish request). When inflate returns 1 (stream end) it stores the stream's total output count (word
// at offset 20) through destLen, calls inflateEnd and returns its result. For any other result it calls inflateEnd
// and returns that result, except that 0 is returned as -5.
// The version text and the record size 56 are what the image passes; the field names below are mine, from the
// layout the code uses. Inferred: the library is zlib 1.1.4; the codes are Z_OK 0, Z_STREAM_END 1, Z_BUF_ERROR -5.
// Not authored here: inflateInit_, inflate, inflateEnd.
extern "C" {
struct NorrathZStream {
    const void* next_in;
    unsigned avail_in;
    unsigned total_in;
    void* next_out;
    unsigned avail_out;
    unsigned total_out;
    const char* message;
    void* state;
    void* allocate;
    void* release;
    void* opaque;
    int data_type;
    unsigned adler;
    unsigned reserved;
};

int inflateInit_(NorrathZStream* stream, const char* version, int stream_size);
int inflate(NorrathZStream* stream, int flush);
int inflateEnd(NorrathZStream* stream);

int uncompress(void* dest, unsigned* dest_length, const void* source, unsigned source_length) {
    NorrathZStream stream;
    stream.next_in = source;
    stream.avail_in = source_length;
    stream.next_out = dest;
    stream.avail_out = *dest_length;
    stream.allocate = 0;
    stream.release = 0;
    int result = inflateInit_(&stream, "1.1.4", 56);
    if (result != 0) return result;
    result = inflate(&stream, 4);
    if (result != 1) {
        inflateEnd(&stream);
        return result == 0 ? -5 : result;
    }
    *dest_length = stream.total_out;
    return inflateEnd(&stream);
}
}
