// Host check entry for MC_SetRamNmPlayers__Fi. Not an EE plan input.
// The authored source is w10-front-a-mc-set-ram-nm-players; its committed check lives in
// w11-front-a-mc-set-ram-nm-players and builds and passes on the 64-bit host. This unit only puts that check
// beside the w13 package so it travels with the source it covers. A paired run against the
// original image on the private interpreter was repeated and shows no differences.
#include "../w11-front-a-mc-set-ram-nm-players/host_check.cpp"
