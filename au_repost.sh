#!/bin/bash

USER_LIST=$(ausearch -k elevated-priv --raw | grep -o 'auid=[0-9]*' | awk -F '=' '{print $2}' | sort -n | uniq | xargs getent passwd | awk -F ':' '{print $1}')

function email_full_text() {
        echo "Privilege Escalation Found!!"
        echo "User List:"
        echo $USER_LIST
        echo "Raw Log"
        echo "-----"
        ausearch -k elevated-priv --raw
        echo "-----"
}

email_full_text | mail -s "Privilege Escalation Found" -a "From: <>" <>