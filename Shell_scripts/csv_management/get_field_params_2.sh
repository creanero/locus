#!/bin/bash

field_id=$2
cat $1 | awk -F, -v awk_field_id=$field_id '{if ($2 ~ awk_field_id ) print $0}'
