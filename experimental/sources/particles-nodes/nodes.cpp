// nodes at 0x007ad000, 121920 bytes, to oniSuckParticle.1175.
// 6096 records of 20. BSS is zero. listInit writes the links later.
// The .data object also named nodes, at 0x00438f08, is the route table
// published as norrath_route_nodes_bytes. This definition is the list pool.
extern "C" {
unsigned char nodes[6096 * 20];
}
static_assert(sizeof nodes == 121920u, "nodes extent");
