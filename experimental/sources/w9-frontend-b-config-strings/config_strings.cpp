// Observed: configStrings at 0x00429388 is a 36-byte array of nine 4-byte
// pointers to NUL-terminated strings, in this order: "c_attack", "c_magic1",
// "c_magic2", "c_action", "c_block", "c_health", "c_mana", "c_automap",
// "c_weapontoggle". Seven of the strings are in read-only data and "c_block"
// and "c_mana" are stored in .sdata in the image.
// Inferred: the strings are text keys for the controller-configuration rows,
// from the "c_" prefix and the existing controller-config source that indexes
// by row; the order is the row order.
// Unknown: which localized text each key resolves to.
extern "C" {
const char* configStrings[9] = {
    "c_attack", "c_magic1", "c_magic2", "c_action", "c_block",
    "c_health", "c_mana",   "c_automap", "c_weapontoggle",
};
}
