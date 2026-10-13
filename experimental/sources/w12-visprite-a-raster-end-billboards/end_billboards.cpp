// Observed: EndBillboards__8VIRaster at 0x0110d1e0, 224 bytes. With the raster
// as this, n = the int at this + 17860 and slot = this + 19492 (a word that
// holds the write pointer P of a DMA packet being built):
//   If n <= 0 the result is -1 and nothing is touched.
//   Otherwise the word at P + 48 * n + 12 is OR-ed with 0x100. The four words
//   at P + 48 * n + 48 are set to 0, 0, 0 and 0x15000014 (word 3). The
//   quadword count c = 3 * n + 3 is stored as a halfword at P + 0, the byte
//   at P + 3 is set to 16 and the word at P + 4 to 0. The slot becomes
//   P + 16 * c + 16, and only after that store is
//   FlushDMABufferIfFull__8VIRaster(this) called. The int at this + 17860 is
//   set to zero and the result is zero.
// Inferred: the 0x15000014 word is a VIF microprogram-call code whose operand
// is the offset 0x14 = (168 - 8) >> 3 the image computes as the difference of
// two small constants (the symbols named o_cu and VIBbRender by the symbol
// dump sit at 8 and 168); P + 0 is a DMA tag whose count halfword and
// top byte are set here; 0x100 marks the last billboard's last quadword.
// Unknown: the names of the packet fields and what bit 0x100 means to the VU
// code.
extern "C" {
void FlushDMABufferIfFull__8VIRaster(void* raster);

int EndBillboards__8VIRaster(void* raster) {
    unsigned char* bytes = static_cast<unsigned char*>(raster);
    int count = *reinterpret_cast<int*>(bytes + 17860);
    if (count <= 0) return -1;
    unsigned char* packet = *reinterpret_cast<unsigned char**>(bytes + 19492);
    *reinterpret_cast<unsigned*>(packet + 48 * count + 12) |= 0x100;
    unsigned* closing = reinterpret_cast<unsigned*>(packet + 48 * count + 48);
    closing[0] = 0;
    closing[1] = 0;
    closing[2] = 0;
    closing[3] = 0x15000000u | ((168 - 8) >> 3);
    unsigned quadwords = 3 * count + 3;
    packet[3] = 16;
    *reinterpret_cast<unsigned*>(packet + 4) = 0;
    *reinterpret_cast<unsigned short*>(packet) = static_cast<unsigned short>(quadwords);
    *reinterpret_cast<unsigned char**>(bytes + 19492) = packet + 16 * quadwords + 16;
    FlushDMABufferIfFull__8VIRaster(raster);
    *reinterpret_cast<int*>(bytes + 17860) = 0;
    return 0;
}
}
