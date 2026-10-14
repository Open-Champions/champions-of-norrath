// Observed: the sound/IOP interface code loaded above the main image has a local reply buffer
// sRecv at 0x011edd00, 512 bytes (the next symbol is 0x200 later), 64-byte aligned, zero in the
// image. IdleTransfer__13VISoundDevice reads its word 1 under the placeholder import name
// norrath_overlay_sRecv_11edd00. A second, unrelated set of sSend/sRecv objects exists near
// 0x011e2580 (the CD stream's, defined in w15-startup-a-data-cdstream-state).
// Authored: 512 bytes of zeroed, 64-byte aligned storage under that import name. If a unit
// that defines the overlay's sRecv under its own name (w12-startup-rt-c-overlay-iop-buffers) is
// planned as well, this name should become an alias of that object instead; plan only one.
// Unknown: who fills the buffer and what the reply holds.
extern "C" {
alignas(64) int norrath_overlay_sRecv_11edd00[128];
}
