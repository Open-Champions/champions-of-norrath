from hk import *
P='w16-tmpl-list-a-'
w=sys.argv[1]
M64=0xffffffffffffffff
CFG={'mat':('material','Z10VIMaterial',496,480,'__10VIMaterial'),'act':('actor','Z7VIActor',176,160,'__7VIActor'),'prim':('primbuffer','ZP12VIPrimBufferRCP12VIPrimBuffer',12,4,None)}
a=w.rstrip('N'); d,t,N,nx,ctor=CFG[a]
TAILB=ARENA+0x1000
def mk(count,head,tail,seed):
    def st(env):
        r=random.Random(seed); randmem(env,r,A,0x40); fill_arena(env,seed,0x3000)
        env.mem.w32(A,count); env.mem.w32(A+4,head); env.mem.w32(A+8,tail); env.mem.w32(B,0x1234abcd+seed)
    return st
sc=[]; k=0
for count in (0,1,5):
    for (head,tail) in ((0,0),(TAILB,TAILB),(ARENA+0x800,TAILB),(0,TAILB)):
        k+=1; sc.append(('c%d_h%x_t%x'%(count,head,tail),mk(count,head,tail,k),(A,B) if a=='prim' else (A,)))
def newh(null):
    def f(c,e):
        sz=c.r[4]&M32; e.log.append(('new',sz))
        return 0 if null else ARENA+0x2000
    return f
H={'__builtin_new':newh(w.endswith('N'))}
if ctor: H[ctor]=lg('ctor',1,ret=0,hs=True)
if a=='prim': go(P+'list-primbuffer-add','Add__t6VIList1'+t,sc,H,compare_ret=True)
else: go(P+'list-%s-add'%d,'Add__t6VIList1'+t,sc,H,compare_ret=True)
