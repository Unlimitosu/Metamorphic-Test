#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <assert.h>
#include "KISA_SHA256.h"


#define SHA256_DIGESTLEN 32
#define MSG_BITLEN 512 * 2
#define MSG_BYTELEN MSG_BITLEN / 8
#define EXCLUSION_BYTELEN 4

void print_hex(char* p, BYTE* src, uint32_t src_bytelen)
{
	printf("[%s]\n", p);

	for (int cnt_i = 0; cnt_i < src_bytelen; cnt_i++)
	{
		if ((cnt_i != 0) && (cnt_i % 16 == 0))
		{
			printf("\n");
		}
		printf("%02X ", src[cnt_i]);
	}
	printf("\n");
}


void Metamorphic_Bit_Exclusion_Test() {

	//! parameters
	BYTE msg[MSG_BYTELEN] = { 0, };
	BYTE* buf = NULL, * msg_copy_buf = NULL;
	BYTE digest[MSG_BYTELEN] = { 0, };
	BYTE copy_digest[MSG_BYTELEN + EXCLUSION_BYTELEN] = { 0, };
	UINT i = 0, j = 0, k = 0;
	UINT same_num = 0, diff_num = 0;

	//! �ʿ��� ���� �����Ҵ�
	buf = (BYTE*)malloc(MSG_BYTELEN);
	assert(buf != NULL);

	msg_copy_buf = (BYTE*)malloc(MSG_BYTELEN + EXCLUSION_BYTELEN);
	assert(msg_copy_buf != NULL);

	// ������ ���� msgä��� (�õ�� �����Ǿ� ����)
	for (i = 0; i < MSG_BYTELEN; i++) {
		msg[i] = (BYTE)(rand() & 0xff);
	}

	for (i = 0; i < MSG_BYTELEN; i++) 
	{
		memset(digest, 0x00, SHA256_DIGESTLEN);	// Ȥ�� �� ������ ����� �ʱ�ȭ
		memcpy(msg_copy_buf, msg, i+1);	// ����� �޸� ���̴� ������
		memcpy(buf, msg, i+1);

		SHA256_Encrpyt(buf, i+1, digest); // ���� �޼��� 

		for (j = 0; j < EXCLUSION_BYTELEN * 8; j++) {
			memset(copy_digest, 0x00, SHA256_DIGESTLEN);
			msg_copy_buf[i + j / 8 + 1] ^= 1 << (j % 8);	// flip

			SHA256_Encrpyt(msg_copy_buf, i+1, copy_digest); // exclusion �޼��� 

			if (memcmp(digest, copy_digest, SHA256_DIGESTLEN)) // �� �迭�� ���� �ٸ� ���� ���� ���
			{
				print_hex("origin_msg", buf, i + 1);
				print_hex("test_msg", msg_copy_buf, i + 1);

				print_hex("origin_dst", digest, SHA256_DIGESTLEN);
				print_hex("test_dst", copy_digest, SHA256_DIGESTLEN);

				fprintf(stderr, "ERROR\n");
				diff_num++;
			}
			same_num++;
		}	
	}
	if(diff_num ==0)
		fprintf(stderr, "Metamorphic_Bit_Exclusion_Test SUCCESS!\n");
	if (msg_copy_buf) free(msg_copy_buf);
	if (buf) free(buf);

	return;
}



int main() {

	srand(1);

	Metamorphic_Bit_Exclusion_Test();

	return;
}


