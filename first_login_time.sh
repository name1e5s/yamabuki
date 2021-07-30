#!/bin/bash

last --time-format iso | grep -v 'wtmp'| grep -v 'reboot' | grep -v 'tty' | grep -v '^$' | awk '{print $1, $4}' | sort | awk '!a[$1]++{print}'