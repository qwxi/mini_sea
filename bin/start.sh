#!/bin/sh


#set env

export logdir=/home/wzx/tmp/fromgithup/mini_sea/log
export shm_keypath=/home/wzx/tmp/fromgithup/mini_sea/ipckeypath/shm
export queue_in_keypath=/home/wzx/tmp/fromgithup/mini_sea/ipckeypath/queue_in
export queue_out_keypath=/home/wzx/tmp/fromgithup/mini_sea/ipckeypath/queue_out

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
