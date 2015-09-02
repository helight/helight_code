#!/bin/sh
A key installation of third-parties
# writed by helightxu on 2015/05/14

# If necessary, you can edit the following
THIRD_PARTY_HOME=`pwd`
# INSTALL_ALL为非空的时候，表示全新安装
INSTALL_ALL=$1

printf "\033[1;33m"
echo "Hi:"
echo " the third-parties will be installed to $THIRD_PARTY_HOME,"
echo " you can edit THIRD_PARTY_HOME in install-thirdparty.sh to change a directory."
echo " THIRD_PARTY_HOME=$THIRD_PARTY_HOME"
printf "\033[m"
echo "begin to install now ..."
sleep 1


function install_tar_gz_package()
{
package_name=$1
if test -f $package_name; then
printf "\n\033[1;33m installing $package_name \033[m\n"
tar_opt=${package_name##*.tar.}
if [ "$tar_opt" = "gz" ]; then
package_basename=`basename $package_name .tar.gz`
tar xzf $package_name -C ./src/
elif [ "$tar_opt" = "bz2" ]; then
package_basename=`basename $package_name .tar.bz2`
tar xjf $package_name -C ./src/
else
printf "\n\033[1;33m can't get the type of package \033[m\n"
exit 1
fi
else
printf "\n\033[1;33m $package_basename NOT FOUND \033[m\n"
fi
printf "\n\033[1;33m installing $package_basename OK\033[m\n"
}

function install_all()
{
echo "Begin to install......"
cd thirdparty
mkdir src

install_tar_gz_package websocket.tar.gz
install_tar_gz_package mysql.tar.gz
install_tar_gz_package go.crypto.tar.gz

cd -
echo "install ok"
}

install_all
