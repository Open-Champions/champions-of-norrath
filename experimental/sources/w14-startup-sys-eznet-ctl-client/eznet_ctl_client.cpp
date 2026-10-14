/* Observed data object, pinned SLUS_205.65: m_cd at 0x011e8500 in the overlay's zero-initialised storage
 * (its symbol is local to the image and has no recorded size). The next label, m_buf, is 64 bytes later,
 * so the extent is taken as 64 bytes. It is the RPC client of the eznetctl IOP service: initEzNetCtl binds
 * it, ezNetCtlDownInterface and ezNetCtlGetStatus call through it.
 * Published as one global so every routine of that library file can share the single object. The units
 * that currently declare their own file-local copies (w8-startup-sce-b-init-ez-net-ctl and
 * vi-launcher-eznet-down) address the same image object; until they refer to this definition they bind
 * and call a different copy at link time.
 * Unknown: the field layout (it is an SIF client record; its size is the label spacing only).
 */
extern "C" {
alignas(8) unsigned char m_cd[64];
}
