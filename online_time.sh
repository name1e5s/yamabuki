#!/bin/bash

ac -p | grep -v 'total' | awk '{print $1,$2}'