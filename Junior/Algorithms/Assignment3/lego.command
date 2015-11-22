#!/bin/bash
cd `dirname $0`
#texts.tar
java LZW - <largezzz.txt >largezzz1.lzw 2>debug5.txt
java LZWmod - n <largezzz.txt >largezzz2.lzw 2>debug5.txt
java LZWmod - r <largezzz.txt >largezzz3.lzw 2>debug5.txt
java LZWmod - m <largezzz.txt >largezzz4.lzw 2>debug5.txt
cp largezzz.txt largezzz5.txt
compress largezzz5.txt
