/* Implement the following API.
 * You can add your own functions, but don't modify below this line.
 */

/* Under the 16-byte (128-bit) key at k and the 12-byte (96-bit) nonce at n, encrypt the plaintext at p and store it at c. 
   The length of the plaintext is a multiple of 16 bytes given at len (e.g., len = 2 for a 32-byte p). The length of the
   ciphertext c is (len+1)*16 bytes. */
void aes128ocb(unsigned char *c, const unsigned char *k, const unsigned char *n, const unsigned char *p, const unsigned int len);
void make_double(unsigned char *L1,unsigned char *L2);
