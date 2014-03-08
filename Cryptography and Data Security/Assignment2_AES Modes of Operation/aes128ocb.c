#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "aes128ocb.h"
#include "aes128e.h"

/* Returns the exponent of the msb of 'value' */
static unsigned int msb(unsigned int value);

void make_double(unsigned char *L1, unsigned char *L2);

/* Returns the number of trailing zeros in 'value' */
static unsigned int ntz(unsigned int value);

/* Under the 16-byte (128-bit) key at k and the 12-byte (96-bit) nonce at n, encrypt the plaintext at p and store it at c. 
   The length of the plaintext is a multiple of 16 bytes given at len (e.g., len = 2 for a 32-byte p). The length of the
   ciphertext c is (len+1)*16 bytes. */
   
void aes128ocb(unsigned char *c, const unsigned char *k, const unsigned char *n, const unsigned char *p, const unsigned int len) {

int i,num,temp,binlenN=96,bottom,j;
unsigned char L_asterisk[16],L_dollar[16],L_send[16],L_receive[16],p1[16],checksum[len+1][16];;
unsigned char ktop[16],stretch[24],offset[len+1][16],L[len+1][16],cipher[16],plaintext_tag[16],c_tag[16],tag[16];
unsigned char plaintext[] = {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char full_nonce[16];

/*START:Generating the L_asterisk*/	
	aes128e(L_asterisk,plaintext,k);
/*ENDS:Generating the L_asterisk*/	

/*START:Generating the L_dollar*/	
	make_double(L_asterisk,L_dollar);
/*ENDS:Generating the L_dollar*/

/*START:Generating the L_is*/	
	make_double(L_dollar,L_receive); //sending the L_dollar and as parameter and the result will be stored in L_receive
	L[0][0]=L_receive[0]; 			 //START:The L_receive is now being stored in L[0][0......15]
	L[0][1]=L_receive[1];
	L[0][2]=L_receive[2];
	L[0][3]=L_receive[3];
	L[0][4]=L_receive[4];
	L[0][5]=L_receive[5];
	L[0][6]=L_receive[6];
	L[0][7]=L_receive[7];
	L[0][8]=L_receive[8];
	L[0][9]=L_receive[9];
	L[0][10]=L_receive[10];
	L[0][11]=L_receive[11];
	L[0][12]=L_receive[12];
	L[0][13]=L_receive[13];
	L[0][14]=L_receive[14];
	L[0][15]=L_receive[15];			//ENDS:The L_receive is now being stored in L[0][0......15]
	
	for(i=1;i<=len;i++)				//START:Initializing the L_send with the immediate previous L. that is L[i-1][1....15]
	{
	L_send[0]=L[i-1][0];
	L_send[1]=L[i-1][1];
	L_send[2]=L[i-1][2];
	L_send[3]=L[i-1][3];
	L_send[4]=L[i-1][4];
	L_send[5]=L[i-1][5];
	L_send[6]=L[i-1][6];
	L_send[7]=L[i-1][7];
	L_send[8]=L[i-1][8];
	L_send[9]=L[i-1][9];
	L_send[10]=L[i-1][10];
	L_send[11]=L[i-1][11];
	L_send[12]=L[i-1][12];
	L_send[13]=L[i-1][13];
	L_send[14]=L[i-1][14];
	L_send[15]=L[i-1][15];			//ENDS:Initializing the L_send with the immediate previous L. that is L[i-1][1....15]
	make_double(L_send,L_receive);  //Calling the make double function. L_send is sent as input and L_receive is received as output
	L[i][0]=L_receive[0];	//START:Initializing the L[i][1....15] using the L_receive[]
	L[i][1]=L_receive[1];
	L[i][2]=L_receive[2];
	L[i][3]=L_receive[3];
	L[i][4]=L_receive[4];
	L[i][5]=L_receive[5];
	L[i][6]=L_receive[6];
	L[i][7]=L_receive[7];
	L[i][8]=L_receive[8];
	L[i][9]=L_receive[9];
	L[i][10]=L_receive[10];
	L[i][11]=L_receive[11];
	L[i][12]=L_receive[12];
	L[i][13]=L_receive[13];
	L[i][14]=L_receive[14];
	L[i][15]=L_receive[15]; //ENDS:Initializing the L[i][1....15] using the L_receive[]
	}
/*ENDS:Generating the L_is*/

/*START:Generating the 128 bit nonce from the given nonce*/
full_nonce[0]=0x00; full_nonce[1]=0x00; full_nonce[2]=0x00; full_nonce[3]=0x00;
full_nonce[3]=0x01; full_nonce[4]=n[0]; full_nonce[5]=n[1]; full_nonce[6]=n[2];
full_nonce[7]=n[3]; full_nonce[8]=n[4]; full_nonce[9]=n[5]; full_nonce[10]=n[6];
full_nonce[11]=n[7]; full_nonce[12]=n[8]; full_nonce[13]=n[9]; full_nonce[14]=n[10]; full_nonce[15]=n[11];
/*ENDS:Generating the 128 bit once from the given nonce*/


/*START:Generating the bottom value*/	
bottom = full_nonce[15]&63;
/*Ends:Generating the bottom value*/


/*START:Generating the KTOP*/	
full_nonce[15]=full_nonce[15] & 0xc0; //as per the algorithm, making the last six digits zero
aes128e(ktop,full_nonce,k);
/*ENDS:Generating the KTOP*/	

	
/*START:Generating the stretch*/	
stretch[0]=ktop[0]; stretch[1]=ktop[1]; stretch[2]=ktop[2]; stretch[3]=ktop[3]; stretch[4]=ktop[4];
stretch[5]=ktop[5]; stretch[6]=ktop[6]; stretch[7]=ktop[7]; stretch[8]=ktop[8]; stretch[9]=ktop[9];
stretch[10]=ktop[10]; stretch[11]=ktop[11]; stretch[12]=ktop[12]; stretch[13]=ktop[13]; stretch[14]=ktop[14];
stretch[15]=ktop[15];

stretch[16]=ktop[0]^ktop[1]; stretch[17]=ktop[1]^ktop[2]; stretch[18]=ktop[2]^ktop[3]; stretch[19]=ktop[3]^ktop[4];
stretch[20]=ktop[4]^ktop[5]; stretch[21]=ktop[5]^ktop[6]; stretch[22]=ktop[6]^ktop[7]; stretch[23]=ktop[7]^ktop[8];
/*ENDS:Generating the stretch*/	
	
/*START:Generating offset_0*/		
int temp_for_offset;
		
	for (i=0;i<16;i++)
	{
	offset[0][i]=stretch[(i*8+bottom)/8]<<(i*8+bottom)%8;
	temp_for_offset=stretch[((i*8+bottom)/8)+1] >> (8-(i*8+bottom)%8);
	offset[0][i]=offset[0][i]^temp_for_offset;
	} 
/*ENDS:Generating the offset_0*/	


/*START:Calculating Checksum_0*/
checksum[0][0]=0x00;
checksum[0][1]=0x00;
checksum[0][2]=0x00;
checksum[0][3]=0x00;
checksum[0][4]=0x00;
checksum[0][5]=0x00;
checksum[0][6]=0x00;
checksum[0][7]=0x00;
checksum[0][8]=0x00;
checksum[0][9]=0x00;
checksum[0][10]=0x00;
checksum[0][11]=0x00;
checksum[0][12]=0x00;
checksum[0][13]=0x00;
checksum[0][14]=0x00;
checksum[0][15]=0x00;
/*ENDS:Caclculating Checksum_0*/

/*START: Generating the cipher text from the plain text*/
	for(i=1;i<=len;i++)
	{
	offset[i][0]=offset[i-1][0]^L[ntz(i)][0]; //START: Initializing the offset[i] as per the algorithm
	offset[i][1]=offset[i-1][1]^L[ntz(i)][1];
	offset[i][2]=offset[i-1][2]^L[ntz(i)][2];
	offset[i][3]=offset[i-1][3]^L[ntz(i)][3];
	
	offset[i][4]=offset[i-1][4]^L[ntz(i)][4];
	offset[i][5]=offset[i-1][5]^L[ntz(i)][5];
	offset[i][6]=offset[i-1][6]^L[ntz(i)][6];
	offset[i][7]=offset[i-1][7]^L[ntz(i)][7];
		
	offset[i][8]=offset[i-1][8]^L[ntz(i)][8];
	offset[i][9]=offset[i-1][9]^L[ntz(i)][9];
	offset[i][10]=offset[i-1][10]^L[ntz(i)][10];
	offset[i][11]=offset[i-1][11]^L[ntz(i)][11];
	
	offset[i][12]=offset[i-1][12]^L[ntz(i)][12];
	offset[i][13]=offset[i-1][13]^L[ntz(i)][13];
	offset[i][14]=offset[i-1][14]^L[ntz(i)][14];
	offset[i][15]=offset[i-1][15]^L[ntz(i)][15]; //ENDS: Initializing the offset[i] as per algorithm
	
			
	p1[0]=p[(i-1)*16+0] ^ offset[i][0]; //Start: XOR in between the plaintext input(ith block) and offset[i] 
	p1[1]=p[(i-1)*16+1] ^ offset[i][1];
	p1[2]=p[(i-1)*16+2] ^ offset[i][2];
	p1[3]=p[(i-1)*16+3] ^ offset[i][3];
	p1[4]=p[(i-1)*16+4] ^ offset[i][4];
	p1[5]=p[(i-1)*16+5] ^ offset[i][5];
	p1[6]=p[(i-1)*16+6] ^ offset[i][6];
	p1[7]=p[(i-1)*16+7] ^ offset[i][7];
	p1[8]=p[(i-1)*16+8] ^ offset[i][8];
	p1[9]=p[(i-1)*16+9] ^ offset[i][9];
	p1[10]=p[(i-1)*16+10] ^ offset[i][10];
	p1[11]=p[(i-1)*16+11] ^ offset[i][11];
	p1[12]=p[(i-1)*16+12] ^ offset[i][12];
	p1[13]=p[(i-1)*16+13] ^ offset[i][13];
	p1[14]=p[(i-1)*16+14] ^ offset[i][14];
	p1[15]=p[(i-1)*16+15] ^ offset[i][15]; //END: XOR in between the plaintext input(ith block) and offset[i] 
	
	aes128e(cipher,p1,k); //As per algorithm. aes128e is the AES 128bit block cipher encryption function
	
	c[(i-1)*16+0] = offset[i][0] ^ cipher[0]; //START: XOR in between offset[i] and the cipher. Generates the final chiphertext for the ith blcok
	c[(i-1)*16+1] = offset[i][1] ^ cipher[1];
	c[(i-1)*16+2] = offset[i][2] ^ cipher[2];
	c[(i-1)*16+3] = offset[i][3] ^ cipher[3];
	c[(i-1)*16+4] = offset[i][4] ^ cipher[4];
	c[(i-1)*16+5] = offset[i][5] ^ cipher[5];
	c[(i-1)*16+6] = offset[i][6] ^ cipher[6];
	c[(i-1)*16+7] = offset[i][7] ^ cipher[7];
	c[(i-1)*16+8] = offset[i][8] ^ cipher[8];
	c[(i-1)*16+9] = offset[i][9] ^ cipher[9];
	c[(i-1)*16+10] = offset[i][10] ^ cipher[10];
	c[(i-1)*16+11] = offset[i][11] ^ cipher[11];
	c[(i-1)*16+12] = offset[i][12] ^ cipher[12];
	c[(i-1)*16+13] = offset[i][13] ^ cipher[13];
	c[(i-1)*16+14] = offset[i][14] ^ cipher[14];
	c[(i-1)*16+15] = offset[i][15] ^ cipher[15]; //ENDS: XOR in between offset[i] and the cipher. Generates the final chiphertext for the ith blcok
	
	checksum[i][0]=checksum[i-1][0]^p[(i-1)*16+0]; //START: Generating the checksum[i][0......15] as per algorithm
	checksum[i][1]=checksum[i-1][1]^p[(i-1)*16+1];
	checksum[i][2]=checksum[i-1][2]^p[(i-1)*16+2];
	checksum[i][3]=checksum[i-1][3]^p[(i-1)*16+3];
	checksum[i][4]=checksum[i-1][4]^p[(i-1)*16+4];
	checksum[i][5]=checksum[i-1][5]^p[(i-1)*16+5];
	checksum[i][6]=checksum[i-1][6]^p[(i-1)*16+6];
	checksum[i][7]=checksum[i-1][7]^p[(i-1)*16+7];
	checksum[i][8]=checksum[i-1][8]^p[(i-1)*16+8];
	checksum[i][9]=checksum[i-1][9]^p[(i-1)*16+9];
	checksum[i][10]=checksum[i-1][10]^p[(i-1)*16+10];
	checksum[i][11]=checksum[i-1][11]^p[(i-1)*16+11];
	checksum[i][12]=checksum[i-1][12]^p[(i-1)*16+12];
	checksum[i][13]=checksum[i-1][13]^p[(i-1)*16+13];
	checksum[i][14]=checksum[i-1][14]^p[(i-1)*16+14];
	checksum[i][15]=checksum[i-1][15]^p[(i-1)*16+15]; //ENDS: Generating the checksum[i][0......15] as per algorithm
	}

	//START: Calculating the taglen bits
	plaintext_tag[0]=checksum[len][0] ^ offset[len][0] ^ L_dollar[0];
	plaintext_tag[1]=checksum[len][1] ^ offset[len][1] ^ L_dollar[1];
	plaintext_tag[2]=checksum[len][2] ^ offset[len][2] ^ L_dollar[2];
	plaintext_tag[3]=checksum[len][3] ^ offset[len][3] ^ L_dollar[3];
	plaintext_tag[4]=checksum[len][4] ^ offset[len][4] ^ L_dollar[4];
	plaintext_tag[5]=checksum[len][5] ^ offset[len][5] ^ L_dollar[5];
	plaintext_tag[6]=checksum[len][6] ^ offset[len][6] ^ L_dollar[6];
	plaintext_tag[7]=checksum[len][7] ^ offset[len][7] ^ L_dollar[7];
	plaintext_tag[8]=checksum[len][8] ^ offset[len][8] ^ L_dollar[8];
	plaintext_tag[9]=checksum[len][9] ^ offset[len][9] ^ L_dollar[9];
	plaintext_tag[10]=checksum[len][10] ^ offset[len][10] ^ L_dollar[10];
	plaintext_tag[11]=checksum[len][11] ^ offset[len][11] ^ L_dollar[11];
	plaintext_tag[12]=checksum[len][12] ^ offset[len][12] ^ L_dollar[12];
	plaintext_tag[13]=checksum[len][13] ^ offset[len][13] ^ L_dollar[13];
	plaintext_tag[14]=checksum[len][14] ^ offset[len][14] ^ L_dollar[14];
	plaintext_tag[15]=checksum[len][15] ^ offset[len][15] ^ L_dollar[15];
	
	aes128e(c_tag,plaintext_tag,k); //as per algorithm, calling the aes encryption function
	
	tag[0] = c_tag[0] ^ plaintext [0];
	tag[1] = c_tag[1] ^ plaintext [1];
	tag[2] = c_tag[2] ^ plaintext [2];
	tag[3] = c_tag[3] ^ plaintext [3];
	tag[4] = c_tag[4] ^ plaintext [4];
	tag[5] = c_tag[5] ^ plaintext [5];
	tag[6] = c_tag[6] ^ plaintext [6];
	tag[7] = c_tag[7] ^ plaintext [7];
	tag[8] = c_tag[8] ^ plaintext [8];
	tag[9] = c_tag[9] ^ plaintext [9];
	tag[10] = c_tag[10] ^ plaintext [10];
	tag[11] = c_tag[11] ^ plaintext [11];
	tag[12] = c_tag[12] ^ plaintext [12];
	tag[13] = c_tag[13] ^ plaintext [13];
	tag[14] = c_tag[14] ^ plaintext [14];
	tag[15] = c_tag[15] ^ plaintext [15];
	
	
	c[(len)*16+0] = tag[0]; //START: Populating the ciphertext array with the taglen bits
	c[(len)*16+1] = tag[1];
	c[(len)*16+2] = tag[2];
	c[(len)*16+3] = tag[3];
	c[(len)*16+4] = tag[4];
	c[(len)*16+5] = tag[5];
	c[(len)*16+6] = tag[6];
	c[(len)*16+7] = tag[7];
	c[(len)*16+8] = tag[8];
	c[(len)*16+9] = tag[9];
	c[(len)*16+10] = tag[10];
	c[(len)*16+11] = tag[11];
	c[(len)*16+12] = tag[12];
	c[(len)*16+13] = tag[13];
	c[(len)*16+14] = tag[14];
	c[(len)*16+15] = tag[15]; //START: Populating the ciphertext array with the taglen bits
	//ENDS: Calculating the taglen bits
	
/*ENDS: Generating the cipher text from the plain text*/
}

/* Returns the exponent of the msb of 'value' */
static unsigned int msb(unsigned int value) {

	unsigned int index=0;

	while (value >>= 1) { 		/* Loop while greater than one */ 
		index++;
	}
	return index;

}

/* Returns the number of trailing zeros in 'value' */
static unsigned int ntz(unsigned int value) {

	unsigned int zeros=0;

	while (!(value & 0x01)) { 	/* Loop while the lsb is zero */
		value >>= 1;				/* Shift to the right; that is, observe the next bit. */
		zeros++;
	}
	return zeros;
	
}

void make_double(unsigned char *L1,unsigned char *L2)
{

int num;
int temp,temp1,i;

	temp1=L1[0];
	num=temp1 & 0x80; //checking whether the the first bit is 1 or 0	

	if(num == 0) //this means the first bit of L1 is zero
	{
		for(i=0;i<16;i++)
		{
		temp1=L1[i];
		L2[i]=temp1 << 1;  //shifting 1 bit to the left
			if(i<15)
			{
				temp1=L1[i+1];
				temp=temp1 & 0x80; 
				if(temp==128) //if the first bit of the next byte is found 1
				{
				L2[i]=L2[i] ^ 0x01;
				}
			}	
		}
	}	
	
	if(num==128) //which means first bit of L1 is 1
	{
		for(i=0;i<16;i++)
		{
		L2[i]=L1[i] << 1; //shifting one bit to the left
			if(i<15)
			{
			temp=L1[i+1] & 0x80;
				if(temp==128) //if the first bit of the next byte is 1
				{
				L2[i]=L2[i] ^ 0x01;
				}
			}
		}
	L2[15]=L2[15] ^ 0x87; //as per the algorithm doing XOR in between the last eight bits with 10000111 i.e: 0x87
	}

}