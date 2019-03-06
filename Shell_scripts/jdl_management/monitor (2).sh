#this script checks the status of a joblist entered at the command line
joblist=$1

echo "Submitted"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Submitted | wc -l

echo "Waiting"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Waiting | wc -l

echo "Ready"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Ready | wc -l

echo "Scheduled"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Scheduled | wc -l

echo "Running"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Running | wc -l

echo "Done"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Done | wc -l

echo "Aborted "
glite-wms-job-status -i $1 | grep "Current Status:" | grep Aborted | wc -l

echo "Cancelled"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Cancelled | wc -l

echo "Cleared"
glite-wms-job-status -i $1 | grep "Current Status:" | grep Cleared | wc -l


