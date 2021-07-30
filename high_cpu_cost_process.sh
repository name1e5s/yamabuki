#!/bin/bash

function report {
cat /tmp/cpu_usage.$$ | 
awk '
{ 
  process[$1]+=$4; 
  user[$1]=$2;
  comm[$1]=$3;
}
END {
  for(i in process) {
    if(process[i] > (50.0 * 20)) {
      printf("%s %s %s %s\n", user[i], i, comm[i],process[i]/20) ;
    }
  }
}' | sort -nrk 4 | head
rm /tmp/cpu_usage.$$
exit 0
}

trap 'report' INT

SECS=20
UNIT_TIME=1

STEPS=$(( $SECS / $UNIT_TIME ))

for((i=0;i<$STEPS;i++)); do
    ps -eo pid,user,comm,pcpu | egrep -v '(0.0)|(%CPU)' >> /tmp/cpu_usage.$$
    sleep $UNIT_TIME
done

report