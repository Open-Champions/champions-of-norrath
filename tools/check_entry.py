#!/usr/bin/env python3
"""Bounded entry instruction-model comparison; never runs a game or kernel.

The candidate must be privately linked at the pinned entry address with the
reviewed dependency addresses. External services are intercepted model inputs,
not original code. This is behavioral evidence, not an instruction-byte match.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from rebuild import Elf32

PIN = '9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0'
ENTRY, LOW, HIGH = 0x100008, 0x486800, 0x8eab1c
GP, ARGS, ROOT, END = 0x4879f0, 0x5192c0, 0x100250, 0x8ec91c
INIT, FLUSH, MAIN, EXIT = 0x2dd8e8, 0x2d5360, 0x2d0818, 0x2ffb08
MASK = (1 << 64) - 1


def require(condition, message):
    if not condition:
        raise ValueError(message)


def signed(value, bits):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value >> (bits - 1) else value


def extent(blob):
    elf = Elf32(blob)
    symbols = [s for s in elf.symbols if s['name'] == '_start']
    require(len(symbols) == 1, 'ambiguous entry')
    symbol = symbols[0]
    require(symbol['type'] == 2 and symbol['value'] == ENTRY and symbol['size'] > 0,
            'unexpected entry symbol')
    section = elf.sections[symbol['section']]
    offset = ENTRY - section[3]
    require(section[2] & 6 == 6 and 0 <= offset <= section[5] - symbol['size'],
            'entry extent outside executable storage')
    return elf.section_data(symbol['section'])[offset:offset + symbol['size']]


def run(code, stack, argc, result):
    # Poison incoming state so missing reset operations cannot pass accidentally.
    regs = [0] + [((i + 1) << 96) | (0x12340000 + i) for i in range(1, 32)]
    kernel_regs = regs[26:28]
    fp = [0xdeadbeef] * 32
    special = dict(hi=1, lo=2, hi1=3, lo1=4, sa=5, acc=6, fcsr=7)
    pc, delayed, cursor, steps = ENTRY, None, LOW, 0
    reset, enabled, arguments_ready = False, False, False
    stores = {1: 0, 16: 0}
    events = []

    def put(index, value, bits=64):
        if index:
            regs[index] = value & ((1 << bits) - 1)

    def poison_callers():
        for index in list(range(2, 16)) + [24, 25]:
            put(index, 0xbad00000 + index)

    while steps < 4000000:
        steps += 1
        if pc in (INIT, FLUSH, MAIN, EXIT):
            require(delayed is None and cursor == HIGH and reset, 'early external transfer')
            require(regs[28] == GP and regs[29] == stack, 'lost GP or kernel-returned stack')
            if pc == INIT:
                events.append(['init'])
            elif pc == FLUSH:
                events.append(['flush', regs[4]])
            elif pc == MAIN:
                require(enabled and arguments_ready, 'main before interrupt/argument setup')
                events.append(['main', regs[4], regs[5]])
            else:
                events.append(['exit', regs[4]])
                break
            return_pc = regs[31]
            poison_callers()
            if pc == MAIN:
                put(2, signed(result, 32))
            pc = return_pc
            continue
        require(ENTRY <= pc <= ENTRY + len(code) - 4 and pc % 4 == 0,
                'control escaped complete entry extent')
        word = struct.unpack_from('<I', code, pc - ENTRY)[0]
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        imm, fn = signed(word, 16), word & 63
        previous, delayed = delayed, None
        next_pc = pc + 4
        if word == 0 or word == 0x40f:  # NOP, SYNC.P
            pass
        elif op == 28 and fn in (0x28, 0x29):
            require(rs == rt == 0 and word & 0x7ff in (0x428, 0x4a9), 'nonzero packed reset')
            put(rd, 0, 128)  # PADDUW zero+zero or POR zero|zero.
        elif word in (0x11, 0x13, 0x70000011, 0x70000013, 0x04190000):
            name = {0x11: 'hi', 0x13: 'lo', 0x70000011: 'hi1',
                    0x70000013: 'lo1', 0x04190000: 'sa'}[word]
            special[name] = 0
        elif op == 17 and rs == 4:  # MTC1
            fp[rd] = regs[rt] & 0xffffffff
        elif word == 0x46010018:  # ADDA.S of reset F0/F1 only.
            require(fp[0] == fp[1] == 0, 'nonzero FP accumulator reset')
            special['acc'] = 0
        elif word == 0x44c0f800:
            special['fcsr'] = 0
            require(all(regs[i] == 0 for i in range(32) if i not in (26, 27))
                    and regs[26:28] == kernel_regs and not any(fp)
                    and not any(special.values()), 'incomplete architectural reset')
            reset = True
        elif op == 15:
            put(rt, signed((word & 0xffff) << 16, 32))
        elif op in (9, 25):
            value = regs[rs] + imm
            put(rt, signed(value, 32) if op == 9 else value)
        elif op in (12, 13):
            put(rt, regs[rs] & (word & 0xffff) if op == 12 else regs[rs] | (word & 0xffff))
        elif op == 0 and fn in (0x21, 0x2d, 0x24, 0x25):
            if fn in (0x21, 0x2d):
                value = regs[rs] + regs[rt]
                if fn == 0x21:
                    value = signed(value, 32)
            else:
                value = regs[rs] & regs[rt] if fn == 0x24 else regs[rs] | regs[rt]
            put(rd, value)
        elif op in (4, 5):
            taken = (regs[rs] & MASK) == (regs[rt] & MASK)
            if op == 5:
                taken = not taken
            delayed = pc + 4 + imm * 4 if taken else pc + 8
        elif op in (2, 3):
            delayed = ((pc + 4) & 0xf0000000) | ((word & 0x3ffffff) << 2)
            if op == 3:
                put(31, pc + 8)
        elif op == 0 and fn == 9:
            delayed = regs[rs]
            put(rd, pc + 8)
        elif op in (0x28, 0x1f):
            width = 1 if op == 0x28 else 16
            address = (regs[rs] + imm) & 0xffffffff
            require(reset and regs[rt] == 0 and address == cursor
                    and address + width <= HIGH and address % width == 0,
                    'BSS store range/order/value differs')
            stores[width] += 1
            cursor += width
        elif op == 35:
            require(arguments_ready and (regs[rs] + imm) & 0xffffffff == ARGS,
                    'unexpected startup load')
            put(rt, signed(argc, 32))
        elif word == 12:
            require(cursor == HIGH and regs[28] == GP, 'syscall before memory/GP setup')
            if regs[3] == 60:
                events.append(['syscall60', *regs[4:9]])
                put(2, stack)
                arguments_ready = True
            elif regs[3] == 61:
                require(regs[29] == stack, 'stack not taken from syscall60')
                events.append(['syscall61', regs[4], regs[5]])
                put(2, 0xbad061)
            else:
                raise ValueError('unexpected syscall')
        elif word == 0x42000038:
            enabled = True
        else:
            raise ValueError('unsupported instruction 0x%08x at 0x%x' % (word, pc))
        regs[0] = 0
        require(previous is None or delayed is None, 'control transfer in delay slot')
        pc = previous if previous is not None else next_pc
    else:
        raise ValueError('entry model exceeded instruction bound')
    expected = [['syscall60', GP, 0x1fc0000, 0x40000, ARGS, ROOT],
                ['syscall61', END, MASK], ['init'], ['flush', 0],
                ['main', signed(argc, 32) & MASK, ARGS + 4],
                ['exit', signed(result, 32) & MASK]]
    require(events == expected, 'startup call/argument order differs')
    require(stores == {1: 12, 16: 287793}, 'clear access widths differ')
    return dict(events=events, byte_stores=stores[1], quadword_stores=stores[16],
                bytes_cleared=cursor - LOW, full_reset_checked=reset)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original', type=Path, required=True)
    parser.add_argument('--candidate', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    original, candidate = args.original.read_bytes(), args.candidate.read_bytes()
    require(hashlib.sha256(original).hexdigest() == PIN, 'wrong original baseline')
    before, after = extent(original), extent(candidate)
    require(len(before) == 576, 'original extent changed')
    checks = []
    for stack, argc, result in [(0x1fbfff0, 0, 0), (0x1fbe000, 3, -1),
                                 (0x1fc0000, 0x7fffffff, 0x80000000)]:
        a, b = run(before, stack, argc, result), run(after, stack, argc, result)
        require(a == b, 'complete startup contract differs')
        checks.append(dict(stack=stack, argc=argc, main_result=result, trace=b))
    mutations = [(0x70000ca9, 'incomplete architectural reset'),
                 (0x7d000000, 'BSS store range/order/value differs'),
                 (0x42000038, 'main before interrupt/argument setup')]
    rejected = []
    for instruction, expected_error in mutations:
        words = list(struct.unpack('<%dI' % (len(after) // 4), after))
        require(words.count(instruction) == 1, 'mutation site changed; review required')
        words[words.index(instruction)] = 0
        try:
            run(struct.pack('<%dI' % len(words), *words), 0x1fbfff0, 0, 0)
        except ValueError as error:
            require(str(error) == expected_error, 'mutation failed for wrong reason')
            rejected.append(expected_error)
        else:
            raise ValueError('bad entry mutation was accepted')
    receipt = dict(baseline_sha256=PIN, candidate_sha256=hashlib.sha256(candidate).hexdigest(),
                   original_extent=len(before), authored_extent=len(after), checks=checks,
                   rejected_mutations=rejected,
                   exact_byte_match=False, kernel_executed=False, game_executed=False)
    with args.output.open('x') as stream:
        stream.write(json.dumps(receipt, indent=2) + '\n')
    print('Three complete original/authored entry traces agree; no kernel or game executed')


if __name__ == '__main__':
    main()
