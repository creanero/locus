
repeat=$1
INDEX=0

#loop calls the delegation script every ten houes up to the number of times the repeat variable is set to.
for (( INDEX = 2 ;  $INDEX <= $repeat+1;  INDEX++  ))
do
sleep 36000

ittd/grid_cdm/scripts/delegation/delegation_2_0.sh

done

