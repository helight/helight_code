#include <openssl/des.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static int Base64Encode(unsigned char* input, int inLen, char* output, int outLen)
{
    if (!input || !output)
    {
        return -1;
    }
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, inLen);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    if (bptr->length > outLen)
    {
        outLen = bptr->length;
        return -2;
    }

    memcpy(output, bptr->data, bptr->length-1);
    output[bptr->length-1] = 0;
    outLen = bptr->length-1;
    BIO_free_all(b64);
    return 0;
}

static int Base64Decode(const char* input, int inLen, unsigned char* output, int outLen)
{
    if (!input || !output)
    {
        return -1;
    }

    char tmp[256];
    tmp[255] = '\n';      // Openssl demand to have '\n' to end the string.
    memcpy(&tmp[0], input, inLen);
    memset(output, 0 , outLen);
    BIO * b642 = BIO_new(BIO_f_base64());
    BIO * bmem2 = BIO_new_mem_buf(&tmp[0], inLen+1);
    bmem2 = BIO_push(b642, bmem2);
    outLen = BIO_read(bmem2, output, outLen);
    BIO_free_all(bmem2);
    return 0;
}
char *unbase64(unsigned char *input, int length)
{
  BIO *b64, *bmem;

  char *buffer = (char *)malloc(length);
  memset(buffer, 0, length);

  b64 = BIO_new(BIO_f_base64());
  bmem = BIO_new_mem_buf(input, length);
  bmem = BIO_push(b64, bmem);

  BIO_read(bmem, buffer, length);

  BIO_free_all(bmem);

  return buffer;
}
void test_base64()
{
	char content_in[1024] = "helightxu";
	char content_de[1024] = {0};
	char content_out[1024] = {0};

	printf("content_in= %s\n", content_in);
	Base64Encode(content_in, strlen(content_in), content_de, sizeof(content_de));
	printf("content_de= %s\n", content_de);
	//Base64Decode(content_de, strlen(content_de), content_out, sizeof(content_out));
	printf("content_out= %s\n", unbase64(content_de, strlen(content_de)));
}
int main(void)
{
	int i=0;
	char de_content[1024] = {0};
	char content_de[1024] = {0};
	char content_in[1024] = {0};
	char content_out[1024] = {0};
	
	const char *key = "abc45678";
	const char *content = "helightxu@gmail.com";
	DES_cblock key_cblock;
	DES_key_schedule schedule;

	DES_string_to_key(key, &key_cblock);
	DES_set_key_checked(&key_cblock, &schedule);
	//printf("key_cblock= %s\n", key_cblock);

	DES_ncbc_encrypt(content, content_in, strlen(content), &schedule, &key_cblock, DES_ENCRYPT  );
	Base64Encode(content_in, strlen(content_in), de_content, sizeof(de_content));
	printf("content= %s\n", content);
	printf("content_in= %s\n", content_in);
	printf("content_out= %s\n", de_content);

	DES_string_to_key(key, &key_cblock);
	DES_set_key_checked(&key_cblock, &schedule);
	//printf("key_cblock= %s\n", key_cblock);

	Base64Encode(de_content, strlen(de_content), content_de, sizeof(content_de));
	DES_ncbc_encrypt(content_de, content_out, strlen(content_de), &schedule, &key_cblock, DES_DECRYPT );
	printf("content_out= %s\n", content_out);
	
/*
	for(; i<20; i++)
	{
		printf("content_in[%x]\n", content_in[i]);
	}
*/
	test_base64();
	return 0;
}

