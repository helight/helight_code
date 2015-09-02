#!/bin/sh
# build ln thirdpaty lib path to app path
# writed by helightxu on 2015/05/14

printf "\033[1;33m"
echo "Hi:"
echo "  Begin to build ... ..."
printf "\033[m"
sleep 1

BUILD_HOME=`pwd`
THIRD_PARTY_HOME="$BUILD_HOME/../third-party/go-package/"

golibs=`ls -l $THIRD_PARTY_HOME|grep "^d"|awk '{print $8}'`

echo $golibs

function build_lib_path()
{
    app_path=$1
    if test -d $app_path; then
        cd $app_path/src
        mkdir thirdpaty/src/ -p
        ls
        cd thirdpaty/src/
        for golib in $golibs
        do
            if [ ! -L $golib ]; then
                cmd="ln -s $THIRD_PARTY_HOME/$golib ./$golib"
                echo $cmd
                $cmd
            fi
        done
        cd -
    else
        printf "\n\033[1;33m $package_basename NOT FOUND \033[m\n"
    fi
    printf "\n\033[1;33m installing $package_basename OK\033[m\n"
}

function install_all()
{
    echo "Begin to install......"

    echo "install ok"
}

install_all
