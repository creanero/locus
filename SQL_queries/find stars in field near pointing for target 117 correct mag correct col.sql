DECLARE @point_ra float SET @point_ra=309.4971506647;
DECLARE @point_dec float SET @point_dec=0.2737432692606;
DECLARE @ra_corr float set @ra_corr = COS(@point_dec);
  
DECLARE @u_target float set @u_target = 19.3826713562;
DECLARE @g_target float set @g_target = 18.15453338623;
DECLARE @r_target float set @r_target = 17.69709205627;
DECLARE @i_target float set @i_target = 17.52318382263;
DECLARE @z_target float set @z_target = 17.46723175049;
  

DECLARE @fov float set @fov = 0.125;
DECLARE @fov_ra float set @fov_ra = @fov/@ra_corr;
DECLARE @mag_limit float set @mag_limit = 2.0;
DECLARE @col_limit float set @col_limit = 0.1;
  
  
  
SELECT p.ra,p.dec,p.u,p.g,p.r,p.i,p.z

FROM PhotoObj AS p
   
WHERE 
   (
	p.ra BETWEEN (@point_ra-@fov_ra)  AND (@point_ra+@fov_ra)
	AND p.dec BETWEEN (@point_dec-@fov) AND (@point_dec+@fov)
	)
AND 
	(
	((p.status & 0x2000)!=0) 
	AND ((p.flags & 0x10000000)!=0) 
	AND ((p.flags & 0x810000080020)=0) 
	AND (p.type = 6) 
	)
AND 
	(
	p.r BETWEEN (@r_target-@mag_limit) AND (@r_target+@mag_limit)
	)

AND 
	(
		(p.g - p.r) BETWEEN ((@g_target-@r_target)-@col_limit)AND((@g_target-@r_target)+@col_limit)
	AND
		(p.r - p.i) BETWEEN ((@r_target-@i_target)-@col_limit)AND((@r_target-@i_target)+@col_limit)
	)