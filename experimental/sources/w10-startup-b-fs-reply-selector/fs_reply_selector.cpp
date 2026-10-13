/* Observed ee_retbuf_sel at 0x004449f0, a 4-byte initialised-data word in the symbol
 * table, zero in the image. _sceFs_Rcv_Intr stores the reply-buffer index it selected
 * (0 or the packet's selector word) here each time a reply arrives.
 * Unknown: whether anything reads it (nothing authored does). Placement differs from
 * the original: a zero-initialised definition here lands in uninitialised data.
 */
extern "C" int ee_retbuf_sel = 0;
