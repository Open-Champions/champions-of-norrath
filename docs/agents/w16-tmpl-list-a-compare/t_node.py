from hk import *
P='w16-tmpl-list-a-'
w=sys.argv[1]
OLD=ARENA+0x40000
M64=0xffffffffffffffff
def mk(count,cap,blk):
    def st(env):
        r=random.Random(count*31+cap*7+(blk!=0))
        randmem(env,r,A,0x40); fill_arena(env,count+cap,0x42000)
        env.mem.w32(A,count); env.mem.w32(A+4,cap); env.mem.w32(A+8,blk)
    return st
def psc(counts=(0,1,2,3)):
    out=[]
    for count in counts:
        for cap in (0,count,count+2):
            for blk in (0,OLD):
                out.append(('n%d_c%d_b%x'%(count,cap,blk),mk(count,cap,blk),(A,)))
    return out
def vdel():
    def f(c,e): e.log.append(('vdel',norm(c.r[4]&M32)))
    return f
def hk(t, null_when=None, dret=0, lret=0, extra=None):
    ln='LinkFreeChain__t6VIPool1%si'%t; dn='DestroyPool__t6VIPool1%s'%t
    H={'__builtin_vec_new':vnew(null_when=null_when),'__builtin_vec_delete':vdel(),
       dn:lg('dpool',1,ret=dret,hs=True), ln:lg('link',2,ret=lret)}
    if extra: H.update(extra)
    return H

w=sys.argv[1]
def chain(count,cap,blk,seed):
    def st(env):
        mk(count,cap,blk)(env)
        r=random.Random(seed)
        if blk:
            order=list(range(max(count,1))); r.shuffle(order)
            for i in range(max(count,1)): env.mem.w32(blk+52*i+44,0xffffffff)
            k=r.randint(0,len(order))
            seq=order[:k]
            for a,b in zip(seq,seq[1:]): env.mem.w32(blk+52*a+44,b)
            env.mem.w32(A+16,seq[0] if seq else 0xffffffff)
    return st
def nsc(counts=(0,1,2,3,4)):
    out=[]
    for count in counts:
        for cap in (0,count,count+2):
            for blk in (0,OLD):
                for seed in range(3 if blk else 1):
                    out.append(('n%d_c%d_b%x_s%d'%(count,cap,blk,seed),chain(count,cap,blk,seed),(A,)))
    return out
def beginh(c,e):
    e.log.append(('begin',norm(c.r[4]&M32))); v=e.mem.u32(A+16); return v if v!=0xffffffff else 0x7fffffff
def endh(c,e):
    e.log.append(('end',norm(c.r[4]&M32))); return 0x7fffffff
t='Z17VIHSpritePlayNode'
suffix=w[len('node'):]
null_when=3*52 if 'n' in suffix else None
dret=0
lret=M64 if 'L' in suffix else 0
H={'__builtin_vec_new':vnew(null_when=null_when),'__builtin_vec_delete':vdel(),
 'Begin__Ct6VIPool1Z17VIHSpritePlayNode':beginh,'End__Ct6VIPool1Z17VIHSpritePlayNode':endh,
 'LinkFreeChain__t6VIPool1Z17VIHSpritePlayNodei':lg('link',2,ret=lret)}
sc=nsc()
if 'n' in suffix: sc=[s for s in sc if s[0].startswith(('n1','n3'))]
go(P+'pool-hspriteplaynode-allocate','Allocate__t6VIPool1'+t,sc,H,compare_ret=True)
