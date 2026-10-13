// Observed texCacheData__FP8_texturei at 0x001727e0, 84 bytes. It appends a request to a ring of 64
// eight-byte slots at decodeQ (0x006e9180) that is indexed by the words decodeQHead (0x00486978, producer)
// and decodeQTail (0x0048697c, consumer). The slot at decodeQHead receives the texture pointer in its first
// word and the second argument in its second word, and decodeQHead advances modulo 64. When advancing would
// make the head equal the tail the ring is full: nothing is written and the head is unchanged. It returns
// nothing. Inferred from the names only: this queues a texture for decompression. Unknown: the meaning of
// the second argument, and the consumer.
struct DecodeRequest {
    void* texture;
    int value;
};
extern DecodeRequest decodeQ[64];
extern int decodeQHead;
extern int decodeQTail;

extern "C" void texCacheData__FP8_texturei(void* texture, int value) {
    int next = (decodeQHead + 1) & 63;
    if (next != decodeQTail) {
        decodeQ[decodeQHead].texture = texture;
        decodeQ[decodeQHead].value = value;
        decodeQHead = next;
    }
}
