##!/bin/bash -vx
#file_lfc-cp
#Oisin Creaner
#ITTD SCG
#11 May 2010

#this script is intended to copy files specified in the first argument PRM_FILE
#from a root in the LFC specified in the second argument SOURCE
#into local storage directory as specified in the third DESTINATION
#at present, it is required that any subdirectoies needed by the files will 
#be opresent in DESTINATION.

#errors will result if the files are not present in SOURCE

#a temporary file name is used here to hold the names of the contents of the
#files in the source directory
PRM_FILE=$1

#for clarity, the arguements are redefined as source and destination
SOURCE=$2
DESTINATION=$3


#this line gets the number of lines in the temp file
#this corresponds to the number of files in the directory
NUM_LINES=`wc $PRM_FILE | awk '{print $1}'`

NAME=Stranger
P=p

#due to the layout of prm text files, it is necessary to use lines 3 onward
for (( NAME_INDEX = 3 ;  NAME_INDEX <= $NUM_LINES;  NAME_INDEX++  ))

do

#this line gets the filename from the temp file and stores it in the variable name.
NAME=`sed -n $NAME_INDEX$P $PRM_FILE`

#this line copies the named file (NAME)from source to destination
lcg-cp --vo cosmo lfn:$DESTINATION$NAME file:$SOURCE$NAME


done




