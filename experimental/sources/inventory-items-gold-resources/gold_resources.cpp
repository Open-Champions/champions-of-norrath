// Observed BSS labels goldModel (0x600d30) and goldTex (0x600d40), each with
// declared size 0. The labels around them are 16 bytes apart (goldAnm 0x600d20,
// goldRestModel 0x600d50), so each object is those 16 zero bytes. msg_draw__4Gold
// passes the address of each to ResourceMap::get. Their resource-map layout is
// not established.
extern "C" {
unsigned char goldModel[16];
unsigned char goldTex[16];
}
