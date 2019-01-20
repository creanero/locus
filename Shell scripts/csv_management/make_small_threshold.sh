#!/bin/bash

TMP=$1


big_start_field_id=$2

big_end_field_id=$3

num_fields=$4

threshold=$5

num_files=0

#run=0
#rerun=0
#camcol=0
#field=0

small_line="0"



split_files_name=`basename ${TMP} .csv`
orig_dir=`dirname ${TMP}`
split_files_dir="${orig_dir}/split"
mkdir -p $split_files_dir/small/
outfile="${split_files_dir}/small/${split_files_name}_${num_files}_small.csv"

previous_field=$big_start_field_id
current_field_count=0




BAKIFS=$IFS
IFS=$(echo -en "\n\b")
exec 3<&0
exec 0<"$TMP"
read -r line

#run=`echo $line | awk -F, '{print $4'}`
#rerun=`echo $line | awk -F, '{print $5'}`
#camcol=`echo $line | awk -F, '{print $6'}`
#field=`echo $line | awk -F, '{print $7'}`




header_line="run,rerun,camcol,field"
#   echo "$header_line"
#exit 0
mkdir -p ${split_files_dir}/over_threshold/
over_threshold_file="${split_files_dir}/over_threshold/${split_files_name}_over_threshold_small.csv"

echo ${header_line} > ${over_threshold_file}

echo ${header_line} > ${outfile}

while read -r line
do

 small_line=`echo $line | awk -F, '{print $4 "," $5 "," $6 "," $7'}`
# rerun=`echo $line | awk -F, '{print $5'}`
# camcol=`echo $line | awk -F, '{print $6'}`
# field=`echo $line | awk -F, '{print $7'}`

 #small_line="${run},${rerun},${camcol},${field}"


 current_field=`echo $line | awk -F, '{print $2'}` 
 if [ $current_field -gt $big_end_field_id ]; then
   exit 0
 fi
 
  #if before the start of the range specified, do nothing
 if [ $current_field -lt $big_start_field_id ]; then
  #do nothing
  echo ""

  #if within the range continue the process
 else

   current_num_fields=`echo $line | awk -F, '{print $1'}`
   if [ $current_num_fields -gt $threshold ]; then
       ( echo ${small_line} ) >> ${over_threshold_file}
   else
   
  
    if [ $current_field -gt $previous_field ]; then
     current_field_count=$((current_field_count + 1))
     ( echo ${small_line} ) >> ${outfile}
  
     if [ $current_field_count -gt $num_fields ]; then
      num_files=$((num_files + 1))
      echo "${num_files}"
      outfile="${split_files_dir}/small/${split_files_name}_${num_files}_small.csv"
      echo $outfile
      echo "$header_line" > $outfile
      ( echo ${small_line} ) >> ${outfile}
  
      current_field_count=1
     fi
     previous_field=$current_field
    fi 
 # echo "$line" 2>&1 >> "${out_file}"
   fi
 fi
done
IFS=$BAKIFS



