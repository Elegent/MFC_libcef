/**
  build with shell:
  gcc -Wall aes.c -lcrypto -o aes
**/
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "AES_API.h"
#include "openssl/evp.h"

void AES_encrypt(unsigned char* in, int inl, unsigned char *out, int* len, unsigned char * key){
	unsigned char iv[8];
	EVP_CIPHER_CTX ctx;
	//��init���Ľ��ǽ�ctx�ڴ� memsetΪ0  
	EVP_CIPHER_CTX_init(&ctx);

	//cipher  = EVP_aes_128_ecb();  
	//ԭ��Ϊint EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx,const EVP_CIPHER *cipher, ENGINE *impl, const unsigned char *key, const unsigned char *iv)   
	//�������ecb�������뱾ģʽ��˵������������ӽ��ܣ�ǰ��û�й�ϵ��Ҳ�ò���iv  
	EVP_EncryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, iv);  

	*len = 0;
	int outl = 0;
	//���EVP_EncryptUpdate��ʵ��ʵ�ʾ��ǽ�in����inl�ĳ���ȥ���ܣ�ʵ�ֻ�ȡ�ø�cipher�Ŀ��С����aes_128��˵��16�ֽڣ�����block-size��������ȥ���ܡ�
	//�������Ϊ50�ֽڣ���˴�������48�ֽڣ�outlҲΪ48�ֽڡ�����in�е�������ֽڿ�����ctx->buf����������  
	//����inlΪblock_size�����������Σ���ctx->buf��û����ǰ����������ʱ��ֱ�Ӽӽ��ܲ�����ʡȥ�ܶ����������  
	EVP_EncryptUpdate(&ctx, out+*len, &outl, in+*len, inl);
   	*len+=outl;
   	//�������n�ֽڡ��˴����д���
   	//�����֧��pading���һ������ݵĻ��ͳ������򣬽�block_size-�������ֽ����������ֽ�����Ϊ�˸�����ֵ����block_size=16,���ݳ���Ϊ4���򽫺����12�ֽ�����Ϊ16-4=12������Ϊһ���������� 
   	//����ǰ��Ϊ������ʱ������������Ϊ16�ֽڣ�����ٵ��ô�Finalʱ�������Ƕ�16��0���м��ܣ������Ĳ��ü��ɣ�Ҳ�����ò��ŵ�һ����Final��
   	int test = inl>>4;
   	if(inl != test<<4){
   		EVP_EncryptFinal_ex(&ctx,out+*len,&outl);  
	   	*len+=outl;
	}
	EVP_CIPHER_CTX_cleanup(&ctx);
}


int AES_decrypt(unsigned char* in, int inl, unsigned char *out, unsigned char *key){
	unsigned char iv[8];
	EVP_CIPHER_CTX ctx;
	//��init���Ľ��ǽ�ctx�ڴ� memsetΪ0  
	EVP_CIPHER_CTX_init(&ctx);

	//cipher  = EVP_aes_128_ecb();  
	//ԭ��Ϊint EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx,const EVP_CIPHER *cipher, ENGINE *impl, const unsigned char *key, const unsigned char *iv)   
	//�������ecb�������뱾ģʽ��˵������������ӽ��ܣ�ǰ��û�й�ϵ��Ҳ�ò���iv  
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, iv); 
	int len = 0;
	int outl = 0;

	EVP_DecryptUpdate(&ctx, out+len, &outl, in+len, inl);
   	len += outl;
   	 
   	EVP_DecryptFinal_ex(&ctx, out+len, &outl);  
   	len+=outl;
	out[len]=0;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return len;
}