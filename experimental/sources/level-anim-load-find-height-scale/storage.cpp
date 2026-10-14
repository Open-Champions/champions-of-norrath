#include "storage.hpp"
// Observed unnamed literal at 0x00480024 in .lit4: the single float 0x38000100 that worldFindHeight at 0x00176378 loads through gp-31180 and multiplies by both orientation halfwords.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char norrath_find_height_orientation_scale_bytes[4] = { 0x00, 0x01, 0x00, 0x38 };
}
