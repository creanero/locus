#!/bin/bash -vx
echo "The|s>aip2" > /tmp/tmp_pwd_file.tmp
#echo "oc12CO454reIpXi7" > /tmp/tmp_pwd_file.tmp
myproxy-get-delegation -dn --voms cosmo -S < /tmp/tmp_pwd_file.tmp

rm /tmp/tmp_pwd_file.tmp

