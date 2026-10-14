// Observed __ti10VIResource at 0x01228970, 8 bytes of zero words in the SRS-loaded region of the reference image.
// It is the root node for VIResource; __tf8VISprite registers against it as its base.
// Inferred from that registration call. Unknown: the node fields after the first word, which __rtti_user writes.
extern "C" {
unsigned char __ti10VIResource[8] __attribute__((aligned(4))) = {};
}
