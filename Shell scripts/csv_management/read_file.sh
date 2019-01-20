#!/bin/bash

TMP=$1


big_start_field_id=$2

big_end_field_id=$3

num_fields=$4


num_files=0


split_files_dir="/tmp/testdir"
mkdir -p $split_files_dir
outfile="${split_files_dir}/${num_files}.csv"

previous_field=$big_start_field_id
current_field_count=0

BAKIFS=$IFS
IFS=$(echo -en "\n\b")
exec 3<&0
exec 0<"$TMP"
read -r line
header_line=$line
#   echo "$header_line"
#exit 0
while read -r line
do
 current_field=`echo $line | awk -F, '{print $2'}` 
 if [ $current_field -gt $big_end_field_id ]; then
   exit 0
 fi
 if [ $current_field -gt $previous_field ]; then
  current_field_count=$((current_field_count + 1))
  if [ $current_field_count -gt $num_fields ]; then
   num_files=$((num_files + 1))
   echo 
   outfile="${split_files_dir}/${num_files}.csv"
   echo $outfile
   echo "$header_line" > $outfile
   current_field_count=1
  fi
  previous_field=$current_field
 fi 
# echo "$line" 2>&1 >> "${out_file}"
 ( echo ${line} ) >> $outfile
done
IFS=$BAKIFS

