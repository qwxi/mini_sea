#!/bin/sh


#set env

cd ../
basedir=`pwd`
cd bin

export logdir=${basedir}/log
export shm_keypath=${basedir}/ipckeypath/shm
export queue_in_keypath=${basedir}/ipckeypath/queue_in
export queue_out_keypath=${basedir}/ipckeypath/queue_out

export SERVER_PORT=9999

#echo ${logdir}
#echo ${shm_keypath}
#echo ${queue_in_keypath}
#echo ${queue_out_keypath}
#echo  ${SERVER_PORT}

echo "----------------------------------"
echo "----------------------------------"
echo "|                                |"
echo "|          Welcome !             |"
echo "|                                |"
echo "|     MINI_SEA  start ...        |"
echo "|                                |"
echo "----------------------------------"
echo "----------------------------------"

./mini_sea
