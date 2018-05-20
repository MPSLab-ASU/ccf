#!/bin/bash
variable="$2"
objfile="$1"

arm-linux-gnueabi-readelf -s $objfile | grep " $variable$" | tr -s ' '|cut -d' ' -f3
exit
