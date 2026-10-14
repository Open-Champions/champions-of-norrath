// Whole readable Exec worker with real protected native word addresses; still HOLD.
#error "Native transport SOURCE HOLD: independent whole review required before any compile"
#include "script_start.hpp"
#include "init.hpp"
#include "memory_compare.hpp"
#include "exec_detail.hpp"
#include "native_frame.hpp"
#include "transport.hpp"
using namespace norrath_exec_detail;
extern "C" unsigned long long norrath_exec_transport::norrath_amx_exec_transport_worker(Context* transport) {
    using namespace norrath_constructor_detail;
    // Own C++ labels, in original opcode order; never copied original addresses.
    static void* const labels[] = {
        &&slot_0, &&slot_1, &&slot_2, &&slot_3, &&slot_4, &&slot_5, &&slot_6, &&slot_7,
        &&slot_8, &&slot_9, &&slot_10, &&slot_11, &&slot_12, &&slot_13, &&slot_14, &&slot_15,
        &&slot_16, &&slot_17, &&slot_18, &&slot_19, &&slot_20, &&slot_21, &&slot_22, &&slot_23,
        &&slot_24, &&slot_25, &&slot_26, &&slot_27, &&slot_28, &&slot_29, &&slot_30, &&slot_31,
        &&slot_32, &&slot_33, &&slot_34, &&slot_35, &&slot_36, &&slot_37, &&slot_38, &&slot_39,
        &&slot_40, &&slot_41, &&slot_42, &&slot_43, &&slot_44, &&slot_45, &&slot_46, &&slot_47,
        &&slot_48, &&slot_49, &&slot_50, &&slot_51, &&slot_52, &&slot_53, &&slot_54, &&slot_55,
        &&slot_56, &&slot_57, &&slot_58, &&slot_59, &&slot_60, &&slot_61, &&slot_62, &&slot_63,
        &&slot_64, &&slot_65, &&slot_66, &&slot_67, &&slot_68, &&slot_69, &&slot_70, &&slot_71,
        &&slot_72, &&slot_73, &&slot_74, &&slot_75, &&slot_76, &&slot_77, &&slot_78, &&slot_79,
        &&slot_80, &&slot_81, &&slot_82, &&slot_83, &&slot_84, &&slot_85, &&slot_86, &&slot_87,
        &&slot_88, &&slot_89, &&slot_90, &&slot_91, &&slot_92, &&slot_93, &&slot_94, &&slot_95,
        &&slot_96, &&slot_97, &&slot_98, &&slot_99, &&slot_100, &&slot_101, &&slot_102, &&slot_103,
        &&slot_104, &&slot_105, &&slot_106, &&slot_107, &&slot_108, &&slot_109, &&slot_110, &&slot_111,
        &&slot_112, &&slot_113, &&slot_114, &&slot_115, &&slot_116, &&slot_117, &&slot_118, &&slot_119,
        &&slot_120, &&slot_121, &&slot_122, &&slot_123, &&slot_124, &&slot_125, &&slot_126, &&slot_127,
        &&slot_128, &&slot_129, &&slot_130,
    };
    // Private query entry: returns this worker's actual 131-label table before
    // any Context member or native-frame access. Public query effects belong
    // to the entry shim; nullptr is never a fabricated execution context.
    if (transport == nullptr) return extend(address(labels));
    using Callback = unsigned long long (*)(void*, int, int*, void*);
    // Each named value retains the original native low64 lane. Address/word
    // operations explicitly wrap32 and sign extend. No shared cache refresh.
    Register& work0 = transport->native[2].lo;
    Register& work1 = transport->native[3].lo;
    Register& argument0 = transport->native[4].lo;
    Register& primary = transport->native[5].lo;
    Register& operand = transport->native[6].lo;
    Register& heap_gap = transport->native[7].lo;
    Register& callback = transport->native[8].lo;
    Register& endian = transport->native[9].lo;
    Register& alternate_below_heap = transport->native[10].lo;
    Register& frame_address = transport->native[11].lo;
    Register& code = transport->native[12].lo;
    Register& entry_stack = transport->native[13].lo;
    Register& cursor = transport->native[16].lo;
    Register& data = transport->native[17].lo;
    Register& alternate_address = transport->native[18].lo;
    Register& heap = transport->native[19].lo;
    Register& stack_address = transport->native[20].lo;
    Register& alternate = transport->native[21].lo;
    Register& stack = transport->native[22].lo;
    Register& state = transport->native[23].lo;
    Register& frame = transport->native[30].lo;
    const Address native_frame = low(transport->native_frame); // Original F; worker runs elsewhere.
    // Every original arithmetic effect updates the actual modeled HI/LO bank.
    // All original raw argument and T4/T5 full128 values were captured before C++ entry.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    state = argument0;
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    alternate = 0ull;
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // This exact physical argument word store precedes the worker call in entry_transport.S.
    { const bool taken = (state != 0ull);
        ; // This exact physical argument word store precedes the worker call in entry_transport.S.
        if (taken) goto flow_0;
    }
    work1 = extend(address(labels));
    work0 = 0ull;
    ; // The source-owned labels address is already complete.
    store_word(low(primary), work1);
    goto flow_73;
    ; // Original no-effect slot.
flow_0:
    callback = load_word(low(state) + 4u);
    { const bool taken = (callback == 0ull);
        work0 = extend(9u);
        if (taken) goto flow_73;
    }
    argument0 = load_word(low(state) + 32u);
    work1 = extend(4294950911u);
    work0 = extend(22u);
    work1 = (argument0 & work1);
    argument0 = (argument0 & 32768ull);
    { const bool taken = (argument0 == 0ull);
        store_word(low(state) + 32u, work1);
        if (taken) goto flow_73;
    }
    work0 = load_word(low(state));
    norrath_exec_native_frame::store_word(native_frame + 12u, work0);
    work0 = extend(7u);
    argument0 = norrath_exec_native_frame::load_word(native_frame + 12u);
    endian = norrath_exec_native_frame::load_word(native_frame + 12u);
    stack = load_word(low(state) + 24u);
    work1 = load_word(low(argument0) + 24u);
    entry_stack = stack;
    primary = load_word(low(endian) + 16u);
    argument0 = load_word(low(argument0) + 12u);
    work1 = (signed_register(work1) < signed_register(entry_stack)) ? 1ull : 0ull;
    heap = load_word(low(state) + 20u);
    data = add_word(endian, primary);
    { const bool taken = (work1 != 0ull);
        code = add_word(endian, argument0);
        if (taken) goto flow_73;
    }
    { const bool taken = (signed_register(heap) < 0ll);
        work0 = extend(8u);
        if (taken) goto flow_73;
    }
    work0 = extend(4294967295u);
    { const bool taken = (operand != work0);
        work0 = norrath_exec_native_frame::load_word(native_frame + 12u);
        if (taken) goto flow_1;
    }
    work1 = load_word(low(work0) + 28u);
    { const bool taken = (signed_register(work1) < 0ll);
        work0 = extend(20u);
        if (taken) goto flow_73;
    }
    cursor = add_word(code, work1);
    goto flow_3;
    ; // Original no-effect slot.
flow_1:
    { const bool taken = (signed_register(operand) >= 0ll);
        argument0 = norrath_exec_native_frame::load_word(native_frame + 12u);
        if (taken) goto flow_2;
    }
    work1 = extend(4294967294u);
    { const bool taken = (operand != work1);
        work0 = extend(20u);
        if (taken) goto flow_73;
    }
    argument0 = norrath_exec_native_frame::load_word(native_frame + 12u);
flow_2:
    work1 = load_signed_half(low(argument0) + 32u);
    work1 = (signed_register(operand) < signed_register(work1)) ? 1ull : 0ull;
    { const bool taken = (work1 == 0ull);
        work0 = extend(20u);
        if (taken) goto flow_73;
    }
    primary = norrath_exec_native_frame::load_word(native_frame + 12u);
    work1 = shift_left(operand, 1u);
    work1 = add_word(work1, operand);
    work0 = load_word(low(primary) + 36u);
    work1 = shift_left(work1, 3u);
    work0 = add_word(primary, work0);
    work0 = add_word(work0, work1);
    argument0 = load_word(low(work0));
    cursor = add_word(code, argument0);
flow_3:
    operand = shift_left(heap_gap, 2u);
    { const bool taken = (signed_register(heap_gap) <= 0ll);
        stack = sub_word(entry_stack, operand);
        if (taken) goto flow_5;
    }
    argument0 = norrath_exec_native_frame::load_word(native_frame + 0u);
    primary = add_word(data, stack);
flow_4:
    work0 = argument0;
    argument0 = add_word(argument0, extend(8u));
    work1 = load_word(low(work0));
    heap_gap = add_word(heap_gap, extend(4294967295u));
    store_word(low(primary), work1);
    { const bool taken = (heap_gap != 0ull);
        primary = add_word(primary, extend(4u));
        if (taken) goto flow_4;
    }
    norrath_exec_native_frame::store_word(native_frame + 0u, argument0);
flow_5:
    stack = add_word(stack, extend(4294967292u));
    heap_gap = add_word(heap, extend(64u));
    work0 = add_word(data, stack);
    stack = add_word(stack, extend(4294967292u));
    stack_address = add_word(data, stack);
    work1 = (signed_register(stack) < signed_register(heap_gap)) ? 1ull : 0ull;
    store_word(low(work0), operand);
    work0 = extend(3u);
    { const bool taken = (work1 != 0ull);
        store_word(low(stack_address), 0ull);
        if (taken) goto flow_73;
    }
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame = 0ull;
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(0ull) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    frame_address = data;
    { const Address next_handler = low(work0);
        alternate_address = data;
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_0: // restore entry stack offset to AMX+24; return status6
    store_word(low(state) + 24u, entry_stack);
    work0 = extend(6u);
    goto flow_73;
    ; // Original no-effect slot.

slot_1: // PRI = word at data+immediate offset
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    primary = load_word(low(work0));
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_2: // ALT = word at data+immediate offset
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = add_word(data, operand);
    frame_address = add_word(data, frame);
    alternate = load_word(low(work0));
    stack_address = add_word(data, stack);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    heap_gap = add_word(heap, extend(64u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work1);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_3: // PRI = word at data+frame+immediate offset
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    work0 = add_word(frame_address, operand);
    stack_address = add_word(data, stack);
    primary = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_4: // ALT = word at data+frame+immediate offset
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = add_word(frame_address, operand);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate = load_word(low(work0));
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work1);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_5: // PRI = word at data+word(data+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    operand = load_word(low(work0));
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    work0 = add_word(data, operand);
    callback = load_word(low(state) + 4u);
    primary = load_word(low(work0));
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_6: // ALT = word at data+word(data+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work1 = add_word(data, operand);
    frame_address = add_word(data, frame);
    operand = load_word(low(work1));
    stack_address = add_word(data, stack);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    heap_gap = add_word(heap, extend(64u));
    work1 = add_word(data, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    alternate = load_word(low(work1));
    callback = load_word(low(state) + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    { const Address next_handler = low(work0);
        alternate_address = add_word(data, alternate);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_7: // PRI = word at data+word(data+frame+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    work0 = add_word(frame_address, operand);
    stack_address = add_word(data, stack);
    operand = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = add_word(data, operand);
    primary = load_word(low(work0));
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_8: // ALT = word at data+word(data+frame+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work1 = add_word(frame_address, operand);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    operand = load_word(low(work1));
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work1 = add_word(data, operand);
    callback = load_word(low(state) + 4u);
    alternate = load_word(low(work1));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    { const Address next_handler = low(work0);
        alternate_address = add_word(data, alternate);
        goto *pointer(next_handler);
    }

slot_9: // checked indirect PRI word load through data+PRI
    work0 = (signed_register(primary) < signed_register(heap)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        work0 = load_word(low(state) + 28u);
        goto flow_6;
    }
    work0 = (signed_register(primary) < signed_register(stack)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = load_word(low(state) + 28u);
flow_6:
    work0 = (signed_register(primary) < signed_register(work0)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = add_word(data, primary);
    frame_address = add_word(data, frame);
    primary = load_word(low(work0));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_10: // checked PRI load width immediate1/2/4; other widths leave PRI unchanged
    work0 = (signed_register(primary) < signed_register(heap)) ? 1ull : 0ull;
    operand = load_word(low(cursor));
    { const bool taken = (work0 != 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_7;
    }
    work0 = (signed_register(primary) < signed_register(stack)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
flow_7:
    work0 = load_word(low(state) + 28u);
    work0 = (signed_register(primary) < signed_register(work0)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = extend(2u);
    { const bool taken = (operand == work0);
        work0 = (signed_register(operand) < signed_register(extend(3u))) ? 1ull : 0ull;
        if (taken) goto flow_10;
    }
    { const bool taken = (work0 == 0ull);
        work0 = extend(4u);
        if (taken) goto flow_8;
    }
    work0 = extend(1u);
    if (operand == work0) {
        work0 = add_word(data, primary);
        goto flow_9;
    }
    work0 = load_word(low(cursor));
    goto flow_13;
    ; // Original no-effect slot.
flow_8:
    if (operand == work0) {
        work0 = add_word(data, primary);
        goto flow_11;
    }
    work0 = load_word(low(cursor));
    goto flow_13;
flow_9:
    primary = load_byte(low(work0));
    goto flow_12;
flow_10:
    work0 = add_word(data, primary);
    primary = load_half(low(work0));
    goto flow_12;
    ; // Original no-effect slot.
flow_11:
    primary = load_word(low(work0));
flow_12:
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
flow_13:
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_11: // PRI = immediate
    primary = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_12: // ALT = immediate
    alternate = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_13: // PRI = frame+immediate
    primary = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    primary = add_word(primary, frame);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_14: // ALT = frame+immediate
    alternate = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = add_word(alternate, frame);
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_15: // store PRI at data+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    store_word(low(work0), primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_16: // store ALT at data+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    store_word(low(work0), alternate);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_17: // store PRI at data+frame+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    store_word(low(work0), primary);
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_18: // store ALT at data+frame+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    store_word(low(work0), alternate);
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_19: // store PRI at data+word(data+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    operand = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = add_word(data, operand);
    store_word(low(work0), primary);
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work1);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_20: // store ALT at data+word(data+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    operand = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = add_word(data, operand);
    store_word(low(work0), alternate);
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work1);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_21: // store PRI at data+word(data+frame+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    operand = load_word(low(work0));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = add_word(data, operand);
    store_word(low(work0), primary);
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work1);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_22: // store ALT at data+word(data+frame+immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    operand = load_word(low(work0));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = add_word(data, operand);
    store_word(low(work0), alternate);
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work1);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_23: // checked store PRI at data+ALT
    if (alternate_below_heap != 0ull) {
        work0 = load_word(low(state) + 28u);
        goto flow_14;
    }
    work0 = (signed_register(alternate) < signed_register(stack)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = load_word(low(state) + 28u);
flow_14:
    work0 = (signed_register(alternate) < signed_register(work0)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_address = add_word(data, alternate);
    frame_address = add_word(data, frame);
    stack_address = add_word(data, stack);
    store_word(low(alternate_address), work0);
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_24: // checked store PRI width immediate1/2/4 at data+ALT; other widths no store
    operand = load_word(low(cursor));
    { const bool taken = (alternate_below_heap != 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_15;
    }
    work0 = (signed_register(alternate) < signed_register(stack)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
flow_15:
    work0 = load_word(low(state) + 28u);
    work0 = (signed_register(alternate) < signed_register(work0)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = extend(2u);
    { const bool taken = (operand == work0);
        work0 = (signed_register(operand) < signed_register(extend(3u))) ? 1ull : 0ull;
        if (taken) goto flow_18;
    }
    { const bool taken = (work0 == 0ull);
        work0 = extend(4u);
        if (taken) goto flow_16;
    }
    work0 = extend(1u);
    if (operand == work0) {
        work0 = norrath_exec_native_frame::load_byte(native_frame + 4u);
        goto flow_17;
    }
    work0 = load_word(low(cursor));
    goto flow_21;
flow_16:
    if (operand == work0) {
        work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
        goto flow_19;
    }
    work0 = load_word(low(cursor));
    goto flow_21;
flow_17:
    alternate_address = add_word(data, alternate);
    store_byte(low(alternate_address), work0);
    goto flow_20;
    ; // Original no-effect slot.
flow_18:
    work0 = norrath_exec_native_frame::load_half(native_frame + 4u);
    alternate_address = add_word(data, alternate);
    store_half(low(alternate_address), work0);
    goto flow_20;
flow_19:
    alternate_address = add_word(data, alternate);
    store_word(low(alternate_address), work0);
flow_20:
    work0 = load_word(low(cursor));
flow_21:
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_25: // checked PRI = word at data+ALT+(PRI<<2)
    work0 = shift_left(primary, 2u);
    operand = add_word(work0, alternate);
    work1 = (signed_register(operand) < signed_register(heap)) ? 1ull : 0ull;
    if (work1 != 0ull) {
        work0 = load_word(low(state) + 28u);
        goto flow_22;
    }
    work0 = (signed_register(operand) < signed_register(stack)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = load_word(low(state) + 28u);
flow_22:
    work0 = (signed_register(operand) < signed_register(work0)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = add_word(data, operand);
    frame_address = add_word(data, frame);
    primary = load_word(low(work0));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_26: // checked PRI = word at data+ALT+(PRI<<immediate)
    operand = load_word(low(cursor));
    work0 = shift_left(primary, operand);
    operand = add_word(work0, alternate);
    work1 = (signed_register(operand) < signed_register(heap)) ? 1ull : 0ull;
    { const bool taken = (work1 != 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_23;
    }
    work0 = (signed_register(operand) < signed_register(stack)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
flow_23:
    work0 = load_word(low(state) + 28u);
    work0 = (signed_register(operand) < signed_register(work0)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_24;
    }
    work0 = add_word(data, operand);
    frame_address = add_word(data, frame);
    primary = load_word(low(work0));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
flow_24:
    work0 = extend(5u);
    goto flow_73;

slot_27: // PRI = ALT+(PRI<<2)
    work0 = shift_left(primary, 2u);
    frame_address = add_word(data, frame);
    primary = add_word(work0, alternate);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_28: // PRI = ALT+(PRI<<immediate)
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = shift_left(primary, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    primary = add_word(work0, alternate);
    stack_address = add_word(data, stack);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_29: // if endian word nonzero and unsigned immediate<4 then PRI xor=4-immediate
    operand = load_word(low(cursor));
    { const bool taken = (endian == 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_25;
    }
    work0 = (operand < extend(4u)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        work0 = load_word(low(cursor));
        goto flow_26;
    }
    work1 = norrath_exec_native_frame::load_word(native_frame + 4u);
    work0 = extend(4u);
    work0 = sub_word(work0, operand);
    primary = (work1 ^ work0);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
flow_25:
    work0 = load_word(low(cursor));
flow_26:
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_30: // if endian word nonzero and unsigned immediate<4 then ALT xor=4-immediate
    operand = load_word(low(cursor));
    { const bool taken = (endian == 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_27;
    }
    work0 = (operand < extend(4u)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        work0 = load_word(low(cursor));
        goto flow_28;
    }
    work0 = extend(4u);
    work0 = sub_word(work0, operand);
    alternate = (alternate ^ work0);
flow_27:
    work0 = load_word(low(cursor));
flow_28:
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_31: // seven-way control read selector; out-of-range leaves PRI unchanged
    operand = load_word(low(cursor));
    work0 = (operand < extend(7u)) ? 1ull : 0ull;
    { const bool taken = (work0 == 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_37;
    }
    work0 = shift_left(operand, 2u);
    switch (low(operand)) {
    case 0u: goto flow_29;
    case 1u: goto flow_30;
    case 2u: goto flow_31;
    case 3u: goto flow_32;
    case 4u: goto flow_33;
    case 5u: goto flow_34;
    case 6u: goto flow_35;
    }
    __builtin_unreachable(); // Only after original unsigned <7 guard.
flow_29:
    endian = norrath_exec_native_frame::load_word(native_frame + 12u);
    primary = load_word(low(endian) + 12u);
    goto flow_36;
    ; // Original no-effect slot.
flow_30:
    work0 = norrath_exec_native_frame::load_word(native_frame + 12u);
    primary = load_word(low(work0) + 16u);
    goto flow_36;
    ; // Original no-effect slot.
flow_31:
    norrath_exec_native_frame::store_word(native_frame + 4u, heap);
    goto flow_37;
flow_32:
    primary = load_word(low(state) + 28u);
    goto flow_36;
flow_33:
    norrath_exec_native_frame::store_word(native_frame + 4u, stack);
    goto flow_37;
flow_34:
    norrath_exec_native_frame::store_word(native_frame + 4u, frame);
    goto flow_37;
flow_35:
    primary = sub_word(cursor, code);
flow_36:
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
flow_37:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_32: // seven-way control write selector; only heap/stack/frame/code-pointer cases mutate
    operand = load_word(low(cursor));
    work0 = (operand < extend(7u)) ? 1ull : 0ull;
    { const bool taken = (work0 == 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_42;
    }
    work0 = shift_left(operand, 2u);
    switch (low(operand)) {
    case 0u: goto flow_42;
    case 1u: goto flow_42;
    case 2u: goto flow_38;
    case 3u: goto flow_42;
    case 4u: goto flow_39;
    case 5u: goto flow_40;
    case 6u: goto flow_41;
    }
    __builtin_unreachable(); // Only after original unsigned <7 guard.
flow_38:
    heap = norrath_exec_native_frame::load_word(native_frame + 4u);
    goto flow_42;
flow_39:
    stack = norrath_exec_native_frame::load_word(native_frame + 4u);
    goto flow_42;
flow_40:
    frame = norrath_exec_native_frame::load_word(native_frame + 4u);
    goto flow_42;
flow_41:
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    cursor = add_word(code, primary);
flow_42:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_33: // PRI = ALT
    primary = alternate;
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(primary) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, primary);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_34: // ALT = PRI
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = primary;
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(primary) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, primary);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_35: // exchange PRI and ALT
    operand = primary;
    primary = alternate;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate = operand;
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_36: // push PRI by stack-=4 then store
    stack = add_word(stack, extend(4294967292u));
    frame_address = add_word(data, frame);
    stack_address = add_word(data, stack);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    store_word(low(stack_address), primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_37: // push ALT by stack-=4 then store
    stack = add_word(stack, extend(4294967292u));
    frame_address = add_word(data, frame);
    stack_address = add_word(data, stack);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    store_word(low(stack_address), alternate);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_39: // push immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    stack = add_word(stack, extend(4294967292u));
    frame_address = add_word(data, frame);
    stack_address = add_word(data, stack);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    store_word(low(stack_address), operand);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_38: // repeat push PRI immediate count using decrement-to-minus-one loop
    operand = load_word(low(cursor));
    work0 = extend(4294967295u);
    operand = add_word(operand, extend(4294967295u));
    { const bool taken = (operand == work0);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_44;
    }
    work1 = extend(4294967295u);
flow_43:
    stack = add_word(stack, extend(4294967292u));
    work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    stack_address = add_word(data, stack);
    operand = add_word(operand, extend(4294967295u));
    ; // Original no-effect slot.
    { const bool taken = (operand != work1);
        store_word(low(stack_address), work0);
        if (taken) goto flow_43;
    }
flow_44:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_40: // push word at data+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    stack = add_word(stack, extend(4294967292u));
    frame_address = add_word(data, frame);
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    work1 = load_word(low(work0));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    store_word(low(stack_address), work1);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_41: // push word at data+frame+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    stack = add_word(stack, extend(4294967292u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(frame_address, operand);
    stack_address = add_word(data, stack);
    work1 = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    store_word(low(stack_address), work1);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_42: // pop PRI then stack+=4
    primary = load_word(low(stack_address));
    stack = add_word(stack, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_43: // pop ALT then stack+=4
    alternate = load_word(low(stack_address));
    stack = add_word(stack, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_44: // ALT = old stack; stack+=immediate; gap and header-limit checks
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = stack;
    stack = add_word(stack, operand);
    work1 = (signed_register(stack) < signed_register(heap_gap)) ? 1ull : 0ull;
    { const bool taken = (work1 != 0ull);
        work0 = extend(3u);
        if (taken) goto flow_73;
    }
    argument0 = norrath_exec_native_frame::load_word(native_frame + 12u);
    work1 = load_word(low(argument0) + 24u);
    work1 = (signed_register(work1) < signed_register(stack)) ? 1ull : 0ull;
    { const bool taken = (work1 != 0ull);
        work0 = extend(7u);
        if (taken) goto flow_73;
    }
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_45: // ALT = old heap; heap+=immediate; gap and negative-heap checks
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = heap;
    heap = add_word(alternate, operand);
    heap_gap = add_word(heap, extend(64u));
    work1 = (signed_register(stack) < signed_register(heap_gap)) ? 1ull : 0ull;
    { const bool taken = (work1 != 0ull);
        work0 = extend(3u);
        if (taken) goto flow_73;
    }
    { const bool taken = (signed_register(heap) < 0ll);
        work0 = extend(8u);
        if (taken) goto flow_73;
    }
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_46: // push old frame; frame = stack
    stack = add_word(stack, extend(4294967292u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    store_word(low(stack_address), frame);
    frame = stack;
    frame_address = stack_address;
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_47: // pop frame/return offset; zero return publishes PRI and restores entry stack
    stack = add_word(stack, extend(4u));
    frame = load_word(low(stack_address));
    work0 = add_word(data, stack);
    stack = add_word(stack, extend(4u));
    operand = load_word(low(work0));
    { const bool taken = (operand != 0ull);
        cursor = add_word(code, operand);
        if (taken) goto flow_46;
    }
    primary = norrath_exec_native_frame::load_word(native_frame + 8u);
    if (primary == 0ull) {
        store_word(low(state) + 24u, entry_stack);
        goto flow_45;
    }
    work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    store_word(low(primary), work0);
    store_word(low(state) + 24u, entry_stack);
flow_45:
    work0 = 0ull;
    goto flow_73;
    ; // Original no-effect slot.
flow_46:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_48: // pop frame/return offset/argument byte count; adjust stack; zero return publishes PRI
    stack = add_word(stack, extend(4u));
    frame = load_word(low(stack_address));
    argument0 = add_word(data, stack);
    stack = add_word(stack, extend(4u));
    work1 = add_word(data, stack);
    operand = load_word(low(argument0));
    work0 = load_word(low(work1));
    cursor = add_word(code, operand);
    work0 = add_word(stack, work0);
    stack = add_word(work0, extend(4u));
    { const bool taken = (operand != 0ull);
        store_word(low(state) + 24u, stack);
        if (taken) goto flow_47;
    }
    endian = norrath_exec_native_frame::load_word(native_frame + 8u);
    if (endian == 0ull) {
        work0 = 0ull;
        goto flow_73;
    }
    work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    store_word(low(endian), work0);
    goto flow_45;
flow_47:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_49: // push code-relative return offset plus4; code pointer = absolute operand cell
    work0 = sub_word(cursor, code);
    stack = add_word(stack, extend(4294967292u));
    work0 = add_word(work0, extend(4u));
    stack_address = add_word(data, stack);
    store_word(low(stack_address), work0);
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    heap_gap = add_word(heap, extend(64u));
    cursor = load_word(low(cursor));
    alternate_address = add_word(data, alternate);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_50: // push code-relative return offset; code pointer = codebase+PRI
    stack = add_word(stack, extend(4294967292u));
    work0 = sub_word(cursor, code);
    stack_address = add_word(data, stack);
    frame_address = add_word(data, frame);
    store_word(low(stack_address), work0);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    cursor = add_word(code, primary);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_51: // code pointer = absolute operand cell
    cursor = load_word(low(cursor));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_52: // relative jump: fetch next handler at old operand pointer+immediate+4
    operand = load_word(low(cursor));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(cursor, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    work1 = load_word(low(work0) + 4u);
    cursor = add_word(work0, extend(8u));
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work1);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_53: // jump absolute operand if PRI equals zero else skip operand
    if (primary != 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_48;
    }
    cursor = load_word(low(cursor));
flow_48:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_54: // jump absolute operand if PRI not zero else skip operand
    if (primary == 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_49;
    }
    cursor = load_word(low(cursor));
flow_49:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_55: // jump absolute operand if PRI equals ALT else skip operand
    if (primary != alternate) {
        cursor = add_word(cursor, extend(4u));
        goto flow_50;
    }
    cursor = load_word(low(cursor));
flow_50:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_56: // jump absolute operand if PRI differs ALT else skip operand
    if (primary == alternate) {
        cursor = add_word(cursor, extend(4u));
        goto flow_51;
    }
    cursor = load_word(low(cursor));
flow_51:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_57: // jump absolute operand if PRI unsigned-less ALT
    work0 = (primary < alternate) ? 1ull : 0ull;
    if (work0 == 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_52;
    }
    cursor = load_word(low(cursor));
flow_52:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_58: // jump absolute operand if PRI unsigned-less-or-equal ALT
    work0 = (alternate < primary) ? 1ull : 0ull;
    if (work0 != 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_53;
    }
    cursor = load_word(low(cursor));
flow_53:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_59: // jump absolute operand if PRI unsigned-greater ALT
    work0 = (alternate < primary) ? 1ull : 0ull;
    if (work0 == 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_54;
    }
    cursor = load_word(low(cursor));
flow_54:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_60: // jump absolute operand if PRI unsigned-greater-or-equal ALT
    work0 = (primary < alternate) ? 1ull : 0ull;
    if (work0 != 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_55;
    }
    cursor = load_word(low(cursor));
flow_55:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_61: // jump absolute operand if PRI signed-less ALT
    work0 = (signed_register(primary) < signed_register(alternate)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_56;
    }
    cursor = load_word(low(cursor));
flow_56:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_62: // jump absolute operand if PRI signed-less-or-equal ALT
    work0 = (signed_register(alternate) < signed_register(primary)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_57;
    }
    cursor = load_word(low(cursor));
flow_57:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_63: // jump absolute operand if PRI signed-greater ALT
    work0 = (signed_register(alternate) < signed_register(primary)) ? 1ull : 0ull;
    if (work0 == 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_58;
    }
    cursor = load_word(low(cursor));
flow_58:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_64: // jump absolute operand if PRI signed-greater-or-equal ALT
    work0 = (signed_register(primary) < signed_register(alternate)) ? 1ull : 0ull;
    if (work0 != 0ull) {
        cursor = add_word(cursor, extend(4u));
        goto flow_59;
    }
    cursor = load_word(low(cursor));
flow_59:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_65: // PRI = PRI shifted-left by ALT low5
    primary = shift_left(primary, alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_66: // PRI = PRI logical-shift-right by ALT low5
    primary = shift_right(primary, alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_67: // PRI = PRI arithmetic-shift-right by ALT low5
    primary = shift_signed(primary, alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_68: // PRI shift-left by immediate low5
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    primary = shift_left(primary, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_69: // ALT shift-left by immediate low5
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = shift_left(alternate, operand);
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_70: // PRI logical-shift-right by immediate low5
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    primary = shift_right(primary, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_71: // ALT logical-shift-right by immediate low5
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = shift_right(alternate, operand);
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_72: // PRI = low32 signed MULT result PRI*ALT; HI/LO side effects retained in original
    primary = norrath_exec_transport::signed_multiply(transport->special, primary, alternate);
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_73: // signed division PRI by ALT with triple-DIV remainder adjustment; zero status11
    { const bool taken = (alternate == 0ull);
        work1 = norrath_exec_native_frame::load_word(native_frame + 4u);
        if (taken) goto flow_60;
    }
    frame_address = add_word(data, frame);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    norrath_exec_transport::signed_divide(transport->special, work1, alternate);
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    work0 = transport->special.hi0;
    work0 = add_word(work0, alternate);
    norrath_exec_transport::signed_divide(transport->special, work0, alternate);
    operand = transport->special.hi0;
    work1 = sub_word(work1, operand);
    norrath_exec_transport::signed_divide(transport->special, work1, alternate);
    alternate = operand;
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    alternate_address = add_word(data, alternate);
    primary = transport->special.lo0;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_74: // signed division ALT by PRI with triple-DIV remainder adjustment; zero status11
    { const bool taken = (primary == 0ull);
        frame_address = add_word(data, frame);
        if (taken) goto flow_60;
    }
    norrath_exec_transport::signed_divide(transport->special, alternate, primary);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    work1 = transport->special.hi0;
    work1 = add_word(work1, primary);
    norrath_exec_transport::signed_divide(transport->special, work1, primary);
    operand = transport->special.hi0;
    work0 = sub_word(alternate, operand);
    alternate = operand;
    norrath_exec_transport::signed_divide(transport->special, work0, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    alternate_address = add_word(data, alternate);
    primary = transport->special.lo0;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_75: // PRI = low32 signed MULT result PRI*ALT (separate dispatch entry)
    primary = norrath_exec_transport::signed_multiply(transport->special, primary, alternate);
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_76: // unsigned divide PRI by ALT; PRI quotient ALT remainder; zero status11
    { const bool taken = (alternate == 0ull);
        frame_address = add_word(data, frame);
        if (taken) goto flow_60;
    }
    work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    norrath_exec_transport::unsigned_divide(transport->special, work0, alternate);
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    primary = transport->special.lo0;
    operand = transport->special.hi0;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate = operand;
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_77: // unsigned divide ALT by PRI; PRI quotient ALT remainder; zero status11
    if (primary != 0ull) {
        norrath_exec_transport::unsigned_divide(transport->special, alternate, primary);
        goto flow_61;
    }
flow_60:
    store_word(low(state) + 24u, entry_stack);
    work0 = extend(11u);
    goto flow_73;
    ; // Original no-effect slot.
flow_61:
    frame_address = add_word(data, frame);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    primary = transport->special.lo0;
    operand = transport->special.hi0;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate = operand;
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_78: // PRI = low32 PRI+ALT
    primary = add_word(primary, alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_79: // PRI = low32 PRI-ALT
    primary = sub_word(primary, alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_80: // PRI = low32 ALT-PRI
    primary = sub_word(alternate, primary);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_81: // PRI bitwise-and ALT
    primary = (primary & alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_82: // PRI bitwise-or ALT
    primary = (primary | alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_83: // PRI bitwise-xor ALT
    primary = (primary ^ alternate);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_84: // PRI = unsigned native-register comparison PRI<1
    primary = (primary < extend(1u)) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_85: // PRI = low32 zero-PRI
    primary = sub_word(0ull, primary);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_86: // PRI = native low64 bitwise-not PRI
    primary = ~(0ull | primary);
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_87: // PRI = low32 PRI+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    primary = add_word(primary, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_88: // PRI = low32 signed MULT PRI*immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    primary = norrath_exec_transport::signed_multiply(transport->special, primary, operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }

slot_89: // PRI = zero
    norrath_exec_native_frame::store_word(native_frame + 4u, 0ull);
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    { const Address next_handler = low(work0);
        primary = 0ull;
        goto *pointer(next_handler);
    }

slot_90: // ALT = zero
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = 0ull;
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = data;
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_91: // store zero word at data+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    store_word(low(work0), 0ull);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_92: // store zero word at data+frame+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    store_word(low(work0), 0ull);
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_93: // if PRI lowbyte>=128 then OR negative256; otherwise retain all existing bits
    work0 = (primary & 255ull);
    work0 = (signed_register(work0) < signed_register(extend(128u))) ? 1ull : 0ull;
    if (work0 != 0ull) {
        work0 = load_word(low(cursor));
        goto flow_62;
    }
    work0 = extend(4294967040u);
    primary = (primary | work0);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    work0 = load_word(low(cursor));
flow_62:
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_94: // if ALT lowbyte>=128 then OR negative256; otherwise retain all existing bits
    work0 = (alternate & 255ull);
    work0 = (signed_register(work0) < signed_register(extend(128u))) ? 1ull : 0ull;
    if (work0 != 0ull) {
        work0 = load_word(low(cursor));
        goto flow_63;
    }
    work0 = extend(4294967040u);
    alternate = (alternate | work0);
    work0 = load_word(low(cursor));
flow_63:
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_95: // PRI = native low64 equality of PRI and ALT
    work0 = (primary ^ alternate);
    frame_address = add_word(data, frame);
    primary = (work0 < extend(1u)) ? 1ull : 0ull;
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_96: // PRI = native low64 inequality of PRI and ALT
    work0 = (primary ^ alternate);
    frame_address = add_word(data, frame);
    primary = (0ull < work0) ? 1ull : 0ull;
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_97: // PRI = unsigned native low64 PRI<ALT
    primary = (primary < alternate) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_98: // PRI = unsigned native low64 PRI<=ALT
    work0 = (alternate < primary) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    primary = (work0 ^ 1ull);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_99: // PRI = unsigned native low64 PRI>ALT
    primary = (alternate < primary) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_100: // PRI = unsigned native low64 PRI>=ALT
    work0 = (primary < alternate) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    primary = (work0 ^ 1ull);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_101: // PRI = signed native low64 PRI<ALT
    primary = (signed_register(primary) < signed_register(alternate)) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_102: // PRI = signed native low64 PRI<=ALT
    work0 = (signed_register(alternate) < signed_register(primary)) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    primary = (work0 ^ 1ull);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_103: // PRI = signed native low64 PRI>ALT
    primary = (signed_register(alternate) < signed_register(primary)) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_104: // PRI = signed native low64 PRI>=ALT
    work0 = (signed_register(primary) < signed_register(alternate)) ? 1ull : 0ull;
    frame_address = add_word(data, frame);
    primary = (work0 ^ 1ull);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    stack_address = add_word(data, stack);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_105: // PRI = native low64 PRI equals sign-extended immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = (primary ^ operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    primary = (work0 < extend(1u)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_106: // PRI = native low64 ALT equals sign-extended immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = (alternate ^ operand);
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    primary = (work0 < extend(1u)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_107: // PRI = low32 PRI+1
    primary = add_word(primary, extend(1u));
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_108: // ALT = low32 ALT+1
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = add_word(alternate, extend(1u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_109: // increment word at data+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    work1 = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work1 = add_word(work1, extend(1u));
    store_word(low(work0), work1);
    argument0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(argument0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_110: // increment word at data+frame+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    work1 = load_word(low(work0));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work1 = add_word(work1, extend(1u));
    store_word(low(work0), work1);
    argument0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(argument0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_111: // increment word at data+PRI without checked-address guard
    work0 = add_word(data, primary);
    frame_address = add_word(data, frame);
    work1 = load_word(low(work0));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    work1 = add_word(work1, extend(1u));
    alternate_address = add_word(data, alternate);
    store_word(low(work0), work1);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_112: // PRI = low32 PRI-1
    primary = add_word(primary, extend(4294967295u));
    frame_address = add_word(data, frame);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_113: // ALT = low32 ALT-1
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate = add_word(alternate, extend(4294967295u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_114: // decrement word at data+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = add_word(data, operand);
    stack_address = add_word(data, stack);
    work1 = load_word(low(work0));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work1 = add_word(work1, extend(4294967295u));
    store_word(low(work0), work1);
    argument0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(argument0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_115: // decrement word at data+frame+immediate
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    work0 = add_word(frame_address, operand);
    heap_gap = add_word(heap, extend(64u));
    work1 = load_word(low(work0));
    alternate_address = add_word(data, alternate);
    callback = load_word(low(state) + 4u);
    work1 = add_word(work1, extend(4294967295u));
    store_word(low(work0), work1);
    argument0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(argument0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }

slot_116: // decrement word at data+PRI without checked-address guard
    work0 = add_word(data, primary);
    frame_address = add_word(data, frame);
    work1 = load_word(low(work0));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    work1 = add_word(work1, extend(4294967295u));
    alternate_address = add_word(data, alternate);
    store_word(low(work0), work1);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const Address next_handler = low(work0);
        endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_117: // real memcpy(data+ALT,data+PRI,immediate); preserve prior PRI and full128 codebase/reset-stack temporaries
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = add_word(data, primary);
    argument0 = alternate_address;
    norrath_exec_transport::spill(native_frame + 16u, transport->native[12]);
    stack_address = add_word(data, stack);
    {
        norrath_exec_transport::spill(native_frame + 32u, transport->native[13]);
        work0 = norrath_exec_transport::call(transport, reinterpret_cast<Address>(&norrath_copy_bytes));
    }
    work1 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    norrath_exec_transport::reload(native_frame + 16u, transport->native[12]);
    { const Address next_handler = low(work1);
        norrath_exec_transport::reload(native_frame + 32u, transport->native[13]);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_118: // real memcmp(data+ALT,data+PRI,immediate); PRI = actual result; preserve full128 temporaries
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = add_word(data, primary);
    argument0 = alternate_address;
    norrath_exec_transport::spill(native_frame + 16u, transport->native[12]);
    stack_address = add_word(data, stack);
    {
        norrath_exec_transport::spill(native_frame + 32u, transport->native[13]);
        work0 = norrath_exec_transport::call(transport, reinterpret_cast<Address>(&norrath_memory_compare));
    }
    primary = work0;
    norrath_exec_transport::reload(native_frame + 16u, transport->native[12]);
    norrath_exec_native_frame::store_word(native_frame + 4u, primary);
    frame_address = add_word(data, frame);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    callback = load_word(low(state) + 4u);
    { const Address next_handler = low(work0);
        norrath_exec_transport::reload(native_frame + 32u, transport->native[13]);
        goto *pointer(next_handler);
    }

slot_119: // fill floor(unsigned immediate/4) words at data+ALT with PRI; ignore trailing bytes
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = (operand < extend(4u)) ? 1ull : 0ull;
    { const bool taken = (work0 != 0ull);
        heap_gap = alternate;
        if (taken) goto flow_65;
    }
    ; // Original no-effect slot.
flow_64:
    argument0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    operand = add_word(operand, extend(4294967292u));
    work1 = add_word(data, heap_gap);
    heap_gap = add_word(heap_gap, extend(4u));
    work0 = (operand < extend(4u)) ? 1ull : 0ull;
    { const bool taken = (work0 == 0ull);
        store_word(low(work1), argument0);
        if (taken) goto flow_64;
    }
flow_65:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_120: // publish PRI if output nonnull; restore entry stack; return immediate status
    work0 = norrath_exec_native_frame::load_word(native_frame + 8u);
    { const bool taken = (work0 == 0ull);
        operand = load_word(low(cursor));
        if (taken) goto flow_66;
    }
    work0 = norrath_exec_native_frame::load_word(native_frame + 4u);
    work1 = norrath_exec_native_frame::load_word(native_frame + 8u);
    store_word(low(work1), work0);
flow_66:
    store_word(low(state) + 24u, entry_stack);
    work0 = operand;
    goto flow_73;
    ; // Original no-effect slot.

slot_121: // require signed PRI>=0 and PRI<=immediate; else restore entry stack and return4
    operand = load_word(low(cursor));
    work0 = (signed_register(operand) < signed_register(primary)) ? 1ull : 0ull;
    { const bool taken = (work0 != 0ull);
        cursor = add_word(cursor, extend(4u));
        if (taken) goto flow_67;
    }
    { const bool taken = (signed_register(primary) < 0ll);
        frame_address = add_word(data, frame);
        if (taken) goto flow_67;
    }
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.
flow_67:
    store_word(low(state) + 24u, entry_stack);
    work0 = extend(4u);
    goto flow_73;
    ; // Original no-effect slot.

slot_122: // call actual AMX+4 callback with AMX/PRI/index-result-address/VM-stack after state publication
    work0 = sub_word(cursor, code);
    endian = extend(native_frame + 4u);
    norrath_exec_transport::spill(native_frame + 16u, transport->native[12]);
    argument0 = state;
    norrath_exec_transport::spill(native_frame + 32u, transport->native[13]);
    operand = endian;
    store_word(low(state) + 20u, heap);
    heap_gap = stack_address;
    store_word(low(state) + 12u, work0);
    store_word(low(state) + 16u, frame);
    {
        const Address called_callback = low(callback);
        store_word(low(state) + 24u, stack);
        work0 = norrath_exec_transport::call(transport, called_callback);
    }
    norrath_exec_transport::reload(native_frame + 16u, transport->native[12]);
    { const bool taken = (work0 != 0ull);
        norrath_exec_transport::reload(native_frame + 32u, transport->native[13]);
        if (taken) goto flow_68;
    }
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_123: // call actual AMX+4 callback with AMX/immediate/index-result-address/VM-stack after state publication
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = sub_word(cursor, code);
    norrath_exec_transport::spill(native_frame + 16u, transport->native[12]);
    store_word(low(state) + 12u, work0);
    work0 = extend(native_frame + 4u);
    norrath_exec_transport::spill(native_frame + 32u, transport->native[13]);
    primary = operand;
    store_word(low(state) + 20u, heap);
    argument0 = state;
    store_word(low(state) + 16u, frame);
    operand = work0;
    store_word(low(state) + 24u, stack);
    {
        const Address called_callback = low(callback);
        heap_gap = stack_address;
        work0 = norrath_exec_transport::call(transport, called_callback);
    }
    norrath_exec_transport::reload(native_frame + 16u, transport->native[12]);
    { const bool taken = (work0 == 0ull);
        norrath_exec_transport::reload(native_frame + 32u, transport->native[13]);
        if (taken) goto flow_69;
    }
flow_68:
    store_word(low(state) + 24u, entry_stack);
    goto flow_73;
flow_69:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_124: // consume byte length and skip code-pointer by that many bytes
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    cursor = add_word(cursor, operand);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_125: // consume two words and publish AMX+36/+40
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    frame_address = add_word(data, frame);
    store_word(low(state) + 36u, work0);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    store_word(low(state) + 40u, work0);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_126: // consume length and metadata words; publish AMX+48/+52/+56 and skip record
    operand = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    frame_address = add_word(data, frame);
    store_word(low(state) + 48u, work0);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    work1 = add_word(cursor, operand);
    store_word(low(state) + 56u, cursor);
    store_word(low(state) + 52u, work0);
    heap_gap = add_word(heap, extend(64u));
    cursor = add_word(work1, extend(4294967292u));
    alternate_address = add_word(data, alternate);
    work0 = load_word(low(work1) - 8u);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_127: // consume two words and publish AMX+48/+52
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    frame_address = add_word(data, frame);
    store_word(low(state) + 48u, work0);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    store_word(low(state) + 52u, work0);
    callback = load_word(low(state) + 4u);
    work0 = load_word(low(cursor));
    { const Address next_handler = low(work0);
        cursor = add_word(cursor, extend(4u));
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_128: // code pointer = codebase+PRI
    cursor = add_word(code, primary);
    frame_address = add_word(data, frame);
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.

slot_129: // scan actual switch table count/default/value-target pairs; choose first matching PRI
    work0 = load_word(low(cursor));
    heap_gap = add_word(work0, extend(4u));
    callback = load_word(low(heap_gap));
    cursor = load_word(low(heap_gap) + 4u);
    { const bool taken = (signed_register(callback) <= 0ll);
        heap_gap = add_word(work0, extend(12u));
        if (taken) goto flow_72;
    }
    work0 = load_word(low(heap_gap));
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    { const bool taken = (work0 == primary);
        ; // Original no-effect slot.
        if (taken) goto flow_71;
    }
    callback = add_word(callback, extend(4294967295u));
    ; // Original no-effect slot.
flow_70:
    { const bool taken = (signed_register(callback) <= 0ll);
        heap_gap = add_word(heap_gap, extend(8u));
        if (taken) goto flow_72;
    }
    work0 = load_word(low(heap_gap));
    ; // Original no-effect slot.
    ; // Original no-effect slot.
    ; // Original no-effect slot.
    ; // Original no-effect slot.
    if (work0 != primary) {
        callback = add_word(callback, extend(4294967295u));
        goto flow_70;
    }
flow_71:
    if (signed_register(callback) > 0ll) {
        cursor = load_word(low(heap_gap) + 4u);
        goto flow_72;
    }
flow_72:
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }

slot_130: // no operation; fetch next actual handler cell
    work0 = load_word(low(cursor));
    cursor = add_word(cursor, extend(4u));
    frame_address = add_word(data, frame);
    primary = norrath_exec_native_frame::load_word(native_frame + 4u);
    alternate_below_heap = (signed_register(alternate) < signed_register(heap)) ? 1ull : 0ull;
    endian = norrath_exec_native_frame::load_word(low(transport->native[28].lo) - 16376u);
    stack_address = add_word(data, stack);
    heap_gap = add_word(heap, extend(64u));
    alternate_address = add_word(data, alternate);
    { const Address next_handler = low(work0);
        callback = load_word(low(state) + 4u);
        goto *pointer(next_handler);
    }
    ; // Original no-effect slot.
flow_73:
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    ; // Exact physical frame allocation/save/restore belongs to entry_transport.S.
    return work0;
}
