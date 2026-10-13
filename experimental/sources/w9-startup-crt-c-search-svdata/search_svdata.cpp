// Observed _search_svdata at 0x002d7f40, 76 bytes, a local routine of the SIF RPC
// library, called by _request_bind. Arguments are a service id and the service list
// (s) the bind handler was registered with.
//
// Observed traversal:
//   s +40       first server record (zero when there are none)
//   record +8   first service entry of that record (zero when it has none)
//   record +20  next server record
//   entry +0    the entry's service id
//   entry +56   next entry of the same record
// It walks every entry of every record in list order and returns the first entry
// whose id equals the argument (word compare), or zero when none does. Nothing is
// written. The names (server record, service entry) are inferred only from
// _request_bind, which hands the found entry's address and its word at +8 back to
// the IOP; what the records are is not established.
extern "C" {
struct RpcServiceEntry {
    unsigned id;
    unsigned char between[52];
    RpcServiceEntry* next;
};

struct RpcServerRecord {
    unsigned char before_entries[8];
    RpcServiceEntry* entries;
    unsigned char between[8];
    RpcServerRecord* next;
};

struct RpcServiceList {
    unsigned char before_records[40];
    RpcServerRecord* records;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcServiceEntry, next) == 56, "layout");
static_assert(__builtin_offsetof(RpcServerRecord, entries) == 8, "layout");
static_assert(__builtin_offsetof(RpcServerRecord, next) == 20, "layout");
static_assert(__builtin_offsetof(RpcServiceList, records) == 40, "layout");
#endif

RpcServiceEntry* _search_svdata(unsigned service_id, RpcServiceList* services) {
    for (RpcServerRecord* record = services->records; record != 0; record = record->next) {
        for (RpcServiceEntry* entry = record->entries; entry != 0; entry = entry->next) {
            if (entry->id == service_id)
                return entry;
        }
    }
    return 0;
}
}
