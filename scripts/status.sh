#!/usr/bin/env bash

ACCESS_TOKEN=40305984aa9c40c9bc63bfdc854718f29d16c1c3
DEVICE=2a003a000347343337373738
URL=https://api.particle.io/v1

FZ_TEMP=$(curl --silent "$URL/devices/$DEVICE/fzTemp?access_token=$ACCESS_TOKEN" | jq .result)
printf "Freezer Temp: %s\n" $FZ_TEMP

FZ_SET=$(curl --silent "$URL/devices/$DEVICE/fzSet?access_token=$ACCESS_TOKEN" | jq .result)
printf "Freezer Set: %s\n" $FZ_SET

FR_TEMP=$(curl --silent "$URL/devices/$DEVICE/frTemp?access_token=$ACCESS_TOKEN" | jq .result)
printf "Fridge Temp: %s\n" $FR_TEMP

FR_SET=$(curl --silent "$URL/devices/$DEVICE/frSet?access_token=$ACCESS_TOKEN" | jq .result)
printf "Fridge Set: %s\n" $FR_SET

BAFFEL_OPEN=$(curl --silent "$URL/devices/$DEVICE/baffelOpen?access_token=$ACCESS_TOKEN" | jq .result)
printf "Baffel Open: %s\n" $BAFFEL_OPEN

COMP_ACTIVE=$(curl --silent "$URL/devices/$DEVICE/compActive?access_token=$ACCESS_TOKEN" | jq .result)
printf "Compressor Active: %s\n" $COMP_ACTIVE

COMP_WAIT=$(curl --silent "$URL/devices/$DEVICE/compWait?access_token=$ACCESS_TOKEN" | jq .result)
printf "Compressor Waiting: %s\n" $COMP_WAIT

FAN_ACTIVE=$(curl --silent "$URL/devices/$DEVICE/fanActive?access_token=$ACCESS_TOKEN" | jq .result)
printf "Fan Active: %s\n" $FAN_ACTIVE

HEAT_ACTIVE=$(curl --silent "$URL/devices/$DEVICE/heatActive?access_token=$ACCESS_TOKEN" | jq .result)
printf "Heat Active: %s\n" $HEAT_ACTIVE

HEAT_WAIT=$(curl --silent "$URL/devices/$DEVICE/heatWait?access_token=$ACCESS_TOKEN" | jq .result)
printf "Heat Waiting: %s\n" $HEAT_WAIT
