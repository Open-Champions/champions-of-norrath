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
T={'play':('Z13VIHSpritePlay','pool-hspriteplay-allocate',104),'att':('Z19VIHSpriteAttachment','pool-hspriteattachment-allocate',16),
   'sprite':('ZP8VISprite','pool-sprite-allocate',12)}
a=w.rstrip('nDL')
suffix=w[len(a):]
t,d,E=T[a]
null_when=3*E if 'n' in suffix else None
dret=M64 if 'D' in suffix else 0
lret=M64 if 'L' in suffix else 0
sc=psc()
if 'n' in suffix: sc=[s for s in sc if s[0].startswith(('n1','n3'))]
go(P+d,'Allocate__t6VIPool1'+t,sc,hk(t[0:] ,null_when,dret,lret),compare_ret=True)
