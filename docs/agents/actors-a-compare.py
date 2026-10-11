#!/usr/bin/env python3
"""Compare authored actor constructors with the private SLUS_205.65 bodies.

The executable stays outside the repository. This runs both the original
instructions and the freshly compiled EE object, with the same stubbed calls,
and fails if the call sequence, point image, object bytes, or return differ.
"""
import hashlib
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ELF_PATH = Path("/Users/syndicatemike/Documents/Codex/projects/open-champions/champions-of-norrath/work/private/SLUS_205.65")
ELF_SHA = "9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0"
TC = Path("/Users/syndicatemike/Documents/Codex/projects/open-champions/champions-of-norrath/work/toolchains/ee/bin")
CXX = TC / "mips64r5900el-ps2-elf-g++"
FLAGS = ["-G0", "-O2", "-ffreestanding", "-fno-asynchronous-unwind-tables", "-fno-exceptions",
         "-fno-pic", "-fno-rtti", "-fno-stack-protector", "-fno-unwind-tables", "-fomit-frame-pointer",
         "-fwrapv", "-mabi=eabi", "-march=r5900", "-mfp32", "-mgp64", "-mlong32", "-mno-abicalls",
         "-msingle-float", "-std=c++17", "-nostdinc", "-nostdinc++"]
GP = 0x004879F0
STUBS = {
    0x001411F0: "norrath_game_object_construct",
    0x00143A80: "norrath_actor_find_tag_string",
    0x00140F60: "norrath_game_object_find_tag_int",
}
OBJECT = 0x31000000
POINT = 0x31001000
ARGUMENTS = 0x31002000
STACK = 0x32000000
CODE = 0x210000


def sext16(value):
    return value - 0x10000 if value & 0x8000 else value


def sext32(value):
    value &= 0xFFFFFFFF
    return value - 0x100000000 if value & 0x80000000 else value


def bits_to_float(bits):
    return struct.unpack("<f", struct.pack("<I", bits & 0xFFFFFFFF))[0]


def elf_loads(data):
    if data[:4] != b"\x7fELF":
        raise SystemExit("private executable is not an ELF")
    e_phoff, e_phentsize, e_phnum = struct.unpack_from("<IHH", data, 28)
    loads = []
    for index in range(e_phnum):
        off = e_phoff + index * e_phentsize
        p_type, p_offset, p_vaddr, _, p_filesz = struct.unpack_from("<IIIII", data, off)
        if p_type == 1 and p_filesz:
            loads.append((p_vaddr, p_offset, p_filesz))
    return loads


def read_vaddr(data, loads, addr, size):
    for vaddr, offset, filesz in loads:
        if vaddr <= addr and addr + size <= vaddr + filesz:
            start = offset + (addr - vaddr)
            return data[start:start + size]
    raise SystemExit("address outside loaded file bytes: %#x" % addr)


class Machine:
    def __init__(self, image, stubs, answers):
        self.mem = {}
        for addr, byte in image.items():
            self.mem[addr] = byte
        self.r = [0] * 32
        self.f = [0] * 32
        self.cond = False
        self.r[28] = sext32(GP)
        self.r[29] = sext32(STACK)
        self.r[31] = 1
        self.pc = 0
        self.stubs = stubs
        self.answers = list(answers)
        self.calls = []
        self.halt = None

    def low(self, index):
        return self.r[index] & 0xFFFFFFFF

    def write32(self, index, value):
        if index != 0:
            self.r[index] = sext32(value)

    def u8(self, addr):
        return self.mem.get(addr & 0xFFFFFFFF, 0)

    def set8(self, addr, value):
        self.mem[addr & 0xFFFFFFFF] = value & 0xFF

    def u32(self, addr):
        addr &= 0xFFFFFFFF
        return (self.u8(addr) | (self.u8(addr + 1) << 8) |
                (self.u8(addr + 2) << 16) | (self.u8(addr + 3) << 24))

    def set32(self, addr, value):
        for shift in range(4):
            self.set8(addr + shift, value >> (8 * shift))

    def merge(self, reg, shift, byte):
        low = self.low(0 if False else reg)
        low = (low & ~(0xFF << shift)) | ((byte & 0xFF) << shift)
        self.write32(reg, low)

    def lwl(self, rt, ea):
        count = (ea & 3) + 1
        start = ea - (count - 1)
        for index in range(count):
            self.merge(rt, (4 - count + index) * 8, self.u8(start + index))

    def lwr(self, rt, ea):
        count = 4 - (ea & 3)
        for index in range(count):
            self.merge(rt, index * 8, self.u8(ea + index))

    def ldl(self, rt, ea):
        count = (ea & 7) + 1
        start = ea - (count - 1)
        value = self.r[rt] & 0xFFFFFFFFFFFFFFFF
        for index in range(count):
            shift = (8 - count + index) * 8
            value = (value & ~(0xFF << shift)) | (self.u8(start + index) << shift)
        if rt:
            self.r[rt] = value

    def ldr(self, rt, ea):
        count = 8 - (ea & 7)
        value = self.r[rt] & 0xFFFFFFFFFFFFFFFF
        for index in range(count):
            shift = index * 8
            value = (value & ~(0xFF << shift)) | (self.u8(ea + index) << shift)
        if rt:
            self.r[rt] = value

    def sdl(self, rt, ea):
        count = (ea & 7) + 1
        start = ea - (count - 1)
        value = self.r[rt] & 0xFFFFFFFFFFFFFFFF
        for index in range(count):
            shift = (8 - count + index) * 8
            self.set8(start + index, (value >> shift) & 0xFF)

    def sdr(self, rt, ea):
        count = 8 - (ea & 7)
        value = self.r[rt] & 0xFFFFFFFFFFFFFFFF
        for index in range(count):
            self.set8(ea + index, (value >> (index * 8)) & 0xFF)

    def execute(self, ins, pc):
        op = ins >> 26
        rs = (ins >> 21) & 31
        rt = (ins >> 16) & 31
        rd = (ins >> 11) & 31
        sa = (ins >> 6) & 31
        funct = ins & 63
        imm = ins & 0xFFFF
        simm = sext16(imm)
        if op == 0:
            if funct == 0:  # sll
                self.write32(rd, self.low(rt) << sa)
            elif funct == 2:  # srl
                self.write32(rd, self.low(rt) >> sa)
            elif funct == 3:  # sra
                self.write32(rd, sext32(self.low(rt)) >> sa)
            elif funct == 8:  # jr
                return ("jump", self.low(rs))
            elif funct == 33:  # addu
                self.write32(rd, self.low(rs) + self.low(rt))
            elif funct == 45:  # daddu, including the original 64-bit move idiom
                if rd:
                    self.r[rd] = (self.r[rs] + self.r[rt]) & 0xFFFFFFFFFFFFFFFF
            elif funct == 35:  # subu
                self.write32(rd, self.low(rs) - self.low(rt))
            elif funct == 36:  # and
                if rd:
                    self.r[rd] = self.r[rs] & self.r[rt]
            elif funct == 37:  # or
                if rd:
                    self.r[rd] = self.r[rs] | self.r[rt]
            elif funct == 38:  # xor
                if rd:
                    self.r[rd] = self.r[rs] ^ self.r[rt]
            elif funct == 43:  # sltu
                self.write32(rd, 1 if self.low(rs) < self.low(rt) else 0)
            elif funct == 42:  # slt
                self.write32(rd, 1 if sext32(self.low(rs)) < sext32(self.low(rt)) else 0)
            elif funct == 0x3C:  # dsll32
                if rd:
                    self.r[rd] = (self.r[rt] & 0xFFFFFFFFFFFFFFFF) << (sa + 32)
                    self.r[rd] &= 0xFFFFFFFFFFFFFFFF
            elif funct == 0x3E:  # dsrl32
                if rd:
                    self.r[rd] = (self.r[rt] & 0xFFFFFFFFFFFFFFFF) >> (sa + 32)
            elif funct == 0 and rd == 0 and rt == 0:
                return None
            else:
                raise SystemExit("unsupported SPECIAL %#x at %#x" % (funct, pc))
            return None
        if op == 1:  # regimm
            kind = rt
            target = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
            if kind == 1:  # bgez
                return ("branch", target, sext32(self.low(rs)) >= 0, False)
            if kind == 0:  # bltz
                return ("branch", target, sext32(self.low(rs)) < 0, False)
            if kind == 3:  # bgezl
                return ("branch", target, sext32(self.low(rs)) >= 0, True)
            if kind == 2:  # bltzl
                return ("branch", target, sext32(self.low(rs)) < 0, True)
            raise SystemExit("unsupported REGIMM %d at %#x" % (kind, pc))
        if op == 3:  # jal
            target = ((pc + 4) & 0xF0000000) | ((ins & 0x03FFFFFF) << 2)
            return ("jal", target)
        if op == 4:  # beq
            target = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
            return ("branch", target, self.low(rs) == self.low(rt), False)
        if op == 5:  # bne
            target = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
            return ("branch", target, self.low(rs) != self.low(rt), False)
        if op == 9:  # addiu
            self.write32(rt, self.low(rs) + simm)
        elif op == 12:  # andi
            self.write32(rt, self.low(rs) & imm)
        elif op == 13:  # ori
            self.write32(rt, self.low(rs) | imm)
        elif op == 15:  # lui
            self.write32(rt, imm << 16)
        elif op == 20:  # beql
            target = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
            return ("branch", target, self.low(rs) == self.low(rt), True)
        elif op == 21:  # bnel
            target = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
            return ("branch", target, self.low(rs) != self.low(rt), True)
        elif op in (32, 36):  # lb / lbu
            value = self.u8(self.low(rs) + simm)
            self.write32(rt, value if op == 36 or value < 128 else value - 256)
        elif op == 33:  # lh
            addr = self.low(rs) + simm
            value = self.u8(addr) | (self.u8(addr + 1) << 8)
            self.write32(rt, value if value < 0x8000 else value - 0x10000)
        elif op == 35:  # lw
            self.write32(rt, self.u32(self.low(rs) + simm))
        elif op == 37:  # lhu
            addr = self.low(rs) + simm
            self.write32(rt, self.u8(addr) | (self.u8(addr + 1) << 8))
        elif op == 34:  # lwl
            self.lwl(rt, self.low(rs) + simm)
        elif op == 38:  # lwr
            self.lwr(rt, self.low(rs) + simm)
        elif op == 26:  # ldl
            self.ldl(rt, self.low(rs) + simm)
        elif op == 27:  # ldr
            self.ldr(rt, self.low(rs) + simm)
        elif op == 40:  # sb
            self.set8(self.low(rs) + simm, self.low(rt))
        elif op == 41:  # sh
            addr = self.low(rs) + simm
            value = self.low(rt)
            self.set8(addr, value)
            self.set8(addr + 1, value >> 8)
        elif op == 43:  # sw
            self.set32(self.low(rs) + simm, self.low(rt))
        elif op == 44:  # sdl
            self.sdl(rt, self.low(rs) + simm)
        elif op == 45:  # sdr
            self.sdr(rt, self.low(rs) + simm)
        elif op == 55:  # ld
            addr = self.low(rs) + simm
            if rt:
                low = self.u32(addr)
                high = self.u32(addr + 4)
                self.r[rt] = low | (high << 32)
        elif op == 63:  # sd
            addr = self.low(rs) + simm
            value = self.r[rt] & 0xFFFFFFFFFFFFFFFF
            self.set32(addr, value)
            self.set32(addr + 4, value >> 32)
        elif op == 49:  # lwc1
            self.f[rt] = self.u32(self.low(rs) + simm)
        elif op == 57:  # swc1
            self.set32(self.low(rs) + simm, self.f[rt])
        elif op == 17:  # cop1
            return self.cop1(ins, pc, rs, rt, rd, sa, funct, simm)
        else:
            raise SystemExit("unsupported opcode %d at %#x (%#x)" % (op, pc, ins))
        return None

    def cop1(self, ins, pc, rs, rt, rd, sa, funct, simm):
        fmt = rs
        if fmt == 0:  # mfc1
            self.write32(rt, self.f[rd])
            return None
        if fmt == 4:  # mtc1
            self.f[rd] = self.low(rt)
            return None
        if fmt == 8:  # bc1
            target = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
            likely = (rt & 2) != 0
            wants_true = (rt & 1) != 0
            taken = self.cond if wants_true else not self.cond
            return ("branch", target, taken, likely)
        if fmt == 0x14 and funct == 0x20:  # cvt.s.w
            fd = sa
            self.f[fd] = struct.unpack("<I", struct.pack("<f", float(sext32(self.f[rd]))))[0]
            return None
        if fmt == 0x10 and funct == 0x32:  # c.eq.s
            ft = rt
            fs = rd
            self.cond = bits_to_float(self.f[fs]) == bits_to_float(self.f[ft])
            return None
        raise SystemExit("unsupported COP1 %#x at %#x" % (ins, pc))

    def run(self, entry, steps=20000):
        self.pc = entry
        for _ in range(steps):
            if self.pc == 1:
                self.halt = "return"
                return
            ins = self.u32(self.pc)
            pc = self.pc
            effect = self.execute(ins, pc)
            if effect is None:
                self.pc = (pc + 4) & 0xFFFFFFFF
                continue
            kind = effect[0]
            delay_pc = (pc + 4) & 0xFFFFFFFF
            if kind == "jump":
                self.execute(self.u32(delay_pc), delay_pc)
                self.pc = effect[1]
                continue
            if kind == "branch":
                target, taken, likely = effect[1], effect[2], effect[3]
                if taken or not likely:
                    nested = self.execute(self.u32(delay_pc), delay_pc)
                    if nested is not None:
                        raise SystemExit("branch in delay slot at %#x" % delay_pc)
                self.pc = target if taken else (pc + 8) & 0xFFFFFFFF
                continue
            if kind == "jal":
                self.execute(self.u32(delay_pc), delay_pc)
                target = effect[1]
                name = self.stubs.get(target)
                if name is None:
                    raise SystemExit("jal leaves the function: %#x from %#x" % (target, pc))
                point = bytes(self.u8(self.low(5) + i) for i in range(12)) if name == "norrath_game_object_construct" else b""
                self.calls.append((name, self.low(4), self.low(5), self.low(6), self.low(7), point))
                if not self.answers:
                    raise SystemExit("stub %s had no scripted answer" % name)
                self.write32(2, self.answers.pop(0))
                self.write32(31, (pc + 8) & 0xFFFFFFFF)
                self.pc = (pc + 8) & 0xFFFFFFFF
                continue
            raise SystemExit("bad effect %s" % (effect,))
        raise SystemExit("step limit at %#x" % self.pc)


def load_image(data, loads, blobs):
    image = {}
    for addr, blob in blobs:
        for index, byte in enumerate(blob):
            image[addr + index] = byte
    return image


def compile_object(source, output):
    subprocess.run([str(CXX), *FLAGS, "-c", str(source), "-o", str(output)], check=True)


def parse_object(path):
    data = path.read_bytes()
    e_shoff = struct.unpack_from("<I", data, 32)[0]
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from("<HHH", data, 46)
    sections = []
    for index in range(e_shnum):
        off = e_shoff + index * e_shentsize
        sections.append(struct.unpack_from("<10I", data, off))
    names = data[sections[e_shstrndx][4]:sections[e_shstrndx][4] + sections[e_shstrndx][5]]

    def name_of(section):
        start = section[0]
        end = names.find(b"\0", start)
        return names[start:end].decode()

    symbols = {}
    text = None
    relocs = []
    for section in sections:
        label = name_of(section)
        if label == ".text":
            text = data[section[4]:section[4] + section[5]]
        elif label == ".symtab":
            string_section = sections[section[6]]
            strings = data[string_section[4]:string_section[4] + string_section[5]]
            for at in range(0, section[5], 16):
                raw = data[section[4] + at:section[4] + at + 16]
                name_off, value, size, info, other, shndx = struct.unpack("<IIIBBH", raw)
                end = strings.find(b"\0", name_off)
                symbols[strings[name_off:end].decode()] = value
        elif label == ".rel.text":
            for at in range(0, section[5], 8):
                offset, info = struct.unpack_from("<II", data, section[4] + at)
                relocs.append((offset, info & 0xFF, info >> 8))
    sym_by_index = {}
    # Reparse symbols with indexes.
    for section in sections:
        if name_of(section) != ".symtab":
            continue
        string_section = sections[section[6]]
        strings = data[string_section[4]:string_section[4] + string_section[5]]
        for index in range(section[5] // 16):
            raw = data[section[4] + index * 16:section[4] + (index + 1) * 16]
            name_off = struct.unpack_from("<I", raw)[0]
            end = strings.find(b"\0", name_off)
            sym_by_index[index] = strings[name_off:end].decode()
    return text, relocs, sym_by_index


def apply_relocations(text, relocs, sym_by_index, bases):
    blob = bytearray(text)
    pending = []
    for offset, kind, sym_index in relocs:
        symbol = sym_by_index[sym_index]
        if kind == 4:  # R_MIPS_26
            target = bases[symbol]
            word = struct.unpack_from("<I", blob, offset)[0]
            word = (word & 0xFC000000) | ((target >> 2) & 0x03FFFFFF)
            struct.pack_into("<I", blob, offset, word)
        elif kind == 5:  # R_MIPS_HI16
            pending.append((offset, symbol))
        elif kind == 6:  # R_MIPS_LO16
            value = bases[symbol]
            while pending:
                hi_off, hi_sym = pending.pop(0)
                if hi_sym != symbol:
                    raise SystemExit("HI16/LO16 symbol mismatch")
                hi = ((value + 0x8000) >> 16) & 0xFFFF
                word = struct.unpack_from("<I", blob, hi_off)[0]
                struct.pack_into("<I", blob, hi_off, (word & 0xFFFF0000) | hi)
            word = struct.unpack_from("<I", blob, offset)[0]
            struct.pack_into("<I", blob, offset, (word & 0xFFFF0000) | (value & 0xFFFF))
        else:
            raise SystemExit("unsupported relocation %d" % kind)
    if pending:
        raise SystemExit("unpaired HI16 relocation")
    return bytes(blob)


def object_bytes(machine, count):
    return bytes(machine.u8(OBJECT + index) for index in range(count))


def run_case(image_bytes, entry, answers, type_bits=0x1234, flags=0):
    machine = Machine({}, STUBS, answers)
    machine.mem.update(image_bytes)
    for index in range(256):
        machine.set8(OBJECT + index, 0x5A)
    machine.set32(OBJECT + 12, flags)
    for index, word in enumerate((0x3F800000, 0x40000000, 0x40400000)):
        machine.set32(POINT + index * 4, word)
    machine.write32(4, OBJECT)
    machine.write32(5, POINT)
    machine.write32(6, type_bits)
    machine.write32(7, ARGUMENTS)
    machine.r[29] = sext32(STACK)
    machine.run(entry)
    return machine


def scenario_answers(strings, ints):
    """construct, tag string, optional tag int, second string, optional int, final int."""
    answers = [0]
    answers.append(0x1 if strings[0] else 0)
    if strings[0]:
        answers.append(ints[0] & 0xFFFFFFFF)
    answers.append(0x1 if strings[1] else 0)
    if strings[1]:
        answers.append(ints[1] & 0xFFFFFFFF)
    answers.append(ints[2] & 0xFFFFFFFF)
    return answers


def main():
    data = ELF_PATH.read_bytes()
    if hashlib.sha256(data).hexdigest() != ELF_SHA:
        raise SystemExit("private executable hash mismatch")
    loads = elf_loads(data)
    original = read_vaddr(data, loads, 0x00216600, 0x150)
    output = Path("/tmp/actors-a-blocker.o")
    compile_object(ROOT / "experimental/sources/actors-a-blocker/blocker.cpp", output)
    text, relocs, symbols = parse_object(output)
    bases = {
        "norrath_game_object_construct": 0x001411F0,
        "norrath_actor_find_tag_string": 0x00143A80,
        "norrath_game_object_find_tag_int": 0x00140F60,
        "norrath_actors_a_view_44c830": 0x0044C830,
        "norrath_actors_a_view_485400": 0x00485400,
        "norrath_actors_a_view_485408": 0x00485408,
        "norrath_actors_a_view_470830": 0x00470830,
    }
    compiled = apply_relocations(text, relocs, symbols, bases)
    failures = 0
    # Initial flags are zero so each OR bit is visible. Expectations are the
    # original instruction effects, checked again on the compiled object.
    cases = [
        ((False, False), (0, 0, 0), 0x1234, 0x41C00000, 0x5A5A5A5A, 0x00000002, 0),
        ((False, False), (0, 0, 7), 0x1234, 0x41C00000, 0x3F800000, 0x00000012, 0x10),
        ((True, False), (0, 0, 0), 0x1234, 0x00000000, 0x5A5A5A5A, 0x00000207, 0x205),
        ((True, True), (5, -3, 0), 0x1234, 0xC0400000, 0x5A5A5A5A, 0x00000002, 0),
        ((False, True), (0, 0, 0), -2, 0x00000000, 0x5A5A5A5A, 0x00000207, 0x205),
        ((True, False), (0, 0, 1), 0x1234, 0x00000000, 0x3F800000, 0x00000217, 0x215),
    ]
    for strings, ints, type_bits, height, marker, flags, ret in cases:
        original_image = {0x00216600 + index: byte for index, byte in enumerate(original)}
        compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
        left = run_case(original_image, 0x00216600, scenario_answers(strings, ints), type_bits)
        right = run_case(compiled_image, CODE, scenario_answers(strings, ints), type_bits)
        same = (left.calls == right.calls and left.low(2) == right.low(2) and
                object_bytes(left, 160) == object_bytes(right, 160))
        got_flags = left.u32(OBJECT + 12)
        got_height = left.u32(OBJECT + 28)
        got_marker = left.u32(OBJECT + 32)
        got_view = left.u32(OBJECT + 72)
        got_type = left.u8(OBJECT + 56) | (left.u8(OBJECT + 57) << 8)
        point = left.calls[0][5] if left.calls else b""
        expect_type = type_bits & 0xFFFF
        independent = (got_flags == flags and got_marker == marker and got_view == 0x0044C830 and
                       got_type == expect_type and left.low(2) == ret and
                       point == bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40]) and
                       (height is None or got_height == height))
        if not same or not independent:
            failures += 1
            print("MISMATCH", strings, ints, "same", same, "independent", independent)
            print(" flags", hex(got_flags), "height", hex(got_height), "marker", hex(got_marker))
            print(" return", hex(left.low(2)), hex(right.low(2)), "type", hex(got_type))
            print(" calls", left.calls)
        else:
            print("match", strings, ints, "return", hex(ret))
    if failures:
        raise SystemExit("%d blocker scenarios failed" % failures)
    print("blocker: %d original/compiled scenarios matched" % len(cases))


if __name__ == "__main__":
    main()
