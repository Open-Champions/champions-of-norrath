#!/usr/bin/env python3
"""Differential check of the w8-hud-text-items-a functions against the private executable.

For each function, the image's instructions and the same function built from the authored
source (compiled and linked with the reviewed EE toolchain at a scratch address) run in a
small R5900 interpreter. Both runs get the same randomized scenario, the same stubbed callees
and the same data addresses (authored data imports are linked to the image's addresses). The
stub call logs (argument registers and pointed-to bytes included), the return registers and
every byte of the compared regions must be equal. The executable and toolchain stay outside
the repository; nothing from the executable is stored and scratch files go to a fresh
temporary directory.

  NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee \\
      python3 docs/agents/w8-hud-text-items-a-compare.py all 400

The interpreter core (loads, stores, branches, integer and single-precision float
instructions) is adapted from docs/agents/combat-reactions-compare.py, with the three-operand
mult, sqrt.s (source in the ft field on the EE) and rsqrt.s added. Float scenario values are
exact binary fractions so the EE's truncating conversion and this interpreter agree; every
float result is rounded to single precision after each operation in both runs.
Any opcode outside the implemented set stops the run with its address.
"""
import math
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


COVERAGE = None   # set to a set() to record executed addresses


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
        if op == 28:   # MMI: three-operand mult/multu
            if fn in (0x18, 0x19):
                a = s32(self.g32(rs)) if fn == 0x18 else self.g32(rs)
                b = s32(self.g32(rt)) if fn == 0x18 else self.g32(rt)
                p = a * b
                self.lo = s32(p & M32) & M64
                self.hi = s32((p >> 32) & M32) & M64
                if rd:
                    self.setr(rd, self.lo)
                return None
            raise Stop('MMI %#x at %#x' % (fn, pc))
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
            elif fn == 4:
                self.setf(fd, math.sqrt(b) if b >= 0 else float('nan'))
            elif fn == 5:
                self.setf(fd, abs(a))
            elif fn == 6:
                self.f[fd] = self.f[fs]
            elif fn == 7:
                self.setf(fd, -a)
            elif fn == 0x16:   # rsqrt.s fd = fs / sqrt(ft)
                self.setf(fd, a / math.sqrt(b) if b > 0 else float('inf'))
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
            if COVERAGE is not None:
                COVERAGE.add(pc)
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
SCRATCH = tempfile.mkdtemp(prefix='w8-hud-text-items-a-compare-')
P = TC + '/bin/mips64r5900el-ps2-elf-'
FLAGS = ['-G0', '-O2', '-ffreestanding', '-fno-asynchronous-unwind-tables', '-fno-exceptions', '-fno-pic',
         '-fno-rtti', '-fno-stack-protector', '-fno-unwind-tables', '-fomit-frame-pointer', '-fwrapv',
         '-mabi=eabi', '-march=r5900', '-mfp32', '-mgp64', '-mlong32', '-mno-abicalls', '-msingle-float',
         '-std=c++17', '-nostdinc', '-nostdinc++', '-x', 'c++']
GP = 0x4879F0
STUB = 0x00500000        # imported-function stubs, 0x10 apart
STACK = 0x3F000000

# Image symbol table, read once through the toolchain's nm. Names with several local copies are dropped.
NM = {}
SIZEOF = {}
_dup = set()
for _l in subprocess.check_output([P + 'nm', '-S', EXE], text=True).splitlines():
    _p = _l.split()
    if len(_p) == 4:
        SIZEOF[_p[3]] = int(_p[1], 16)
    if len(_p) in (3, 4):
        if _p[-1] in NM:
            _dup.add(_p[-1])
        NM[_p[-1]] = int(_p[0], 16)
for _n in _dup:
    NM.pop(_n, None)

# Data that the authored object imports under a different name, or that has no unique image name.
# logical name -> address in the image. Authored objects that define the symbol themselves (the
# static-initialization units) are located in the authored link by `AUTH_DEFINED`.
OVERRIDE = {
    'anim': 0x7D0DC8,
    'norrath_player_view_31f540': 0x31F540,
    'norrath_inventory_anim_state_view': 0x7CC5C8,
    'norrath_quest_info_anim_state_view': 0x7D0440,
    'norrath_shop_anim_state_view': 0x7CCAD8,
    'torchParticle': 0x7CC9E8,
    'heatParticle': 0x7CCA38,
    'candleParticle': 0x7CCA88,
    'shopList': 0x7CCBE0,
    'p': 0x6E93D0,
    'cache': 0x6E5D80,
    'dummyTex': 0x6E5E40,
    'inventory': 0x427288,
}
SIZES = {'norrath_quest_info_anim_state_view': 160, 'norrath_inventory_anim_state_view': 1056,
         'norrath_shop_anim_state_view': 264, 'torchParticle': 80, 'heatParticle': 80, 'candleParticle': 80,
         'shopList': 2 * 4616}
AUTH_DEFINED = {'norrath_quest_info_anim_state_view', 'norrath_inventory_anim_state_view', 'norrath_shop_anim_state_view',
                'torchParticle', 'heatParticle', 'candleParticle', 'shopList'}


def orig_addr(name):
    if name in OVERRIDE:
        return OVERRIDE[name]
    if name in NM:
        return NM[name]
    raise SystemExit('no image address for ' + name)


class Ctx:
    """Per-run symbol view: authored data imports share the image addresses; symbols the authored
    object defines itself live at the authored link addresses."""

    def __init__(self, authored, elf, stubs_by_name):
        self.authored = authored
        self.elf = elf
        self.stubs = stubs_by_name       # logical stub name -> address for this run

    def addr(self, name):
        if self.authored and name in AUTH_DEFINED:
            return symbol_address(self.elf, name)
        return orig_addr(name)

    def name_of(self, a):
        # (symbol, offset) for an address inside a known object, else the raw address
        best = None
        for n, size in SIZES.items():
            try:
                base = self.addr(n)
            except SystemExit:
                continue
            if base <= a < base + size:
                best = (n, a - base)
        return best if best else a


class Script:
    def __init__(self, seed, params):
        self.rng = random.Random(seed ^ 0x5eed)
        self.p = params


def cstr(mm, a, limit=96):
    out = bytearray()
    while len(out) < limit:
        c = mm.u8(a + len(out))
        if c == 0:
            break
        out.append(c)
    return bytes(out)


def s16(v):
    return sx16(v)


def make_handlers(m, script, ctx):
    rng = script.rng
    p = script.p

    def lg(*items):
        m.log.append(items)

    def ret(v):
        m.setr32(2, v)

    H = {}

    def h(name):
        def deco(fn):
            H[name] = fn
            return fn
        return deco

    @h('memFree__FPv')
    def _(mm):
        lg('memFree', mm.g32(4))

    @h('create__4itemPcPQ24item11descriptionii')
    def _(mm):
        lg('create', cstr(mm, mm.g32(4)), mm.read(mm.g32(5) + 16, 18), mm.g32(6), mm.g32(7))
        d = mm.g32(5)
        mm.write(d, bytes(rng.randrange(256) for _ in range(16)))
        mm.w32(d, p['item_ptr'])
        mm.w32(d + 4, 0)

    @h('blockAlloc__Fi')
    def _(mm):
        lg('blockAlloc', mm.g32(4))
        ret(rng.choice(p.get('alloc_answers', [0, 0x31200000])))

    @h('getProxyMeshId__4itemPQ24item11description')
    def _(mm):
        lg('proxy', mm.read(mm.g32(4), 4))
        ret(rng.randrange(p.get('proxy_max', 6)))

    @h('__6PhPropG6Point3PcN22liPQ24item11descriptionf')
    def _(mm):
        lg('PhProp', mm.g32(4), mm.read(mm.g32(5), 12), cstr(mm, mm.g32(6)), cstr(mm, mm.g32(7)), cstr(mm, mm.g32(8)),
           mm.g32(9), mm.g32(10), mm.read(mm.g32(11), 4), mm.read(mm.g32(11) + 16, 50), mm.f[12])

    @h('strcmp')
    def _(mm):
        a, b = cstr(mm, mm.g32(4)), cstr(mm, mm.g32(5))
        lg('strcmp', a, b)
        ret(0 if a == b else (-1 if a < b else 1))

    @h('scriptGetGameState__FPc')
    def _(mm):
        lg('getState', cstr(mm, mm.g32(4)))
        ret(rng.choice([0, 0, 1]))

    @h('scriptSetGameState__FPci')
    def _(mm):
        lg('setState', cstr(mm, mm.g32(4)), mm.g32(5))

    @h('textGetString__FPc')
    def _(mm):
        k = cstr(mm, mm.g32(4))
        lg('text', k)
        ret(0x31300000 + (sum(k) & 0xFF) * 16)

    @h('hudAddHelpMessage__FiPUsT1')
    def _(mm):
        lg('help', mm.g32(4), mm.g32(5), mm.g32(6))

    @h('multiHudAddHelpMessage__FiPUsT1i')
    def _(mm):
        lg('multiHelp', mm.g32(4), mm.g32(5), mm.g32(6), mm.g32(7))

    @h('playerRecomputeWeight__Fi')
    def _(mm):
        lg('weight', mm.g32(4))

    @h('playerGetPotionsLeft__Fii')
    def _(mm):
        lg('potions', mm.g32(4), mm.g32(5))
        ret(rng.randrange(4))

    @h('inventoryCheckWieldingStatus__FiPQ24item11description')
    def _(mm):
        lg('check', mm.g32(4), mm.read(mm.g32(5), 4))
        ret(rng.choice(p.get('wield_answers', [0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8])))

    @h('netPlayerIsLocal__Fi')
    def _(mm):
        lg('isLocal', mm.g32(4))
        ret(rng.choice([0, 1]))

    @h('lumpFindResource__FPcT0')
    def _(mm):
        lg('lump', cstr(mm, mm.g32(4)), cstr(mm, mm.g32(5)))
        ret(0x31F00000)

    @h('soundPlaySequence__FP15AnimationHeaderP10GameObjectf')
    def _(mm):
        lg('sound', mm.g32(4), mm.g32(5), mm.f[12])

    @h('scriptStart__FPc')
    def _(mm):
        lg('scriptStart', cstr(mm, mm.g32(4)))

    @h('playerGetFeatRank__Fix')
    def _(mm):
        lg('feat', mm.g32(4), mm.r[5] & M64)
        mm.setr(2, rng.choice([0, 0, 1, 2, -1 & M64]))

    @h('resetPlayerAnimations__Fi')
    def _(mm):
        lg('reset', mm.g32(4))

    @h('inventorySetAnim__FiP14AnimationStatei')
    def _(mm):
        lg('setAnim', mm.g32(4), mm.g32(5), mm.g32(6))

    @h('playerRecomputeAbilityModifiers__Fv')
    def _(mm):
        lg('recompute')

    @h('sprintf')
    def _(mm):
        text = cstr(mm, mm.g32(5))
        lg('sprintf', text)
        mm.write(mm.g32(4), text + b'\0')

    @h('evalHermite__Ffffff')
    def _(mm):
        lg('hermite', mm.f[12], mm.f[13], mm.f[14], mm.f[15], mm.f[16])
        mm.setf(0, mm.fv(12) * 0.5 + 0.125)

    @h('lerpShortRot__Fssfff')
    def _(mm):
        lg('lerp', s16(mm.g32(4)), s16(mm.g32(5)), mm.f[12], mm.f[13], mm.f[14])
        ret(s16(mm.g32(4) + mm.g32(5) + int(mm.fv(12) * 16)))

    @h('BuildMatrix__6CamerafffP6Point3')
    def _(mm):
        lg('build', mm.g32(4), mm.f[12], mm.f[13], mm.f[14], mm.g32(5))

    @h('findTagString__10GameObjectPci')
    def _(mm):
        lg('findTag', mm.g32(4), cstr(mm, mm.g32(5)), mm.g32(6))
        ret(p.get('tag_ptr', 0))

    @h('sscanf')
    def _(mm):
        text, fmt = cstr(mm, mm.g32(4)), cstr(mm, mm.g32(5))
        lg('sscanf', text, fmt)
        targets = [mm.g32(6), mm.g32(7), mm.g32(8), mm.g32(9)]
        vals = []
        for part in text.split(b','):
            try:
                vals.append(int(part))
            except ValueError:
                break
        for t, v in zip(targets, vals):
            mm.w32(t, v)
        ret(min(len(vals), 4))

    @h('icos__Fi')
    def _(mm):
        lg('icos', mm.g32(4))
        mm.setf(0, math.cos((mm.g32(4) & 0xFFFF) * 2 * math.pi / 65536))

    @h('isin__Fi')
    def _(mm):
        lg('isin', mm.g32(4))
        mm.setf(0, math.sin((mm.g32(4) & 0xFFFF) * 2 * math.pi / 65536))

    @h('distanceToPlane__FP6Point3N20')
    def _(mm):
        lg('plane', mm.read(mm.g32(4), 12), mm.read(mm.g32(5), 12), mm.read(mm.g32(6), 12))
        mm.setf(0, rng.choice([-2.0, -0.5, 0.0, 0.5, 3.0]))

    @h('_$_14AnimationState')
    def _(mm):
        lg('animDtor', ctx.name_of(mm.g32(4)), mm.g32(5))

    @h('__11ParticleDefiiiiiiiifffiiiPFPv_v')
    def _(mm):
        sp = mm.g32(29)
        lg('ParticleDef', ctx.name_of(mm.g32(4)), [mm.g32(r) for r in (5, 6, 7, 8, 9, 10, 11)], mm.u32(sp),
           mm.f[12], mm.f[13], mm.f[14], mm.u32(sp + 8), mm.u32(sp + 16), mm.u32(sp + 24), mm.u32(sp + 32))

    return H


NAMES = list(make_handlers(None, Script(0, {'item_ptr': 0}), Ctx(False, None, None)))


def build_authored(rel, work):
    base = os.path.basename(rel).replace('.cpp', '')
    obj = os.path.join(work, base + '.o')
    subprocess.check_call([P + 'g++', *FLAGS, '-c', os.path.join(SRC, rel), '-o', obj])
    und = [l.split()[-1] for l in subprocess.check_output([P + 'nm', '-u', obj], text=True).splitlines() if l.strip()]
    defsyms = {}
    for name in und:
        if name in NAMES:
            defsyms[name] = STUB + 0x10 * NAMES.index(name)
        else:
            defsyms[name] = orig_addr(name)
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


def run_one(image, entry, authored, elf, scenario, seed):
    mem = dict(scenario['mem'])
    stubs = {}
    m = Machine(image, mem, stubs, GP)
    ctx = Ctx(authored, elf, None)
    for name, blob in scenario.get('symmem', {}).items():
        m.write(ctx.addr(name), blob)
    script = Script(seed, scenario.get('params', {}))
    handlers = make_handlers(m, script, ctx)
    for name, fn in handlers.items():
        addr = STUB + 0x10 * NAMES.index(name) if authored else orig_addr(name)
        stubs[addr] = fn
    m.r[29] = STACK
    for i, v in enumerate(scenario['args']):
        m.r[4 + i] = v & M32
    for i, v in scenario.get('fargs', {}).items():
        m.f[i] = v
    m.log = []
    m.run(entry)
    region = {a: m.u8(a) for a in scenario['compare']}
    for name, size in scenario.get('csym', {}).items():
        base = ctx.addr(name)
        for i in range(size):
            region[(name, i)] = m.u8(base + i)
    ret = None
    if scenario.get('ret') == 'int':
        ret = m.g32(2)
    elif scenario.get('ret') == 'float':
        ret = m.f[0]
    return m.log, region, ret


def compare(name, runs=200):
    spec = FUNCTIONS[name]
    work = os.path.join(SCRATCH, name)
    os.makedirs(work, exist_ok=True)
    exe = Image(EXE)
    elf = build_authored(spec['src'], work)
    auth = Image(elf)
    auth_entry = symbol_address(elf, spec['symbol'])
    bad = 0
    skipped = 0
    global COVERAGE
    covered = set()
    for seed in range(runs):
        sc = spec['scenario'](random.Random(seed))
        try:
            COVERAGE = covered
            try:
                a = run_one(exe, spec['orig'], False, None, sc, seed)
            finally:
                COVERAGE = None
        except Stop as e:
            if str(e) == 'step limit':
                skipped += 1   # the image itself does not finish on this scenario (for example a cyclic equip chain)
                continue
            print('seed %d: stopped: %s' % (seed, e))
            return 2
        try:
            b = run_one(auth, auth_entry, True, elf, sc, seed)
        except Stop as e:
            print('seed %d: authored stopped: %s' % (seed, e))
            bad += 1
            continue
        if a != b:
            bad += 1
            if bad <= 3:
                print('MISMATCH seed %d' % seed)
                if a[0] != b[0]:
                    n = min(len(a[0]), len(b[0]))
                    k = next((i for i in range(n) if a[0][i] != b[0][i]), n)
                    print('  log differs at %d:\n   orig %s\n   auth %s' % (k, a[0][k:k + 2], b[0][k:k + 2]))
                diff = [x for x in a[1] if a[1][x] != b[1].get(x)][:10]
                if diff:
                    print('  memory differs at', [hex(x) if isinstance(x, int) else x for x in diff])
                if a[2] != b[2]:
                    print('  return', a[2], b[2])
    words = spec['size'] // 4
    lo = spec['orig']
    hit = sum(1 for a in covered if lo <= a < lo + spec['size'])
    print('%s: %d runs (%d skipped), %d mismatches, %d/%d original words executed' % (name, runs, skipped, bad, hit, words))
    return 1 if bad else 0


# ---- scenario helpers ----

def fill(rng, mem, base, size):
    for i in range(size):
        mem[base + i] = rng.randrange(256)


def put32(mem, a, v):
    v &= M32
    for i in range(4):
        mem[a + i] = (v >> (8 * i)) & 0xFF


def put16(mem, a, v):
    mem[a] = v & 0xFF
    mem[a + 1] = (v >> 8) & 0xFF


def putf(mem, a, x):
    put32(mem, a, f2b(x))


def putstr(mem, a, text):
    for i, c in enumerate(text.encode() + b'\0'):
        mem[a + i] = c


def frac(rng, lo, hi, step):
    return lo + step * rng.randrange(int((hi - lo) / step) + 1)

FUNCTIONS = {}


def reg(symbol, orig, src, scenario, **kw):
    FUNCTIONS[symbol] = dict(symbol=symbol, orig=orig, src=src, scenario=scenario, size=SIZEOF[symbol], **kw)


def G(name):
    return orig_addr(name)


def cells(*names):
    out = []
    for n in names:
        a = G(n)
        out += list(range(a, a + 4))
    return out


def rng_range(base, size):
    return list(range(base, base + size))


# ---- fontColor ----
FONT_CELLS = ('reglistLoopCount', 'nmChars', 'nmDirectQWords', 'lastFontColor', 'lastFontCharAlpha', 'fontCharAlpha')


def scenario_font_color(rng):
    mem = {}
    buf, loop = 0x30000000, 0x30001000
    fill(rng, mem, buf, 0x100)
    fill(rng, mem, loop, 0x10)
    p = G('p')
    put32(mem, p, buf + 16 * rng.randrange(4))
    for k in (4, 8, 12):
        put32(mem, p + k, rng.randrange(1 << 32))
    put32(mem, G('reglistLoopCount'), loop + 2 * rng.randrange(4))
    put32(mem, G('nmChars'), rng.randrange(0, 40))
    put32(mem, G('nmDirectQWords'), rng.randrange(0, 1000))
    for n in ('lastFontColor', 'lastFontCharAlpha', 'fontCharAlpha'):
        put32(mem, G(n), rng.randrange(1 << 32))
    args = [rng.randrange(1 << 32), rng.choice([0, 1, 77, -5, 255, 300]), rng.choice([0, 0, 1, -1, 7])]
    return {'mem': mem, 'args': args, 'compare': rng_range(buf, 0x100) + rng_range(loop, 0x10) + rng_range(p, 16) + cells(*FONT_CELLS)}


reg('fontColor__FUiii', 0x172DC0, 'w8-hud-text-items-a-font-color/font_color.cpp', scenario_font_color)


# ---- colorScale ----
def scenario_color_scale(rng):
    return {'mem': {}, 'args': [rng.randrange(1 << 32)], 'fargs': {12: f2b(frac(rng, -1.0, 3.0, 1 / 16))}, 'compare': [], 'ret': 'int'}


reg('colorScale__FUif', 0x191A20, 'w8-hud-text-items-a-color-scale/color_scale.cpp', scenario_color_scale)


# ---- DLG_SetMutex / DLG_Clear ----
def scenario_set_mutex(rng):
    mem = {}
    put32(mem, G('dialogMutex'), rng.choice([0, 0, 1, -1, 5, 0x80000000]))
    return {'mem': mem, 'args': [], 'compare': cells('dialogMutex'), 'ret': 'int'}


reg('DLG_SetMutex__Fv', 0x1D38F0, 'w8-hud-text-items-a-dlg-set-mutex/set_mutex.cpp', scenario_set_mutex)

DLG_CELLS = ('dialogCreature', 'dialogLumpName', 'dialogMutex', 'curDlg', 'activePlayer', 'optionsShowing', 'swooping',
             'DLG_SwoopMode', 'realDlgPos', 'dlgPos')


def scenario_dlg_clear(rng):
    mem = {}
    fill(rng, mem, G('anim'), 32)
    for k in (0, 16):
        put32(mem, G('anim') + k, rng.choice([0, 0x31000000 + 16 * rng.randrange(100)]))
    for n in DLG_CELLS:
        put32(mem, G(n), rng.randrange(1 << 32))
    return {'mem': mem, 'args': [], 'compare': rng_range(G('anim'), 32) + cells(*DLG_CELLS)}


reg('DLG_Clear__Fv', 0x1D3830, 'w8-hud-text-items-a-dlg-clear/dlg_clear.cpp', scenario_dlg_clear)


# ---- cacheClear ----
def scenario_cache_clear(rng):
    mem = {}
    tex = 0x30010000
    fill(rng, mem, tex, 16 * 0x40)
    fill(rng, mem, G('cache'), 192)
    for i in range(16):
        put32(mem, G('cache') + 12 * i, tex + 0x40 * rng.randrange(16))
    put32(mem, G('cacheHead'), rng.randrange(16))
    put32(mem, G('cacheTail'), rng.randrange(16))
    return {'mem': mem, 'args': [], 'compare': rng_range(tex, 16 * 0x40) + rng_range(G('cache'), 192) + cells('cacheHead', 'cacheTail')}


reg('cacheClear__Fv', 0x16CE88, 'w8-hud-text-items-a-cache-clear/cache_clear.cpp', scenario_cache_clear)


# ---- dropItem ----
def scenario_drop_item(rng):
    mem = {}
    point, name = 0x30020000, 0x30020100
    for k in range(3):
        putf(mem, point + 4 * k, frac(rng, -64.0, 64.0, 0.25))
    putstr(mem, name, rng.choice(['sword', 'a', 'potion_of_x']))
    table = G('norrath_player_view_31f540')
    for i in range(8):
        base = 0x30030000 + 0x40 * i
        putstr(mem, base, 'model%d' % rng.randrange(100))
        putstr(mem, base + 0x20, 'tex%d' % rng.randrange(100))
        put32(mem, table + 16 * i, base)
        put32(mem, table + 16 * i + 4, base + 0x20)
        put32(mem, table + 16 * i + 8, 0)
        put32(mem, table + 16 * i + 12, 0)
    return {'mem': mem, 'args': [point, name], 'compare': [], 'params': {'item_ptr': 0x30040000, 'proxy_max': 8}}


reg('dropItem__4itemG6Point3Pc', 0x100CA0, 'w8-hud-text-items-a-item-drop/item_drop.cpp', scenario_drop_item)


# ---- static initialization entries ----
def scenario_static(*syms):
    def make(rng):
        symmem = {n: bytes(rng.randrange(256) for _ in range(SIZES[n])) for n in syms}
        return {'mem': {}, 'symmem': symmem, 'args': [], 'compare': [], 'csym': {n: SIZES[n] for n in syms}}
    return make


reg('_GLOBAL_$I$displayQuestInfo__FP11questStructi', 0x1C1C88, 'w8-hud-text-items-a-quest-info-static-init/quest_info_static_init.cpp',
    scenario_static('norrath_quest_info_anim_state_view'))
reg('_GLOBAL_$D$displayQuestInfo__FP11questStructi', 0x1C1CA8, 'w8-hud-text-items-a-quest-info-static-init/quest_info_static_init.cpp',
    scenario_static('norrath_quest_info_anim_state_view'))
reg('_GLOBAL_$I$inventoryLoad__Fv', 0x1B1FE8, 'w8-hud-text-items-a-inventory-static-init/inventory_static_init.cpp',
    scenario_static('norrath_inventory_anim_state_view', 'torchParticle', 'heatParticle'))
reg('_GLOBAL_$D$inventoryLoad__Fv', 0x1B2008, 'w8-hud-text-items-a-inventory-static-init/inventory_static_init.cpp',
    scenario_static('norrath_inventory_anim_state_view', 'torchParticle', 'heatParticle'))
reg('_GLOBAL_$I$shopActiveFrame', 0x1BC5E0, 'w8-hud-text-items-a-shop-static-init/shop_static_init.cpp',
    scenario_static('norrath_shop_anim_state_view', 'candleParticle', 'shopList'))
reg('_GLOBAL_$D$shopActiveFrame', 0x1BC600, 'w8-hud-text-items-a-shop-static-init/shop_static_init.cpp',
    scenario_static('norrath_shop_anim_state_view', 'candleParticle', 'shopList'))


def scenario_font_static(rng):
    mem = {}
    p = G('p')
    for k in (0, 4, 8, 12):
        put32(mem, p + k, rng.randrange(1 << 32))
    put32(mem, G('glistp'), rng.randrange(1 << 32))
    return {'mem': mem, 'args': [], 'compare': rng_range(p, 16)}


reg('_GLOBAL_$I$_fontStringSize__FP4FontPci', 0x1745B8, 'w8-hud-text-items-a-font-static-init/font_static_init.cpp', scenario_font_static)


# ---- addItemToInventory ----
ITEM_NAMES = ['heart', 'shell', 'recallpotion', 'sword', 'shield', 'gem']


def make_items(rng, mem, base, names, n):
    """n item definitions at base (0x40 apart): word 0 = name pointer, byte 28 = type, 29 = hand, 30 = sub."""
    items = []
    for i in range(n):
        a = base + 0x40 * i
        fill(rng, mem, a, 0x40)
        nm = base + 0x1000 + 0x20 * i
        putstr(mem, nm, rng.choice(names))
        put32(mem, a, nm)
        items.append(a)
    return items


def scenario_add_item(rng):
    mem = {}
    player = G('player')
    index = rng.randrange(2)
    items = make_items(rng, mem, 0x30100000, ITEM_NAMES, 6)
    hand_of = {}
    for it in items:
        mem[it + 28] = rng.choice([0, 1, 2, 3, 5, 8])
        mem[it + 29] = rng.choice([0, 1, 2])
    kind = rng.randrange(3)
    desc = 0x30102000
    fill(rng, mem, desc, 72)
    it0 = rng.choice(items)
    mem[it0 + 29] = kind
    put32(mem, desc, it0)
    put16(mem, desc + 4, rng.randrange(1, 50))
    flags = rng.randrange(1 << 32) | (rng.randrange(1 << 32) << 32)
    flags = (flags & ~0x100) | (0x100 if rng.random() < 0.6 else 0)
    put32(mem, desc + 8, flags)
    put32(mem, desc + 12, flags >> 32)
    mem[desc + 32] = rng.choice([0xFF, 0, 1])
    mem[desc + 33] = rng.choice([0xFF, 0, 1])
    base = player + index * 23984 + 328 + kind * 4616
    count = rng.choice([0, 1, 2, 5, 17, 63, 64, rng.randrange(65)])
    fill(rng, mem, base, 4616)
    for i in range(64):
        slot = base + 72 * i
        if i < count or rng.random() < 0.05:
            if rng.random() < 0.4:
                for k in range(72):
                    mem[slot + k] = mem[desc + k]
                if rng.random() < 0.5:
                    mem[slot + 32] = rng.choice([0xFF, 0, 1])
            else:
                put32(mem, slot, rng.choice(items + [items[0]]))
        else:
            put32(mem, slot, 0)
    put32(mem, base + 4608, count)
    put32(mem, G('gameDifficulty'), rng.choice([0, 1, 1, 2]))
    return {'mem': mem, 'args': [index, desc, rng.choice([0, 1, 5])],
            'compare': rng_range(base, 4616) + rng_range(desc, 72) + cells('gameDifficulty'), 'ret': 'int',
            'params': {}}


reg('addItemToInventory__FiPQ24item11descriptioni', 0x1ADF38, 'w8-hud-text-items-a-add-item-to-inventory/add_item.cpp', scenario_add_item)


# ---- equipItem ----
def scenario_equip_item(rng):
    mem = {}
    player = G('player')
    index = rng.randrange(2)
    P = player + index * 23984
    items = []
    for i in range(8):
        a = 0x30210000 + 0x40 * i
        fill(rng, mem, a, 0x40)
        mem[a + 28] = rng.choice([0, 0, 1, 2, 3, 3, 5, 8, 4, 15 if i == 7 and rng.random() < 0.2 else 1])
        mem[a + 29] = rng.choice([0, 0, 1, 2])
        mem[a + 30] = rng.choice([0, 1, 2, 3, 4, 4, 5, 6, 6, 7])
        items.append(a)
    descs = []
    for i in range(8):
        d = 0x30200000 + 0x80 * i
        fill(rng, mem, d, 0x80)
        put32(mem, d, items[i] if rng.random() > 0.05 else 0)
        flag_bits = [0x800000, 0x200000, 0x400000, 0x40000, 0x100000, 0x80000, 0x1000000, 0x20000000, 0x4000000, 0x2000000]
        flags = 0
        for b in flag_bits:
            if rng.random() < 0.3:
                flags |= b
        flags |= rng.randrange(1 << 8) << 40
        put32(mem, d + 8, flags & M32)
        put32(mem, d + 12, flags >> 32)
        descs.append(d)
    D = descs[rng.randrange(8)] if rng.random() > 0.05 else descs[0]
    region = P + 21880
    for off in range(0, 96, 4):
        put32(mem, region + off, rng.choice([0, 0, D, rng.choice(descs)]))
    for off in (21960, 21964, 21968):
        put32(mem, P + off, rng.choice([0, 0, D, rng.choice(descs)]))
    mem[P + 23572] = rng.choice([0, 1, 0xFF])
    mem[P + 23582] = rng.randrange(8)
    inv = G('inventory')
    put32(mem, inv + 56, rng.choice([0, 1, 5]))
    put32(mem, inv + 172, rng.choice([0, 1, 5]))
    wield = rng.choice([0, 0, 0, 0, 0, 0, 1, 3, 8])
    return {'mem': mem, 'args': [index, D, rng.choice([0, 0, 1, 5])],
            'compare': rng_range(P + 21880, 128) + rng_range(P + 23560, 40) + rng_range(inv + 48, 140),
            'params': {'wield_answers': [wield]}}


reg('equipItem__FiPQ24item11descriptioni', 0x1AEC78, 'w8-hud-text-items-a-equip-item/equip_item.cpp', scenario_equip_item)


# ---- AdvanceDialogSwoop ----
def scenario_advance(rng):
    mem = {}
    cam = 0x30300000
    fill(rng, mem, cam, 256)
    for off in (0, 4, 8, 12, 16, 20):
        putf(mem, cam + off, frac(rng, -64.0, 64.0, 0.25))
    putf(mem, cam + 168, rng.choice([-0.5, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0, 1.25]))
    putf(mem, cam + 172, rng.choice([0.0, 0.25, 1.0, 4.0]))
    out = 0x30301000
    fill(rng, mem, out, 4)
    count = rng.choice([-45, -8, -2, -1, 1, 2, 4, 8, 45, 3, -3])
    return {'mem': mem, 'args': [cam, count, rng.choice([0, out])], 'compare': rng_range(cam, 256) + rng_range(out, 4), 'ret': 'int'}


reg('AdvanceDialogSwoop__6CameraiPf', 0x26CDE8, 'w8-hud-text-items-a-advance-dialog-swoop/advance_swoop.cpp', scenario_advance)


# ---- StartDialogSwoop ----
def scenario_start(rng):
    mem = {}
    obj, cam = 0x30400000, 0x30401000
    fill(rng, mem, obj, 0x40)
    fill(rng, mem, cam, 256)
    put16(mem, obj + 56, rng.randrange(65536))
    for k in range(3):
        putf(mem, obj + 16 + 4 * k, frac(rng, -128.0, 128.0, 0.25))
    wc = G('worldCameraPos')
    for k in range(3):
        putf(mem, wc + 4 * k, frac(rng, -300.0, 300.0, 0.25))
    tag = 0
    if rng.random() < 0.6:
        tag = 0x30402000
        vals = [rng.randrange(-60, 60) for _ in range(4)]
        text = ','.join(str(v) for v in vals)
        if rng.random() < 0.1:
            text = str(vals[0]) + ',x'
        putstr(mem, tag, text)
    count = rng.randrange(4)
    put32(mem, G('nmPlayersAlive'), count)
    player = G('player')
    for i in range(4):
        put32(mem, G('playersAlive') + 4 * i, rng.randrange(4))
    creatures = []
    for k in range(4):
        c = 0x30410000 + 0x40 * k
        fill(rng, mem, c, 0x40)
        for j in range(3):
            near = frac(rng, -90.0, 90.0, 0.25) if rng.random() < 0.8 else frac(rng, -300.0, 300.0, 0.25)
            putf(mem, c + 16 + 4 * j, rng.choice([-1, 1]) * 0 + (mem_f(mem, obj + 16 + 4 * j) + near))
        creatures.append(c)
    for idx in range(4):
        put32(mem, player + idx * 23984 + 21872, rng.choice(creatures))
    mode = rng.choice([0, 1, 1, 1, 2])
    return {'mem': mem, 'args': [cam, obj, mode], 'compare': rng_range(cam, 256),
            'params': {'tag_ptr': tag}}


def mem_f(mem, a):
    return b2f(sum(mem[a + i] << (8 * i) for i in range(4)))


reg('StartDialogSwoop__6CameraP10GameObjecti', 0x26C770, 'w8-hud-text-items-a-start-dialog-swoop/start_swoop.cpp', scenario_start)


if __name__ == '__main__':
    runs = int(sys.argv[2]) if len(sys.argv) > 2 else 200
    names = list(FUNCTIONS) if sys.argv[1] == 'all' else [sys.argv[1]]
    worst = 0
    for n in names:
        worst = max(worst, compare(n, runs))
    sys.exit(worst)
