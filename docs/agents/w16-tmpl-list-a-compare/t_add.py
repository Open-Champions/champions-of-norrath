from hk import *
P='w16-tmpl-list-a-'
w=sys.argv[1]
D={'anim':('hspriteanim','P13VIHSpriteAnim'),'mpal':('materialpal','P13VIMaterialPal'),'spell':('spelleffect','P13VISpellEffect'),
   'part':('particledefinition','P20VIParticleDefinition'),'font':('rfont','P7VIRFont'),'surf':('surface','P9VISurface'),'sprite':('sprite-existing','P8VISprite')}
a=w.rstrip('N')
d,t=D[a]
BLK=ARENA
def mk(free,last,first,cap,seed):
    def st(env):
        r=random.Random(seed); randmem(env,r,A,0x40); fill_arena(env,seed,0x400)
        env.mem.w32(A,r.randint(0,5)); env.mem.w32(A+4,cap); env.mem.w32(A+8,BLK)
        env.mem.w32(A+12,free&M32); env.mem.w32(A+16,first&M32); env.mem.w32(A+20,last&M32)
        env.mem.w32(B,0xabc00000+seed)
    return st
def allochook(retv, headv):
    def f(c,e):
        e.log.append(('alloc',norm(c.r[4]&M32),e.mem.u32(A+4),e.mem.u32(A+12)))
        if headv is not None and retv>=0:
            e.mem.w32(A+12,headv)
            # make entry headv chain end
            e.mem.w32(BLK+12*headv+4,0xffffffff if headv<3 else 1)
        return retv&M64
    return f
M64=0xffffffffffffffff
sc=[]
k=0
for free in (0,1,2,5):
    for last in (-1,0,1,3):
        for first in (-1,2):
            k+=1
            sc.append(('f%d_l%d_s%d'%(free,last,first),mk(free,last,first,8,k),(A,B)))
for last in (-1,2):
    for cap in (0,1,3):
        for ret,hd in ((0,1),(0,2),(-1,None)):
            k+=1
            sc.append(('N_l%d_c%d_r%d'%(last,cap,ret),mk(-1,last,-1 if last<0 else 0,cap,k),(A,B)))
H={'Allocate__t6VIPool1Z'+t:None}
# per-scenario alloc hook: pick behaviour from the capacity word
def ah(c,e):
    cap=e.mem.u32(A+4)
    e.log.append(('alloc',norm(c.r[4]&M32),cap,e.mem.u32(A+12)))
    if cap==0: return 0   # unreachable (cap doubled to >=1)
    if cap in (1,):       # failure case
        return M64
    e.mem.w32(A+12,cap%3)   # new head
    e.mem.w32(BLK+12*(cap%3)+4,0xfffffffe if cap==6 else 1)
    return 0
H={'Allocate__t6VIPool1Z'+t:ah}
if a=='sprite':
    import os
    SRC_=SRC; 
    go('w12-vicont-a-add-pool-sprite','Add__t6VIPool1ZP8VISpriteRCP8VISprite',sc,H,compare_ret=True)
else:
    go(P+'pool-%s-add'%d,'Add__t6VIPool1Z%sRC%s'%(t,t),sc,H,compare_ret=True)
