select count(*)

FROM PhotoObj AS p
   
WHERE 
   (
(p.run = 2964 and p.rerun = 40 and p.camcol = 1 and p.field = 275)
or (p.run = 2964 and p.rerun = 40 and p.camcol = 1 and p.field = 278)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 1 and p.field = 105)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 1 and p.field = 108)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 2 and p.field = 107)
or (p.run = 2964 and p.rerun = 40 and p.camcol = 1 and p.field = 274)
or (p.run = 2964 and p.rerun = 40 and p.camcol = 1 and p.field = 277)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 1 and p.field = 106)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 1 and p.field = 109)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 2 and p.field = 108)
or (p.run = 2964 and p.rerun = 40 and p.camcol = 1 and p.field = 276)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 1 and p.field = 107)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 2 and p.field = 106)
or (p.run = 3059 and p.rerun = 42 and p.camcol = 2 and p.field = 109)
)
and ((p.status & 0x2000)!=0) --yes good status
and ((p.flags & 0x10000000)!=0) --yes good flags
and ((p.flags & 0x810000080020)=0) --not bad flags
and (p.type = 6) -- type = stars



