from hk import *
import os
w=sys.argv[1]
M64=0xffffffffffffffff
OLD=ARENA+0x40000
def mk(count,cap,blk):
    def st(env):
        r=random.Random(count*31+cap*7+(blk!=0))
        randmem(env,r,A,0x40); fill_arena(env,count+cap,0x42000)
        env.mem.w32(A,count); env.mem.w32(A+4,cap); env.mem.w32(A+8,blk)
    return st
def psc(counts=(0,1,2,3),argsf=lambda: (A,)):
    out=[]
    for count in counts:
        for cap in (0,count,count+2):
            for blk in (0,OLD):
                out.append(('n%d_c%d_b%x'%(count,cap,blk),mk(count,cap,blk),(A,)))
    return out
def vdel():
    def f(c,e): e.log.append(('vdel',norm(c.r[4]&M32)))
    return f
def poolH(t,E,null=False,dret=0,lret=0):
    return {'__builtin_vec_new':vnew(null_when=3*E if null else None),'__builtin_vec_delete':vdel(),
      'DestroyPool__t6VIPool1'+t:lg('dpool',1,ret=dret,hs=True),'LinkFreeChain__t6VIPool1'+t+'i':lg('link',2,ret=lret)}
def run(unit,sym,sc,H): return go(unit,sym,sc,H,compare_ret=True)
P12='w12-vicont-a-'
def chain12(count,cap,blk,seed):
    def st(env):
        mk(count,cap,blk)(env)
        r=random.Random(seed)
        if blk:
            n=max(count,1)
            order=list(range(n)); r.shuffle(order)
            for i in range(n): env.mem.w32(blk+12*i+4,0xffffffff)
            k=r.randint(0,n); seq=order[:k]
            for a_,b_ in zip(seq,seq[1:]): env.mem.w32(blk+12*a_+4,b_)
            env.mem.w32(A+16,seq[0] if seq else 0xffffffff)
    return st
def csc():
    out=[]
    for count in (0,1,2,3,4):
        for cap in (0,count,count+2):
            for blk in (0,OLD):
                for seed in range(3 if blk else 1):
                    out.append(('n%d_c%d_b%x_s%d'%(count,cap,blk,seed),chain12(count,cap,blk,seed),(A,)))
    return out
def walkH(t):
    def begin(c,e):
        e.log.append(('begin',norm(c.r[4]&M32))); v=e.mem.u32(A+16); return v if v!=0xffffffff else 0x7fffffff
    def end(c,e):
        e.log.append(('end',norm(c.r[4]&M32))); return 0x7fffffff
    def nxt(c,e):
        i=c.r[5]&M32; e.log.append(('next',norm(c.r[4]&M32),i))
        v=e.mem.u32(e.mem.u32(A+8)+12*i+4)
        return 0x7fffffff if v==0xffffffff else v
    return {'Begin__Ct6VIPool1'+t:begin,'End__Ct6VIPool1'+t:end,'Next__Ct6VIPool1'+t+'i':nxt}
if w.startswith('walk'):
    _,t,E,unit=w.split(':'); E=int(E)
    for nul,d,l in ((False,0,0),(True,0,0),(False,0,M64)):
        sc=csc()
        if nul: sc=[s for s in sc if s[0].startswith(('n1','n3'))]
        H=poolH(t,E,nul,d,l); H.update(walkH(t))
        run(unit,'Allocate__t6VIPool1'+t,sc,H)
elif w.startswith('palloc'):
    # palloc:<type>:<E>
    _,t,E,unit=w.split(':'); E=int(E)
    for nul,d,l in ((False,0,0),(True,0,0),(False,M64,0),(False,0,M64)):
        sc=psc()
        if nul: sc=[s for s in sc if s[0].startswith(('n1','n3'))]
        run(unit,'Allocate__t6VIPool1'+t,sc,poolH(t,E,nul,d,l))
elif w=='arrint':
    sc=[]
    for k,cnt in enumerate([0,1,2,3,5]):
        def st(env,cnt=cnt,k=k):
            randmem(env,random.Random(k),A,0x40); fill_arena(env,k); env.mem.w32(A,cnt)
        sc.append(('c%d'%cnt,st,(A,)))
    run(P12+'allocate-array-int','Allocate__t7VIArray1Zi',sc,{'__builtin_vec_new':vnew()})
    run(P12+'allocate-array-int','Allocate__t7VIArray1Zi',sc[3:5],{'__builtin_vec_new':vnew(null_when=12)})
elif w=='sde':
    t='Z18VISceneDisplayElem'
    for nul,d in ((False,0),(True,0),(False,M64)):
        sc=psc()
        sc=[ (n,s,a) for (n,s,a) in sc]
        H={'__builtin_vec_new':vnew(null_when=36 if nul else None),'__builtin_vec_delete':vdel(),'DestroyPool__t8VIVector1Z18VISceneDisplayElem':lg('dpool',1,ret=d,hs=True)}
        # vector layout: +0 count, +4 cap, +8 block
        if nul: sc=[s for s in sc if s[0].startswith(('n1','n3'))]
        run(P12+'allocate-vector-scenedisplayelem','Allocate__t8VIVector1Z18VISceneDisplayElem',sc,H)
elif w=='cbegin':
    def sc():
        out=[]
        for i,v in enumerate([0,1,5,0x7fffffff,0xffffffff,0xfffffffe,0x80000000]):
            def st(env,v=v,i=i):
                randmem(env,random.Random(i),A,0x40); env.mem.w32(A+16,v)
            out.append(('v%x'%v,st,(A,)))
        return out
    run(P12+'begin-pool-collbuffer','Begin__Ct6VIPool1ZP12VICollBuffer',sc(),{'End__Ct6VIPool1ZP12VICollBuffer':lg('end',1,ret=lambda c,e:0x7fff0000+(e.mem.u32(c.r[4]&M32)&0xff))})
elif w=='lclear':
    NODE=ARENA
    def mkc(n,seed):
        def st(env):
            r=random.Random(seed); randmem(env,r,A,0x40); fill_arena(env,seed,0x4000)
            nodes=[ARENA+0x600*i for i in range(n)]
            for i,nd in enumerate(nodes): env.mem.w32(nd+480,nodes[i+1] if i+1<n else 0)
            env.mem.w32(A,n); env.mem.w32(A+4,nodes[0] if n else 0); env.mem.w32(A+8,nodes[-1] if n else 0)
        return st
    sc=[('n%d'%n,mkc(n,n),(A,)) for n in (0,1,2,4)]
    H={'_$_10VIMatrix33':lg('mdtor',2),'__builtin_delete':lg('del',1)}
    run('w12-vicont-a-clear-list-material','Clear__t6VIList1Z10VIMaterial',sc,H)
