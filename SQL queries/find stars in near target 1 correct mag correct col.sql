select count(*)

FROM PhotoObj AS p
   
WHERE --Locational parameters
   (
	p.ra BETWEEN (174.6017-0.3779)  AND (174.6017+0.3779)
	AND p.dec BETWEEN (48.58279-0.25) AND (48.58279+0.25)
	)
AND -- Local Catlogue
	(
	((p.status & 0x2000)!=0) --yes good status
	AND ((p.flags & 0x10000000)!=0) --yes good flags
	AND ((p.flags & 0x810000080020)=0) --not bad flags
	AND (p.type = 6) -- type = stars
	)
AND -- Magnitude
	(
	p.r BETWEEN (15.786-2) AND (15.786+2)
	)

AND -- Colour
	(
		(p.g - p.r) BETWEEN ((16.24654-15.786)-0.1)AND((16.24654-15.786)+0.1)
	AND
		(p.r - p.i) BETWEEN ((15.786-15.62841)-0.1)AND((15.786-15.62841)+0.1)
	)

