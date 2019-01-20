#Copying from LFC to GridUI
#5th January 2011

#This script copies files from the LFC in location LFC_LOCATION
#to a destination on the local machine LOCAL_DESTINATION
#which are of sixe SIZE_OF_FILE

#first argument is the location on the LFC
#second argument is the destination on the local machine
#third argument is the size of the files to be copied
LFC_LOCATION=$1
LOCAL_DESTINATION=$2
SIZE_OF_FILE=$3

#makes a temporary file and puts the contents of that directory into it
umask 022
TMP_PRT_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
lfc-ls -l ${LFC_LOCATION} | grep ${SIZE_OF_FILE} | awk '{print $9}' >> ${TMP_PRT_FILE}

while read line 
do

  lcg-cp lfn:${LFC_LOCATION}/${line} file:${LOCAL_DESTINATION}/${line}

done < ${TMP_PRT_FILE}


