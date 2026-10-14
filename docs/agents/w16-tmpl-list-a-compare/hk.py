from tutil import *
ARENA=0x1a10000
def vnew(null_when=None):
    """__builtin_vec_new hook: distinct arena block per call; returns 0 when size == null_when."""
    def f(c,e):
        n=sum(1 for x in e.log if x[0]=='vnew')
        sz=c.r[4]&M32
        e.log.append(('vnew',sz))
        if null_when is not None and sz==null_when: return 0
        return ARENA+0x8000*n
    return f
def newh():
    def f(c,e):
        n=sum(1 for x in e.log if x[0]=='new')
        sz=c.r[4]&M32
        e.log.append(('new',sz))
        return ARENA+0x20000+0x1000*n
    return f
def fill_arena(env,seed=7,n=0x30000):
    randmem(env,random.Random(seed),ARENA,n)
