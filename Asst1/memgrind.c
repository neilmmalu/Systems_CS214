#include "mymalloc.c"
#include<time.h>
void partA(char *test[1000]);
void partB(char *test[1000]);
void partC(char *test[1000]);
void partD(char *test[1000]);

void partA(char *test[1000]){
	struct timeval tv1;
	struct timeval tv2;
	gettimeofday(&tv1,0);
	int i=0;
	for(i=0;i<1000;i++){
		test[i] = (char*)malloc(sizeof(char));
	}
	for(i=0;i<1000;i++){
	//	free(test[i]);
	}
	long average=0;
	gettimeofday(&tv2,0);
	average += ((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
	printf("%ld\n",average/100); 
	return;
}
void partB(char *test[1000]){
	int i=0;
	for(i=0;i<1000;i++){
		test[i]=(char*)malloc(sizeof(char));
	//	free(test[i]);
	}
	return;
}
void partC(char *test[1000]){
	int k=0;
	while(k<1000){
		int r =rand() %2;
		switch(r){
			case 0:
				test[k] = (char*)malloc(sizeof(char));
				k++;
				break;
			case 1:
				if(k!=0){
			//	free(test[k-1]);
				}else{
				//freeing first empty block (error)
				}
				break;
			default:
				break;
			}
		}
//	free(test);
	return;
}
void partD(char *test[1000]){
	int k=0;
	while(k<1000){
		int r=rand() %2;
		int bytes;
		switch(r){
			case 0:
				
				bytes = rand()%64;
				bytes+=1;
				k+=bytes;
				break;
			case 1:
				//free first pointer it finds
				break;
			default:
				break;
			}
	}
//	free(test);
	return;
}
int main(int argc, char** argv){
	char *test[1000];
	partA(test);
	partB(test);
	partC(test);
	partD(test);

	return 0;
	}

