#Timer script
#7th May 2010
#Oisin Creaher

#This is a timer script
#first this script gets the start time.
#it then runs the script or executable specified as an argument
#then it gets the end time

LOG_FILE=logfile.txt

echo "Start time"
date

time ./$1 $2 $3 $4 $5 $6 >$LOG_FILE

echo "End time"

date
 

