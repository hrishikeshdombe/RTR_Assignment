#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>

int main(void)
{
	int number,result;
	typedef int(*Square)(int);
	Square sqr;
	
	void *HANDLE=dlopen("./Server.so",RTLD_LAZY);
	if(!HANDLE)
	{
		printf("Cannot open DLL");
		exit(EXIT_FAILURE);
	}

	sqr=(Square)dlsym(HANDLE,"Square");

	printf("**********************************************\n");
	printf("Please Enter the Number for Squaring : ");
	scanf("%d",&number);

	result=sqr(number);

	printf("Square of %d is %d\n",number,result);
	printf("**********************************************\n");

	dlclose(HANDLE);

	return(0);
}