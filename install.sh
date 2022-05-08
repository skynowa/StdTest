#!/usr/bin/env bash
#
# \file  install.sh
# \brief build project
#


# set -ex

SUDO="sudo"

# https://stackoverflow.com/questions/3466166/how-to-check-if-running-in-cygwin-mac-or-linux
if [[ "${OSTYPE}" == "win"* || "${OSTYPE}" == "msys"* ]]; then
	SUDO=""
fi

# vars
PROJECT_NAME="StdTest"
DIR_PROJECT="$PWD"
DIR_BUILD="../${PROJECT_NAME}_build"

# Build
echo "Build/Install ..."

mkdir -p $DIR_BUILD
cd $DIR_BUILD

# sudo
cmake $DIR_PROJECT
${SUDO} cmake --build . --target install
