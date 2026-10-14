// mStarParticleDist: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: mStarParticleDist at 0x0118f118, size 0x180, type t (read-only, in the text section).
//   - Caller: w11-vi-render-a-atmosphere-init/init.cpp reads it in steps of three bytes.
// Inferred:
//   - 384 bytes, from the symbol size and the caller stride.
// Unknown:
//   - The table values. The text-section placement is an observation of the image only.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
extern const unsigned char mStarParticleDist[384] = {};
}
static_assert(sizeof mStarParticleDist == 384u, "mStarParticleDist extent");
