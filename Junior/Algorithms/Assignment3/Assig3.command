#!/bin/bash
cd `dirname $0`
javac LZWmod.java
javac LZW.java
#large.txt
java LZW - <large.txt >large1.lzw 2>debug5.txt
java LZWmod - n <large.txt >large2.lzw 2>debug5.txt
java LZWmod - r <large.txt >large3.lzw 2>debug5.txt
java LZWmod - m <large.txt >large4.lzw 2>debug5.txt
cp large.txt large5.txt
compress large5.txt
#winnt256.bmp
java LZW - <winnt256.bmp >winnt2561.lzw 2>debug5.txt
java LZWmod - n <winnt256.bmp >winnt2562.lzw 2>debug5.txt
java LZWmod - r <winnt256.bmp >winnt2563.lzw 2>debug5.txt
java LZWmod - m <winnt256.bmp >winnt2564.lzw 2>debug5.txt
cp winnt256.bmp winnt2565.bmp
compress winnt2565.bmp
#wacky.bmp
java LZW - <wacky.bmp >wacky1.lzw 2>debug5.txt
java LZWmod - n <wacky.bmp >wacky2.lzw 2>debug5.txt
java LZWmod - r <wacky.bmp >wacky3.lzw 2>debug5.txt
java LZWmod - m <wacky.bmp >wacky4.lzw 2>debug5.txt
cp wacky.bmp wacky5.bmp
compress wacky5.bmp
#texts.tar
java LZW - <texts.tar >texts1.lzw 2>debug5.txt
java LZWmod - n <texts.tar >texts2.lzw 2>debug5.txt
java LZWmod - r <texts.tar >texts3.lzw 2>debug5.txt
java LZWmod - m <texts.tar >texts4.lzw 2>debug5.txt
cp texts.tar texts5.tar
compress texts5.tar
#medium.txt
java LZW - <medium.txt >medium1.lzw 2>debug5.txt
java LZWmod - n <medium.txt >medium2.lzw 2>debug5.txt
java LZWmod - r <medium.txt >medium3.lzw 2>debug5.txt
java LZWmod - m <medium.txt >medium4.lzw 2>debug5.txt
cp medium.txt medium5.txt
compress medium5.txt
#gone_fishing.bmp
java LZW - <gone_fishing.bmp >gone_fishing1.lzw 2>debug5.txt
java LZWmod - n <gone_fishing.bmp >gone_fishing2.lzw 2>debug5.txt
java LZWmod - r <gone_fishing.bmp >gone_fishing3.lzw 2>debug5.txt
java LZWmod - m <gone_fishing.bmp >gone_fishing4.lzw 2>debug5.txt
cp gone_fishing.bmp gone_fishing5.bmp
compress gone_fishing5.bmp
#frosty.jpg
java LZW - <frosty.jpg >frosty1.lzw 2>debug5.txt
java LZWmod - n <frosty.jpg >frosty2.lzw 2>debug5.txt
java LZWmod - r <frosty.jpg >frosty3.lzw 2>debug5.txt
java LZWmod - m <frosty.jpg >frosty4.lzw 2>debug5.txt
cp frosty.jpg frosty5.jpg
compress frosty5.jpg
#edit.exe
java LZW - <edit.exe >edit1.lzw 2>debug5.txt
java LZWmod - n <edit.exe >edit2.lzw 2>debug5.txt
java LZWmod - r <edit.exe >edit3.lzw 2>debug5.txt
java LZWmod - m <edit.exe >edit4.lzw 2>debug5.txt
cp edit.exe edit5.exe
compress edit5.exe
#code2.txt
java LZW - <code2.txt >code21.lzw 2>debug5.txt
java LZWmod - n <code2.txt >code22.lzw 2>debug5.txt
java LZWmod - r <code2.txt >code23.lzw 2>debug5.txt
java LZWmod - m <code2.txt >code24.lzw 2>debug5.txt
cp code2.txt code25.txt
compress code25.txt
#code.txt
java LZW - <code.txt >code1.lzw 2>debug5.txt
java LZWmod - n <code.txt >code2.lzw 2>debug5.txt
java LZWmod - r <code.txt >code3.lzw 2>debug5.txt
java LZWmod - m <code.txt >code4.lzw 2>debug5.txt
cp code.txt code5.txt
compress code5.txt
#bmps.tar
java LZW - <bmps.tar >bmps1.lzw 2>debug5.txt
java LZWmod - n <bmps.tar >bmps2.lzw 2>debug5.txt
java LZWmod - r <bmps.tar >bmps3.lzw 2>debug5.txt
java LZWmod - m <bmps.tar >bmps4.lzw 2>debug5.txt
cp bmps.tar bmps5.tar
compress bmps5.tar
#assig2.doc
java LZW - <assig2.doc >assig21.lzw 2>debug5.txt
java LZWmod - n <assig2.doc >assig22.lzw 2>debug5.txt
java LZWmod - r <assig2.doc >assig23.lzw 2>debug5.txt
java LZWmod - m <assig2.doc >assig24.lzw 2>debug5.txt
cp assig2.doc assig25.doc
compress assig25.doc
#all.tar
java LZW - <all.tar >all1.lzw 2>debug5.txt
java LZWmod - n <all.tar >all2.lzw 2>debug5.txt
java LZWmod - r <all.tar >all3.lzw 2>debug5.txt
java LZWmod - m <all.tar >all4.lzw 2>debug5.txt
cp all.tar all5.tar
compress all5.tar
#LZWmod.java
java LZW - <LZWmod.java >LZWmod1.lzw 2>debug5.txt
java LZWmod - n <LZWmod.java >LZWmod2.lzw 2>debug5.txt
java LZWmod - r <LZWmod.java >LZWmod3.lzw 2>debug5.txt
java LZWmod - m <LZWmod.java >LZWmod4.lzw 2>debug5.txt
cp LZWmod.java LZWmod5.java
compress LZWmod5.java
#LZWmod.class
java LZW - <LZWmod.class >LZWmodclass1.lzw 2>debug5.txt
java LZWmod - n <LZWmod.class >LZWmodclass2.lzw 2>debug5.txt
java LZWmod - r <LZWmod.class >LZWmodclass3.lzw 2>debug5.txt
java LZWmod - m <LZWmod.class >LZWmodclass4.lzw 2>debug5.txt
cp LZWmod.class LZWmodclass5.class
compress LZWmodclass5.class
