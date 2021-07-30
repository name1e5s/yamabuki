#!/bin/bash

last | awk '{print $1}' | grep -v 'wtmp' | grep -v '^$'  | sort -n | uniq -c | awk '{print $2,$1}'