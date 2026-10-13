/* Packet pool of the multi-client SIF RPC module, defined from the executable image.
 *
 * Observed: the object is the file-local `_data_table` at 0x008e8c00 in .bss, 64 bytes up to the
 * next symbol (`param.0` at 0x008e8c40). sceSifMBindRpcParam hands its address to the module's
 * packet getter (0x002e4700), so it is the pool header that getter walks.
 * Unknown: the field layout of the pool; no field is named or initialised here. The file-local
 * name is not unique in the image (other SIF modules have their own `_data_table`), so the
 * published name is a view.
 */
extern "C" {
alignas(64) unsigned char norrath_sif_m_rpc_packet_pool_view[64];
}
