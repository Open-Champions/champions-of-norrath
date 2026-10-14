#!/usr/bin/env python3
"""Differential check of the combat-reaction bodies against the private executable.

Runs one function from the private SLUS_205.65 image and the same function built
from the authored source (compiled and linked with the reviewed EE toolchain at a
scratch address) in a small R5900 interpreter. Both runs get the same randomized
scenario and the same stubbed callees. The stub call logs (argument registers and
point bytes included) and every byte of the scenario object afterwards must be
equal. The executable and toolchain stay outside the repository.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/combat-reactions-compare.py all 1500

Scenario floats are small integers, so the EE's truncating rounding and this
interpreter's rounding agree. Only the instructions these bodies use exist here;
any other opcode stops the run with its address. Nothing from the executable is
stored; scratch files go to a fresh temporary directory.
"""
import os
import random
import struct
import subprocess
import sys
import tempfile

M32 = 0xFFFFFFFF
M64 = 0xFFFFFFFFFFFFFFFF


def s32(v):
    v &= M32
    return v - (1 << 32) if v & 0x80000000 else v


def s64(v):
    v &= M64
    return v - (1 << 64) if v & (1 << 63) else v


def sx16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def f2b(x):
    return struct.unpack('<I', struct.pack('<f', x))[0]


def b2f(b):
    return struct.unpack('<f', struct.pack('<I', b & M32))[0]


def fround(x):
    try:
        return struct.unpack('<f', struct.pack('<f', x))[0]
    except OverflowError:
        return float('inf') if x > 0 else float('-inf')


class Image:
    """Read-only loaded segments of an ELF file."""

    def __init__(self, path):
        data = open(path, 'rb').read()
        if data[:4] != b'\x7fELF':
            raise SystemExit('not an ELF: ' + str(path))
        phoff, = struct.unpack_from('<I', data, 28)
        phentsize, phnum = struct.unpack_from('<HH', data, 42)
        self.segs = []
        for i in range(phnum):
            p_type, off, vaddr, _, filesz, memsz = struct.unpack_from('<IIIIII', data, phoff + i * phentsize)
            if p_type == 1 and memsz:
                blob = data[off:off + filesz] + b'\0' * (memsz - filesz)
                self.segs.append((vaddr, blob))

    def byte(self, addr):
        for vaddr, blob in self.segs:
            if vaddr <= addr < vaddr + len(blob):
                return blob[addr - vaddr]
        return 0


class Stop(Exception):
    pass


class Machine:
    def __init__(self, image, overlay, stubs, gp):
        self.image = image
        self.mem = overlay          # dict addr -> byte, written/initial scenario memory
        self.stubs = stubs          # addr -> handler(machine) returning None
        self.r = [0] * 32
        self.f = [0] * 32
        self.cond = False
        self.hi = 0
        self.lo = 0
        self.pc = 0
        self.r[28] = gp & M32
        self.log = []
        self.steps = 0

    # memory
    def u8(self, a):
        a &= M32
        v = self.mem.get(a)
        return self.image.byte(a) if v is None else v

    def w8(self, a, v):
        self.mem[a & M32] = v & 0xFF

    def u16(self, a):
        return self.u8(a) | (self.u8(a + 1) << 8)

    def u32(self, a):
        return self.u16(a) | (self.u16(a + 2) << 16)

    def w16(self, a, v):
        self.w8(a, v); self.w8(a + 1, v >> 8)

    def w32(self, a, v):
        self.w16(a, v); self.w16(a + 2, v >> 16)

    def read(self, a, n):
        return bytes(self.u8(a + i) for i in range(n))

    def write(self, a, blob):
        for i, b in enumerate(blob):
            self.w8(a + i, b)

    def setr(self, i, v):
        if i:
            self.r[i] = v & M64

    def setr32(self, i, v):
        self.setr(i, s32(v))

    def g32(self, i):
        return self.r[i] & M32

    def fv(self, i):
        return b2f(self.f[i])

    def setf(self, i, x):
        self.f[i] = f2b(fround(x))

    # execute one non-control instruction or describe a control transfer
    def exe(self, ins, pc):
        op = ins >> 26
        rs = (ins >> 21) & 31
        rt = (ins >> 16) & 31
        rd = (ins >> 11) & 31
        sa = (ins >> 6) & 31
        fn = ins & 63
        imm = ins & 0xFFFF
        simm = sx16(imm)
        tgt = (pc + 4 + (simm << 2)) & M32
        R = self.r
        if op == 0:
            if fn == 0:
                self.setr32(rd, self.g32(rt) << sa)
            elif fn == 2:
                self.setr32(rd, self.g32(rt) >> sa)
            elif fn == 3:
                self.setr32(rd, s32(self.g32(rt)) >> sa)
            elif fn == 4:
                self.setr32(rd, self.g32(rt) << (self.g32(rs) & 31))
            elif fn == 6:
                self.setr32(rd, self.g32(rt) >> (self.g32(rs) & 31))
            elif fn == 7:
                self.setr32(rd, s32(self.g32(rt)) >> (self.g32(rs) & 31))
            elif fn == 8:
                return ('jump', self.g32(rs), None)
            elif fn == 9:
                t = self.g32(rs)
                self.setr32(rd, pc + 8)
                return ('jump', t, 'link')
            elif fn == 0x0A:
                if self.g32(rt) == 0 and (R[rt] & M64) == 0:
                    self.setr(rd, R[rs])
            elif fn == 0x0B:
                if (R[rt] & M64) != 0:
                    self.setr(rd, R[rs])
            elif fn == 0x10:
                self.setr(rd, self.hi)
            elif fn == 0x11:
                self.hi = R[rs]
            elif fn == 0x12:
                self.setr(rd, self.lo)
            elif fn == 0x13:
                self.lo = R[rs]
            elif fn in (0x18, 0x19):
                a = s32(self.g32(rs)) if fn == 0x18 else self.g32(rs)
                b = s32(self.g32(rt)) if fn == 0x18 else self.g32(rt)
                p = a * b
                self.lo = s32(p & M32) & M64
                self.hi = s32((p >> 32) & M32) & M64
                if rd:
                    self.setr(rd, self.lo)
            elif fn in (0x1A, 0x1B):
                if fn == 0x1A:
                    a, b = s32(self.g32(rs)), s32(self.g32(rt))
                    if b:
                        q = abs(a) // abs(b)
                        q = -q if (a < 0) != (b < 0) else q
                        self.lo = s32(q) & M64
                        self.hi = s32(a - q * b) & M64
                else:
                    a, b = self.g32(rs), self.g32(rt)
                    if b:
                        self.lo = s32(a // b) & M64
                        self.hi = s32(a % b) & M64
            elif fn in (0x20, 0x21):
                self.setr32(rd, self.g32(rs) + self.g32(rt))
            elif fn in (0x22, 0x23):
                self.setr32(rd, self.g32(rs) - self.g32(rt))
            elif fn == 0x24:
                self.setr(rd, R[rs] & R[rt])
            elif fn == 0x25:
                self.setr(rd, R[rs] | R[rt])
            elif fn == 0x26:
                self.setr(rd, R[rs] ^ R[rt])
            elif fn == 0x27:
                self.setr(rd, ~(R[rs] | R[rt]))
            elif fn == 0x2A:
                self.setr(rd, 1 if s64(R[rs]) < s64(R[rt]) else 0)
            elif fn == 0x2B:
                self.setr(rd, 1 if (R[rs] & M64) < (R[rt] & M64) else 0)
            elif fn in (0x2C, 0x2D):
                self.setr(rd, R[rs] + R[rt])
            elif fn in (0x2E, 0x2F):
                self.setr(rd, R[rs] - R[rt])
            elif fn == 0x38:
                self.setr(rd, R[rt] << sa)
            elif fn == 0x3A:
                self.setr(rd, (R[rt] & M64) >> sa)
            elif fn == 0x3B:
                self.setr(rd, s64(R[rt]) >> sa)
            elif fn == 0x3C:
                self.setr(rd, R[rt] << (sa + 32))
            elif fn == 0x3E:
                self.setr(rd, (R[rt] & M64) >> (sa + 32))
            elif fn == 0x3F:
                self.setr(rd, s64(R[rt]) >> (sa + 32))
            else:
                raise Stop('SPECIAL %#x at %#x' % (fn, pc))
            return None
        if op == 1:
            v = s64(R[rs])
            if rt == 0:
                return ('br', tgt, v < 0, False)
            if rt == 1:
                return ('br', tgt, v >= 0, False)
            if rt == 2:
                return ('br', tgt, v < 0, True)
            if rt == 3:
                return ('br', tgt, v >= 0, True)
            raise Stop('REGIMM at %#x' % pc)
        if op == 2:
            return ('jump', ((pc + 4) & 0xF0000000) | ((ins & 0x3FFFFFF) << 2), None)
        if op == 3:
            self.setr32(31, pc + 8)
            return ('jump', ((pc + 4) & 0xF0000000) | ((ins & 0x3FFFFFF) << 2), 'link')
        if op == 4:
            return ('br', tgt, (R[rs] & M64) == (R[rt] & M64), False)
        if op == 5:
            return ('br', tgt, (R[rs] & M64) != (R[rt] & M64), False)
        if op == 6:
            return ('br', tgt, s64(R[rs]) <= 0, False)
        if op == 7:
            return ('br', tgt, s64(R[rs]) > 0, False)
        if op in (8, 9):
            self.setr32(rt, self.g32(rs) + simm)
        elif op in (24, 25):
            self.setr(rt, R[rs] + simm)
        elif op == 10:
            self.setr(rt, 1 if s64(R[rs]) < simm else 0)
        elif op == 11:
            self.setr(rt, 1 if (R[rs] & M64) < (simm & M64) else 0)
        elif op == 12:
            self.setr(rt, R[rs] & imm)
        elif op == 13:
            self.setr(rt, R[rs] | imm)
        elif op == 14:
            self.setr(rt, R[rs] ^ imm)
        elif op == 15:
            self.setr32(rt, imm << 16)
        elif op == 17:
            return self.cop1(ins, pc, rs, rt, rd, sa, fn, tgt)
        elif op == 20:
            return ('br', tgt, (R[rs] & M64) == (R[rt] & M64), True)
        elif op == 21:
            return ('br', tgt, (R[rs] & M64) != (R[rt] & M64), True)
        elif op == 22:
            return ('br', tgt, s64(R[rs]) <= 0, True)
        elif op == 23:
            return ('br', tgt, s64(R[rs]) > 0, True)
        else:
            a = (self.g32(rs) + simm) & M32
            if op == 32:
                v = self.u8(a); self.setr32(rt, v - 256 if v & 0x80 else v)
            elif op == 33:
                self.setr32(rt, sx16(self.u16(a)))
            elif op == 35:
                self.setr32(rt, self.u32(a))
            elif op == 36:
                self.setr(rt, self.u8(a))
            elif op == 37:
                self.setr(rt, self.u16(a))
            elif op == 39:
                self.setr(rt, self.u32(a))
            elif op == 40:
                self.w8(a, self.g32(rt))
            elif op == 41:
                self.w16(a, self.g32(rt))
            elif op == 43:
                self.w32(a, self.g32(rt))
            elif op == 55:
                self.setr(rt, self.u32(a) | (self.u32(a + 4) << 32))
            elif op == 63:
                v = R[rt] & M64
                self.w32(a, v); self.w32(a + 4, v >> 32)
            elif op == 26:   # ldl
                v = R[rt] & M64
                for k in range(8 - (a & 7)):
                    pass
                n = (a & 7) + 1
                cur = v
                for k in range(n):
                    cur = (cur & ~(0xFF << (8 * (7 - k)))) | (self.u8(a - k) << (8 * (7 - k)))
                self.setr(rt, cur)
            elif op == 27:   # ldr
                v = R[rt] & M64
                n = 8 - (a & 7)
                cur = v
                for k in range(n):
                    cur = (cur & ~(0xFF << (8 * k))) | (self.u8(a + k) << (8 * k))
                self.setr(rt, cur)
            elif op == 44:   # sdl
                v = R[rt] & M64
                n = (a & 7) + 1
                for k in range(n):
                    self.w8(a - k, v >> (8 * (7 - k)))
            elif op == 45:   # sdr
                v = R[rt] & M64
                n = 8 - (a & 7)
                for k in range(n):
                    self.w8(a + k, v >> (8 * k))
            elif op == 34:   # lwl
                cur = self.g32(rt)
                for k in range((a & 3) + 1):
                    cur = (cur & ~(0xFF << (8 * (3 - k)))) | (self.u8(a - k) << (8 * (3 - k)))
                self.setr32(rt, cur)
            elif op == 38:   # lwr
                cur = self.g32(rt)
                for k in range(4 - (a & 3)):
                    cur = (cur & ~(0xFF << (8 * k))) | (self.u8(a + k) << (8 * k))
                self.setr32(rt, cur)
            elif op == 42:   # swl
                v = self.g32(rt)
                for k in range((a & 3) + 1):
                    self.w8(a - k, v >> (8 * (3 - k)))
            elif op == 46:   # swr
                v = self.g32(rt)
                for k in range(4 - (a & 3)):
                    self.w8(a + k, v >> (8 * k))
            elif op == 0x1E:   # lq (low 64 bits only; upper half kept in a side table)
                self.setr(rt, self.u32(a & ~15) | (self.u32((a & ~15) + 4) << 32))
                self.q = getattr(self, 'q', {})
                self.q[rt] = self.u32((a & ~15) + 8) | (self.u32((a & ~15) + 12) << 32)
            elif op == 0x1F:   # sq
                q = getattr(self, 'q', {}).get(rt, 0)
                v = R[rt] & M64
                b = a & ~15
                self.w32(b, v); self.w32(b + 4, v >> 32); self.w32(b + 8, q); self.w32(b + 12, q >> 32)
            elif op == 49:
                self.f[rt] = self.u32(a)
            elif op == 57:
                self.w32(a, self.f[rt])
            else:
                raise Stop('opcode %d at %#x' % (op, pc))
        return None

    def cop1(self, ins, pc, rs, rt, rd, sa, fn, tgt):
        if rs == 0:
            self.setr32(rt, self.f[rd]); return None
        if rs == 4:
            self.f[rd] = self.g32(rt); return None
        if rs == 8:
            taken = self.cond if (rt & 1) else not self.cond
            return ('br', tgt, taken, bool(rt & 2))
        fs, ft, fd = rd, rt, sa
        if rs == 0x10:
            a, b = self.fv(fs), self.fv(ft)
            if fn == 0:
                self.setf(fd, a + b)
            elif fn == 1:
                self.setf(fd, a - b)
            elif fn == 2:
                self.setf(fd, a * b)
            elif fn == 3:
                self.setf(fd, a / b if b else float('inf'))
            elif fn == 5:
                self.setf(fd, abs(a))
            elif fn == 6:
                self.f[fd] = self.f[fs]
            elif fn == 7:
                self.setf(fd, -a)
            elif fn == 0x24:
                self.f[fd] = struct.unpack('<I', struct.pack('<i', int(a)))[0]
            elif fn == 0x30:
                self.cond = False
            elif fn == 0x32:
                self.cond = a == b
            elif fn == 0x34:
                self.cond = a < b
            elif fn == 0x36:
                self.cond = a <= b
            else:
                raise Stop('COP1.S %#x at %#x' % (fn, pc))
            return None
        if rs == 0x14 and fn == 0x20:
            self.setf(fd, float(s32(self.f[fs])))
            return None
        raise Stop('COP1 %#x at %#x' % (ins, pc))

    def run(self, entry, steps=200000):
        self.pc = entry
        self.r[31] = 1
        while True:
            if self.pc == 1:
                return
            self.steps += 1
            if self.steps > steps:
                raise Stop('step limit')
            pc = self.pc
            eff = self.exe(self.u32(pc), pc)
            if eff is None:
                self.pc = (pc + 4) & M32
                continue
            dpc = (pc + 4) & M32
            if eff[0] == 'jump':
                target = eff[1]
                nested = self.exe(self.u32(dpc), dpc)
                if nested is not None:
                    raise Stop('control transfer in delay slot at %#x' % dpc)
                if target in self.stubs:
                    self.stubs[target](self)
                    self.pc = (pc + 8) & M32 if eff[2] == 'link' else self.g32(31)
                else:
                    self.pc = target
                continue
            _, target, taken, likely = eff
            if taken or not likely:
                nested = self.exe(self.u32(dpc), dpc)
                if nested is not None:
                    raise Stop('control transfer in delay slot at %#x' % dpc)
            self.pc = target if taken else (pc + 8) & M32




EXE = os.environ.get('NORRATH_EXE')
TC = os.environ.get('NORRATH_EE')
if not EXE or not TC:
    raise SystemExit('set NORRATH_EXE to the private executable and NORRATH_EE to the reviewed EE toolchain')
SRC = os.environ.get('NORRATH_SRC') or os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'experimental', 'sources')
SCRATCH = tempfile.mkdtemp(prefix='combat-reactions-compare-')
P = TC + '/bin/mips64r5900el-ps2-elf-'
FLAGS = ['-G0', '-O2', '-ffreestanding', '-fno-asynchronous-unwind-tables', '-fno-exceptions', '-fno-pic',
         '-fno-rtti', '-fno-stack-protector', '-fno-unwind-tables', '-fomit-frame-pointer', '-fwrapv',
         '-mabi=eabi', '-march=r5900', '-mfp32', '-mgp64', '-mlong32', '-mno-abicalls', '-msingle-float',
         '-std=c++17', '-nostdinc', '-nostdinc++', '-x', 'c++']
GP = 0x4879F0
FRAME_NM = 0x4844A4
OBJ = 0x30000000
OBJ_SIZE = 0x400
TABLE = 0x30010000
VT = 0x30020000
THING = 0x30030000       # THING + 0x100 * i
NODE = 0x30040000        # NODE + 0x20 * i
STACK = 0x3F000000
VCALL = 0x00600000       # virtual-call stub entries, 0x10 apart
STUB = 0x00500000        # imported-function stubs, 0x10 apart

# logical stub name -> (original address, authored symbol)
STUBS = {
    'eRand': (0x153FA8, 'norrath_actor_random'),
    'addTransition': (0x160128, 'norrath_animation_add_transition'),
    'addOneShot': (0x160010, 'animAddOneShot__FP14AnimationStateP15AnimationHeaderffi'),
    'resetRouting': (0x113460, 'resetRouting__8Creature'),
    'smartDestination': (0x10D250, 'getSmartRandomDestination__8Creaturei'),
    'stateProcessing': (0x1129F0, 'setStateProcessing__8CreaturePi'),
    'animSetToEnd': (0x163BA0, 'norrath_animation_set_to_end'),
    'animGlobalSync': (0x1639E8, 'animGlobalSync__FP14AnimationState'),
    'floorAdd': (0x191EF8, 'floorAdd__FG6Point3sfii'),
    'findInBox': (0x13EF30, 'objectFindInBox__FG6Point3T0PP10GameObjecti'),
    'listDestroy': (0x194300, 'norrath_list_destroy'),
    'listLink': (0x194390, 'norrath_list_oneway_link'),
    'lumpFind': (0x165DC0, 'norrath_lump_find_resource'),
    'soundPlay': (0x24D308, 'norrath_sound_play_sequence'),
    'giveGold': (0x1C4190, 'hudGivePlayerGold__FiG6Point3i'),
    'addDelete': (0x1437E8, 'norrath_object_add_to_delete_list'),
}
NAMES = list(STUBS)
AUTH_TO_LOGICAL = {v[1]: k for k, v in STUBS.items()}


class Script:
    """Deterministic responses shared by both runs."""

    def __init__(self, seed, params):
        self.rng = random.Random(seed ^ 0x5eed)
        self.p = params
        self.things = params.get('things', [])


def make_handlers(m, script, order):
    rng = script.rng
    p = script.p

    def lg(*items):
        m.log.append(items)

    def h_eRand(mm):
        v = rng.choice(p.get('rand_pool', [0, 1, 7, 59, 119, 120, 299, 479, 1799, 65535, 0x18005, 0xFFFFFFFF, 12345]))
        lg('eRand', mm.g32(4))
        mm.setr32(2, v)

    def h_addTransition(mm):
        lg('addTransition', mm.g32(4), mm.g32(5), mm.f[12], mm.g32(6))

    def h_addOneShot(mm):
        lg('addOneShot', mm.g32(4), mm.g32(5), mm.f[12], mm.f[13], mm.g32(6))

    def h_resetRouting(mm):
        lg('resetRouting', mm.g32(4))

    def h_smart(mm):
        lg('smartDestination', mm.g32(5), mm.g32(6))
        blob = bytes(rng.randrange(256) for _ in range(12))
        mm.write(mm.g32(4), blob)

    def h_state(mm):
        lg('stateProcessing', mm.g32(4))
        if rng.random() < p.get('rewrite_chance', 0.3):
            mm.w32(mm.g32(5), rng.choice([0, 1, 3, 5, 6, 8, 9, 10, 2, 12]))
        mm.setr32(2, 1 if rng.random() < p.get('block_chance', 0.2) else 0)

    def h_end(mm):
        lg('animSetToEnd', mm.g32(4))

    def h_sync(mm):
        lg('animGlobalSync', mm.g32(4))

    def h_floor(mm):
        a1 = mm.g32(5)
        lg('floorAdd', mm.read(mm.g32(4), 12), a1 & 0xFFFF, mm.f[12], mm.g32(6), mm.g32(7))

    def h_box(mm):
        lg('findInBox', mm.read(mm.g32(4), 12), mm.read(mm.g32(5), 12), mm.g32(7))
        out = mm.g32(6)
        chosen = rng.sample(range(len(script.things)), rng.randint(0, len(script.things)))
        for i, t in enumerate(chosen):
            mm.w32(out + 4 * i, script.things[t])
        mm.w32(out + 4 * len(chosen), 0)

    def h_destroy(mm):
        lg('listDestroy', mm.g32(4))

    def h_link(mm):
        lg('listLink', mm.g32(4), mm.g32(5), mm.g32(6))

    def cstr(mm, a):
        out = bytearray()
        while len(out) < 64:
            c = mm.u8(a + len(out))
            if c == 0:
                break
            out.append(c)
        return bytes(out)

    def h_lump(mm):
        lg('lumpFind', cstr(mm, mm.g32(4)), cstr(mm, mm.g32(5)))
        mm.setr32(2, 0x31F00000)

    def h_sound(mm):
        lg('soundPlay', mm.g32(4), mm.g32(5), mm.f[12])

    def h_gold(mm):
        lg('giveGold', mm.g32(4), mm.read(mm.g32(5), 12), mm.g32(6))

    def h_delete(mm):
        lg('addDelete', mm.g32(4))

    handlers = {
        'lumpFind': h_lump, 'soundPlay': h_sound, 'giveGold': h_gold, 'addDelete': h_delete,
        'eRand': h_eRand, 'addTransition': h_addTransition, 'addOneShot': h_addOneShot,
        'resetRouting': h_resetRouting, 'smartDestination': h_smart, 'stateProcessing': h_state,
        'animSetToEnd': h_end, 'animGlobalSync': h_sync, 'floorAdd': h_floor, 'findInBox': h_box,
        'listDestroy': h_destroy, 'listLink': h_link,
    }

    def v_enemy(mm):
        lg('isEnemy', mm.g32(4), mm.g32(5))
        mm.setr32(2, rng.choice(p.get('enemy_answers', [0, 1, 1, 2, 3])))

    def v_death(mm):
        lg('deathSlot', mm.g32(4))

    return handlers, {'enemy': v_enemy, 'death': v_death}


def build_authored(rel, work):
    base = os.path.basename(rel).replace('.cpp', '')
    obj = os.path.join(work, base + '.o')
    subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
    und = [l.split()[-1] for l in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines() if l.strip()]
    defsyms = {}
    for name in und:
        if name == 'frameNm':
            defsyms[name] = FRAME_NM
        elif name in AUTH_TO_LOGICAL:
            defsyms[name] = STUB + 0x10 * NAMES.index(AUTH_TO_LOGICAL[name])
        else:
            raise SystemExit('unmapped authored import: ' + name)
    out = os.path.join(work, base + '.elf')
    cmd = [P + 'ld', '-m', 'elf32lr5900', '-Ttext=0x00400000', '--no-warn-mismatch']
    cmd += ['--defsym=%s=%#x' % kv for kv in defsyms.items()]
    cmd += ['-o', out, obj]
    subprocess.check_call(cmd)
    return out


def symbol_address(elf, name):
    for l in subprocess.check_output([P + 'nm', elf], text=True).splitlines():
        parts = l.split()
        if len(parts) == 3 and parts[2] == name:
            return int(parts[0], 16)
    raise SystemExit('missing symbol ' + name)


def run_one(image, entry, stub_addrs, scenario, seed):
    mem = dict(scenario['mem'])
    stubs = {}
    m = Machine(image, mem, stubs, GP)
    script = Script(seed, scenario.get('params', {}))
    handlers, vh = make_handlers(m, script, NAMES)
    for name, h in handlers.items():
        stubs[stub_addrs[name]] = h
    stubs[VCALL] = vh['enemy']
    stubs[VCALL + 0x10] = vh['death']
    m.r[29] = STACK
    for i, v in enumerate(scenario['args']):
        m.r[4 + i] = v & M32
    for i, v in scenario.get('fargs', {}).items():
        m.f[i] = v
    m.log = []
    m.run(entry)
    return m


def compare(name, runs=200):
    spec = FUNCTIONS[name]
    work = os.path.join(SCRATCH, name)
    os.makedirs(work, exist_ok=True)
    exe = Image(EXE)
    elf = build_authored(spec['src'], work)
    auth = Image(elf)
    auth_entry = symbol_address(elf, spec['symbol'])
    orig_stubs = {k: v[0] for k, v in STUBS.items()}
    auth_stubs = {k: STUB + 0x10 * NAMES.index(k) for k in NAMES}
    bad = 0
    stats = {}
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed))
        results = []
        for image, entry, stubs in ((exe, spec['orig'], orig_stubs), (auth, auth_entry, auth_stubs)):
            try:
                m = run_one(image, entry, stubs, sc, seed)
            except Stop as e:
                print('seed %d: stopped: %s' % (seed, e))
                return 2
            region = {a: m.u8(a) for a in sc['compare']}
            results.append((m.log, region, m.g32(2) if spec.get('returns') else None))
        for e in results[0][0]:
            stats[e[0]] = stats.get(e[0], 0) + 1
        stats['runs_with_link'] = stats.get('runs_with_link', 0) + (1 if any(e[0] == 'listLink' for e in results[0][0]) else 0)
        if results[0] != results[1]:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                a, b = results
                if a[0] != b[0]:
                    n = min(len(a[0]), len(b[0]))
                    k = next((i for i in range(n) if a[0][i] != b[0][i]), n)
                    print('  log differs at %d:\n   orig %s\n   auth %s' % (k, a[0][k:k + 2], b[0][k:k + 2]))
                diff = [hex(x) for x in a[1] if a[1][x] != b[1][x]][:10]
                if diff:
                    print('  memory differs at', diff)
                if a[2] != b[2]:
                    print('  return', a[2], b[2])
    print('%s: %d runs, %d mismatches' % (name, runs, bad))
    if os.environ.get('STATS'):
        print('  calls seen in the original:', stats)
    return 1 if bad else 0


# ---- scenarios ----

def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


def put32(mem, a, v):
    v &= M32
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def putf(mem, a, x):
    put32(mem, a, f2b(x))


def base_creature(rng, mem):
    fill(rng, mem, OBJ, OBJ_SIZE)
    fill(rng, mem, TABLE, 0x100)
    for off in range(0, 0x100, 4):
        put32(mem, TABLE + off, 0x31000000 + rng.randrange(0x1000) * 4)
    fill(rng, mem, VT, 0x100)
    put32(mem, OBJ + 36, TABLE)
    put32(mem, OBJ + 72, VT)
    for slot, fnaddr in ((160, VCALL), (192, VCALL + 0x10), (184, VCALL + 0x20)):
        mem[VT + slot] = rng.choice([0, 8, 0xF0, 0x10, 0x20])
        mem[VT + slot + 1] = 0xFF if mem[VT + slot] == 0xF0 else 0
        put32(mem, VT + slot + 4, fnaddr)
    put32(mem, FRAME_NM, rng.choice([0, 1000, 50000, 0xFFFFFF00]))


def state_arg(rng):
    return rng.choice([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 20, 0x10005, -1, -5, rng.randrange(0, 16)])


def scenario_state(rng):
    mem = {}
    base_creature(rng, mem)
    put32(mem, OBJ + 808, rng.choice([0, 100, 5000, 0xFFFFFFF0]))
    putf(mem, OBJ + 336, rng.choice([-3.0, 0.0, 4.0]))
    put32(mem, OBJ + 160, rng.choice([0, 0x400000, 0x800000, 0xC00000, 0x100000, 0xFFFFFFFF]))
    put32(mem, OBJ + 12, rng.choice([0, 2, 9, 0x100, 0x102, 0xFFFFFFFF]))
    for i in range(3):
        putf(mem, OBJ + 16 + 4 * i, float(rng.randrange(-20, 20)))
    return {'mem': mem, 'args': [OBJ, state_arg(rng)], 'compare': list(range(OBJ, OBJ + OBJ_SIZE)), 'params': {}}


def scenario_charm(rng):
    mem = {}
    fill(rng, mem, OBJ, OBJ_SIZE)
    put32(mem, OBJ + 12, rng.choice([0, 0x20, 0xFFFFFFFF, 0x1F, 0xFFFFFFDF]))
    return {'mem': mem, 'args': [OBJ, rng.choice([0, 1, -1, 0x12345, 7])], 'compare': list(range(OBJ, OBJ + OBJ_SIZE))}


def scenario_gold(rng):
    mem = {}
    fill(rng, mem, OBJ, OBJ_SIZE)
    fill(rng, mem, THING, 0x400)
    mem[OBJ + 76] = rng.choice([0, 1, 0x80, 0xFF])
    put32(mem, OBJ + 128, rng.choice([0, 25, 0xFFFFFFFF]))
    put32(mem, THING + 12, rng.choice([0, 0x40, 0x20000, 0x20040, 0xFFFFFFFF, 0xFFFDFFBF, 0xFFFDFFFF]) if rng.random() < 0.9 else rng.randrange(1 << 32))
    put32(mem, THING + 960, rng.choice([0, 1, 3, 0xFFFFFFFF]))
    return {'mem': mem, 'args': [OBJ, THING], 'compare': list(range(OBJ, OBJ + OBJ_SIZE)) + list(range(THING, THING + 0x400)),
            'params': {}}


def scenario_update_enemy(rng):
    mem = {}
    base_creature(rng, mem)
    things = []
    for i in range(6):
        t = THING + 0x100 * i
        things.append(t)
        fill(rng, mem, t, 0x40)
        for k in range(3):
            putf(mem, t + 16 + 4 * k, float(rng.randrange(-30, 30)))
        putf(mem, t + 28, float(rng.choice([0, 3, 10, 40, 400, 1000])))
    for i in range(2):
        n = NODE + 0x20 * i
        fill(rng, mem, n, 0x20)
        put32(mem, n + 12, rng.choice(things))
    put32(mem, OBJ + 576, rng.choice([0, NODE, NODE]))
    put32(mem, OBJ + 752, rng.choice([0, NODE + 0x20, NODE + 0x20]))
    mem[OBJ + 570] = rng.choice([0, 0, 0, 0, 1, 0xFF])
    mem[OBJ + 796] = rng.choice([0, 0, 0, 0, 3])
    mem[OBJ + 797] = 0
    put32(mem, OBJ + 160, rng.choice([0, 0, 0, 0, 0x4000000]))
    put32(mem, OBJ + 164, rng.choice([0, 0x4000000]))
    for k in range(3):
        putf(mem, OBJ + 16 + 4 * k, float(rng.randrange(-30, 30)))
        putf(mem, OBJ + 776 + 4 * k, float(rng.randrange(-30, 30)))
    putf(mem, OBJ + 28, float(rng.choice([0, 3, 10, 40])))
    putf(mem, OBJ + 788, float(rng.choice([0, 5, 10, 20, 30])))
    fr = rng.choice([1000, 1001, 1031, 2000, 40])
    put32(mem, FRAME_NM, fr)
    put32(mem, OBJ + 812, rng.choice([0, fr - 481, fr - 480, fr - 479, fr + 5, 0xFFFFFFFF]))
    mem[OBJ + 569] = rng.choice([fr & 0x1F, (fr & 0x1F) ^ 1, (fr & 0x1F) | 0x20, 0x80 | (fr & 0x1F), rng.randrange(256)])
    if rng.random() < 0.6:
        # structured edge family: held or attacker target just around each threshold
        mem[OBJ + 570] = 0
        mem[OBJ + 796] = 0; mem[OBJ + 797] = 0
        put32(mem, OBJ + 160, 0); put32(mem, OBJ + 164, 0)
        for k in range(3):
            putf(mem, OBJ + 776 + 4 * k, 0.0)
        leash = float(rng.choice([8, 10, 12]))
        putf(mem, OBJ + 788, leash)
        cx = float(rng.randrange(-4, 14)); cy = float(rng.randrange(-3, 4))
        tx = float(rng.randrange(6, 20)); ty = float(rng.randrange(-4, 5))
        putf(mem, OBJ + 16, cx); putf(mem, OBJ + 20, cy); putf(mem, OBJ + 24, 0.0)
        own = (tx - cx) ** 2 + (ty - cy) ** 2
        t = things[0]
        putf(mem, t + 16, tx); putf(mem, t + 20, ty); putf(mem, t + 24, 0.0)
        putf(mem, OBJ + 28, float(rng.choice([0, 2])))
        sz = own - 50 - (0 if mem[OBJ + 28] == 0 else 2) + rng.choice([-1, 0, 0, 1])
        putf(mem, t + 28, float(sz))
        # make the other things plausible search results too
        for i in range(1, 6):
            putf(mem, things[i] + 16, float(rng.randrange(-12, 12))); putf(mem, things[i] + 20, float(rng.randrange(-12, 12)))
            putf(mem, things[i] + 24, 0.0)
        put32(mem, NODE + 12, t)
        put32(mem, NODE + 0x20 + 12, rng.choice([t, things[1]]))
        put32(mem, OBJ + 576, rng.choice([0, NODE, NODE]))
        put32(mem, OBJ + 752, rng.choice([0, NODE + 0x20, NODE + 0x20]))
        put32(mem, OBJ + 812, rng.choice([fr - 481, fr - 480, fr - 479, 0, fr + 3]))
        mem[OBJ + 569] = rng.choice([fr & 0x1F, (fr & 0x1F) ^ 2])
    cmp_addrs = list(range(OBJ, OBJ + OBJ_SIZE))
    return {'mem': mem, 'args': [OBJ], 'compare': cmp_addrs,
            'params': {'things': things, 'enemy_answers': [0, 1, 1, 1, 1, 2, 3]}}


FUNCTIONS = {}


def reg(symbol, orig, src, scenario, **kw):
    FUNCTIONS[symbol] = dict(symbol=symbol, orig=orig, src=src, scenario=scenario, **kw)


reg('setState__11SmallSpideri', 0x209A10, 'combat-reactions-set-state-spider/set_state_spider.cpp', scenario_state)
reg('setState__3Cati', 0x1EE8A0, 'combat-reactions-set-state-cat/set_state_cat.cpp', scenario_state)
reg('setState__7Mermaidi', 0x1F7F08, 'combat-reactions-set-state-mermaid/set_state_mermaid.cpp', scenario_state)
reg('setState__14GnomeNavigatori', 0x2021B0, 'combat-reactions-set-state-gnome/set_state_gnome.cpp', scenario_state)
reg('charm__8Creaturei', 0x112BA0, 'combat-reactions-charm/charm.cpp', scenario_charm)
reg('msg_collision__4GoldP10GameObject', 0x154DB0, 'combat-reactions-collision-gold/collision_gold.cpp', scenario_gold)
reg('updateEnemy__8Creature', 0x10B558, 'combat-reactions-update-enemy/update_enemy.cpp', scenario_update_enemy)

if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
