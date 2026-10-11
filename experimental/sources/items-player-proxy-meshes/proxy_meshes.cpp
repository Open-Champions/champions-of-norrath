// Observed _4item$globalItem at 0x31f540, 106 records of 16 bytes.
// The two strings are relocated. Both trailing words are zero in every record.
struct ProxyMeshEntry {
    const char* model;
    const char* texture;
    unsigned unused_a;
    unsigned unused_b;
};
static_assert(sizeof(ProxyMeshEntry) == 16, "proxy mesh entry");
extern "C" const ProxyMeshEntry norrath_player_view_31f540[] = {
#include "rows.hpp"
};
