// Observed pad at 0x0034beb8. 352 bytes in .data. Every file byte is zero.
// main reads the word at offset 8. launchSRS writes the words at offsets
// 4, 92, 180, and 268.
extern "C" {
alignas(4) unsigned char pad[0x160] = {};
}
