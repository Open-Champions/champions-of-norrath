// Observed _sceSifSendCmd at 0x002d7558, 308 bytes, the local routine behind
// sceSifSendCmd (flags 0) and isceSifSendCmd (flags 1). It queues one command
// packet, and optionally one block of extra data, on the SIF DMA channel.
//
// Arguments, in order (names inferred from how they are used below):
//   command       word stored in the packet at offset 8
//   flags         bit 0: queue with isceSifSetDma instead of sceSifSetDma
//                 bit 2: write the extra data back from the data cache first
//   packet        EE-side command packet
//   packet_size   byte size of the packet
//   extra_source  EE address of the extra data
//   extra_dest    destination address of the extra data
//   extra_size    byte size of the extra data
//
// Observed behavior:
//  * packet_size outside 16 through 112 (unsigned compare of size - 16 against 97)
//    returns 0 and touches nothing.
//  * With extra_size > 0 (signed): packet word 0 becomes (its low byte) |
//    (extra_size << 8), word 1 becomes extra_dest, and the first of the DMA
//    descriptors (four words each: source, destination, size, attributes) is
//    {extra_source, extra_dest, extra_size, 0}. With bit 2 of flags set, the cache
//    write-back runs over the extra data before the packet is touched further.
//    Otherwise word 0 keeps only its low byte (upper bytes cleared) and word 1 is 0,
//    and no extra descriptor is made.
//  * The packet's descriptor is {packet, <word at 0x008dea20>, packet_size, 0x44}.
//    That word is at offset 8 of the SIF command state at 0x008dea18 (published as
//    norrath_sound_command_dma_destination_word_view) and is read after the extra
//    descriptor is built. Packet word 2 becomes command and
//    the low byte of word 0 becomes packet_size. The write-back runs over the
//    packet last.
//  * The descriptor list (one or two entries) goes to sceSifSetDma, or
//    isceSifSetDma when bit 0 of flags is set; their result is returned.
// Unknown: what attribute 0x44 means to the DMA layer, and whether the first
// packet word is a length or a count on the IOP side.
extern "C" {
extern void* norrath_sound_command_dma_destination_word_view[1];

void sceSifWriteBackDCache(void* address, int bytes);
int sceSifSetDma(void* descriptors, int count);
int isceSifSetDma(void* descriptors, int count);

struct SifDmaDescriptor {
    void* source;
    void* destination;
    int size;
    unsigned attributes;
};

#if defined(__mips__)
static_assert(sizeof(SifDmaDescriptor) == 16, "descriptor layout");
#endif

int _sceSifSendCmd(unsigned command, unsigned flags, unsigned char* packet, int packet_size,
                   void* extra_source, void* extra_dest, int extra_size) {
    if (static_cast<unsigned>(packet_size - 16) >= 97u)
        return 0;
    SifDmaDescriptor descriptors[2];
    unsigned* words = reinterpret_cast<unsigned*>(packet);
    int count = 0;
    if (extra_size > 0) {
        words[0] = packet[0] | (static_cast<unsigned>(extra_size) << 8);
        words[1] = reinterpret_cast<__UINTPTR_TYPE__>(extra_dest);
        count = 1;
        descriptors[0].source = extra_source;
        descriptors[0].destination = extra_dest;
        descriptors[0].size = extra_size;
        descriptors[0].attributes = 0;
        if (flags & 4u)
            sceSifWriteBackDCache(extra_source, extra_size);
    } else {
        words[0] = packet[0];
        words[1] = 0;
    }
    descriptors[count].source = packet;
    descriptors[count].destination = norrath_sound_command_dma_destination_word_view[0];
    descriptors[count].size = packet_size;
    words[2] = command;
    packet[0] = static_cast<unsigned char>(packet_size);
    descriptors[count].attributes = 0x44;
    sceSifWriteBackDCache(packet, packet_size);
    ++count;
    if (flags & 1u)
        return isceSifSetDma(descriptors, count);
    return sceSifSetDma(descriptors, count);
}
}
