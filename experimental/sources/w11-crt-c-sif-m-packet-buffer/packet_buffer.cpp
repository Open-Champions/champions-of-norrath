/* Observed: the symbol table names a file-local .bss object _packet_buffer at
 * 0x008e8400; the next symbol, _data_table (the packet pool header
 * norrath_sif_m_rpc_packet_pool_view), is at 0x008e8c00, so the span is 2048
 * bytes. sceSifMInitRpc stores 32 as the pool's packet count and 64 as the
 * offset it works at inside this buffer, and 32 packets of 64 bytes fill the
 * span exactly. The object is 1024-byte aligned in the original; 64 is kept
 * here.
 * Inferred: the span is 32 packets of 64 bytes.
 * Unknown: the packet layout beyond the words sceSifMInitRpc touches.
 */
extern "C" {
alignas(64) unsigned char norrath_sif_m_rpc_packet_buffer_view[2048];
}
