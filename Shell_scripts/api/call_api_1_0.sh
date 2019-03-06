##!/bin/bash -vx

echo "make_jdl_1_0.sh"
echo "this is a test script"
echo "this script uses script variables"
echo "31th March 2010"

echo "sets up script variables for making new scripts"
PROJ_ROOT=/proj/ittd_scg/grid_cdm
echo PROJ_ROOT
echo $PROJ_ROOT

FITS_ROOT=/data/catalogues/SDSS/raw/imaging
echo FITS_ROOT
echo $FITS_ROOT

PRM_ROOT=/data/parameters/prm
echo PRM_ROOT
echo $PRM_ROOT

TEST_ROOT=/test/test_scripting_10_3_2010
echo TEST_ROOT
echo $TEST_ROOT


echo "returns to root directory"
cd /
echo
pwd

mkdir $PROJ_ROOT$PRM_ROOT/test_prm/
mkdir $PROJ_ROOT$PRM_ROOT/test_txt/

echo "copies diagnose executable from permanent (workspace/debug) to temporary (test)"
cp /home/creanero/proj/ittd_scg/grid_cdm/workspace/version_5_0/diagnose_5_0/Debug/diagnose_5_0 $PROJ_ROOT$TEST_ROOT/diagnose_5_0

echo "copies extract executable from permanent (workspace/debug) to temporary (test)"
cp /home/creanero/proj/ittd_scg/grid_cdm/workspace/version_5_0/extract_5_0/Debug/extract_5_0 $PROJ_ROOT$TEST_ROOT/extract_5_0

echo "copies Parameter prm and txt files from permanent (workspace/debug) to temporary (test)"
cp $PROJ_ROOT$TEST_ROOT/test_prm/test_script_10_15.prm $PROJ_ROOT$PRM_ROOT/test_prm/
cp $PROJ_ROOT$TEST_ROOT/test_txt/test_script_10_15.txt $PROJ_ROOT$PRM_ROOT/test_txt/

#Need to put in some sort of text function here that looks into the .prm
#can't quite find the function right now
#will check when I get back
#files copied into local storage manually for now

echo "executes diagnose"
.$PROJ_ROOT$TEST_ROOT/diagnose_5_0 $PROJ_ROOT$PRM_ROOT/test_prm/test_script_10_15.prm

echo "executes extract"
.$PROJ_ROOT$TEST_ROOT/extract_5_0 $PROJ_ROOT$PRM_ROOT/test_prm/test_script_10_15.prm



echo "transfers local files to "permanent" storage"
cp $PROJ_ROOT$FITS_ROOT/*_local.fit $PROJ_ROOT$TEST_ROOT$FITS_ROOT

#cleans up, commented out to save replacing when needed
#rm $PROJ_ROOT$FITS_ROOT/*.fit
