#!/bin/bash -vx
#verification of SDSS catalogue
#wget download if not complete
#24th May 2010

#this script goes through all of the prm files and uses them to go through all of 
#the fits files in the LFC to see if they exist and to note whether or not the file
#has in fact been created as intended.  A file corresponding to each prm file 
#with the list of which files are present and which not will be generated in the same 
#directory with the same name as the prm but with the extension .ver

#In addition, it generates a file consisting of a list of prm files with a count of 
#the fits files in each that *are* present and indicating whether that prm file could be used

COSMO_ROOT=/grid/cosmo
DAS_ROOT=http://das.sdss.org/imaging
ITTD_ROOT=ittd/grid_cdm/data/catalogues/sdss/raw/

#first argume` is the folder containing the prm files
#second argument is the name of the file to which the results will be stored
PRT_LOCATION=$1
VER_OVERALL=$2

#finds if the directory specified for the ver files exists and if not makes it
VER_LOCATION=`dirname ${VER_OVERALL}`
if [ ! -e ${VER_LOCATION} ]; then
 mkdir -p ${VER_LOCATION}
fi

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
GRAND_DOWNLOAD_COUNT=0
GRAND_DOWNLOAD_FAIL_COUNT=0
GRAND_UPLOAD_FAIL_COUNT=0
FILE_TOTAL=0
FILE_COUNT=0


#makes a temporary file and puts the contents of that directory into it
umask 022
TMP_PRT_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
ls -1 ${PRT_LOCATION}/*.txt > ${TMP_PRT_FILE}

while read line 
do
# echo $line
 VER_PREFIX=`basename $line .txt`
 VER_FILE="${VER_LOCATION}/${VER_PREFIX}.ver"

 echo "" > ${VER_FILE}

 PRT_FILE="${PRT_LOCATION}/${VER_PREFIX}.txt"


 #this line gets the number of lines in the temp file
 #this corresponds to the number of files in the directory
 NUM_LINES=`wc $PRT_FILE | awk '{print $1}'`
 
 #makes the counters that track the contents of the individual PRM file
 COUNT=0
 DOWNLOAD_COUNT=0
 DOWNLOAD_FAIL_COUNT=0
 UPLOAD_FAIL_COUNT=0
 
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
   #IF THE FILE IS NOT IN THE LFC
   #make the filename to be used in the DAS
   DAS_NAME=`echo ${FITS_NAME} | sed "s|${ITTD_ROOT}|${DAS_ROOT}|g"`
   #get the file from the DAS to the PWD
   wget ${DAS_NAME}
   #make the name of the file just downloaded
   TMP_FITS_NAME=`basename ${FITS_NAME}`
   #check that that file exists
   ls ${TMP_FITS_NAME} &> /dev/null
   test_tmp_file_exists=$?  
   if [ x"${test_tmp_file_exists}" != x"0" ]; then
    #IF THAT FILE FAILED TO DOWNLOAD, note that it failed
    echo "${LFC_NAME} failed to download" >> ${VER_FILE}
    DOWNLOAD_FAIL_COUNT=`expr ${DOWNLOAD_FAIL_COUNT} + 1`
   else
    #IF THE DOWNLOAD SUCCEEDED
    DOWNLOAD_COUNT=`expr ${DOWNLOAD_COUNT} + 1`
    #make the directory name the new file will have to go into
    LFC_DIR_NAME=dirname ${LFC_NAME}
    lfc-ls -d ${LFC_DIR_NAME} &> /dev/null
    lfc_test_dir_exists=$?
    if [ x"${lfc_test_dir_exists}" != x"0" ]; then
     #IF THE DIRECTORY IS NOT IN THE LFC
     #make it
     lfc-mkdir -p ${LFC_DIR_NAME}
    fi
    #now copy and register the newly downloaded file in the LFC
    lcg-cr  --vo cosmo -l lfn:${LFC_NAME} file:${TMP_FITS_NAME}
    #check that the file is there 
    lfc-ls ${LFC_NAME} &> /dev/null
    lfc_test_file_exists=$?
    if [ x"${lfc_test_file_exists}" != x"0" ]; then
     #IF THAT FILE HAS NOT UPLOADED
     #note and count the failure
     echo "${LFC_NAME} failed to upload" >> ${VER_FILE}
     UPLOAD_FAIL_COUNT=`expr ${UPLOAD_FAIL_COUNT} + 1`
    else
     #IF THE FILE HAS DOWNLOADED AND UPLOADED
     #note the success
     echo "${LFC_NAME} downloaded from SDSS and uploaded to LFC" >> ${VER_FILE}
     COUNT=`expr ${COUNT} + 1`
    fi
    #remove the temporary copy of the fits file
    rm ${TMP_FITS_NAME}
   fi

  else
   #IF THE FILE IS IN THE LFC
   #note its existance
   echo "${LFC_NAME} exists" >> ${VER_FILE}
   COUNT=`expr ${COUNT} + 1`
  fi
 done
 
 TOTAL=`expr ${NAME_INDEX} - 3`

 echo "${COUNT} out of ${TOTAL} files in ${PRT_FILE} exist" >> ${VER_OVERALL}
 if [ ${COUNT} -lt  ${TOTAL} ]; then
  #"File ${PRT_FILE} incomplete"
  #adds up the totals to a grand total for the whole directory
  GRAND_TOTAL=`expr ${GRAND_TOTAL} + ${TOTAL}`
  GRAND_COUNT=`expr ${GRAND_COUNT} + ${COUNT}`
  GRAND_DOWNLOAD_COUNT=`expr ${GRAND_DOWNLOAD_COUNT} + ${DOWNLOAD_COUNT}`
  GRAND_DOWNLOAD_FAIL_COUNT=`expr ${GRAND_DOWNLOAD_FAIL_COUNT} + ${DOWNLOAD_FAIL_COUNT}`
  GRAND_UPLOAD_FAIL_COUNT=`expr ${GRAND_UPLOAD_FAIL_COUNT} + ${UPLOAD_FAIL_COUNT}`
  FILE_TOTAL=`expr ${FILE_TOTAL} + 1`
 else
  # "File ${PRT_FILE} complete"
  #adds up the totals to a grand total for the whole directory
  GRAND_TOTAL=`expr ${GRAND_TOTAL} + ${TOTAL}`
  GRAND_COUNT=`expr ${GRAND_COUNT} + ${COUNT}`
  GRAND_DOWNLOAD_COUNT=`expr ${GRAND_DOWNLOAD_COUNT} + ${DOWNLOAD_COUNT}`
  GRAND_DOWNLOAD_FAIL_COUNT=`expr ${GRAND_DOWNLOAD_FAIL_COUNT} + ${DOWNLOAD_FAIL_COUNT}`
  GRAND_UPLOAD_FAIL_COUNT=`expr ${GRAND_UPLOAD_FAIL_COUNT} + ${UPLOAD_FAIL_COUNT}`
  FILE_TOTAL=`expr ${FILE_TOTAL} + 1`
  FILE_COUNT=`expr ${FILE_COUNT} + 1`
 fi


done < ${TMP_PRT_FILE}

echo "${GRAND_COUNT} out of ${GRAND_TOTAL} files in prm files located in directory ${PRT_LOCATION} exist" >> ${VER_OVERALL}
echo "${GRAND_DOWNLOAD_COUNT} files downloaded from SDSS" >> ${VER_OVERALL}
echo "${GRAND_DOWNLOAD_FAIL_COUNT} files failed to download from SDSS" >> ${VER_OVERALL}
echo "${GRAND_UPLOAD_FAIL_COUNT} files failed to upload to the LFC" >> ${VER_OVERALL}


echo "${FILE_COUNT} out of ${FILE_TOTAL} files in directory ${PRT_LOCATION} are complete" >> ${VER_OVERALL}


if [ ${GRAND_COUNT} -lt  ${GRAND_TOTAL} ]; then
 echo "Directory ${PRT_LOCATION} incomplete"
else
 echo "Directory ${PRT_LOCATION} complete"
fi
