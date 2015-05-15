PATH_H=-I/home/wws/lab/include/
PATH_O=-L/usr/lib/i386-linux-gnu/
cc=gcc
asm=nasm
asmflags=-g -F stabs
cflags=-g  -Wall
ld_liba=-lpthread
all:tt
tt:t.o draw.o run.o trans_bitBlock.o kbd.o ../lib/matrix/MMmul.o bmath.o scene.o mesh.o camera.o renderlist.o init.o pipeline.o face.o
	$(cc) -o tt $(PATH_O) t.o draw.o run.o trans_bitBlock.o kbd.o ../lib/matrix/MMmul.o bmath.o scene.o mesh.o camera.o renderlist.o init.o pipeline.o face.o -g $(ld_liba) -lm
#t.o rely on run.h
../lib/matrix/MMmul.o:../lib/matrix/MMmul.c ../lib/matrix/matrix.h 
	$(cc) -c -o ../lib/matrix/MMmul.o ../lib/matrix/MMmul.c $(PATH_H) $(cflags) -std=gnu99
t.o:t.c t.h /home/wws/lab/include/run.h bmath.h ../lib/matrix/matrix.h
	$(cc) -c t.c $(PATH_H) $(cflags) -std=gnu99
mesh.o:bmath.h mesh.h  face.h mesh.c
	$(cc)  -c mesh.c $(PATH_H) $(cflags) -std=gnu99
scene.o:mesh.h scene.h bmath.h scene.c
	$(cc) -c scene.c $(PATH_H) $(cflags) -std=gnu99
bmath.o:bmath.h bmath.c
	$(cc) -c bmath.c $(PATH_H) $(cflags) -std=gnu99
camera.o:camera.h bmath.h camera.c
	$(cc) -c camera.c $(PATH_H) $(cflags) -std=gnu99
renderlist.o:renderlist.h bmath.h renderlist.c
	$(cc) -c renderlist.c $(PATH_H) $(cflags) -std=gnu99
init.o:init.h camera.h scene.h renderlist.h init.c
	$(cc) -c init.c $(PATH_H) $(cflags) -std=gnu99
pipeline.o:pipeline.h camera.h scene.h renderlist.h pipeline.c 
	$(cc) -c pipeline.c $(PATH_H) $(cflags) -std=gnu99
draw.o:draw.h draw.c 
	$(cc) -c -O2 draw.c $(PATH_H) $(cflags) -std=gnu99 
face.o:face.h bmath.h face.c
	$(cc) -c face.c $(PATH_H) $(cflags) -std=gnu99
run.o:/home/wws/lab/include/run.h run.c
	$(cc) -c run.c $(PATH_H) $(cflags) -std=gnu99
trans_bitBlock.o:trans_bitBlock.asm
	$(asm) -f elf -o trans_bitBlock.o trans_bitBlock.asm $(asmflags)
kbd.o:kbd.h kbd.c
	$(cc) -c kbd.c $(PATH_H) $(cflags) -std=gnu99
