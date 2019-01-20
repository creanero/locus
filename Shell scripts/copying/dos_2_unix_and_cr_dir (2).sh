#Copying from LFC to GridUI
#5th January 2011

#This script copies files to the LFC in location LFC_LOCATION
#from a location on the local machine LOCAL_DESTINATION

#first argument is the location on the LFC
#second argument is the destination on the local machine
#third argument is the size of the files to be copied
LFC_LOCATION=$2
LOCAL_LOCATION=$1

# lfc-mkdir and lfc-ls to make a dir if it isn't there
lfc-ls -d ${LFC_LOCATION}
lfc_test_dir_exists=$?
if [ x"${lfc_test_dir_exists}" != x"0" ]; then
 lfc-mkdir -p ${LFC_LOCATION}
fi


#makes a temporary file and puts the contents of that directory into it
umask 022
TMP_PRT_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
ls ${LOCAL_LOCATION}  >> ${TMP_PRT_FILE}

while read line 
do
  dos2unix ${LOCAL_LOCATION}/${line}
  lcg-cr --vo cosmo -l lfn:${LFC_LOCATION}/${line} file:${LOCAL_LOCATION}/${line}

done < ${TMP_PRT_FILE}

