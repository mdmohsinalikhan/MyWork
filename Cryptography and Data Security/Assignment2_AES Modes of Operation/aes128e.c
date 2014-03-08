#include <stdint.h>
#include <stdio.h>
#include "aes128e.h"

/* Multiplication by two in GF(2^8). Multiplication by three is xtime(a) ^ a */
#define xtime(a) ( ((a) & 0x80) ? (((a) << 1) ^ 0x1b) : ((a) << 1) )
#define Nk 4
#define Nr 10
#define Nb 4


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

/* The round constant table (needed in KeyExpansion) */
static const unsigned char rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 
    0x20, 0x40, 0x80, 0x1b, 0x36 };
	
/* The state Matrix and Key Schedule Matrix */
unsigned char state[4][4];
unsigned char w[Nb][Nb*(Nr+1)];

/* Under the 16-byte key at k, encrypt the 16-byte plaintext at p and store it at c. */
void aes128e(unsigned char *c, const unsigned char *p, const unsigned char *k) {

int i,j;

/** Testing segment **/
/** Testing segment ENDS**/

/* State matrix preparation is being started */
	for(i=0;i<16;i++){
		state[i%Nb][i/Nb]=p[i];
	}

	///printf("Below is the input text/plain text\n");
	for(i=0;i<Nb;i++){
		for(j=0;j<Nb;j++){
		///printf("%x ",state[i][j]);	
		}
	///printf("\n");
	}
/* State matrix preparation ENDs */

keyExpansion(k);	
addroundkey(0);


//c[0]=0x39; c[1]=0x25; c[2]=0x84; c[3]=0x1d; c[4]=0x02; c[5]=0xdc; c[6]=0x09; c[7]=0xfb; c[8]=0xdc;
//c[9]=0x11; c[10]=0x85; c[11]=0x97; c[12]=0x19; c[13]=0x6a; c[14]=0x0b; c[15]=0x32; 
	
	
	for(i=1;i<Nr;i++){
	///printf("\n\nRound: %d\n",i);
	subbytes();
	shiftrows();
	mixcolumns();
	addroundkey(i);
	}
		
	subbytes();
	shiftrows();
	addroundkey(Nr);
	
	
	c[0]=state[0][0]; c[4]=state[0][1]; c[8]=state[0][2]; c[12]=state[0][3];
	c[1]=state[1][0]; c[5]=state[1][1]; c[9]=state[1][2]; c[13]=state[1][3];
	c[2]=state[2][0]; c[6]=state[2][1]; c[10]=state[2][2]; c[14]=state[2][3];
	c[3]=state[3][0]; c[7]=state[3][1]; c[11]=state[3][2]; c[15]=state[3][3];
	
	
	
	for(i=0;i<16;i++)
	{
	///printf("%x ",c[i]);
	}
	
}

void addroundkey (int rounds) {
int i,j;
///printf("Add round key function is called for the %d.\n",rounds);

	for(i=0;i<Nb;i++)
	{
		for(j=0;j<Nb;j++){
		state[i][j]=state[i][j]^w[i][Nb*rounds+j];
		///printf("%x ",state[i][j]);
		}
	///printf("\n");
	} 
}


void subbytes() {

int i,j;
int base = 0x10,row,column;

///printf("Sub bytes function is called.\n");
		for (i=0;i<Nb;i++)
		{
			for(j=0;j<Nb;j++)
			{
			row=state[i][j]/base;
			column=state[i][j]%base;
			state[i][j]=sbox[row*16+column];
			///printf("%x ",state[i][j]);
			}
		//printf ("\n");
		}
}


void shiftrows() {
int i,j,for_swap,for_swap1;

		//shifting the second row		
		for_swap = state[1][0];
		state[1][0]=state[1][1];
		state[1][1]=state[1][2];
		state[1][2]=state[1][3];
		state[1][3]=for_swap;
		//ends
		
		//shifting the 3rd row
		for_swap = state[2][0];
		state[2][0]=state[2][2];
		state[2][2]=for_swap;
		for_swap1 = state[2][1];
		state[2][1]=state[2][3];
		state[2][3]=for_swap1;
		//Ends
		
		//Shfting the fourth row
		for_swap = state[3][3];
		state[3][3]=state[3][2];
		state[3][2]=state[3][1];
		state[3][1]=state[3][0];
		state[3][0]=for_swap;
		//ends
		
	///printf("Shift rows function is called\n");

	for(i=0;i<Nb;i++)
	{
		for(j=0;j<Nb;j++){
		///printf("%x ",state[i][j]);
		}
	///printf("\n");
	} 
		
}


void mixcolumns() {
int i,j;
///printf("Mix column function is called.\n");

int a[4][4];
int temp_state [4][4];

a[0][0]=0x02; a[0][1]=0x03; a[0][2]=0x01; a[0][3]=0x01;
a[1][0]=0x01; a[1][1]=0x02; a[1][2]=0x03; a[1][3]=0x01;
a[2][0]=0x01; a[2][1]=0x01; a[2][2]=0x02; a[2][3]=0x03;
a[3][0]=0x03; a[3][1]=0x01; a[3][2]=0x01; a[3][3]=0x02;

temp_state[0][0]= ((xtime(state[0][0])) ^ (xtime(state[1][0])^state[1][0]) ^ (state[2][0]) ^ (state[3][0]))%256;
temp_state[1][0]= ((state[0][0]) ^ (xtime(state[1][0])) ^ (xtime(state[2][0])^state[2][0]) ^ (state[3][0]))%256;
temp_state[2][0]= ((state[0][0]) ^ (state[1][0]) ^ (xtime(state[2][0])) ^ (xtime(state[3][0])^state[3][0]))%256;
temp_state[3][0]= ((xtime(state[0][0])^state[0][0]) ^ (state[1][0]) ^ (state[2][0]) ^ (xtime(state[3][0])))%256;

state[0][0]=temp_state[0][0];
state[1][0]=temp_state[1][0];
state[2][0]=temp_state[2][0];
state[3][0]=temp_state[3][0];

temp_state[0][1]= ((xtime(state[0][1])) ^ (xtime(state[1][1])^state[1][1]) ^ (state[2][1]) ^ (state[3][1]))%256;
temp_state[1][1]= ((state[0][1]) ^ (xtime(state[1][1])) ^ (xtime(state[2][1])^state[2][1]) ^ (state[3][1]))%256;
temp_state[2][1]= ((state[0][1]) ^ (state[1][1]) ^ (xtime(state[2][1])) ^ (xtime(state[3][1])^state[3][1]))%256;
temp_state[3][1]= ((xtime(state[0][1])^state[0][1]) ^ (state[1][1]) ^ (state[2][1]) ^ (xtime(state[3][1])))%256;

state[0][1]=temp_state[0][1];
state[1][1]=temp_state[1][1];
state[2][1]=temp_state[2][1];
state[3][1]=temp_state[3][1];


temp_state[0][2]= ((xtime(state[0][2])) ^ (xtime(state[1][2])^state[1][2]) ^ (state[2][2]) ^ (state[3][2]))%256;
temp_state[1][2]= ((state[0][2]) ^ (xtime(state[1][2])) ^ (xtime(state[2][2])^state[2][2]) ^ (state[3][2]))%256;
temp_state[2][2]= ((state[0][2]) ^ (state[1][2]) ^ (xtime(state[2][2])) ^ (xtime(state[3][2])^state[3][2]))%256;
temp_state[3][2]= ((xtime(state[0][2])^state[0][2]) ^ (state[1][2]) ^ (state[2][2]) ^ (xtime(state[3][2])))%256;

state[0][2]=temp_state[0][2];
state[1][2]=temp_state[1][2];
state[2][2]=temp_state[2][2];
state[3][2]=temp_state[3][2];

temp_state[0][3]= ((xtime(state[0][3])) ^ (xtime(state[1][3])^state[1][3]) ^ (state[2][3]) ^ (state[3][3]))%256;
temp_state[1][3]= ((state[0][3]) ^ (xtime(state[1][3])) ^ (xtime(state[2][3])^state[2][3]) ^ (state[3][3]))%256;
temp_state[2][3]= ((state[0][3]) ^ (state[1][3]) ^ (xtime(state[2][3])) ^ (xtime(state[3][3])^state[3][3]))%256;
temp_state[3][3]= ((xtime(state[0][3])^state[0][3]) ^ (state[1][3]) ^ (state[2][3]) ^ (xtime(state[3][3])))%256;

state[0][3]=temp_state[0][3];
state[1][3]=temp_state[1][3];
state[2][3]=temp_state[2][3];
state[3][3]=temp_state[3][3];

	for(i=0;i<Nb;i++)
	{
		for(j=0;j<Nb;j++){
		///printf("%x ",state[i][j]);
		}
	///printf("\n");
	} 

}

/************************************************************* Key Expansion Function Starts **********************************************/

void keyExpansion(unsigned char *k){

unsigned char i,j,word_row,word_col,for_swap,base = 0x10,rcon_control=0;
unsigned char temp[4][1];
unsigned char rcon_local[4][1];


/* inserting the initial key in the Key Schedule */
	for ( i=0;i<16;i++){
		//if(i%Nk==0){printf("\n");}
		word_col=i/Nk;
		word_row=i%Nk;
		w[word_row][word_col] = k[i];
	}
/* inserting the initial key in the Key Schedule ENDS*/
	

/* Here starts the 10 Rounds for Key Expansion */	
	for (i = Nk;i< Nb*(Nr+1);i++)
	{
	
	temp[0][0]=w[0][i-1];
	temp[1][0]=w[1][i-1];
	temp[2][0]=w[2][i-1];
	temp[3][0]=w[3][i-1];
	
	
		if(i% Nk ==0)
		{
		/*Intializing the rcon array*/
		rcon_local[0][0]=rcon[rcon_control];
		rcon_local[1][0]=0x00;
		rcon_local[2][0]=0x00;
		rcon_local[3][0]=0x00;
		rcon_control++;
		/*Intializing the rcon array ENDS*/
		
		/* rotword */
		for_swap=temp[0][0];
		temp[0][0]=temp[1][0];
		temp[1][0]=temp[2][0];
		temp[2][0]=temp[3][0];
		temp[3][0]=for_swap;
		/* rotword ends */
		
		/* sub word */
		for(j=0;j<Nb;j++){
		word_row = temp[j][0]/base;
		word_col = temp[j][0]%base;
		temp[j][0] = sbox[word_row*16+word_col];
		}
		/* sub word ends */
			
		/* Doing the bitwise XOR operation with rcon	*/
		temp[0][0]=rcon_local[0][0] ^ temp[0][0];
		temp[1][0]=rcon_local[1][0] ^ temp[1][0];
		temp[2][0]=rcon_local[2][0] ^ temp[2][0];
		temp[3][0]=rcon_local[3][0] ^ temp[3][0];
		/* Doing the bitwise XOR operation with rcon	ENDS */
		
		}
		
	/* Doing the XOR Operation with i-Nk words and inserting the ith word in key schedule w.*/
	w[0][i] = temp[0][0] ^ w[0][i-Nk];
	w[1][i] = temp[1][0] ^ w[1][i-Nk];
	w[2][i] = temp[2][0] ^ w[2][i-Nk];
	w[3][i] = temp[3][0] ^ w[3][i-Nk];
	/* Doing the XOR Operation with i-Nk words and inserting the ith word in key schedule w.ENDS*/
	}

/* Here the 10 Rounds for Key Expansion ENDS. Now we have the key schedule in the array w*/		

//printf("Below is the derived key schedule\n");
	for(i=0;i<Nb;i++)
	{
		for(j=0;j<Nb*(Nr+1);j++)
		{
		//printf("%x ",w[i][j]);
		}
	//printf("\n");
	}
}

/************************************************************* Key Expansion Function Ends **********************************************/
