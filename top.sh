#/bin/bash

PID=$(pgrep -fl ORM-SLauncher | awk '{print $1}')

if [ -n "$PID" ]; then
    top -p $PID
else
    echo "ORM-SLauncher is not running."
fi