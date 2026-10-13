// Observed: _6VIFile$sIopStream is a 4-byte word at 0x01192c84; the image word is zero. Read by
// IsIopStreamingEnabled__6VIFile, Open__6VIFilePCci and CDSync__6VIFilei; EnableIopStreaming__6VIFilei
// sits next to the reader in the image.
// Inferred: the class-wide switch between the direct disc path and the IOP stream path.
// Unknown: its writers beyond the enable function, and the range of values.
extern "C" {
int norrath_file_iop_stream __asm__("_6VIFile$sIopStream") = 0;
}
