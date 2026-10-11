#error "SOURCE HOLD: independent whole transport review and deployment domain required"
#include "transport.hpp"
namespace norrath_exec_transport {
extern "C" { Address norrath_exec_transport_free=0; Address norrath_exec_transport_active=0; }
// Caller owns live Context objects and disjoint stack allocations until it
// uninstalls at quiescence. This does not allocate retail storage or pick a
// gameplay recursion limit. False is configuration rejection, not an Exec result.
extern "C" bool norrath_exec_transport_install(Context* nodes, unsigned count, const StackSpan* spans) {
    if(norrath_exec_transport_active!=0) return false;
    if(count==0) { norrath_exec_transport_free=0; return true; }
    if(nodes==nullptr || spans==nullptr) return false;
    const Address base=norrath_constructor_detail::address(nodes);
    const Register end=Register(base)+Register(count)*sizeof(Context);
    if((base&15u)!=0 || end>0x100000000ull) return false;
    const Address span_base=norrath_constructor_detail::address(spans);
    const Register span_end=Register(span_base)+Register(count)*sizeof(StackSpan);
    if(span_end>0x100000000ull || (Register(span_base)<end && span_end>base)) return false;
    for(unsigned i=0;i<count;++i) {
        const Address low=spans[i].low, top=spans[i].top;
        if((low&15u)!=0 || (top&15u)!=0 || low>=top) return false;
        if(Register(low)<end && Register(top)>base) return false;
        for(unsigned j=0;j<i;++j)
            if(low<spans[j].top && top>spans[j].low) return false;
    }
    // No register payload or original frame byte is initialized here.
    for(unsigned i=0;i<count;++i) {
        nodes[i].next=i+1<count ? base+(i+1u)*sizeof(Context) : 0;
        nodes[i].parent=0;
        nodes[i].stack_low=spans[i].low;
        nodes[i].stack_top=spans[i].top;
    }
    norrath_exec_transport_free=base;
    return true;
}
}
