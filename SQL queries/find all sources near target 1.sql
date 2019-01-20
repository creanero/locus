select count(*)
  
FROM PhotoObj AS p
   
WHERE 
   p.ra BETWEEN (174.6017-0.3779)  AND (174.6017+0.3779)
   AND p.dec BETWEEN (48.58279-0.25) AND (48.58279+0.25)
   
   