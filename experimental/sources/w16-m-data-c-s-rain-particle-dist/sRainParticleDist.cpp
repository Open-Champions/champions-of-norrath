// sRainParticleDist: zeroed storage for an unresolved import. Closes the link name and extent only.
// Observed (source and reference-image level):
// - Reference image: a read-only symbol of size 0x258 (600 bytes) at 0x0118eec0 (reviewed EE nm).
// - Caller: w11-vi-render-a-atmosphere-init/init.cpp declares const unsigned char[600] and indexes it by 3 * i + k.
// - The published unit w15-m-data-b-s-rain-particle-dist defines the same name with a file-scope const
//   and no extern. Compiled with the reviewed EE flags, its object contains no sRainParticleDist symbol,
//   so it does not satisfy this import. This unit is the external definition; the w15 unit should be fixed or removed.
// Inferred:
// - Three bytes per entry, from the caller's index expression.
// Unknown:
// - The 600 table values. This unit supplies zeros.
// This unit does not establish any field value.
extern "C" const unsigned char sRainParticleDist[600] = {0};
static_assert(sizeof sRainParticleDist == 600u, "sRainParticleDist extent");
