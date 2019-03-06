#!/bin/bash -vx
# "call_parameterise_1_0.sh"
# "this is a test script"
# "this script uses script variables"
# "29th March 2010"


# "sets up script variables used in running parameterise"
#this variable needs to be changed if the chosen file changes
$CSV_FILE=master.csv
$NUM_PRM=1000

# The following variables are used generally
COSMO_ROOT=/grid/cosmo

PROJ_ROOT=ittd/grid_cdm
mkdir ittd
mkdir $PROJ_ROOT

FITS_ROOT=/data/catalogues/sdss/raw/imaging
#mkdir $PROJ_ROOT/data
#mkdir $PROJ_ROOT/data/sdss
#mkdir SPROJ_ROOT/data/sdss/raw
#mkdir $PROJ_ROOT$FITS_ROOT

PRM_ROOT=/data/parameters/prm
mkdir $PROJ_ROOT/data
mkdir $PROJ_ROOT/parameters
mkdir $PROJ_ROOT$PRM_ROOT

CSV_ROOT=/data/csv
mkdir $PROJ_ROOT$CSV_ROOT

TEST_ROOT=/test/test_scripting_10_3_2010

# "makes a directory for parameter files"
mkdir $PROJ_ROOT$TEST_ROOT
mkdir $PROJ_ROOT$TEST_ROOT/test_prm/
mkdir $PROJ_ROOT$TEST_ROOT/test_txt/

# "copies CSV file from \"permanent\" (LFC) to \"temporary\" (node) storage"
cp /mnt/Share/test_run_745_1_2_subset.csv $PROJ_ROOT$CSV_ROOT/test_run_745_1_2_subset.csv

echo "copies parameterise executable from permanent (workspace/debug) to temporary (test)"
cp /home/creanero/proj/ittd_scg/grid_cdm/workspace/version_5_0/parameterisation_5_0/Debug/parameterisation_5_0 $PROJ_ROOT$TEST_ROOT/parameterise_5_0



echo "runs parameterise with the appropriate parameters"
echo
.$PROJ_ROOT$TEST_ROOT/parameterise_5_0 5 $PROJ_ROOT$PRM_ROOT/test_script $PROJ_ROOT$CSV_ROOT/test_run_745_1_2_subset.csv $PROJ_ROOT$FITS_ROOT
echo

echo "transfers parameter files to \"permanent\" storage"
cp $PROJ_ROOT$PRM_ROOT/*.prm $PROJ_ROOT$TEST_ROOT/test_prm/
cp $PROJ_ROOT$PRM_ROOT/*.txt $PROJ_ROOT$TEST_ROOT/test_txt/

#cleans up
rm $PROJ_ROOT$PRM_ROOT/*.*
