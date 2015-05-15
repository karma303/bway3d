#include<linux/input.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include"kbd.h"
static int keys_fd;
static struct input_event t;
void open_event4(){
	keys_fd=open("/dev/input/event4",O_RDONLY|O_NONBLOCK);
	if(keys_fd==-1){
		printf("can not open /dev/input/event4\n");
		exit(-1);
	}
	else{
		printf("keyListener loaded successful..\n");
	}
}

void updatekbd(){
	while(read(keys_fd,&t,sizeof(t))==sizeof(t)){
		if(t.type==EV_KEY&&(t.value==0||t.value==1)){
			//if(t.value==KEY_PRESS) kbd[t.code]=KEY_PRESSED=1
			//if(t.value==KEY_RELEASE) kbd[t.code]=KEY_RELEASED=0;
			kbd[t.code]=t.value;
			printf("key:%d,%s\n",t.code,(t.value?"pressed":"released"));
		}
	}
}
