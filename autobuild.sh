
#########################################################################
# File Name: autobuild.sh
# Author: corner
# mail: corner@88.com
# Created Time: 2024年07月13日 星期日 13时14分28秒
#########################################################################
#!/bin/bash

set -x

rm -rf `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make
