#! /bin/bash
#********************************************************************
# $ID: build.sh      Sat 2016-01-16 14:36:45 +0800  dannywang       *
#                                                                   *
# Description:                                                      *
#                                                                   *
# Maintainer:  (danny.wang)  <dannywang@zhytek.com>                 *
#                                                                   *
# CopyRight (c) 2016 ZHYTEK                                         *
#   All rights reserved.                                            *
#                                                                   *
# This file is free software;                                       *
#   you are free to modify and/or redistribute it                   *
#   under the terms of the GNU General Public Licence (GPL).        *
#                                                                   *
# Last modified:                                                    *
#                                                                   *
# No warranty, no liability, use this at your own risk!             *
#*******************************************************************/

APPPATH=$(cd `dirname $0`/../; pwd)
CURRPATH=`pwd`
LIBQTDIR=$CURRPATH/QT
SRCOSLA=$APPPATH
QMAKE_DEF=$CURRPATH/qmake.def

QTDIR=$LIBQTDIR
#QMAKE_INCDIR=$LIBQTDIR/include
QMAKE_INCDIR_QT=$LIBQTDIR/include
QMAKE_LIBDIR_QT=$LIBQTDIR/lib

function create_qmake_def()
{
    echo "" >$QMAKE_DEF
    echo "QMAKE_UIC=$LIBQTDIR/bin/uic" >>$QMAKE_DEF
    echo "QMAKE_RCC=$LIBQTDIR/bin/rcc" >>$QMAKE_DEF
    echo "QMAKE_MOC=$LIBQTDIR/bin/moc" >>$QMAKE_DEF
    echo "QMAKE_INCDIR_QT=$LIBQTDIR/include" >>$QMAKE_DEF
    echo "QMAKE_LIBDIR_QT=$LIBQTDIR/lib" >>$QMAKE_DEF
    echo "" >>$QMAKE_DEF
}

if [ $1.x == "x86.x" ] ; then
    export QMAKESPEC=$LIBQTDIR/mkspecs/linux-g++
    export QT_QWS_FONTDIR=/tftpboot/filesys-firedc/opt/nfs
    create_qmake_def;
    $LIBQTDIR/bin/qmake -makefile $SRCOSLA/osla.pro
    echo "QMAKE Create Makefile for X86"
elif [ $1.x == "clean.x" ] ; then
    rm controller  Makefile  qmake.def  setup  wizard -fr
elif [ $1.x == "env.x" ] ; then
    rm controller  Makefile  qmake.def  setup  wizard -fr
    create_qmake_def;
else
    export QMAKESPEC=$QTDIR/mkspecs/qws/linux-arm-gnueabi-am335x-g++
    create_qmake_def;
    $LIBQTDIR/bin/qmake -makefile $SRCOSLA/osla.pro -r -spec $QMAKESPEC -cache $QMAKE_DEF
#    rm $QMAKE_DEF -f
    echo "QMAKE Create Makefile for ARM"
fi




#******************** End Of File: build.sh ********************/
