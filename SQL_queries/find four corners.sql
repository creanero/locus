SELECT p.ra, p.dec
FROM   PhotoObj AS p, (SELECT MIN(p.ra) AS minra,
                            MAX(p.ra) AS maxra,
MIN(p.dec) AS mindec,
                            MAX(p.dec) AS maxdec
                     FROM   PhotoObj AS p
WHERE (p.run = 2964 and p.rerun = 40 and p.camcol = 1 and p.field = 278)) AS m
WHERE  (m.maxra = p.ra
       OR m.minra = p.ra
OR m.maxdec = p.dec
       OR m.mindec = p.dec) 