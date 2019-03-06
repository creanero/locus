#verification of SDSS catalogue
#21st May 2010

#this script goes through all of the prm files and uses them to go through all of 
#the fits files in the LFC to see if they exist and to note whether or not the file
#has in fact been created as intended.  A file corresponding to each prm file 
#with the list of which files are present and which not will be generated in the same 
#directory with the same name as the prm but with the extension .ver

#In addition, it generates a file consisting of a list of prm files with a count of 
#the fits files in each that *are* present and indicating whether that prm file could be used

#assigns the first argument to the text parameter file name
PRT_FILE=$1
LIS_FILE=$2

echo "" > ${LIS_FILE}

COSMO_ROOT=/grid/cosmo
DAS_ROOT=http://das.sdss.org/imaging
ITTD_ROOT=ittd/grid_cdm/data/catalogues/sdss/raw/

#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`

COUNT=0

#copied from call API step
#transfers local files to permanent storag
#due to the layout of prm text files, it is necessary to use lines 3 onward
#and use NUM_LINES+1 instead of numlines
for (( NAME_INDEX = 3 ;  $NAME_INDEX <= $NUM_LINES+1;  NAME_INDEX++  ))
do

#this line gets the filename from the temp file and stores it in the variable name.
FITS_NAME=`sed -n ${NAME_INDEX}p $PRT_FILE`

LFC_NAME=${COSMO_ROOT}/${FITS_NAME}

#use lfc-ls to make see if the file already exists
lfc-ls ${LFC_NAME} &> /dev/null 
lfc_test_file_exists=$?
if [ x"${lfc_test_file_exists}" != x"0" ]; then
 echo "${LFC_NAME} does not exist"
 DAS_NAME=`echo ${FITS_NAME} | sed "s|${ITTD_ROOT}|${DAS_ROOT}|g"`
 echo "${DAS_NAME}" >> ${LIS_FILE}
else
 echo "${LFC_NAME} exists"
 COUNT=`expr ${COUNT} + 1`
fi
done

TOTAL=`expr ${NAME_INDEX} - 3`

echo "${COUNT} out of ${TOTAL} files in ${PRT_FILE} exist"

