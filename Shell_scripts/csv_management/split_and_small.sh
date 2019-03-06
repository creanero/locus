
#split and make small

#!/bin/bash

#arguments are named, but read clearer in this short a script without their full names, so they are shown here with the names commented out
#TMP=$1
#big_start_field_id=$2
#big_end_field_id=$3
#num_fields=$4
#threshold=$5

ittd/grid_cdm/scripts/csv_management/read_file_threshold.sh $1 $2 $3 $4 $5
split_error=$?
echo "split returned ${split_error}"

ittd/grid_cdm/scripts/csv_management/make_small_threshold.sh $1 $2 $3 $4 $5
make_small_error=$?
echo "make small returned ${make_small_error}"


