#include "mymalloc.c"
#include<time.h>
#include<sys/time.h>
int numMallocs = 0;
int numFrees = 0;

void partA(char *test[1000]);
void partB(char *test[1000]);
void partC(char *test[1000]);
void partD(char *test[1000]);
void PartE(char *test[1000]);
void PartF(char *test[1000]);
void endGrind(char *test[1000]);

void partA(char *test[1000]){
	struct timeval beginTime;
	struct timeval endTime;
	
	long average=0;
	int x = 0;
	for(x = 0; x < 100; x++){
		gettimeofday(&beginTime,0);
		int i=0;
		for(i=0;i<1000;i++){
			test[i] = (char*)malloc(sizeof(char));
			numMallocs++;
			// printf("numMallocs : %li\n",numMallocs++);
		}
		for(i=0;i<1000;i++){
			free(test[i]);
			numFrees++;
			// printf("numFrees: %li\n", numFrees++);
		}
		gettimeofday(&endTime,0);
		average += ((endTime.tv_sec-beginTime.tv_sec)*1000000 + endTime.tv_usec-beginTime.tv_usec);
	}
	printf("Grind A time: %ld microseconds\n",average/100); 
	endGrind(test);
	return;
}
void partB(char *test[1000]){
	struct timeval beginTime;
	struct timeval endTime;
	long average=0;
	int x = 0;
	
	for(x = 0; x < 100; x++){
		gettimeofday(&beginTime,0);
		int i=0;
		for(i=0;i<1000;i++){
			test[i]=(char*)malloc(sizeof(char));
			numMallocs++;
			free(test[i]);
			numFrees++;
		}
		gettimeofday(&endTime,0);
		average += ((endTime.tv_sec-beginTime.tv_sec)*1000000 + endTime.tv_usec-beginTime.tv_usec);
	}

	printf("Grind B time: %ld microseconds\n",average/100); 
	endGrind(test);
	return;
}
void partC(char *test[1000]){
	struct timeval beginTime;
	struct timeval endTime;

	long average=0;

	int k=0;
	time_t t;
	srand((unsigned) time(&t));
	int x = 0;
	
	for(x = 0; x < 100; x++){	
		gettimeofday(&beginTime,0);
		while(k<1000){
			int r =rand() %2;
			switch(r){
				case 0:
					test[k] = (char*)malloc(sizeof(char));
					k++;
					numMallocs++;
					break;
				case 1:
					if(k!=0){
						free(test[k-1]);
						numFrees++;
					}else{
						free(test[0]);
						numFrees++;
					}
					break;
				default:
					break;
			}
		}
		free(test);
		gettimeofday(&endTime,0);
		average += ((endTime.tv_sec-beginTime.tv_sec)*1000000 + endTime.tv_usec-beginTime.tv_usec);
	}
	
	printf("Grind C time: %ld microseconds\n",average/100); 
	endGrind(test);
	return;
}

void partD(char *test[1000]){
	struct timeval beginTime;
	struct timeval endTime;
	int x = 0;
	long average=0;
	time_t t;
	srand((unsigned) time(&t));
	for(x = 0; x < 100; x++){
		gettimeofday(&beginTime,0);
		int k=0;
		while(k<1000){
			int r=rand() %2;
			int bytes=0;
			switch(r){
				case 0:
					
					bytes = rand()%64;
					bytes+=1;
					test[k]=(char*)malloc(bytes*sizeof(char));
					k++;
					
					break;
				case 1:
					free(test[k-bytes]);
					break;
				default:
					break;
				}
		}
		gettimeofday(&endTime,0);
		average += ((endTime.tv_sec-beginTime.tv_sec)*1000000 + endTime.tv_usec-beginTime.tv_usec);
	}	

	printf("%ld\n",average/100); 
	return;
}
void partE(char *test[1000]){
	struct timeval beginTime;
	struct timeval endTime;
	int x=0;
	long average=0;
	time_t t;
	srand((unsigned) time(&t));
	
	for(x=0;x<100;x++){
		gettimeofday(&beginTime,0);
		int k=0;
		for(k = 0; k < 1000; k++){
			test[k] = (char *)malloc(4998 * sizeof(char));
			numMallocs++;
			free(test[k]);
			numFrees++;
		}
		gettimeofday(&endTime,0);
		average +=((endTime.tv_sec-beginTime.tv_sec)*1000000 + endTime.tv_usec-beginTime.tv_usec);
	}

	free(test);
	printf("Grind E time: %ld microseconds\n",average/100); 
	endGrind(test);
}	
//Part F prints out errors
void partF(char *test[1000]){
	struct timeval beginTime;
	struct timeval endTime;
	int x=0;
	long average=0;
	time_t t;
	srand((unsigned) time(&t));
	for(x=0;x<100;x++){
		gettimeofday(&beginTime,0);
		int k=0;
		test[x] = (char *)malloc(5005 * sizeof(char));
			// numMallocs++;
		free(test[x]);
			// numFrees++;
		
		gettimeofday(&endTime,0);
		average +=((endTime.tv_sec-beginTime.tv_sec)*1000000 + endTime.tv_usec-beginTime.tv_usec);
	}

	free(test);
	printf("Grind F time: %ld microseconds\n",average/100); 
	endGrind(test);
	printf("PartF is an error test case, there will be a lot of errors printed out; this is not our program crashing or working improperly, it is showing how our malloc wont allow over 5000 bytes to be allocated and shows how it doesn't explode\n");
}


int main(int argc, char** argv){
	char *test[1000];
	partA(test);
	partB(test);
	partC(test);
	//partD(test);
	partE(test);
	partF(test);
	return 0;
	}

void endGrind(char *test[1000]){
	printf("Number of mallocs: %d\n", numMallocs);
	printf("Number of frees: %d\n", numFrees);
	numFrees = 0;
	numMallocs = 0;
	free(test);
	printf("\n");
}
