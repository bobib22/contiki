#!/bin/bash

#grep ';name' $trace | sed 's/.*;\(m3-[0-9]\{1,3\}\);name: \([0-1]\+\)/s\/\2\/\1-BF\/g/g' | sort -t '-' -nk 2

lille-bf-mapping(){
sed '
s/0110100101100000001010000111110100101011/m3-3-BF/g
s/0100110101110010101010001011110101101000/m3-7-BF/g
s/1110111100000000001110000110010110111001/m3-11-BF/g
s/0110100100110000001010001110110101101001/m3-71-BF/g
s/0111111110000000001010000111010100101001/m3-75-BF/g
s/0110100100010000001010001110110101101001/m3-77-BF/g
s/0111111100010000101011001110110101101001/m3-110-BF/g
s/0110100100100000001010000110100101101001/m3-115-BF/g
s/0100100111111100001010001010110101111010/m3-139-BF/g
s/0110100100001000001010100110000100111101/m3-141-BF/g
s/0110110101100010001010000111100100101001/m3-163-BF/g
s/0110100100010000001011101110110111111001/m3-173-BF/g
s/0110100100001000001010000110100100101001/m3-183-BF/g
s/1111111110010000001011001110110101101001/m3-185-BF/g
s/0110100101101000101010010110100100111101/m3-209-BF/g
s/0100100101110010001010001011110111101000/m3-213-BF/g
s/1110110101110000001110101010110101101000/m3-215-BF/g
s/0111101111100100011010000110100100101001/m3-235-BF/g
s/0100101101110000101010001010110101101101/m3-237-BF/g
s/0110100101100000101011000111101100101001/m3-241-BF/g
'
}

strasbourg-bf-mapping(){
sed '
s/STRASBOURG/STRASBOURG/g
s/0110100101100000001010000111110100101011/m3-9-BF/g
s/0100110101110010101010001011110101101000/m3-23-BF/g
s/0110100101101000101010010110100100111101/m3-24-BF/g
s/1110111100000000001110000110010110111001/m3-32-BF/g
s/0110100100001000001010000110100100101001/m3-33-BF/g
s/0110100100010000001011101110110111111001/m3-35-BF/g
s/0111111100010000101011001110110101101001/m3-36-BF/g
s/1110110101110000001110101010110101101000/m3-37-BF/g
s/0110110101100010001010000111100100101001/m3-40-BF/g
s/0110100100001000001010100110000100111101/m3-49-BF/g
s/0111111110000000001010000111010100101001/m3-50-BF/g
s/0100100111111100001010001010110101111010/m3-53-BF/g
s/0110100100100000001010000110100101101001/m3-54-BF/g
s/0110100100010000001010001110110101101001/m3-55-BF/g
s/0110100100110000001010001110110101101001/m3-56-BF/g
s/1111111110010000001011001110110101101001/m3-57-BF/g
s/0110100101100000101011000111101100101001/m3-60-BF/g
s/0100101101110000101010001010110101101101/m3-61-BF/g
s/0111101111100100011010000110100100101001/m3-63-BF/g
s/0100100101110010001010001011110111101000/m3-64-BF/g
'
}



case $1 in
	"lille")
	lille-bf-mapping
	;;
	"grenoble")
	echo "to do grenoble"
	;;
	"strasbourg")
	strasbourg-bf-mapping
	;;
	*)
	echo "no platform specified"
	echo './bloom_mapping.sh <site>'
	exit 1
esac
