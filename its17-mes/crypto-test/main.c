#include <stdio.h>
#include "crypto/ciphers.h"
#include "crypto/modes/cbc.h"
#include "crypto/twofish.h"
#include "hashes/sha256.h"

static uint8_t twofish_key[] = {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
    0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF
};

static uint8_t aes_key[] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

void twofish_demo(void);
void aes_cbc_demo(void);
void sha256_demo(void);
void print_bytes(char *msg, uint8_t *bytes, int n);
void read_line_without_trailing_new_line(char *buf, int n);

int main(void){
    puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    // all demos read a string from the command line, encrypt the
    // entered message, decrypt it again, and print the data at various
    // stages throughout the process
    twofish_demo();
    aes_cbc_demo();
    sha256_demo();

    return 0;
}

void twofish_demo(void) {
    cipher_context_t ctx_encrypt;
    cipher_context_t ctx_decrypt;
    int err;
    uint8_t data[TWOFISH_BLOCK_SIZE] = {0};
    uint8_t encrypted[TWOFISH_BLOCK_SIZE] = {0};
    uint8_t decrypted[TWOFISH_BLOCK_SIZE] = {0};

    printf("=== RUNNING TWOFISH ===\n");
    printf("Enter up to %d chars:\n", sizeof(data) - 1);
    read_line_without_trailing_new_line((char*) data, sizeof(data));
    printf("\"%s\" is in hex: ", (char*) data);
    print_bytes("0x", data, strlen((char*) data));

    // encryption
    err = twofish_init(&ctx_encrypt, twofish_key, TWOFISH_KEY_SIZE);
    printf("err = %d\n", err);
    err = twofish_encrypt(&ctx_encrypt, data, encrypted);
    printf("err = %d\n", err);
    print_bytes("cipher bytes: 0x", encrypted, sizeof(encrypted));

    // decryption
    err = twofish_init(&ctx_decrypt, twofish_key, TWOFISH_KEY_SIZE);
    printf("err = %d\n", err);
    err = twofish_decrypt(&ctx_decrypt, encrypted, decrypted);
    printf("err = %d\n", err);
    print_bytes("decrypted bytes: 0x", decrypted, sizeof(decrypted));
    printf("decrypted message: %s\n", decrypted);
    printf("== TWOFISH FINISHED ===\n");
}

void aes_cbc_demo(void){
    cipher_t cipher_enc;
    cipher_t cipher_dec;
    int len, err;
    uint8_t data[64] = {0};
    uint8_t encrypted[64] = {0};
    uint8_t decrypted[64] = {0};
    // samr21-xpro has no HW-(P)RNG, therefore we'll just take whatever is in
    // memory - should be good enough for the demonstration
    uint8_t iv[8];

    printf("=== RUNNING AES IN CBC MODE ===\n");
    printf("Enter up to %d chars:\n", sizeof(data) - 1);
    read_line_without_trailing_new_line((char*) data, sizeof(data));
    printf("\"%s\" is in hex: ", (char*) data);
    print_bytes("0x", data, strlen((char*) data));

    // encryption
    err = cipher_init(&cipher_enc, CIPHER_AES_128, aes_key, sizeof(aes_key));
    printf("err = %d\n", err);
    len = cipher_encrypt_cbc(&cipher_enc, iv, data, sizeof(data), encrypted);
    printf("len = %d\n", len);
    print_bytes("encrypted bytes: 0x", encrypted, len);

    // decryption
    err = cipher_init(&cipher_dec, CIPHER_AES_128, aes_key, sizeof(aes_key));
    printf("err = %d\n", err);
    len = cipher_decrypt_cbc(&cipher_dec, iv, encrypted, len, decrypted);
    printf("len = %d\n", len);
    print_bytes("decrypted bytes: 0x", decrypted, len);
    printf("decrypted message: %s\n", decrypted);
    printf("=== AES FINISHED ===\n");
}

void sha256_demo(void){
    static unsigned char hash[SHA256_DIGEST_LENGTH];
    sha256_context_t sha256;
    char data[64] = {0};

    printf("=== RUNNING SHA256 ===\n");
    printf("Enter up to %d chars:\n", sizeof(data) - 1);
    read_line_without_trailing_new_line(data, sizeof(data));
    printf("\"%s\" is in hex: ", data);
    print_bytes("0x", (uint8_t*) data, strlen((char*) data));

    sha256_init(&sha256);
    sha256_update(&sha256, (uint8_t*) data, strlen(data));
    sha256_final(&sha256, hash);

    print_bytes("sha256 is: 0x", hash, SHA256_DIGEST_LENGTH);
    printf("=== SHA256 FINISHED ===\n");
}

// as the name states, reads until the first '\n' (inclusive) and
// replaces it with a '\0'
void read_line_without_trailing_new_line(char *buf, int n){
    void *check;

    if ( buf != NULL && n > 0){
        check = fgets(buf, n, stdin);
        if ( check != NULL ){
            for(int ii = n; ii >= 0; ii--){
                if ( buf[ii] == '\n' ){
                    buf[ii] = '\0';
                    return;
                }
            }
        }
    }
}

// print bytes in hex, with two digits each. adds leading 0s if necessary
// msg is optional and printed before the hex-string
void print_bytes(char *msg, uint8_t *bytes, int n){
    if ( bytes != NULL && n > -1){
        if ( msg != NULL ){
            printf("%s", msg);
        }
        for(int ii = 0; ii < n; ii++){
            printf("%02x", bytes[ii]);
        }
        printf("\n");
    }
}
