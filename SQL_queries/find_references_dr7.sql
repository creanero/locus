-- Use this script to find reference stars from a pointing in the SDSS DR7 catalogue 
-- IMPORTANT NOTE: USE THE DR7 Context in CAS

-- Enter the RA and Dec of the POINTING here
DECLARE @point_ra float SET @point_ra=309.4971506647; -- RA in DEGREES
DECLARE @point_dec float SET @point_dec=0.2737432692606; -- DEC in DEGREES
-- The system will then automatically generate a correction for angle in RA
DECLARE @ra_corr float set @ra_corr = COS(@point_dec);
  
-- Enter the ugriz magnitudes of the target
DECLARE @u_target float set @u_target = 19.3826713562;
DECLARE @g_target float set @g_target = 18.15453338623;
DECLARE @r_target float set @r_target = 17.69709205627;
DECLARE @i_target float set @i_target = 17.52318382263;
DECLARE @z_target float set @z_target = 17.46723175049;
  
-- Enter the observational parameters
DECLARE @fov float set @fov = 0.25; -- FoV size in DEGREES for a Square FoV
-- The system will automatically correct the FoV Size in RA
DECLARE @fov_ra float set @fov_ra = @fov/@ra_corr;
DECLARE @mag_limit float set @mag_limit = 2.0; -- magnitude difference limit
DECLARE @col_limit float set @col_limit = 0.1;
  
  
-- Select position and magnitude, and calculate rating  
SELECT p.ra,p.dec,p.u,p.g,p.r,p.i,p.z,
  --for the definition of rating used here, see Creaner et al, 2019, The Locus Algorithm: A technique for identifying optimised pointings for differential photometry
  (1-abs(((p.g-@g_target)-((p.r-@r_target)))/@col_limit))*(1-abs(((p.r-@r_target)-((p.i-@i_target)))/@col_limit)) as rating
  
  -- from the Photometric Objects Catalogue
FROM PhotoObj AS p
   
WHERE 
  (
    -- Reference in a FoV centred on the pointing
     p.ra BETWEEN (@point_ra-(@fov_ra/2))  AND (@point_ra+(@fov_ra/2))
     AND p.dec BETWEEN (@point_dec-(@fov/2)) AND (@point_dec+(@fov/2))
  )
  
AND 
  (
    -- Reference passes the Clean Sample of Point Sources tests
	((p.status & 0x2000)!=0) 
	AND ((p.flags & 0x10000000)!=0) 
	AND ((p.flags & 0x810000080020)=0) 
	AND (p.type = 6) 
  )
  
AND 
  (
    -- Reference magnitude is in the correct range
	p.r BETWEEN (@r_target-@mag_limit) AND (@r_target+@mag_limit)
  )
  
AND 
  (
    -- Reference colour is in the correct range
		(p.g - p.r) BETWEEN ((@g_target-@r_target)-@col_limit)AND((@g_target-@r_target)+@col_limit)
	AND
		(p.r - p.i) BETWEEN ((@r_target-@i_target)-@col_limit)AND((@r_target-@i_target)+@col_limit)
  )
  
