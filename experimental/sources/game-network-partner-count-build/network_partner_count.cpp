#include "network_local.hpp"

// One genuine four-byte mutable word, using the existing byte-view interface.
// Independent zero initialization; no network startup routine is supplied.
// Original LOCAL identity is documented in the private source contract.
extern "C" {
    alignas(4) unsigned char norrath_network_partner_count_word_view[4] = {};
}
static_assert(sizeof(norrath_network_partner_count_word_view) == 4,
              "one complete original word representation");
