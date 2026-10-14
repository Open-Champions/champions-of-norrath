// Host contract for waterStep; not an EE input. The expected values for the spike case were worked out separately by a small
// row-at-a-time script. The authored function was also compared with the original in a private R5900 interpreter extended with the
// multimedia instructions the original uses (30 scenarios of random, extreme and saturating halfwords and random mask words; all
// original words but a trailing padding word executed; every scenario's changed bytes outside the scratchpad register save area
// matched).
#include "water_step.cpp"
#include <cstring>
alignas(16) static short heights[33 * 40];
alignas(16) static short velocities[33 * 32];
alignas(16) static unsigned short masks[64];
static short& height(int row, int lane) { return heights[(row + 1) * 40 + lane]; }
static short& velocity(int row, int lane) { return velocities[(row + 1) * 32 + lane]; }
static void clear(unsigned short mask) {
    std::memset(heights, 0, sizeof heights);
    std::memset(velocities, 0, sizeof velocities);
    for (unsigned short& m : masks) m = mask;
}
// the pointers handed over are those of row 0 (the routine also writes the row before it)
static void run() { waterStep(heights + 40, velocities + 32, masks); }
int main() {
    // a spike spreads to its neighbours: 800 at (5, 10) becomes 602 with velocity -198, and each of the 8 surrounding cells in the rows
    // 4..6 gets 25 (the three cells of rows 4 and 6, and the two beside it on row 5)
    clear(0xffff);
    height(5, 10) = 800;
    run();
    for (int row = -1; row < 32; ++row)
        for (int lane = 0; lane < 32; ++lane) {
            short want_h = 0, want_v = 0;
            if (row >= 4 && row <= 6 && lane >= 9 && lane <= 11) want_h = want_v = 25;
            if (row == 5 && lane == 10) { want_h = 602; want_v = -198; }
            if (height(row, lane) != want_h || velocity(row, lane) != want_v) return 1;
        }
    // the five unused lanes of every row are never touched
    clear(0xffff);
    for (int row = -1; row < 32; ++row)
        for (int lane = 32; lane < 40; ++lane) height(row, lane) = static_cast<short>(1000 + lane);
    height(3, 3) = 100;
    run();
    for (int row = -1; row < 32; ++row)
        for (int lane = 32; lane < 40; ++lane) if (height(row, lane) != static_cast<short>(1000 + lane)) return 2;
    // masks with the top bit clear stop every update: velocities become 0 and heights keep their values (no row above the first set)
    clear(0x0000);
    height(5, 10) = 800;
    run();
    for (int row = -1; row < 32; ++row)
        for (int lane = 0; lane < 32; ++lane) {
            if (velocity(row, lane) != 0) return 3;
            if (height(row, lane) != (row == 5 && lane == 10 ? 800 : 0)) return 4;
        }
    // one mask bit per row, most significant first: with only bit 10 set in every word, iteration k = 5 (which updates row 4) and
    // iteration k = 21 (row 20, from the second half of the table) are the only ones that let a velocity through
    clear(0x0400);
    height(5, 10) = 800;
    run();
    for (int row = -1; row < 32; ++row)
        for (int lane = 0; lane < 32; ++lane) {
            const short want = (row == 4 && lane >= 9 && lane <= 11) ? 25 : 0;
            if (velocity(row, lane) != want) return 5;
            if (height(row, lane) != (row == 5 && lane == 10 ? 800 : want)) return 7;
        }
    // saturation: the largest value never wraps
    clear(0xffff);
    for (int row = -1; row < 32; ++row)
        for (int lane = 0; lane < 32; ++lane) { height(row, lane) = 32767; velocity(row, lane) = 32767; }
    run();
    for (int row = 0; row < 31; ++row)
        for (int lane = 0; lane < 32; ++lane) if (height(row, lane) < 0 || velocity(row, lane) < 0) return 6;
    return 0;
}
