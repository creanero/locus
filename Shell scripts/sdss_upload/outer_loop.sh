#Outer loop verification of SDSS catalogue
#25th May 2010

#this script goes through all of the prm files and runs inner loop on them

#In addition, it generates a file consisting of a list of prm files with a count of 
#the fits files in each that *are* present and indicating whether that prm file could be used


#first argument is the folder containing the prm files
#second argument is the name of the file to which the results will be stored
PRT_LOCATION=$1
VER_OVERALL=$2

#finds if the directory specified for the ver files exists and if not makes it
VER_LOCATION=`dirname ${VER_OVERALL}`
if [ ! -e ${VER_LOCATION} ]; then
 mkdir -p ${VER_LOCATION}
fi


#checks to see if the directory of PRM files exists and if it doesn't, aborts
if [ ! -e ${PRT_LOCATION} ]; then
 echo ${PRT_LOCATION} does not exist
 exit 1
fi



#makes a temporary file and puts the contents of that directory into it
umask 022
TMP_PRT_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
ls -1 ${PRT_LOCATION}/*.txt > ${TMP_PRT_FILE}

while read line 
do
# echo $line
 VER_PREFIX=`basename $line .txt`
 VER_FILE="${VER_LOCATION}/${VER_PREFIX}.ver"

 LIS_FILE="${VER_LOCATION}/${VER_PREFIX}.lis"
 PRT_FILE="${PRT_LOCATION}/${VER_PREFIX}.txt"

 ./ittd/grid_cdm/scripts/sdss_upload/inner_loop_1_0.sh ${PRT_FILE} ${LIS_FILE} ${VER_LOCATION}

done < ${TMP_PRT_FILE}

ittd/grid_cdm/scripts/verification/verification_2_0.sh ${PRT_LOCATION} ${VER_OVERALL}
