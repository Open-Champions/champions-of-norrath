// Observed drewHelpfulMessageHelpBar at 0x00484914, 4 bytes, section .sdata/.data (local d in the image).
// A local initialised word of 4 bytes, zero at start; only doHelpfulMessage__FiiP12screenStructiiPQ24item11description loads and stores it, as a whole word.
// Inferred: An int flag, zero at start, exported here under its image name so that callers resolve.
// Unknown: Exact set and clear points beyond what the callers show.
extern "C" {
int drewHelpfulMessageHelpBar = 0;
}
