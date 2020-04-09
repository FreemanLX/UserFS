#ifndef DECRYPT_H_INCLUDED
#define DECRYPT_H_INCLUDED
#include "maximum.h"
#define copyar(n,m,size) forp(i, size) n[i] = m[i]
string key;
void importkey_1(const char *file)
{
    ifstream ha(file);
    if (ha.is_open())
	{
        getline(ha, key);
		ha.close();
	}

	else cout << "Unable to open file";
}


void InverseMixColumns(unsigned char * state) {
	unsigned char tmp[16];
	tmp[0] = (unsigned char)mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]];
	tmp[1] = (unsigned char)mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]];
	tmp[2] = (unsigned char)mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]];
	tmp[3] = (unsigned char)mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]];
	tmp[4] = (unsigned char)mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]];
	tmp[5] = (unsigned char)mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]];
	tmp[6] = (unsigned char)mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]];
	tmp[7] = (unsigned char)mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]];
	tmp[8] = (unsigned char)mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]];
	tmp[9] = (unsigned char)mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]];
	tmp[10] = (unsigned char)mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]];
	tmp[11] = (unsigned char)mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]];
	tmp[12] = (unsigned char)mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]];
	tmp[13] = (unsigned char)mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]];
	tmp[14] = (unsigned char)mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]];
	tmp[15] = (unsigned char)mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]];
    copyar(state, tmp, 16);
}


void ShiftRows_de(unsigned char * state) {
	unsigned char tmp[16];
	tmp[0] = state[0];
	tmp[1] = state[13];
	tmp[2] = state[10];
	tmp[3] = state[7];
	tmp[4] = state[4];
	tmp[5] = state[1];
	tmp[6] = state[14];
	tmp[7] = state[11];
	tmp[8] = state[8];
	tmp[9] = state[5];
	tmp[10] = state[2];
	tmp[11] = state[15];
	tmp[12] = state[12];
	tmp[13] = state[9];
	tmp[14] = state[6];
	tmp[15] = state[3];
    copyar(state, tmp, 16);
}


void Round_de(unsigned char * state, unsigned char * key, int e) {
	forp(i, 16) state[i] ^= key[i];
    if(e == 0) InverseMixColumns(state);
	ShiftRows_de(state);
    forp(i, 16) state[i] = inv_s[state[i]];
}


void AESDecrypt(unsigned char * encryptedMessage, unsigned char * expandedKey, unsigned char * decryptedMessage, int size){
	unsigned char state[16]; 
    copyar(state, encryptedMessage, 16);
    state[size] = '\0';
	Round_de(state, expandedKey+160, 1);
    form(i,8) Round_de(state, expandedKey + (16 * (i + 1)), 0);
	forp(i, 16) state[i] ^= expandedKey[i];
    copyar(decryptedMessage, state, 16);
    decryptedMessage[strlen((char*)decryptedMessage)] = '\0';
}

void decryption(string msgstr, char *return_char){
    unsigned char encryptedMessage[128], expandedKey[176];;
	copyar(encryptedMessage, msgstr, msgstr.size());
    encryptedMessage[msgstr.size()] = '\0';
    importkey_1("headers/keyfile"); 
	KeyExpansion((unsigned char*)key.c_str(), expandedKey);
	unsigned char * decryptedMessage = new unsigned char[msgstr.size()];
	for (int i = 0; i < msgstr.size(); i += 16) AESDecrypt(encryptedMessage + i, expandedKey, decryptedMessage + i, msgstr.size());
    copyar(return_char, decryptedMessage, msgstr.size());
    return_char[msgstr.size()] = '\0';
    delete[] decryptedMessage;
}
#endif // DECRYPT_H_INCLUDED
