#verification of SDSS catalogue
#11th June 2010

#this script goes through all of the prm files and uses them to go through all of 
#the fits files in the LFC to see if they exist and to note whether or not the file
#has in fact been created as intended.  A file corresponding to each prm file 
#with the list of which files are present and which not will be generated in the same 
#directory with the same name as the prm but with the extension .ver

#In addition, it generates a file consisting of a list of prm files with a count of 
#the fits files in each that *are* present and indicating whether that prm file could be used

COSMO_ROOT=/grid/cosmo

#first argument is the folder containing the prm files
#second argument is the name of the file to which the results will be stored
PRT_LOCATION=$1
VER_OVERALL=$2

#finds if the directory specified for the ver files exists and if not makes it
VER_LOCATION=`dirname ${VER_OVERALL}`
if [ ! -e ${VER_LOCATION} ]; then
 mkdir -p ${VER_LOCATION}
fi

INCOMPLETE_LOCATION=${VER_LOCATION}/txt_incomplete/

mkdir -p ${INCOMPLETE_LOCATION}

#initialises thefile
echo "" > ${VER_OVERALL}

#checks to see if the directory of PRM files exists and if it doesn't, aborts
if [ ! -e ${PRT_LOCATION} ]; then
 echo ${PRT_LOCATION} does not exist
 exit 1
fi

#makes variables to hold the various counters that track the catalogue
GRAND_TOTAL=0
GRAND_COUNT=0
FILE_TOTAL=0
FILE_COUNT=0


#makes a temporary file and puts the contents of that directory into it
umask 022
TMP_PRT_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
ls -1 ${PRT_LOCATION}/*.txt > ${TMP_PRT_FILE}

while read line 
do
 ./ittd/grid_cdm/scripts/delegation/delegation_2_0.sh

# echo $line
 VER_PREFIX=`basename $line .txt`
 VER_FILE="${VER_LOCATION}/${VER_PREFIX}.ver"

 echo "" > ${VER_FILE}

 PRT_FILE="${PRT_LOCATION}/${VER_PREFIX}.txt"


 #this line gets the number of lines in the temp file
 #this corresponds to the number of files in the directory
 NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`
 
 COUNT=0
 
 #copied from call API step
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
   echo "${LFC_NAME} does not exist" >> ${VER_FILE}
  else
   echo "${LFC_NAME} exists" >> ${VER_FILE}
   COUNT=`expr ${COUNT} + 1`
  fi
 done
 
 TOTAL=`expr ${NAME_INDEX} - 3`

 echo "${COUNT} out of ${TOTAL} files in ${PRT_FILE} exist" >> ${VER_OVERALL}
 if [ ${COUNT} -lt  ${TOTAL} ]; then
  #"File ${PRT_FILE} incomplete"
  GRAND_TOTAL=`expr ${GRAND_TOTAL} + ${TOTAL}`
  GRAND_COUNT=`expr ${GRAND_COUNT} + ${COUNT}`
  FILE_TOTAL=`expr ${FILE_TOTAL} + 1`
  #copies file to the directory of incomplete files
  cp ${PRT_FILE} ${INCOMPLETE_LOCATION}/
 else
  # "File ${PRT_FILE} complete"
  GRAND_TOTAL=`expr ${GRAND_TOTAL} + ${TOTAL}`
  GRAND_COUNT=`expr ${GRAND_COUNT} + ${COUNT}`
  FILE_TOTAL=`expr ${FILE_TOTAL} + 1`
  FILE_COUNT=`expr ${FILE_COUNT} + 1`
 fi


done < ${TMP_PRT_FILE}

echo "${GRAND_COUNT} out of ${GRAND_TOTAL} files in prm files located in directory ${PRT_LOCATION} exist" >> ${VER_OVERALL}

echo "${FILE_COUNT} out of ${FILE_TOTAL} files in directory ${PRT_LOCATION} are complete" >> ${VER_OVERALL}


if [ ${GRAND_COUNT} -lt  ${GRAND_TOTAL} ]; then
 echo "Directory ${PRT_LOCATION} incomplete"
else
 echo "Directory ${PRT_LOCATION} complete"
fi


