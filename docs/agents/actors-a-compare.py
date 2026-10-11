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
    0x00143830: "norrath_actor_add_run_list",
    0x0017DB78: "norrath_world_check_for_water",
    0x0013D230: "norrath_light_add",
    0x00165DC0: "norrath_lump_find_resource",
    0x00223BD8: "norrath_construct_AutoPropLP",
    0x00175440: "norrath_world_find_closest_door",
    0x0017D348: "norrath_world_set_door_collision",
    0x0017D3D0: "norrath_world_get_door_height",
    0x001437E8: "norrath_object_add_to_delete_list",
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
        self.floats = []
        self.snapshots = []
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
            elif funct == 0x38:  # dsll
                if rd:
                    self.r[rd] = ((self.r[rt] & 0xFFFFFFFFFFFFFFFF) << sa) & 0xFFFFFFFFFFFFFFFF
            elif funct == 0x3A:  # dsrl
                if rd:
                    self.r[rd] = (self.r[rt] & 0xFFFFFFFFFFFFFFFF) >> sa
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
                point = b""
                point_reg = 6 if name in ("norrath_construct_AutoPropLP", "norrath_construct_AutoProp") else 5
                if name in ("norrath_game_object_construct", "norrath_world_check_for_water",
                            "norrath_construct_AutoPropLP", "norrath_construct_AutoProp",
                            "norrath_world_find_closest_door"):
                    point = bytes(self.u8(self.low(point_reg) + i) for i in range(12))
                if name == "norrath_world_check_for_water":
                    self.floats.append(self.f[12])
                if name == "norrath_light_add":
                    self.snapshots.append(bytes(self.u8(self.low(4) + i) for i in range(64)))
                if name == "norrath_construct_AutoPropLP":
                    self.snapshots.append(struct.pack("<I", self.low(8)))
                if name == "norrath_world_find_closest_door":
                    self.snapshots.append(struct.pack("<II", self.u32(OBJECT + 12), self.u32(OBJECT + 72)))
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
    named = {}
    for section in sections:
        label = name_of(section)
        named[label] = data[section[4]:section[4] + section[5]]
        if label == ".text":
            text = named[label]
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
    return text, relocs, sym_by_index, named


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


def run_case(image_bytes, entry, answers, type_bits=0x1234, flags=0, extra=None, regs=None,
             stack=None, object_size=256):
    machine = Machine({}, STUBS, answers)
    machine.mem.update(image_bytes)
    for index in range(object_size):
        machine.set8(OBJECT + index, 0x5A)
    machine.set32(OBJECT + 12, flags)
    for index, word in enumerate((0x3F800000, 0x40000000, 0x40400000)):
        machine.set32(POINT + index * 4, word)
    if extra:
        for addr, value in extra.items():
            machine.set32(addr, value)
    machine.write32(4, OBJECT)
    machine.write32(5, POINT)
    machine.write32(6, type_bits)
    machine.write32(7, ARGUMENTS)
    if regs:
        for index, value in regs.items():
            machine.write32(index, value)
    if stack:
        for index, value in enumerate(stack):
            machine.set32(STACK + index * 8, value)
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
    text, relocs, symbols, _named = parse_object(output)
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
    compare_candle2(data, loads)
    compare_container(data, loads)
    compare_candle(data, loads)
    compare_container_resources(data, loads)
    compare_eggsack(data, loads)
    compare_doorsecret(data, loads)


def compare_candle2(data, loads):
    original = read_vaddr(data, loads, 0x002310A8, 0x78)
    output = Path("/tmp/actors-a-candle2.o")
    compile_object(ROOT / "experimental/sources/actors-a-candle2/candle2.cpp", output)
    text, relocs, symbols, _named = parse_object(output)
    bases = {
        "norrath_game_object_construct": 0x001411F0,
        "norrath_actor_add_run_list": 0x00143830,
        "norrath_actors_a_view_44a930": 0x0044A930,
    }
    compiled = apply_relocations(text, relocs, symbols, bases)
    original_image = {0x002310A8 + index: byte for index, byte in enumerate(original)}
    compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
    left = run_case(original_image, 0x002310A8, [0, 0x55], flags=0x100)
    right = run_case(compiled_image, CODE, [0, 0x55], flags=0x100)
    point = bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40])
    def used(call):
        name, a0, a1, a2, a3, copied = call
        if name == "norrath_actor_add_run_list":
            return (name, a0)
        return (name, a0, a2, a3, copied)
    ok = (list(map(used, left.calls)) == list(map(used, right.calls)) and
          object_bytes(left, 96) == object_bytes(right, 96) and
          left.low(2) == right.low(2) == 0x55 and
          left.calls[0][0] == "norrath_game_object_construct" and left.calls[0][3] == 0 and
          left.calls[0][4] == 0 and left.calls[0][5] == point and
          used(left.calls[1]) == ("norrath_actor_add_run_list", OBJECT) and
          left.u32(OBJECT + 12) == 0x101 and left.u8(OBJECT + 40) == 0xF0 and
          left.u32(OBJECT + 72) == 0x0044A930 and left.u8(OBJECT + 76) == 0 and left.u8(OBJECT + 77) == 0)
    if not ok:
        print("flags", hex(left.u32(OBJECT + 12)), "b40", hex(left.u8(OBJECT + 40)),
              "view", hex(left.u32(OBJECT + 72)), "b76", left.u8(OBJECT + 76), left.u8(OBJECT + 77))
        print("point", left.calls[0][5], point)
        print("candle2 calls", left.calls, right.calls)
        print("return", hex(left.low(2)), hex(right.low(2)))
        for index, (a, b) in enumerate(zip(object_bytes(left, 96), object_bytes(right, 96))):
            if a != b:
                print(" byte", index, hex(a), hex(b))
        raise SystemExit("candle2 comparison failed")
    print("candle2: original/compiled scenario matched")


def compare_container(data, loads):
    original = read_vaddr(data, loads, 0x0021D578, 0xB4)
    output = Path("/tmp/actors-a-container.o")
    compile_object(ROOT / "experimental/sources/actors-a-container/container.cpp", output)
    text, relocs, symbols, named = parse_object(output)
    pool = 0x220000
    bases = {
        "norrath_game_object_construct": 0x001411F0,
        "norrath_world_check_for_water": 0x0017DB78,
        "norrath_actors_a_view_44ccb8": 0x0044CCB8,
        "norrath_factory_world_pointer_word": 0x004844A0,
        ".LC0": pool,
    }
    compiled = apply_relocations(text, relocs, symbols, bases)
    original_image = {0x0021D578 + index: byte for index, byte in enumerate(original)}
    compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
    for index, byte in enumerate(named.get(".rodata.cst4", b"")):
        compiled_image[pool + index] = byte
    extra = {0x004844A0: 0x20001000}
    # Base returns 0. Water returns 0x1a5 so the stored byte and V0 can differ.
    left = run_case(original_image, 0x0021D578, [0, 0x1A5], type_bits=0x00AB, extra=extra)
    right = run_case(compiled_image, CODE, [0, 0x1A5], type_bits=0x00AB, extra=extra)
    incoming = bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40])
    position = bytes([0x5A]) * 12
    ok = (left.low(2) == right.low(2) == 0x1A5 and object_bytes(left, 140) == object_bytes(right, 140) and
          left.calls[0][0] == right.calls[0][0] == "norrath_game_object_construct" and
          left.calls[0][3] == right.calls[0][3] == 0 and left.calls[0][4] == right.calls[0][4] == ARGUMENTS and
          left.calls[0][5] == right.calls[0][5] == incoming and
          left.calls[1][0] == right.calls[1][0] == "norrath_world_check_for_water" and
          left.calls[1][1] == right.calls[1][1] == 0x20001000 and left.calls[1][3] == right.calls[1][3] == 0 and
          left.calls[1][5] == right.calls[1][5] == position and
          left.floats == right.floats == [0x41400000] and
          left.u32(OBJECT + 12) == 0x10 and left.u32(OBJECT + 72) == 0x0044CCB8 and
          left.u8(OBJECT + 56) == 0xAB and left.u8(OBJECT + 57) == 0 and
          left.u32(OBJECT + 116) == 0 and left.u32(OBJECT + 120) == 0xFFFFFFFF and
          left.u8(OBJECT + 124) == 0xA5)
    if not ok:
        print("container calls", left.calls)
        print("compiled", right.calls)
        print("floats", [hex(x) for x in left.floats], [hex(x) for x in right.floats])
        print("return", hex(left.low(2)), hex(right.low(2)))
        print("flags", hex(left.u32(OBJECT + 12)), "view", hex(left.u32(OBJECT + 72)),
              "half", hex(left.u8(OBJECT + 56) | (left.u8(OBJECT + 57) << 8)),
              "b124", hex(left.u8(OBJECT + 124)))
        for index, (a, b) in enumerate(zip(object_bytes(left, 140), object_bytes(right, 140))):
            if a != b:
                print(" byte", index, hex(a), hex(b))
        raise SystemExit("container comparison failed")
    print("container: original/compiled scenario matched")


def compare_candle(data, loads):
    original = read_vaddr(data, loads, 0x002301A8, 0x14C)
    output = Path("/tmp/actors-a-candle.o")
    compile_object(ROOT / "experimental/sources/actors-a-candle/candle.cpp", output)
    text, relocs, symbols, named = parse_object(output)
    bases = {
        "norrath_game_object_construct": 0x001411F0,
        "norrath_actor_add_run_list": 0x00143830,
        "norrath_light_add": 0x0013D230,
        "norrath_actors_a_view_44a9c0": 0x0044A9C0,
        "norrath_actors_a_view_481980": 0x00481980,
    }
    for symbol in symbols.values():
        if symbol.startswith(".LC"):
            bases[symbol] = 0x220000
    compiled = apply_relocations(text, relocs, symbols, bases)
    original_image = {0x002301A8 + index: byte for index, byte in enumerate(original)}
    compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
    for index, byte in enumerate(named.get(".rodata.cst4", b"")):
        compiled_image[0x220000 + index] = byte
    extra = {0x00481980: 0x3F000000}
    left = run_case(original_image, 0x002301A8, [0, 0, 0x66], type_bits=0x77, flags=0x10, extra=extra)
    right = run_case(compiled_image, CODE, [0, 0, 0x66], type_bits=0x77, flags=0x10, extra=extra)
    incoming = bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40])
    ok = (left.low(2) == right.low(2) == 0x66 and object_bytes(left, 160) == object_bytes(right, 160) and
          left.snapshots == right.snapshots and len(left.snapshots) == 1 and
          [call[0] for call in left.calls] == [call[0] for call in right.calls] ==
          ["norrath_game_object_construct", "norrath_light_add", "norrath_actor_add_run_list"] and
          left.calls[0][3] == 0 and left.calls[0][4] == 0 and left.calls[0][5] == incoming and
          left.calls[1][1] == right.calls[1][1] == OBJECT + 76 and left.calls[2][1] == OBJECT and
          left.u32(OBJECT + 12) == 0x11 and left.u8(OBJECT + 40) == 0xF0 and left.u8(OBJECT + 85) == 1 and
          left.u32(OBJECT + 72) == 0x0044A9C0 and left.u32(OBJECT + 144) == 0x77 and
          left.u8(OBJECT + 94) == 51 and left.u8(OBJECT + 96) == 39 and left.u8(OBJECT + 98) == 18)
    if not ok:
        print("candle calls", left.calls)
        print("compiled", right.calls)
        print("return", hex(left.low(2)), hex(right.low(2)), "snaps", len(left.snapshots), len(right.snapshots))
        if left.snapshots and right.snapshots and left.snapshots[0] != right.snapshots[0]:
            for index, (a, b) in enumerate(zip(left.snapshots[0], right.snapshots[0])):
                if a != b:
                    print(" light", index, hex(a), hex(b))
        for index, (a, b) in enumerate(zip(object_bytes(left, 160), object_bytes(right, 160))):
            if a != b:
                print(" byte", index, hex(a), hex(b))
        raise SystemExit("candle comparison failed")
    print("candle: original/compiled scenario matched")


def compare_container_resources(data, loads):
    original = read_vaddr(data, loads, 0x0021D6F0, 0x17C)
    output = Path("/tmp/actors-a-container-resources.o")
    compile_object(ROOT / "experimental/sources/actors-a-container-resources/container_resources.cpp", output)
    text, relocs, symbols, named = parse_object(output)
    bases = {
        "norrath_game_object_construct": 0x001411F0,
        "norrath_lump_find_resource": 0x00165DC0,
        "norrath_world_check_for_water": 0x0017DB78,
        "norrath_actors_a_view_44ccb8": 0x0044CCB8,
        "norrath_factory_world_pointer_word": 0x004844A0,
    }
    for symbol in symbols.values():
        if symbol.startswith(".LC"):
            bases[symbol] = 0x220000
    compiled = apply_relocations(text, relocs, symbols, bases)
    original_image = {0x0021D6F0 + index: byte for index, byte in enumerate(original)}
    compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
    pool = 0x220000
    for index, byte in enumerate(named.get(".rodata.cst4", b"")):
        compiled_image[pool + index] = byte
    extra = {0x004844A0: 0x20001000}
    names = {7: 0x1001, 8: 0x1002, 9: 0x1003, 10: 0x1004, 11: 0x1005}
    filled = bytes([0x5A] * 12)
    incoming = bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40])
    scenarios = [
        (0x00AB, [5, 0xFFFFFFFD, 4, ARGUMENTS], 0x100, 0x00100110, 0x40A00000, 0xC0400000, 4),
        (-2 & 0xFFFFFFFF, [0, 0, 1, ARGUMENTS], 0, 0x10, 0, 0, 1),
    ]
    for type_bits, stack, flags, expect_flags, amount_bits, other_bits, bits in scenarios:
        left = run_case(original_image, 0x0021D6F0, [0, 0x11111111, 0x22222222, 0x1A5],
                        type_bits=type_bits, flags=flags, extra=extra, regs=names, stack=stack)
        right = run_case(compiled_image, CODE, [0, 0x11111111, 0x22222222, 0x1A5],
                         type_bits=type_bits, flags=flags, extra=extra, regs=names, stack=stack)
        names_ok = ([call[0] for call in left.calls] == [call[0] for call in right.calls] ==
                    ["norrath_game_object_construct", "norrath_lump_find_resource",
                     "norrath_lump_find_resource", "norrath_world_check_for_water"])
        calls_ok = (names_ok and left.calls[0][3] == right.calls[0][3] == 0 and
                    left.calls[0][4] == right.calls[0][4] == ARGUMENTS and left.calls[0][5] == incoming and
                    left.calls[1][1:3] == right.calls[1][1:3] == (0x1001, 0x1002) and
                    left.calls[2][1:3] == right.calls[2][1:3] == (0x1001, 0x1003) and
                    left.calls[3][1] == right.calls[3][1] == 0x20001000 and
                    left.calls[3][3] == right.calls[3][3] == 0 and left.calls[3][5] == filled and
                    left.floats == right.floats == [0x41400000])
        state_ok = (object_bytes(left, 160) == object_bytes(right, 160) and left.low(2) == right.low(2) == 0x1A5 and
                    left.u8(OBJECT + 124) == 0xA5 and left.u32(OBJECT + 12) == expect_flags and
                    left.u32(OBJECT + 28) == amount_bits and left.u32(OBJECT + 32) == other_bits and
                    left.u32(OBJECT + 72) == 0x0044CCB8 and left.u32(OBJECT + 96) == 0x11111111 and
                    left.u32(OBJECT + 100) == 0x22222222 and left.u32(OBJECT + 104) == bits and
                    left.u32(OBJECT + 116) == 0 and left.u32(OBJECT + 120) == 0xFFFFFFFF and
                    left.u32(OBJECT + 76) == 0x1001 and left.u32(OBJECT + 80) == 0x1002 and
                    left.u32(OBJECT + 84) == 0x1003 and left.u32(OBJECT + 88) == 0x1004 and
                    left.u32(OBJECT + 92) == 0x1005 and
                    (left.u8(OBJECT + 56) | (left.u8(OBJECT + 57) << 8)) == (type_bits & 0xFFFF))
        if not calls_ok or not state_ok:
            print("resources calls", left.calls)
            print("compiled", right.calls)
            print("floats", [hex(v) for v in left.floats], [hex(v) for v in right.floats])
            print("return", hex(left.low(2)), hex(right.low(2)), "flags", hex(left.u32(OBJECT + 12)), hex(right.u32(OBJECT + 12)))
            for index, (a, b) in enumerate(zip(object_bytes(left, 160), object_bytes(right, 160))):
                if a != b:
                    print(" byte", index, hex(a), hex(b))
            raise SystemExit("container resources comparison failed")
    print("container resources: 2 original/compiled scenarios matched")


def compare_eggsack(data, loads):
    original = read_vaddr(data, loads, 0x002260B8, 0x88)
    output = Path("/tmp/actors-a-eggsack.o")
    compile_object(ROOT / "experimental/sources/actors-a-eggsack/eggsack.cpp", output)
    text, relocs, symbols, _named = parse_object(output)
    bases = {
        "norrath_construct_AutoPropLP": 0x00223BD8,
        "norrath_actors_a_view_44f2a0": 0x0044F2A0,
    }
    compiled = apply_relocations(text, relocs, symbols, bases)
    original_image = {0x002260B8 + index: byte for index, byte in enumerate(original)}
    compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
    incoming = bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40])
    scenarios = [(0xAB, 0x100, 0x102), (0xFFFFFFFE, 0, 2)]
    for type_bits, seed, expect in scenarios:
        regs = {5: 0x1001, 6: POINT, 7: type_bits, 8: ARGUMENTS}
        extra = {OBJECT + 316: seed}
        left = run_case(original_image, 0x002260B8, [0], extra=extra, regs=regs, object_size=480)
        right = run_case(compiled_image, CODE, [0], extra=extra, regs=regs, object_size=480)
        ok = (left.low(2) == right.low(2) == OBJECT + 436 and
              object_bytes(left, 480) == object_bytes(right, 480) and
              left.calls[0][0] == right.calls[0][0] == "norrath_construct_AutoPropLP" and
              left.calls[0][1] == OBJECT and left.calls[0][2] == 0x1001 and
              left.calls[0][4] == right.calls[0][4] == (type_bits & 0xFFFFFFFF) and
              left.calls[0][5] == incoming and left.snapshots == right.snapshots ==
              [struct.pack("<I", ARGUMENTS)] and left.u32(OBJECT + 72) == 0x0044F2A0 and
              left.u32(OBJECT + 316) == expect and left.u32(OBJECT + 440) == 2 and
              left.u32(OBJECT + 444) == 2 and left.u32(OBJECT + 448) == 2 and
              left.u32(OBJECT + 452) == 2)
        if not ok:
            print("eggsack", hex(type_bits), left.calls, right.calls)
            print("return", hex(left.low(2)), hex(right.low(2)), "snaps", left.snapshots, right.snapshots)
            for index, (a, b) in enumerate(zip(object_bytes(left, 480), object_bytes(right, 480))):
                if a != b:
                    print(" byte", index, hex(a), hex(b))
            raise SystemExit("eggsack comparison failed")
    print("eggsack: 2 original/compiled scenarios matched")


def compare_doorsecret(data, loads):
    original = read_vaddr(data, loads, 0x00212F08, 0x14C)
    output = Path("/tmp/actors-a-doorsecret.o")
    compile_object(ROOT / "experimental/sources/actors-a-doorsecret/doorsecret.cpp", output)
    text, relocs, symbols, _named = parse_object(output)
    bases = {
        "norrath_game_object_construct": 0x001411F0,
        "norrath_actor_find_tag_string": 0x00143A80,
        "norrath_world_find_closest_door": 0x00175440,
        "norrath_world_set_door_collision": 0x0017D348,
        "norrath_world_get_door_height": 0x0017D3D0,
        "norrath_object_add_to_delete_list": 0x001437E8,
        "norrath_actors_a_view_44d0a8": 0x0044D0A8,
        "norrath_actors_a_view_4498b0": 0x004498B0,
        "norrath_actors_a_view_485358": 0x00485358,
        "norrath_factory_world_pointer_word": 0x004844A0,
    }
    compiled = apply_relocations(text, relocs, symbols, bases)
    original_image = {0x00212F08 + index: byte for index, byte in enumerate(original)}
    compiled_image = {CODE + index: byte for index, byte in enumerate(compiled)}
    extra = {0x004844A0: 0x20001000}
    incoming = bytes([0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40])
    filled = bytes([0x5A] * 12)
    # tag absent / positive door, then tag present / negative door.
    scenarios = [
        ([0, 0, 5, 0, 0x1234, 0], 0x100, 0x00100103, 0x00100103, 5, 0x1234,
         ["norrath_game_object_construct", "norrath_actor_find_tag_string",
          "norrath_world_find_closest_door", "norrath_world_set_door_collision",
          "norrath_world_get_door_height", "norrath_world_set_door_collision"]),
        ([0, 1, 0x8000, 0, 0xAB, 0x99, 0], 0, 3, 0x00100003, 0x8000, 0xAB,
         ["norrath_game_object_construct", "norrath_actor_find_tag_string",
          "norrath_world_find_closest_door", "norrath_world_set_door_collision",
          "norrath_world_get_door_height", "norrath_object_add_to_delete_list",
          "norrath_world_set_door_collision"]),
    ]
    for answers, flags, mid_flags, final_flags, door, height, names in scenarios:
        signed = door if door < 0x8000 else door - 0x10000
        left = run_case(original_image, 0x00212F08, answers, flags=flags, extra=extra, regs={6: ARGUMENTS})
        right = run_case(compiled_image, CODE, answers, flags=flags, extra=extra, regs={6: ARGUMENTS})
        names_ok = [call[0] for call in left.calls] == [call[0] for call in right.calls] == names
        snap = struct.pack("<II", mid_flags, 0x0044D0A8)
        ok = (names_ok and left.low(2) == right.low(2) == 0x00100000 and
              object_bytes(left, 96) == object_bytes(right, 96) and
              left.snapshots == right.snapshots == [snap] and
              left.calls[0][3] == 0 and left.calls[0][4] == ARGUMENTS and left.calls[0][5] == incoming and
              left.calls[1][1] == OBJECT and left.calls[1][2] == 0x00485358 and left.calls[1][3] == 0 and
              left.calls[2][1] == 0x20001000 and left.calls[2][5] == filled and
              left.u32(OBJECT + 12) == final_flags and left.u32(OBJECT + 72) == 0x004498B0 and
              (left.u8(OBJECT + 76) | (left.u8(OBJECT + 77) << 8)) == door and
              (left.u8(OBJECT + 78) | (left.u8(OBJECT + 79) << 8)) == height)
        for side in (left, right):
            for call in side.calls:
                if call[0] == "norrath_world_set_door_collision":
                    ok = ok and call[1] == 0x20001000 and call[2] == (signed & 0xFFFFFFFF) and call[3] == 1
                if call[0] == "norrath_world_get_door_height":
                    ok = ok and call[1] == 0x20001000 and call[2] == (signed & 0xFFFFFFFF)
                if call[0] == "norrath_object_add_to_delete_list":
                    ok = ok and call[1] == OBJECT
        if not ok:
            print("doorsecret", names)
            print("left", left.calls)
            print("right", right.calls)
            print("return", hex(left.low(2)), hex(right.low(2)), "flags", hex(left.u32(OBJECT + 12)), hex(right.u32(OBJECT + 12)))
            print("snaps", left.snapshots, right.snapshots)
            for index, (a, b) in enumerate(zip(object_bytes(left, 96), object_bytes(right, 96))):
                if a != b:
                    print(" byte", index, hex(a), hex(b))
            raise SystemExit("doorsecret comparison failed")
    print("doorsecret: 2 original/compiled scenarios matched")


if __name__ == "__main__":
    main()
