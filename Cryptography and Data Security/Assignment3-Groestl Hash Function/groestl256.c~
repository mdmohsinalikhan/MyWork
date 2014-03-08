#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "groestl256.h"

#define NUMBYTES 64

/*START:Variables declared by me */
unsigned char initial_state[8][8];
unsigned char block_content[8][8],block_content_q[8][8];
unsigned char round_constant[8][8];
/*END:Variables declared by me */



/* Multiplication by two in GF(2^8). Multiplication by three is xtime(a) ^ a */
#define xtime(a) ( ((a) & 0x80) ? (((a) << 1) ^ 0x1b) : ((a) << 1) )

/* The S-box table */
static const unsigned char sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
	0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
	0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
	0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
	0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
	0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
	0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
	0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
	0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
	0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
	0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
	0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
	0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
	0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
	0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
	0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
	0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static void groestl_pad(unsigned char **m_pad, unsigned long long *len_pad, const unsigned char *m, const unsigned long long n);

void P();
void Q();
void addroundconstantp();
void addroundconstantq();
void subbyte();
void subbyteq();
void shiftbytep();
void shiftbyteq();
void mixbyte();
void mixbyteq();
//unsigned char multiplication_in_2_8_field(unsigned char to_be_multipled,unsigned char multiplier);

/* Hash the message at m and store the 32-byte hash at h. The length of m in bytes is given at n. */
void groestl256(unsigned char *h, const unsigned char *m, unsigned long long n) {

	unsigned char *m_pad;					/* An array for the padded message */
	unsigned long long len_pad;			/* The length of the padded message (in 64-byte blocks) */

	/*START:Variables declared by me */
	int i,j,k,r,s;
	unsigned long var1,no_of_blocks;
	int my_array[100];
	/*END:Variables declared by me */

	groestl_pad(&m_pad,&len_pad,m,n);	/* Perform the message padding */

	
/*START: Preparing the initial vector */
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(i==6 && j== 7){initial_state[i][j]=0x01;}
			else{initial_state[i][j]=0x00;}
		}

	}

/*END: Preparing the initial vector *


/*START: Finding  the number of blocks */
	var1=n%64;

	if(var1 > 55) { var1 = n+(64-(n%64))+64;}
	else{var1 = n+(64-(n%64));}
	no_of_blocks = var1/64;

/*ENDS: Finding  the number of blocks */


	


/*START: Rounds */
	for(i=0;i<no_of_blocks;i++)
	{

		/*START: Initializing the block content */

			for(j=0;j<8;j++)
			{
				for(k=0;k<8;k++)
				{
					block_content[k][j]=m_pad[i*64+8*j+k];
					block_content_q[k][j]=m_pad[i*64+8*j+k];
				}
			}

		/*START: Initializing the block content */



		/*START: P */

				/*START: H+M */
				for(r=0;r<8;r++)
				{
					for(s=0;s<8;s++)
					{
						block_content[r][s]=initial_state[r][s] ^ block_content[r][s];
					}				
				}

			P(); // call the p function here P
				
		/*END: P */

		/*START: Q */

			Q(); // call the q function here Q
				
		/*END: P */

			
			for(r=0;r<8;r++)
			{
				for(s=0;s<8;s++)
				{
				block_content[r][s]=block_content_q[r][s] ^ block_content[r][s] ^ initial_state[r][s];
				}				
			}


		/*start: updating the chaining value*/
				for(r=0;r<8;r++)
				{
					for(s=0;s<8;s++)
					{
						initial_state[r][s] = block_content[r][s];
					}				
				}
		/*end: updating the chaining value*/


		


	
	}


			

/*END: Rounds */


			for(r=0;r<8;r++)
			{
				for(s=0;s<8;s++)
				{
				block_content_q[r][s]=block_content[r][s];
				}				
			}

P();


			for(r=0;r<8;r++)
			{
				for(s=0;s<8;s++)
				{
				block_content[r][s]=block_content[r][s] ^ block_content_q[r][s];
				}				
			}


			for(r=4;r<8;r++)
			{
				for(s=0;s<8;s++)
				{
				h[(r-4)*8+s]=block_content[s][r];
				}				
			}




	free(m_pad); 								/* Should be at the end. */

}

/* Performs the message padding. The original message and its length (in bytes) are given at
	m and n, respectively. Reserves memory for the	padded message and stores it at m_pad. 
	The length (in 512-bit blocks) is stored at len_pad. */
static void groestl_pad(unsigned char **m_pad, unsigned long long *len_pad, const unsigned char *m, const unsigned long long n) {

	/* Compute the length of the padded message (in 64-byte blocks) */
	*len_pad = (n*8 + 65 + (-n*8 - 65) % 512) / 512;	

	/* Allocate memory for the padded message */
	*m_pad = (unsigned char*) calloc(*len_pad*NUMBYTES, sizeof(unsigned char));

	/* Copy m to m_pad */
	memcpy(*m_pad, m, n);

	/* Add a bit to the end of the original message */
	(*m_pad)[n] = 0x80;

	/* Add the 64-bit representation of ((N+w+65)/512) in the end of m_pad */
	(*m_pad)[*len_pad*NUMBYTES-8] = (unsigned char) (*len_pad >> 56);
	(*m_pad)[*len_pad*NUMBYTES-7] = (unsigned char) (*len_pad >> 48);
	(*m_pad)[*len_pad*NUMBYTES-6] = (unsigned char) (*len_pad >> 40);
	(*m_pad)[*len_pad*NUMBYTES-5] = (unsigned char) (*len_pad >> 32);
	(*m_pad)[*len_pad*NUMBYTES-4] = (unsigned char) (*len_pad >> 24);
	(*m_pad)[*len_pad*NUMBYTES-3] = (unsigned char) (*len_pad >> 16);
	(*m_pad)[*len_pad*NUMBYTES-2] = (unsigned char) (*len_pad >> 8);
	(*m_pad)[*len_pad*NUMBYTES-1] = (unsigned char) (*len_pad);

}








void P()
{

int i,j;
//printf("\nFunction P has been called");	



		/*START: Initializing the Round Constant */
		for(i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			{
				if(i==0 && j==1){round_constant[i][j]=0x10;}
				//else if (i==0 && j==1){round_constant[i][j]=0x10;}
				else if (i==0 && j==2){round_constant[i][j]=0x20;}
				else if (i==0 && j==3){round_constant[i][j]=0x30;}
				else if (i==0 && j==4){round_constant[i][j]=0x40;}
				else if (i==0 && j==5){round_constant[i][j]=0x50;}
				else if (i==0 && j==6){round_constant[i][j]=0x60;}
				else if (i==0 && j==7){round_constant[i][j]=0x70;}
				else {round_constant[i][j]=0x00;}
			}
		}


		/*END: Initializing the Round Constant*/

			for(i=0;i<10;i++)
			{	
				/*START: Modifying the Round Constant */
				
				round_constant[0][0]=0x00 ^ i;
				round_constant[0][1]=0x10 ^ i;
				round_constant[0][2]=0x20 ^ i;
				round_constant[0][3]=0x30 ^ i;
				round_constant[0][4]=0x40 ^ i;
				round_constant[0][5]=0x50 ^ i;
				round_constant[0][6]=0x60 ^ i;
				round_constant[0][7]=0x70 ^ i;

				/*END: Modifying the round constant */

				addroundconstantp();
				subbyte();
				shiftbytep();
				mixbyte();
			}

	
}



void Q()
{

int i,j;


		/*START: Initializing the Round Constant */
		for(i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			{
				if(i==7 && j==1){round_constant[i][j]=0xef;}
				//else if (i==7 && j==1){round_constant[i][j]=0x10;}
				else if (i==7 && j==2){round_constant[i][j]=0xdf;}
				else if (i==7 && j==3){round_constant[i][j]=0xcf;}
				else if (i==7 && j==4){round_constant[i][j]=0xbf;}
				else if (i==7 && j==5){round_constant[i][j]=0xaf;}
				else if (i==7 && j==6){round_constant[i][j]=0x9f;}
				else if (i==7 && j==7){round_constant[i][j]=0x8f;}
				else {round_constant[i][j]=0xff;}
			}
		}

	
		/*END: Initializing the Round Constant*/

			for(i=0;i<10;i++)
			{	
				/*START: Modifying the Round Constant */
				
				round_constant[7][0]=0xff ^ i;
				round_constant[7][1]=0xef ^ i;
				round_constant[7][2]=0xdf ^ i;
				round_constant[7][3]=0xcf ^ i;
				round_constant[7][4]=0xbf ^ i;
				round_constant[7][5]=0xaf ^ i;
				round_constant[7][6]=0x9f ^ i;
				round_constant[7][7]=0x8f ^ i;

				/*END: Modifying the round constant */

				addroundconstantq();
				subbyteq();
				shiftbyteq();
				mixbyteq();
			}

	
}



void addroundconstantp()
{

int r,s,j,k;
			for(r=0;r<8;r++)
			{
				for(s=0;s<8;s++)
				{
				block_content[r][s]=round_constant[r][s] ^ block_content[r][s];
				}				
			}


}


void addroundconstantq()
{

int r,s,j,k;
			for(r=0;r<8;r++)
			{
				for(s=0;s<8;s++)
				{
				block_content_q[r][s]=round_constant[r][s] ^ block_content_q[r][s];
				}				
			}

}

void subbyte() {

int i,j,k;
int base = 0x10,row,column;

		for (i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			{
			row=block_content[i][j]/base;
			column=block_content[i][j]%base;
			block_content[i][j]=sbox[row*16+column];
			}
		}

}


void subbyteq() {

int i,j,k;
int base = 0x10,row,column;

///printf("Sub bytes function is called.\n");
		for (i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			{
			row=block_content_q[i][j]/base;
			column=block_content_q[i][j]%base;
			block_content_q[i][j]=sbox[row*16+column];
			}
		}

}




void shiftbytep() {

int i,j,k;
unsigned char temp,temp1,temp2,temp3,temp4,temp5,temp6;


			/*START:Shifting the second row */
				temp=block_content[1][0];
				block_content[1][0]=block_content[1][1];
				block_content[1][1]=block_content[1][2];
				block_content[1][2]=block_content[1][3];
				block_content[1][3]=block_content[1][4];
				block_content[1][4]=block_content[1][5];
				block_content[1][5]=block_content[1][6];
				block_content[1][6]=block_content[1][7];
				block_content[1][7]=temp;
			/*END:Shifting the second row */


			/*START:Shifting the third row */
				temp=block_content[2][0];
				temp1=block_content[2][1];
				block_content[2][0]=block_content[2][2];
				block_content[2][1]=block_content[2][3];
				block_content[2][2]=block_content[2][4];
				block_content[2][3]=block_content[2][5];
				block_content[2][4]=block_content[2][6];
				block_content[2][5]=block_content[2][7];
				block_content[2][6]=temp;
				block_content[2][7]=temp1;
			/*END:Shifting the third row */


			/*START:Shifting the fourth row */
				temp=block_content[3][0];
				temp1=block_content[3][1];
				temp2=block_content[3][2];
				block_content[3][0]=block_content[3][3];
				block_content[3][1]=block_content[3][4];
				block_content[3][2]=block_content[3][5];
				block_content[3][3]=block_content[3][6];
				block_content[3][4]=block_content[3][7];
				block_content[3][5]=temp;
				block_content[3][6]=temp1;
				block_content[3][7]=temp2;
			/*END:Shifting the fourth row */


			/*START:Shifting the fifth row */
				temp=block_content[4][0];
				temp1=block_content[4][1];
				temp2=block_content[4][2];
				temp3=block_content[4][3];
				block_content[4][0]=block_content[4][4];
				block_content[4][1]=block_content[4][5];
				block_content[4][2]=block_content[4][6];
				block_content[4][3]=block_content[4][7];
				block_content[4][4]=temp;
				block_content[4][5]=temp1;
				block_content[4][6]=temp2;
				block_content[4][7]=temp3;
			/*END:Shifting the fifth row */


			/*START:Shifting the sixth row */
				temp=block_content[5][0];
				temp1=block_content[5][1];
				temp2=block_content[5][2];
				temp3=block_content[5][3];
				temp4=block_content[5][4];
				block_content[5][0]=block_content[5][5];
				block_content[5][1]=block_content[5][6];
				block_content[5][2]=block_content[5][7];
				block_content[5][3]=temp;
				block_content[5][4]=temp1;
				block_content[5][5]=temp2;
				block_content[5][6]=temp3;
				block_content[5][7]=temp4;
			/*END:Shifting the sixth row */


			/*START:Shifting the seventh row */
				temp=block_content[6][0];
				temp1=block_content[6][1];
				temp2=block_content[6][2];
				temp3=block_content[6][3];
				temp4=block_content[6][4];
				temp5=block_content[6][5];
				block_content[6][0]=block_content[6][6];
				block_content[6][1]=block_content[6][7];
				block_content[6][2]=temp;
				block_content[6][3]=temp1;
				block_content[6][4]=temp2;
				block_content[6][5]=temp3;
				block_content[6][6]=temp4;
				block_content[6][7]=temp5;
			/*END:Shifting the seventh row */


			/*START:Shifting the eighth row */
				temp=block_content[7][0];
				temp1=block_content[7][1];
				temp2=block_content[7][2];
				temp3=block_content[7][3];
				temp4=block_content[7][4];
				temp5=block_content[7][5];
				temp6=block_content[7][6];
				block_content[7][0]=block_content[7][7];
				block_content[7][1]=temp;
				block_content[7][2]=temp1;
				block_content[7][3]=temp2;
				block_content[7][4]=temp3;
				block_content[7][5]=temp4;
				block_content[7][6]=temp5;
				block_content[7][7]=temp6;
			/*END:Shifting the eighth row */

}


void shiftbyteq() {

int i,j,k;
unsigned char temp,temp1,temp2,temp3,temp4,temp5,temp6;

			
			/*START:Shifting the second row */
				temp=block_content_q[0][0];
				block_content_q[0][0]=block_content_q[0][1];
				block_content_q[0][1]=block_content_q[0][2];
				block_content_q[0][2]=block_content_q[0][3];
				block_content_q[0][3]=block_content_q[0][4];
				block_content_q[0][4]=block_content_q[0][5];
				block_content_q[0][5]=block_content_q[0][6];
				block_content_q[0][6]=block_content_q[0][7];
				block_content_q[0][7]=temp;
			/*END:Shifting the second row */


			/*START:Shifting the second row */
				temp=block_content_q[1][0];
				temp1=block_content_q[1][1];
				temp2=block_content_q[1][2];
				block_content_q[1][0]=block_content_q[1][3];
				block_content_q[1][1]=block_content_q[1][4];
				block_content_q[1][2]=block_content_q[1][5];
				block_content_q[1][3]=block_content_q[1][6];
				block_content_q[1][4]=block_content_q[1][7];
				block_content_q[1][5]=temp;
				block_content_q[1][6]=temp1;
				block_content_q[1][7]=temp2;
			/*END:Shifting the second row */


			/*START:Shifting the third row */
				temp=block_content_q[2][0];
				temp1=block_content_q[2][1];
				temp2=block_content_q[2][2];
				temp3=block_content_q[2][3];
				temp4=block_content_q[2][4];
				block_content_q[2][0]=block_content_q[2][5];
				block_content_q[2][1]=block_content_q[2][6];
				block_content_q[2][2]=block_content_q[2][7];
				block_content_q[2][3]=temp;
				block_content_q[2][4]=temp1;
				block_content_q[2][5]=temp2;
				block_content_q[2][6]=temp3;
				block_content_q[2][7]=temp4;
			/*END:Shifting the third row */


			/*START:Shifting the fourth row */
				temp=block_content_q[3][0];
				temp1=block_content_q[3][1];
				temp2=block_content_q[3][2];
				temp3=block_content_q[3][3];
				temp4=block_content_q[3][4];
				temp5=block_content_q[3][5];
				temp6=block_content_q[3][6];
				block_content_q[3][0]=block_content_q[3][7];
				block_content_q[3][1]=temp;
				block_content_q[3][2]=temp1;
				block_content_q[3][3]=temp2;
				block_content_q[3][4]=temp3;
				block_content_q[3][5]=temp4;
				block_content_q[3][6]=temp5;
				block_content_q[3][7]=temp6;
			/*END:Shifting the fourth row */



			/*START:Shifting the sixth row */
				temp=block_content_q[5][0];
				temp1=block_content_q[5][1];
				block_content_q[5][0]=block_content_q[5][2];
				block_content_q[5][1]=block_content_q[5][3];
				block_content_q[5][2]=block_content_q[5][4];
				block_content_q[5][3]=block_content_q[5][5];
				block_content_q[5][4]=block_content_q[5][6];
				block_content_q[5][5]=block_content_q[5][7];
				block_content_q[5][6]=temp;
				block_content_q[5][7]=temp1;
			/*END:Shifting the sixth row */


			/*START:Shifting the seventh row */
				temp=block_content_q[6][0];
				temp1=block_content_q[6][1];
				temp2=block_content_q[6][2];
				temp3=block_content_q[6][3];
				block_content_q[6][0]=block_content_q[6][4];
				block_content_q[6][1]=block_content_q[6][5];
				block_content_q[6][2]=block_content_q[6][6];
				block_content_q[6][3]=block_content_q[6][7];
				block_content_q[6][4]=temp;
				block_content_q[6][5]=temp1;
				block_content_q[6][6]=temp2;
				block_content_q[6][7]=temp3;
			/*END:Shifting the seventh row */


			/*START:Shifting the eighth row */
				temp=block_content_q[7][0];
				temp1=block_content_q[7][1];
				temp2=block_content_q[7][2];
				temp3=block_content_q[7][3];
				temp4=block_content_q[7][4];
				temp5=block_content_q[7][5];
				block_content_q[7][0]=block_content_q[7][6];
				block_content_q[7][1]=block_content_q[7][7];
				block_content_q[7][2]=temp;
				block_content_q[7][3]=temp1;
				block_content_q[7][4]=temp2;
				block_content_q[7][5]=temp3;
				block_content_q[7][6]=temp4;
				block_content_q[7][7]=temp5;
			/*END:Shifting the eighth row */


}

void mixbyte()
{

int j,k;
unsigned char temp_block_content[8][8];

		

			for(j=0;j<8;j++)		
			{

				temp_block_content[0][j]	= 	xtime(block_content[0][j]) ^
									xtime(block_content[1][j]) ^
									xtime(block_content[2][j])^block_content[2][j] ^
									xtime(xtime(block_content[3][j])) ^
									xtime(xtime(block_content[4][j]))^block_content[4][j] ^
									xtime(block_content[5][j])^block_content[5][j] ^
									xtime(xtime(block_content[6][j]))^block_content[6][j] ^
									xtime(xtime(block_content[7][j])^block_content[7][j])^block_content[7][j];

				temp_block_content[1][j]	= 	xtime(xtime(block_content[0][j])^block_content[0][j])^block_content[0][j] ^
									xtime(block_content[1][j]) ^
									xtime(block_content[2][j]) ^
									xtime(block_content[3][j])^block_content[3][j] ^
									xtime(xtime(block_content[4][j])) ^
									xtime(xtime(block_content[5][j]))^block_content[5][j] ^
									xtime(block_content[6][j])^block_content[6][j] ^
									xtime(xtime(block_content[7][j]))^block_content[7][j];

				temp_block_content[2][j]	= 	xtime(xtime(block_content[0][j]))^block_content[0][j] ^
									xtime(xtime(block_content[1][j])^block_content[1][j])^block_content[1][j] ^
									xtime(block_content[2][j]) ^
									xtime(block_content[3][j]) ^
									xtime(block_content[4][j])^block_content[4][j] ^
									xtime(xtime(block_content[5][j])) ^
									xtime(xtime(block_content[6][j]))^block_content[6][j] ^
									xtime(block_content[7][j])^block_content[7][j];


				temp_block_content[3][j]	= 	xtime(block_content[0][j])^block_content[0][j] ^
									xtime(xtime(block_content[1][j]))^block_content[1][j] ^
									xtime(xtime(block_content[2][j])^block_content[2][j])^block_content[2][j] ^
									xtime(block_content[3][j]) ^
									xtime(block_content[4][j]) ^
									xtime(block_content[5][j])^block_content[5][j] ^
									xtime(xtime(block_content[6][j])) ^
									xtime(xtime(block_content[7][j]))^block_content[7][j];


				temp_block_content[4][j]	= 	xtime(xtime(block_content[0][j]))^block_content[0][j] ^ 
									xtime(block_content[1][j])^block_content[1][j] ^
									xtime(xtime(block_content[2][j]))^block_content[2][j] ^
									xtime(xtime(block_content[3][j])^block_content[3][j])^block_content[3][j] ^
									xtime(block_content[4][j]) ^
									xtime(block_content[5][j]) ^
									xtime(block_content[6][j])^block_content[6][j] ^
									xtime(xtime(block_content[7][j]));

				temp_block_content[5][j]	= 	xtime(xtime(block_content[0][j])) ^
									xtime(xtime(block_content[1][j]))^block_content[1][j] ^ 
									xtime(block_content[2][j])^block_content[2][j] ^
									xtime(xtime(block_content[3][j]))^block_content[3][j] ^
									xtime(xtime(block_content[4][j])^block_content[4][j])^block_content[4][j] ^
									xtime(block_content[5][j]) ^
									xtime(block_content[6][j]) ^
									xtime(block_content[7][j])^block_content[7][j];

				temp_block_content[6][j]	= 	xtime(block_content[0][j])^block_content[0][j] ^
									xtime(xtime(block_content[1][j])) ^
									xtime(xtime(block_content[2][j]))^block_content[2][j] ^ 
									xtime(block_content[3][j])^block_content[3][j] ^
									xtime(xtime(block_content[4][j]))^block_content[4][j] ^
									xtime(xtime(block_content[5][j])^block_content[5][j])^block_content[5][j] ^
									xtime(block_content[6][j]) ^
									xtime(block_content[7][j]);


				temp_block_content[7][j]	= 	xtime(block_content[0][j]) ^
									xtime(block_content[1][j])^block_content[1][j] ^
									xtime(xtime(block_content[2][j])) ^
									xtime(xtime(block_content[3][j]))^block_content[3][j] ^ 
									xtime(block_content[4][j])^block_content[4][j] ^
									xtime(xtime(block_content[5][j]))^block_content[5][j] ^
									xtime(xtime(block_content[6][j])^block_content[6][j])^block_content[6][j] ^
									xtime(block_content[7][j]);


					block_content[0][j]=temp_block_content[0][j];
					block_content[1][j]=temp_block_content[1][j];
					block_content[2][j]=temp_block_content[2][j];
					block_content[3][j]=temp_block_content[3][j];
					block_content[4][j]=temp_block_content[4][j];
					block_content[5][j]=temp_block_content[5][j];
					block_content[6][j]=temp_block_content[6][j];
					block_content[7][j]=temp_block_content[7][j];


			}


}




void mixbyteq()
{

int j,k;
unsigned char temp_block_content[8][8];


			for(j=0;j<8;j++)		
			{

				temp_block_content[0][j]	= 	xtime(block_content_q[0][j]) ^
									xtime(block_content_q[1][j]) ^
									xtime(block_content_q[2][j])^block_content_q[2][j] ^
									xtime(xtime(block_content_q[3][j])) ^
									xtime(xtime(block_content_q[4][j]))^block_content_q[4][j] ^
									xtime(block_content_q[5][j])^block_content_q[5][j] ^
									xtime(xtime(block_content_q[6][j]))^block_content_q[6][j] ^
									xtime(xtime(block_content_q[7][j])^block_content_q[7][j])^block_content_q[7][j];

				temp_block_content[1][j]	= 	xtime(xtime(block_content_q[0][j])^block_content_q[0][j])^block_content_q[0][j] ^
									xtime(block_content_q[1][j]) ^
									xtime(block_content_q[2][j]) ^
									xtime(block_content_q[3][j])^block_content_q[3][j] ^
									xtime(xtime(block_content_q[4][j])) ^
									xtime(xtime(block_content_q[5][j]))^block_content_q[5][j] ^
									xtime(block_content_q[6][j])^block_content_q[6][j] ^
									xtime(xtime(block_content_q[7][j]))^block_content_q[7][j];

				temp_block_content[2][j]	= 	xtime(xtime(block_content_q[0][j]))^block_content_q[0][j] ^
									xtime(xtime(block_content_q[1][j])^block_content_q[1][j])^block_content_q[1][j] ^
									xtime(block_content_q[2][j]) ^
									xtime(block_content_q[3][j]) ^
									xtime(block_content_q[4][j])^block_content_q[4][j] ^
									xtime(xtime(block_content_q[5][j])) ^
									xtime(xtime(block_content_q[6][j]))^block_content_q[6][j] ^
									xtime(block_content_q[7][j])^block_content_q[7][j];


				temp_block_content[3][j]	= 	xtime(block_content_q[0][j])^block_content_q[0][j] ^
									xtime(xtime(block_content_q[1][j]))^block_content_q[1][j] ^
									xtime(xtime(block_content_q[2][j])^block_content_q[2][j])^block_content_q[2][j] ^
									xtime(block_content_q[3][j]) ^
									xtime(block_content_q[4][j]) ^
									xtime(block_content_q[5][j])^block_content_q[5][j] ^
									xtime(xtime(block_content_q[6][j])) ^
									xtime(xtime(block_content_q[7][j]))^block_content_q[7][j];


				temp_block_content[4][j]	= 	xtime(xtime(block_content_q[0][j]))^block_content_q[0][j] ^ 
									xtime(block_content_q[1][j])^block_content_q[1][j] ^
									xtime(xtime(block_content_q[2][j]))^block_content_q[2][j] ^
									xtime(xtime(block_content_q[3][j])^block_content_q[3][j])^block_content_q[3][j] ^
									xtime(block_content_q[4][j]) ^
									xtime(block_content_q[5][j]) ^
									xtime(block_content_q[6][j])^block_content_q[6][j] ^
									xtime(xtime(block_content_q[7][j]));

				temp_block_content[5][j]	= 	xtime(xtime(block_content_q[0][j])) ^
									xtime(xtime(block_content_q[1][j]))^block_content_q[1][j] ^ 
									xtime(block_content_q[2][j])^block_content_q[2][j] ^
									xtime(xtime(block_content_q[3][j]))^block_content_q[3][j] ^
									xtime(xtime(block_content_q[4][j])^block_content_q[4][j])^block_content_q[4][j] ^
									xtime(block_content_q[5][j]) ^
									xtime(block_content_q[6][j]) ^
									xtime(block_content_q[7][j])^block_content_q[7][j];

				temp_block_content[6][j]	= 	xtime(block_content_q[0][j])^block_content_q[0][j] ^
									xtime(xtime(block_content_q[1][j])) ^
									xtime(xtime(block_content_q[2][j]))^block_content_q[2][j] ^ 
									xtime(block_content_q[3][j])^block_content_q[3][j] ^
									xtime(xtime(block_content_q[4][j]))^block_content_q[4][j] ^
									xtime(xtime(block_content_q[5][j])^block_content_q[5][j])^block_content_q[5][j] ^
									xtime(block_content_q[6][j]) ^
									xtime(block_content_q[7][j]);


				temp_block_content[7][j]	= 	xtime(block_content_q[0][j]) ^
									xtime(block_content_q[1][j])^block_content_q[1][j] ^
									xtime(xtime(block_content_q[2][j])) ^
									xtime(xtime(block_content_q[3][j]))^block_content_q[3][j] ^ 
									xtime(block_content_q[4][j])^block_content_q[4][j] ^
									xtime(xtime(block_content_q[5][j]))^block_content_q[5][j] ^
									xtime(xtime(block_content_q[6][j])^block_content_q[6][j])^block_content_q[6][j] ^
									xtime(block_content_q[7][j]);


					block_content_q[0][j]=temp_block_content[0][j];
					block_content_q[1][j]=temp_block_content[1][j];
					block_content_q[2][j]=temp_block_content[2][j];
					block_content_q[3][j]=temp_block_content[3][j];
					block_content_q[4][j]=temp_block_content[4][j];
					block_content_q[5][j]=temp_block_content[5][j];
					block_content_q[6][j]=temp_block_content[6][j];
					block_content_q[7][j]=temp_block_content[7][j];


			}


}
