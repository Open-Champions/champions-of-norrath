// Observed storage in the pinned SLUS_205.65 image. Initial bytes are zero.
// Where the symbol has no size the span is the gap to the next symbol.
//   udpLaunchConnection      0x004839ec  4 bytes (D)
//   netLaunchTimer           0x004839f4  4 bytes (D)
//   netLaunchExpirationCount 0x00486830  gap 8 (local symbol, uninitialised)
//   netNewLaunchMaybeQuit    0x00486808  gap 1 (uninitialised)
//   netNewLaunchMaybeLaunch  0x00486809  gap 1 (uninitialised)
// Inferred: the first three are 32-bit values and the last two are single
// bytes, from the authored netDoNewLaunch and netStartNetworkGameInit that use
// them. Only 4 of the 8 bytes at netLaunchExpirationCount are claimed.
// Unknown: what the other 4 bytes of that gap hold.
extern "C" {
int udpLaunchConnection;
int netLaunchTimer;
int netLaunchExpirationCount;
signed char netNewLaunchMaybeQuit;
signed char netNewLaunchMaybeLaunch;
}
