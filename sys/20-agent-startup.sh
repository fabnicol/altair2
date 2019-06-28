#!/bin/sh
#
# This file is executed at Plasma shutdown.
# Uncomment the following lines to kill the agents
# that were started at session startup.

/etc/init.d/actualiser_warning start
