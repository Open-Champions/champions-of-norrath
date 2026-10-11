#pragma once
#include "exec_detail.hpp"
#include "special.hpp"
namespace norrath_exec_transport {
// Register is imported by special.hpp.
using Address = norrath_exec_detail::Address;
// Native lower64 scalar writes update lo only. hi is captured, never seeded.
struct alignas(16) Reg128 { Register lo; Register hi; };
struct alignas(16) Context {
    Address next, parent, stack_low, stack_top;
    Register native_frame, worker_sp, worker_ra, target;
    Register reserved0, reserved1;
    Reg128 native[32];
    Reg128 worker_saved[32];
    Special special;
    Special worker_special;
};
struct StackSpan { Address low, top; };
static_assert(sizeof(Reg128)==16 && alignof(Reg128)==16, "full native register");
static_assert(__builtin_offsetof(Context,native_frame)==16, "shim layout");
static_assert(__builtin_offsetof(Context,worker_sp)==24, "shim layout");
static_assert(__builtin_offsetof(Context,worker_ra)==32, "shim layout");
static_assert(__builtin_offsetof(Context,target)==40, "shim layout");
static_assert(__builtin_offsetof(Context,native)==64, "shim layout");
static_assert(__builtin_offsetof(Context,worker_saved)==576, "shim layout");
static_assert(__builtin_offsetof(Context,special)==1088 && __builtin_offsetof(Context,worker_special)==1120, "special banks");
static_assert(sizeof(Special)==32 && sizeof(Context)==1152, "shim layout");
// These two globals are NEW private transport metadata, not retail data owners.
// Installation and every entry require single-threaded, non-IRQ transport use.
extern "C" Address norrath_exec_transport_free;
extern "C" Address norrath_exec_transport_active;
extern "C" bool norrath_exec_transport_install(Context*, unsigned, const StackSpan*);
extern "C" Register norrath_exec_transport_call(Context*);
extern "C" Register norrath_amx_exec_transport_worker(Context*);
// Each sequence moves the entire physical quadword, with no C++ read of
// indeterminate native backing. The context itself is live and initialized by
// entry capture. Extra accesses are private and excluded from game aliases.
__attribute__((always_inline)) inline void spill(Address at, const Reg128& value) {
    asm volatile("lq $24,0(%0)\n\tsq $24,0(%1)" : : "r"(&value), "r"(at) : "$24", "memory");
}
__attribute__((always_inline)) inline void reload(Address at, Reg128& value) {
    asm volatile("lq $24,0(%1)\n\tsq $24,0(%0)" : : "r"(&value), "r"(at) : "$24", "memory");
}
inline Register call(Context* c, Address target) {
    c->target=norrath_exec_detail::extend(target);
    return norrath_exec_transport_call(c);
}
}
