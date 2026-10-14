// Storage for route_dirs at 0x008d8230 in the private image (SLUS_205.65), BSS. The next symbol
// (clericroots) is 0x60 bytes later, which is eight three-float directions. Observed: routeMakeLattice
// writes eight unit vectors at angles of 8192 * k (k = 0..7) here, z zero; routePlot reads them.
// Unknown: whether a fourth float per direction is padding (the extent fits exactly twelve bytes
// per direction, so there is none). Nothing here ran on the EE.
extern "C" {
alignas(16) float route_dirs[8][3];
}
