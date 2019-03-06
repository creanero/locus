-- This query does a table JOIN between the imaging (PhotoObj) and spectra
-- (SpecObj) tables and includes the necessary columns in the SELECT to upload
-- the results to the DAS (Data Archive Server) for FITS file retrieval.
select DISTINCT 
  p.run, p.rerun, p.camcol, p.field
  
FROM PhotoObj AS p
   
WHERE 
   p.ra BETWEEN (174.6017-0.3779)  AND (174.6017+0.3779)    AND p.dec BETWEEN (48.58279-0.25) AND (48.58279+0.25)
   