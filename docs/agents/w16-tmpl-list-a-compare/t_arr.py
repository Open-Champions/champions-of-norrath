from hk import *
P='w16-tmpl-list-a-'
w=sys.argv[1]
def sc(counts, nulls=True):
    out=[]
    for k,cnt in enumerate(counts):
        def st(env,cnt=cnt,k=k):
            randmem(env,random.Random(k),A,0x40); fill_arena(env,k)
            env.mem.w32(A,cnt)
        out.append(('c%d'%cnt,st,(A,)))
    return out
CN=[0,1,2,3,5,0x7fffffff-0x7fffffff+7]
def emit_ctor():
    def f(c,e):
        a=c.r[4]&M32
        e.log.append(('ector',norm(a)))
        e.mem.write(a,bytes((a+i)&0xff for i in range(48)))
    return f
U={
 'trig':(P+'array-hspritetrigger-allocate','Allocate__t7VIArray1Z16VIHSpriteTrigger',{'__builtin_vec_new':vnew()},CN),
 'trign':(P+'array-hspritetrigger-allocate','Allocate__t7VIArray1Z16VIHSpriteTrigger',{'__builtin_vec_new':vnew(null_when=48)},[3,2]),
 'emit':(P+'array-cspritepartemitter-allocate','Allocate__t7VIArray1Z20VICSpritePartEmitter',{'__builtin_vec_new':vnew(),'__17VIParticleEmitter':emit_ctor()},[0,1,2,3]),
 'emitn':(P+'array-cspritepartemitter-allocate','Allocate__t7VIArray1Z20VICSpritePartEmitter',{'__builtin_vec_new':vnew(null_when=2112),'__17VIParticleEmitter':emit_ctor()},[2,1]),
}
u,sym,H,cn=U[w]
go(u,sym,sc(cn),H,compare_ret=True)
