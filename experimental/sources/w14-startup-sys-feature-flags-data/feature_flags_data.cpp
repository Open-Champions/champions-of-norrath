/* Observed data object, pinned SLUS_205.65: sFeatureFlags at 0x0118e540, 4 bytes, in the overlay data range
 * (its symbol is local to the image). The word is zero at load. VIGetFeatureFlags returns it and
 * VISetFeatureFlags stores a new value (both read and write it by a single lui/lw or lui/sw pair).
 * Inferred: a bit set of optional engine features; no bit meanings are established here.
 * Unknown: which bits are ever set, and whether the original file-local static was zero-initialised
 * explicitly.
 */
extern "C" {
unsigned sFeatureFlags;
}
