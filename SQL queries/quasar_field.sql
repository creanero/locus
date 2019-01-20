select count (*) 
into mydb.quasar_count
from 
  (
    select 
      (q.ra - (0.16667/cos(radians(q.dec)))) as westlimit,
      (q.ra + (0.16667/cos(radians(q.dec)))) as eastlimit,
      (q.dec - 0.16667) as southlimit,
      (q.dec + 0.16667) as northlimit
      
      from qsobest as q
  )
  as l, field as f
where
  (
    (
      f.ramax between l.westlimit and l.eastlimit
    )
    or
    (
      f.ramin between l.westlimit and l.eastlimit
    )
  )
  AND
  (
    
    (
      f.decmax between l.southlimit and l.northlimit
    )
    OR
    (
      f.decmin between l.southlimit and l.northlimit
    )
  )