#include<stdio.h>

int main(void)
{
	union u_tag{
		int ivalue;
		float fvalue;
		short svalue;
	}u;

	u.ivalue=45;
	printf("u.ivalue = %d \n",u.ivalue);

	u.fvalue=21.5;
	printf("u.fvalue = %f \n",u.fvalue);

	u.svalue=5;
	printf("u.svalue = %d \n",u.svalue);

	return(0);
}