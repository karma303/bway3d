//the main-ill is funcion-args' passing,for temporary,only empty-args function can be called
#include<run.h>
#include<stdio.h>
void run(void(*pt_func)(),int call_count,int judge_count){
	printf("run start-----------\ncall_count:%d",call_count);
	int total=0;
	int cost;
	for(int i=0;i<judge_count;i++){
		gettimeofday(&start,NULL);	
		for(int j=0;j<call_count;j++){
			pt_func();
		}
		gettimeofday(&end,NULL);
		cost=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
		total+=cost;
		printf("%dth,timeCost:%d\n",i,cost);
	}
	printf("average:%d\n",total/judge_count);
	printf("run exit-----------\n");
}
