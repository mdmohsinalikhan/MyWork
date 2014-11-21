#include<stdio.h>


int main(int argc,char *argv[])
{
int a,b,c,p,ri_2,ri_1,ri_0,qi_0,xi_2,xi_1,xi_0,yi_2,yi_1,yi_0,i;
//printf("Please enter your number a:\n");
//scanf("%d",&a);


//printf("Please enter your number b:\n");
//scanf("%d",&b);


//printf("Please enter your number p:\n");
//scanf("%d",&p);


sscanf(argv[1], "%d", &a);
sscanf(argv[2], "%d", &b);
sscanf(argv[3], "%d", &p);

//a = argv[2];
//b = *argv[3];
//p = *argv[4];

//printf("Here are the values: a = %d,	b = %d,	p=%d",a,b,p);

//printf("\nEuclid's Extended Algorithm:\n\n");

//printf("i	q	r	x	y	\n");
//printf("========================================\n");
//printf("0		%d	1	0	\n",p);
//printf("1		%d	0	1	\n",b);

ri_2 = p;
ri_1 = b;

xi_2 = 1;
xi_1 = 0;

yi_2 = 0;
yi_1 = 1;


for(i=0;i>=0;i++)
{

ri_0 = ri_2%ri_1;
qi_0 = ri_2/ri_1;

ri_0 = ri_2%ri_1;
xi_0 = xi_2 - xi_1*qi_0;
yi_0 = yi_2 - yi_1*qi_0;

//printf("%d	%d	%d	%d	%d	\n",i+2,qi_0,ri_0,xi_0,yi_0);




	if(ri_0 == 0 && ri_1 == 1)
	{
		c = (yi_1*a)%p;

		if(c < 0)
			c = p + c;

		printf("%d / %d mod %d = %d\n",a,b,p,c);
		
		break;
	}
	else if(ri_0 == 0 && ri_1 != 1)
	{
		
	}


ri_2 = ri_1;
ri_1 = ri_0;

xi_2 = xi_1;
xi_1 = xi_0;

yi_2 = yi_1;
yi_1 = yi_0;


}



return 0;
}
