// Observed storage of the audio library the image links in (lgAud*).
//   RPC client at 0x008ea780: passed to sceSifCallRpc and sceSifCheckStatRpc by lgAudOpen,
//   lgAudARead, lgAudAWrite, lgAudASync and lgAudStartPlayback. It is defined with 40 bytes, the
//   size of the client object other libraries in this image (libmtap) use; no symbol marks its end.
//   Asynchronous record at 0x008ea930: word 0 and word 1 receive the transfer address and the
//   count pointer, word 2 the result the completion routine leaves; lgAudASync reads word 2. It is
//   defined with 16 bytes, the most the observed code touches. The next symbol is 80 bytes later
//   (0x008ea980), so the true extent is between 12 and 80 bytes.
// Unknown: the client object's layout and the record's fourth word and beyond. Placement differs
// from the original: the link decides where these land.
extern "C" {
alignas(8) unsigned char norrath_lgaud_rpc_client_view[40];
alignas(4) unsigned norrath_lgaud_async_record_view[4];
}
