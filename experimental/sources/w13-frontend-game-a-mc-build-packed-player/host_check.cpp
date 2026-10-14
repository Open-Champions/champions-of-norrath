// Host check entry for MC_BuildPackedPlayer__FP11playerEntryi. Not an EE plan input.
// The authored source is w10-front-a-mc-build-packed-player; its committed check lives in
// w11-front-a-mc-build-packed-player and builds and passes on the 64-bit host. This unit only puts that check
// beside the w13 package so it travels with the source it covers. A paired run against the
// original image on the private interpreter was repeated and shows no differences.
#include "../w11-front-a-mc-build-packed-player/host_check.cpp"
