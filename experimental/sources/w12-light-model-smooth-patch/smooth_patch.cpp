#include "../w11-rend-a-common/rend.hpp"
// Observed smoothPatch__FP12_worldHeaderP16WaterPolysetData at 0x00178840, extent 632. Arguments: a
// world header and a water polyset record d. Returns nothing. Record words used: signed halfwords
// d+0, +2, +4, +6 (four neighbour patch numbers, -1 for none) and the word d+8, the address of
// this patch's grid. world+36 is a table of 60-byte patch records; word 0 of a record points
// at a patch header whose word +8 is that patch's grid address. A grid is 32 x 32 signed halfwords
// laid out with 80-byte rows starting 80 bytes in (cell (row, col) at grid + 80 + row * 80 + col * 2).
// Neighbours (grid addresses n0..n3, or null) are numbered d+0 -> n0, d+2 -> n1, d+4 -> n2, d+6 -> n3.
// For every cell (row 0..31, col 0..31), with k = 0.1f (a literal at 0x0048004c):
//  - n0 (when col < 10): weight (10 - col) * k, value = n0 halfword at +142 + row * 80 (its column 31).
//  - n2 (when row < 10): weight (10 - row) * k, value = n2 halfword at +2560 + col * 2 (its row 31).
//  - n1 (when col >= 22): weight (col - 21) * k, value = n1 halfword at +80 + row * 80 (its column 0).
//  - n3 (when row >= 22): weight (row - 21) * k, value = n3 halfword at +80 + col * 2 (its row 0).
// The terms add in that order, a sum of weights W and a sum of weight * value S (the first
// applicable term sets them). With W below 1.0f (including no terms) W becomes 1.0f. The cell gets
// (int)(S / W) as a halfword. A second 32 x 32 halfword grid at d.grid + 2704 (rows of 64 bytes) is
// cleared to zero. All cells are written, none of the neighbours' data is.
// Inferred: blends a water patch's edge heights with its four neighbours' adjacent edges over a
// ten-cell border so patches meet smoothly. Unknown: what the second grid is for, the meaning of
// the cell value (a height?), and why rows are 80 bytes wide for 32 cells. Nothing here was
// executed on the EE.
extern "C" {
void smoothPatch__FP12_worldHeaderP16WaterPolysetData(unsigned char* world, unsigned char* data) {
    using namespace rend;
    unsigned char* nb[4];
    for (int i = 0; i < 4; ++i) {
        const int index = at<i16>(data, i * 2);
        nb[i] = 0;
        if (index != -1) nb[i] = ptr(ptr(ptr(world, 36) + index * 60, 0), 8);
    }
    unsigned char* grid = ptr(data, 8);
    const float k = 0x1.999998p-4f;
    for (int row = 0; row < 32; ++row) {
        for (int col = 0; col < 32; ++col) {
            float sum = 0.0f;
            float weight = 0.0f;
            if (col < 10 && nb[0] != 0) {
                const float w = static_cast<float>(10 - col) * k;
                sum = static_cast<float>(at<i16>(nb[0], 142 + row * 80)) * w;
                weight = w;
            }
            if (row < 10 && nb[2] != 0) {
                const float w = static_cast<float>(10 - row) * k;
                weight = weight + w;
                sum = sum + static_cast<float>(at<i16>(nb[2], 2560 + col * 2)) * w;
            }
            if (col >= 22 && nb[1] != 0) {
                const float w = static_cast<float>(col - 21) * k;
                weight = weight + w;
                sum = sum + static_cast<float>(at<i16>(nb[1], 80 + row * 80)) * w;
            }
            if (row >= 22 && nb[3] != 0) {
                const float w = static_cast<float>(row - 21) * k;
                weight = weight + w;
                sum = sum + static_cast<float>(at<i16>(nb[3], 80 + col * 2)) * w;
            }
            if (weight < 1.0f) weight = 1.0f;
            at<i16>(grid, 2704 + row * 64 + col * 2) = 0;
            at<i16>(grid, 80 + row * 80 + col * 2) = static_cast<short>(static_cast<int>(sum / weight));
        }
    }
}
}
