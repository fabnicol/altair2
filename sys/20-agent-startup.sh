#!/bin/sh
#
# This file is executed at Plasma shutdown.
# Uncomment the following lines to kill the agents
# that were started at session startup.

a=$(rc-status | grep ajuster_version | grep start)

if test "y$a" != "y"; then
 chmod +x  /home/fab/Dev/altair/linux/warning
 exec /home/fab/Dev/altair/linux/warning
fi

