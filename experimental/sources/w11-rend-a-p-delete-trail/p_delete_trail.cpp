// Observed P_DeleteTrail__Fi at 0x00184080, 60 bytes. The argument is a trail handle as
// returned by P_StartTrail. A zero handle returns at once. Otherwise slot = handle & 0xff
// selects the 16-byte header at trail + slot*1040 + 1024 (trail is the 64-record table at
// 0x00780b00). When the signed halfword at header+0 equals the full handle, that halfword
// is set to zero (which makes the slot free for P_StartTrail); otherwise nothing changes.
// Nothing is returned.
// Inferred: a stale handle (older sequence number for the same slot) is ignored.
extern "C" {
extern unsigned char trail[66560];
void P_DeleteTrail__Fi(int handle) {
    if (handle == 0) return;
    short* header = reinterpret_cast<short*>(trail + (handle & 0xff) * 1040 + 1024);
    if (*header == handle) *header = 0;
}
}
