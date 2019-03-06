#!/bin/bash
CSV_FILE=$1

big_start_field_id=$2

big_end_field_id=$3

num_fields=$4



if [ -f $CSV_FILE ]
then
while read line 
do
 echo $line

	field_id=$big_start_field_id
	start_field_id=$big_start_field_id
	while [ $field_id -lt  $big_end_field_id ]
	do
		end_field_id=$((start_field_id + num_fields))
	
		echo "header"
		for (( field_id = $start_field_id ;  $field_id < $end_field_id;  field_id++  ))
		do
			${line} | awk -F, -v awk_field_id=$field_id '{if ($2 ~ awk_field_id ) print $0}'
		done
		start_field_id=$end_field_id
	done
EOF
done < ${CSV_FILE}

else
	exit 1
fi


exit 0
