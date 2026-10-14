from hk import *
P='w16-tmpl-list-a-'
w=sys.argv[1]
OLD=ARENA+0x40000
def mk(count,cap,blk):
    def st(env):
        r=random.Random(count*31+cap*7+(blk!=0))
        randmem(env,r,A,0x40); fill_arena(env,count+cap,0x42000)
        env.mem.w32(A,count); env.mem.w32(A+4,cap); env.mem.w32(A+8,blk)
    return st
def vsc(extra=()):
    out=[]
    for count in (0,1,2,3):
        for cap in (0,count,count+2):
            for blk in (0,OLD):
                out.append(('n%d_c%d_b%x'%(count,cap,blk),mk(count,cap,blk),(A,)))
    return out
def mcopy():
    def f(c,e):
        d,s=c.r[4]&M32,c.r[5]&M32
        e.log.append(('mcopy',norm(d),norm(s),bytes(e.mem.read(s,64))))
        e.mem.write(d,bytes(e.mem.read(s,64))); return d
    return f
def mdtor():
    def f(c,e):
        e.log.append(('mdtor',norm(c.r[4]&M32),c.r[5]&M32,e.mem.u32(A),e.mem.u32(A+8)))
    return f
def vdel():
    def f(c,e): e.log.append(('vdel',norm(c.r[4]&M32)))
    return f
def dpool(ret):
    def f(c,e):
        e.log.append(('dpool',norm(c.r[4]&M32),e.mem.u32(A),e.mem.u32(A+8)))
        return ret(c,e) if callable(ret) else ret
    return f
M32_=0xffffffffffffffff
if w=='m44':
    H={'__builtin_vec_new':vnew(),'__builtin_vec_delete':vdel(),'__10VIMatrix44RC10VIMatrix44':mcopy(),'_$_10VIMatrix44':mdtor()}
    go(P+'vector-matrix44-allocate','Allocate__t8VIVector1Z10VIMatrix44',vsc(),H,compare_ret=True)
elif w=='m44n':
    H={'__builtin_vec_new':vnew(null_when=(3<<6)),'__builtin_vec_delete':vdel(),'__10VIMatrix44RC10VIMatrix44':mcopy(),'_$_10VIMatrix44':mdtor()}
    go(P+'vector-matrix44-allocate','Allocate__t8VIVector1Z10VIMatrix44',[s for s in vsc() if 'n1' in s[0] or 'n3' in s[0]],H,compare_ret=True)
elif w in('node','noden','nodeneg'):
    ret={'node':0,'noden':0,'nodeneg':M32_}[w]
    H={'__builtin_vec_new':vnew(null_when=(3<<8) if w=='noden' else None),'__builtin_vec_delete':vdel(),'__10VIMatrix44RC10VIMatrix44':mcopy(),'DestroyPool__t8VIVector1Z13VIHSpriteNode':dpool(ret)}
    go(P+'vector-hspritenode-allocate','Allocate__t8VIVector1Z13VIHSpriteNode',vsc(),H,compare_ret=True)
