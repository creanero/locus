#!/bin/bash -vx
echo "call_parameterise_1_0.sh"
echo "this is a test script"
echo "this script uses script variables"
echo "10th March 2010"

echo "sets up script variables used in running parameterise"
PROJ_ROOT=/proj/ittd_scg/grid_cdm
echo PROJ_ROOT
echo $PROJ_ROOT

FITS_ROOT=/data/catalogues/SDSS/raw/imaging
echo FITS_ROOT
echo $FITS_ROOT

PRM_ROOT=/data/parameters/prm
echo PRM_ROOT
echo $PRM_ROOT

CSV_ROOT=/data/csv
echo CSV_ROOT
echo $CSV_ROOT

TEST_ROOT=/test/test_scripting_10_3_2010
echo TEST_ROOT
echo $TEST_ROOT

echo "returns to root directory"
cd /
echo
pwd

echo "makes a directory for parameter files"
mkdir $PROJ_ROOT$TEST_ROOT
mkdir $PROJ_ROOT$TEST_ROOT/test_prm/
mkdir $PROJ_ROOT$TEST_ROOT/test_txt/

echo "copies CSV file from \"permanent\" (mnt) to \"temporary\" (proj) storage"
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
