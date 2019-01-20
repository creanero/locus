--sql query to locate reference stars for an entry in the output catalogue, given pointing and target info
--variables used for SDSS J203733.62+001953.5 (top target in sample)
--declare pointing coordinates
declare @pointing_ra float;
set @pointing_ra = 309.4971506647;
declare @pointing_dec float;
set @pointing_dec = 0.2737432692606;
  
--declare target g, r, i
declare @target_g float;
set @target_g = 18.15453338623;
declare @target_r float;
set @target_r = 17.69709205627;
declare @target_i float;
set @target_i = 17.52318382263; 
  
--declare fov size  
declare @fov_size float;
set @fov_size = 0.25;  
declare @fov_size_ra float;
set @fov_size = 0.25/cos(@pointing_dec);  --calculate fov size in RA
  
--declare mag match limit
declare @mag_lim float;
set @mag_lim = 2.0;   
  
--declare colour match limit
declare @col_lim float;
set @col_lim = 0.1; 
  
  
select ra, dec from photoobjall
  where ( --Local Catalogue clean stars
		mode = 1
		AND type = 6
		AND((flags_r & 0x10000000) != 0) --good flags
		AND ((flags_r & 0x80020) = 0) --bad flags_1
		AND ((CAST((flags_r/0x100000000) as INT) & 0x8100) = 0 )--bad flags_2
  ) 
    -- in FoV
    AND (ra<(@pointing_ra+(@fov_size_ra/2)))AND (ra>(@pointing_ra-(@fov_size_ra/2)))
    AND (dec<(@pointing_dec+(@fov_size/2)))AND (dec>(@pointing_dec-(@fov_size/2)))
    
    -- mag match
    AND (r<(@target_r+@mag_lim))AND (r>(@target_r-@mag_lim))
    
    -- col match
    AND ((g-r)<((@target_g-@target_r)+(@col_lim)AND ((g-r)>((@target_g-@target_r)-(@col_lim)
    AND ((r-i)<((@target_r-@target_i)+(@col_lim)AND ((r-i)>((@target_r-@target_i)-(@col_lim)