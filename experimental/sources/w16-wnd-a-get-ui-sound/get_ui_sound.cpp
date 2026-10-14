// Observed: GetUISound__F11VIUISoundId at 0x01031038 is 44 bytes. It takes the sound id as an unsigned value and
// returns the address of the id-th 8-byte record of the table UISounds (0x011c6100; 17 records, each an integer
// followed by a float in the image). An id of 17 or more yields the address of record 0 instead.
// Inferred: the integer in each record repeats its own index and the float is the volume passed on by the caller
// (SoundPlay__4VIUI11VIUISoundId reads the float at offset 4); the clamp to record 0 is a bounds guard.
// Unknown: the table's element type name and what the integer is for.
struct UISoundRecord {
    int index;
    float volume;
};
extern "C" {
extern UISoundRecord UISounds[17];

const UISoundRecord* GetUISound__F11VIUISoundId(unsigned id) {
    if (id < 17) return &UISounds[id];
    return &UISounds[0];
}
}
