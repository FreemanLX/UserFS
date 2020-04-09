#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED
#include "maximum.h"
#define copyar(n,m,size) forp(i, size) n[i] = m[i]

void importkey(const char *file, string& key)
{
    ifstream ha(file);
    if (ha.is_open())
	{
        getline(ha, key);
		ha.close();
	}

	else cout << "Unable to open file";
}


void ShiftRows(unsigned char * state) {
	unsigned char tmp[16];
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];
	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];
	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];
	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];
    copyar(state,tmp, 16);
}


void MixColumns(unsigned char * state) {
	unsigned char tmp[16];
	tmp[0] = (unsigned char) mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	tmp[1] = (unsigned char) state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	tmp[2] = (unsigned char) state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	tmp[3] = (unsigned char) mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];
	tmp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	tmp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	tmp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	tmp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];
	tmp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	tmp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	tmp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	tmp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];
	tmp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	tmp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	tmp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	tmp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];
    copyar(state,tmp, 16);
}

void Round(unsigned char * state, unsigned char *key, int fix)
{
    forp(i,16) state[i] = s[state[i]];
    ShiftRows(state);
    if(fix == 0) MixColumns(state);
    forp(i,16) state[i] ^= key[i];
}


void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
	unsigned char state[16];
    copyar(state, message, 16);
    forp(i,16) state[i] ^= expandedKey[i];
	forp(i,9) Round(state, expandedKey + (16 * (i+1)), 0);
	Round(state, expandedKey + 160, 1);
    copyar(encryptedMessage, state, 16);
}


void encryption(char *message, char *returencrypt)
{
    int originalLen = strlen(message), paddedMessageLen = strlen(message);
	if ((paddedMessageLen % 16) != 0) paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
    unsigned char expandedKey[176];
    unsigned char * paddedMessage = new unsigned char[paddedMessageLen];
    unsigned char * encryptedMessage = new unsigned char[paddedMessageLen];
    forp(i,paddedMessageLen) paddedMessage[i] = (i >= originalLen) ? 0 : message[i]; 
    string key;
    importkey("headers/keyfile", key);
	KeyExpansion((unsigned char*)key.c_str(), expandedKey);
    forp(i, paddedMessageLen) {AESEncrypt(paddedMessage+i, expandedKey, encryptedMessage+i); i+=15;}
	copyar(returencrypt, encryptedMessage, paddedMessageLen);
    returencrypt[paddedMessageLen] = '\0';    
    delete[] paddedMessage;
	delete[] encryptedMessage;
}

#endif // ENCRYPT_H_INCLUDED
