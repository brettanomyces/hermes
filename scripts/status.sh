#!/usr/bin/env bash

ACCESS_TOKEN=40305984aa9c40c9bc63bfdc854718f29d16c1c3
DEVICE=2a003a000347343337373738
URL=https://api.particle.io/v1

FZ_TEMP=$(curl --silent "$URL/devices/$DEVICE/fzTemp?access_token=$ACCESS_TOKEN" | jq .result)
FZ_SET=$(curl --silent "$URL/devices/$DEVICE/fzSet?access_token=$ACCESS_TOKEN" | jq .result)
FR_TEMP=$(curl --silent "$URL/devices/$DEVICE/frTemp?access_token=$ACCESS_TOKEN" | jq .result)
FR_SET=$(curl --silent "$URL/devices/$DEVICE/frSet?access_token=$ACCESS_TOKEN" | jq .result)
BAFFEL_OPEN=$(curl --silent "$URL/devices/$DEVICE/baffelOpen?access_token=$ACCESS_TOKEN" | jq .result)
COMP_ACTIVE=$(curl --silent "$URL/devices/$DEVICE/compActive?access_token=$ACCESS_TOKEN" | jq .result)
COMP_WAIT=$(curl --silent "$URL/devices/$DEVICE/compWait?access_token=$ACCESS_TOKEN" | jq .result)
FAN_ACTIVE=$(curl --silent "$URL/devices/$DEVICE/fanActive?access_token=$ACCESS_TOKEN" | jq .result)
HEAT_ACTIVE=$(curl --silent "$URL/devices/$DEVICE/heatActive?access_token=$ACCESS_TOKEN" | jq .result)
HEAT_WAIT=$(curl --silent "$URL/devices/$DEVICE/heatWait?access_token=$ACCESS_TOKEN" | jq .result)

printf "Freezer Set: %s\n" $FZ_SET
printf "Freezer Temp: %s\n" $FZ_TEMP

printf "Fridge Set: %s\n" $FR_SET
printf "Fridge Temp: %s\n" $FR_TEMP

printf "Baffel Open: %s\n" $BAFFEL_OPEN

printf "Compressor Active: %s\n" $COMP_ACTIVE
printf "Compressor Waiting: %s\n" $COMP_WAIT

printf "Fan Active: %s\n" $FAN_ACTIVE

printf "Heat Active: %s\n" $HEAT_ACTIVE
printf "Heat Waiting: %s\n" $HEAT_WAIT

