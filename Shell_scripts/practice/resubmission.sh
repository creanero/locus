#this script is  intended to generate a colection for resubmission of 
#failed or aborted grid jobs base on whether  their associated fits
#file has been generated
TARGET_DIR=$1
GRID_ROOT=/grid/cosmo/

lfc-ls $GRID_ROOT/$TARGET_DIR | sed 's/_out.fit/.jdl/g' > /tmp/ls_lfc_test.out

ls $TARGET_DIR > /tmp/ls_jdl_test.out

diff /tmp/ls_lfc_test.out /tmp/ls_jdl_test.out | sed 's/> //g' | sed '/a/d' | sed '/</d' | sed '/,/d'  > diff_test.out

