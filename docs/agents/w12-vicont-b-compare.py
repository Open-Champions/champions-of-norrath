from common import *
import sys, itertools
OLD = OBJ + 0x400; NEW = 0x1a20000
def base(env, words, nbytes=0x100, seed=1):
    r = random.Random(seed)
    env.mem.write(OBJ, bytes(r.randrange(256) for _ in range(nbytes)))
    env.mem.write(OLD, bytes(r.randrange(256) for _ in range(0x200)))
    for off, v in words.items(): env.mem.w32(OBJ + off, v & 0xffffffff)
def SC(label, words, args=(OBJ,), pre=None, **par):
    def st(env):
        base(env, words); env.P = par; env.run_init = reset_run
        if pre: pre(env)
    return (label, st, args)
def P_(env, k, d=0): return getattr(env, 'P', {}).get(k, d)
tot = 0
from collections import Counter
import r5900
VIS = Counter(); RD = []
_step = r5900.CPU.step
def _cstep(self):
    if self.pc not in self.hooks: VIS[self.pc] += 1
    _step(self)
r5900.CPU.step = _cstep
_u32 = r5900.Mem.u32
def _mu32(self, a):
    if 0x1a00000 <= a < 0x1a40000: RD.append(a)
    return _u32(self, a)
r5900.Mem.u32 = _mu32
def loop_post(env, mine): return (max(VIS.values()) if VIS else 0, tuple(RD))
def reset_run(env): VIS.clear(); RD.clear()
def pr(d, f, sym, scs, hooks, **kw):
    global tot
    path = P + d + '/' + f
    import os
    mu = os.environ.get('MUT')
    if mu and mu.split('::')[0] == d:
        _, old, new = mu.split('::')
        txt = open(path).read(); assert old in txt, 'mutation text absent'
        os.makedirs('/tmp/mutb', exist_ok=True); path = '/tmp/mutb/' + f
        open(path, 'w').write(txt.replace(old, new, 1))
    m = paired(path, sym, scs, hooks=hooks, **kw); tot += m; print(d, len(scs), 'scenarios, mismatches', m)
    return m
def h_vdel(c, env): env.log.append(('vdel', norm(c.r[4] & M32)))
def h_del(c, env): env.log.append(('del', norm(c.r[4] & M32)))
def h_destroy(name):
    def h(c, env):
        env.log.append((name, norm(c.r[4] & M32)))
        if P_(env, 'clr'): env.mem.w32(OBJ + P_(env, 'off'), 0)
        return 0
    return h
S = sys.argv[1] if len(sys.argv) > 1 else 'all'

if S in ('all', 'dcall'):
    def mk(tok, chain, begin):
        T = 'Ct6VIPool1' + tok
        def hb(c, env): env.log.append(('B', norm(c.r[4] & M32))); return begin & M64
        def he(c, env): env.log.append(('E', norm(c.r[4] & M32))); return 99
        def hn(c, env):
            a = c.r[5] & M64; env.log.append(('N', norm(c.r[4] & M32), a)); return chain.get(a, 99)
        return {'Begin__' + T: hb, 'End__' + T: he, 'Next__%si' % T: hn}
    for tok, slug in (('ZP7VIRFont', 'pool-rfont'), ('ZP8VIRefMap', 'pool-refmap'), ('ZP9VISurface', 'pool-surface'), ('ZP16VIStaticLighting', 'pool-staticlighting')):
        for chain, begin in (({}, 99), ({1: 3, 3: 7, 7: 5}, 1), ({1: 2}, 1), ({5: 6, 6: 99}, 5)):
            pr('destroy-' + slug, 'destroy.cpp', 'DestroyPool__t6VIPool1' + tok, [SC('x', {})], mk(tok, chain, begin))

if S in ('all', 'dinline'):
    for tok, slug in (('ZP13VIMaterialPal', 'pool-materialpal'), ('ZP8VISprite', 'pool-sprite')):
        scs = []
        chains = {'none': (-1, {}), 'one': (0, {0: -1}), 'three': (0, {0: 2, 2: 5, 5: -1}), 'mid': (3, {3: 1, 1: 4, 4: -1}), 'back': (6, {6: 2, 2: 0, 0: -1}), 'neg': (4, {4: -1})}
        for name, (first, nxt) in chains.items():
            def pre(env, nxt=nxt):
                for i in range(8): env.mem.w32(OLD + 12 * i + 4, nxt.get(i, 0xffffffff))
                for i in range(8): env.mem.w32(OLD + 12 * i, 0x100 + i); env.mem.w32(OLD + 12 * i + 8, 0x200 + i)
            scs.append(SC(name, {16: first, 8: OLD, 0: 3, 4: 8, 12: 7, 20: 6}, pre=pre))
        pr('destroy-' + slug, 'destroy.cpp', 'DestroyPool__t6VIPool1' + tok, scs, {}, post=loop_post)

if S in ('all', 'darray'):
    for cont, tok, slug in (('7VIArray1', 'Z11VIClipEntry', 'array-clipentry'), ('7VIArray1', 'Z7VIPlane', 'array-plane'), ('7VIArray1', 'Z7VIVect3', 'array-vect3'), ('7VIArray1', 'Zi', 'array-int'), ('8VIVector1', 'Z18VISceneDisplayElem', 'vector-scenedisplayelem')):
        scs = [SC('n%d' % n, {0: n, 4: 0x1234, 8: 0x5678}) for n in (0, 1, 2, 3, 7, 100)]
        pr('destroy-' + slug, 'destroy.cpp', 'DestroyPool__t%s%s' % (cont, tok), scs, {}, post=loop_post)

if S in ('all', 'end'):
    pr('end-pool-collbuffer', 'end.cpp', 'End__Ct6VIPool1ZP12VICollBuffer', [SC('a', {}, args=(OBJ,)), SC('b', {}, args=(0,))], {})

if S in ('all', 'init'):
    def hc(c, env): env.log.append(('clear', norm(c.r[4] & M32))); return P_(env, 'c') & M64
    def ha(c, env): env.log.append(('alloc', norm(c.r[4] & M32), env.mem.u32(c.r[4] & M32))); return P_(env, 'a') & M64
    for tok, slug in (('Z11VIClipEntry', 'array-clipentry'), ('Z13VIWorldRegion', 'array-worldregion'), ('Z6VIZone', 'array-zone'), ('Z7VIPlane', 'array-plane'), ('Z7VIVect3', 'array-vect3')):
        scs = []
        for c, a, n in itertools.product((-5, -1, 0, 3), (-2, -1, 0, 4), (0, 17, -3)):
            scs.append(SC('c%d a%d n%d' % (c, a, n), {0: 0x1111, 4: 0x2222}, args=(OBJ, n & M64), c=c, a=a))
        pr('init-' + slug, 'init.cpp', 'Init__t7VIArray1%si' % tok, scs, {'Clear__t7VIArray1' + tok: hc, 'Allocate__t7VIArray1' + tok: ha})

if S in ('all', 'dtor'):
    items = [('6VIPool1', 'ZP13VISpellEffect', 'pool-spelleffect', 8), ('6VIPool1', 'ZP20VIParticleDefinition', 'pool-particledefinition', 8),
             ('7VIArray1', 'Z13VIWorldRegion', 'array-worldregion', 4), ('7VIArray1', 'Z17VIRadialFloraInst', 'array-radialflorainst', 4), ('7VIArray1', 'Z6VIZone', 'array-zone', 4)]
    for kind, tok, slug, off in items:
        D = 'DestroyPool__t%s%s' % (kind, tok)
        scs = []
        for stor, clr, fl in itertools.product((0, OLD), (0, 1), (0, 1, 2, 3, -1, -2)):
            if stor == 0 and clr: continue
            scs.append(SC('s%x c%d f%d' % (stor, clr, fl), {off: stor, 0: 5, 12: 9}, args=(OBJ, fl & M64), clr=clr, off=off))
        pr('dtor-' + slug, 'dtor.cpp', '_$_t%s%s' % (kind, tok), scs, {D: h_destroy('destroy'), '__builtin_vec_delete': h_vdel, '__builtin_delete': h_del}, compare_ret=False)

if S in ('all', 'ctor'):
    for tok, slug in (('ZP13VISpellEffect', 'pool-spelleffect'), ('ZP20VIParticleDefinition', 'pool-particledefinition')):
        pr('ctor-' + slug, 'ctor.cpp', '__t6VIPool1' + tok, [SC('r%d' % i, {}) for i in range(3)], {}, compare_ret=False)
    for tok, slug in (('Z13VIWorldRegion', 'array-worldregion'), ('Z6VIZone', 'array-zone')):
        pr('ctor-' + slug, 'ctor.cpp', '__t7VIArray1' + tok, [SC('r%d' % i, {}) for i in range(3)], {}, compare_ret=False)
print('TOTAL mismatches', tot)
