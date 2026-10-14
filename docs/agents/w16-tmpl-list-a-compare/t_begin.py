from tutil import *
P='w16-tmpl-list-a-'
def sc(vals):
    out=[]
    for i,v in enumerate(vals):
        def st(env,v=v,i=i):
            randmem(env,random.Random(i),A,0x40); env.mem.w32(A+16,v)
        out.append(('v%x'%v,st,(A,)))
    return out
V=[0,1,5,0x7fffffff,0xffffffff,0xfffffffe,0x80000000,0x1234]
def endh(name):
    return {name: lg('end',1,ret=lambda c,e:0x7fff0000+(e.mem.u32(c.r[4]&M32)&0xff))}
U={
 'pb':(P+'pool-primbuffer-begin','Begin__Ct6VIPool1ZP12VIPrimBuffer','End__Ct6VIPool1ZP12VIPrimBuffer'),
 'cb':(P+'pool-collbuffer-begin','Begin__Ct6VIPool1ZP12VICollBuffer','End__Ct6VIPool1ZP12VICollBuffer'),
 'clb':(P+'pool-colorbuffer-begin','Begin__Ct6VIPool1ZP13VIColorBuffer','End__Ct6VIPool1ZP13VIColorBuffer'),
 'ff':(P+'list-flatfile-begin','Begin__Ct6VIList1ZP10VIFlatFile','End__Ct6VIList1ZP10VIFlatFile'),
}
w=sys.argv[1]
if w=='pn':
    def st(env,v=0): pass
    s=[]
    for i,v in enumerate(V+[0x7ffffffe]):
        def st(env,v=v,i=i):
            randmem(env,random.Random(i),A,0x40); env.mem.w32(A+16,v)
        s.append(('v%x'%v,st,(A,)))
    go(P+'pool-playnode-begin','Begin__Ct6VIPool1Z17VIHSpritePlayNode',s,{},compare_ret=True)
else:
    u,sym,e=U[w]
    if w=='ff':
        s=[]
        for i,v in enumerate(V):
            def st(env,v=v,i=i):
                randmem(env,random.Random(i),A,0x40); env.mem.w32(A+4,v)
            s.append(('v%x'%v,st,(A,)))
    else: s=sc(V)
    go(u,sym,s,endh(e),compare_ret=True)
