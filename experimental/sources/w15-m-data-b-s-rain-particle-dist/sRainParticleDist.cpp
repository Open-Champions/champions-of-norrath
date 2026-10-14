// sRainParticleDist: zeroed storage for an unresolved import of the w15-m-data-b package.
// Observed (source level):
// - Reference image: symbol size 0x258 (600 bytes), in the text section (nm type t), at 0x0118eec0 (reviewed EE nm). The caller declares it as const unsigned char[600].
// - Callers in published units that name it:
//     w11-vi-render-a-atmosphere-init/init.cpp (indexes the table by 3 * i + k for rain particle setup)
// Inferred:
// - Three bytes per entry, as the caller's 3 * i + k index suggests.
// Unknown:
// - The 600 table values. This unit is zero-filled storage only. A host contract in another package holds values of its own, which this unit does not reuse.
// This unit provides the link name and extent only. It does not establish any field value.
extern "C" {
const unsigned char sRainParticleDist[600] = {0};
}
static_assert(sizeof sRainParticleDist == 600u, "sRainParticleDist extent");
