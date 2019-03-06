##!/bin/bash -vx
#mass_lfc-cp
#Oisin Creaner
#ITTD SCG
#11 May 2010
#this script is intended to copy the entire contents of a directory specified
#in the first argument $1 to corresponding filenames in a LFC directory 
#specified as an LFN in the second argument, $2

#a temporary file name is used here to hold the names of the contents of the
#files in the source directory
TEMP_FILE=/tmp/source_ls.txt

#for clarity, the arguements are redefined as source and destination
SOURCE=$1
DESTINATION=$2

#this line gets the contents of the source directory and puts it in TEMP_FILE
echo "`ls $SOURCE`">>$TEMP_FILE

#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $TEMP_FILE | awk '{print $1}'`

NAME=Stranger
P=p

for (( NAME_INDEX = 1 ;  NAME_INDEX <= $NUM_LINES;  NAME_INDEX++  ))

do

#this line gets the filename from the temp file and stores it in the variable name.
NAME=`sed -n $NAME_INDEX$P $TEMP_FILE`

#this line copies the named file (NAME)from source to destination
lcg-cr --vo cosmo -l lfn:$DESTINATION$NAME file:$SOURCE$NAME


done


#at end of process, remove the temp file
rm $TEMP_FILE

