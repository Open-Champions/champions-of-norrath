/* Observed sceSdTransToIOP at 0x002e03d0, 132 bytes. Arguments: source
 * address, destination (IOP) address, size and a mode. A zero mode returns -1
 * with nothing else done. Otherwise the four words of a function-local
 * transfer record (the original keeps it as the local static transData.15 at
 * 0x008e59c0) are set to the source, destination, size and 0, and
 * sceSifSetDma(record, 1) queues it. A zero id returns -1. A mode of exactly 1
 * then polls sceSifDmaStat(id) until it is negative; any other nonzero mode
 * returns without waiting. The return value is 0 on both of those exits.
 * What the record attribute word 0 and the poll result mean to the IOP is
 * not established here.
 */
struct SifDmaData {
    void* source;
    unsigned destination;
    unsigned size;
    unsigned attribute;
};

extern "C" {
int sceSifSetDma(SifDmaData* data, int count);
int sceSifDmaStat(int id);

int sceSdTransToIOP(void* source, unsigned destination, unsigned size, int mode) {
    static SifDmaData transfer;
    if (mode == 0) return -1;
    transfer.source = source;
    transfer.destination = destination;
    transfer.size = size;
    transfer.attribute = 0;
    const int id = sceSifSetDma(&transfer, 1);
    if (id == 0) return -1;
    if (mode == 1) {
        while (sceSifDmaStat(id) >= 0) {
        }
    }
    return 0;
}
}
