#include <stdio.h>
#include "crypto/twofish.h"

static uint8_t twofish_key_01[] = {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
    0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF
};

void twofish(void);
void print_bytes(char *msg, uint8_t *bytes, int n);
void read_line_without_trailing_new_line(char *buf, int n);

int main(void){
    puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    twofish();

    return 0;
}

void twofish(void) {
    cipher_context_t ctx_encrypt;
    cipher_context_t ctx_decrypt;
    int err;
    uint8_t data[TWOFISH_BLOCK_SIZE] = {0};
    uint8_t encrypted[TWOFISH_BLOCK_SIZE] = {0};
    uint8_t decrypted[TWOFISH_BLOCK_SIZE] = {0};

    printf("Enter up to %d chars:\n", sizeof(data) - 1);
    read_line_without_trailing_new_line((char*) data, sizeof(data));
    printf("\"%s\" is in hex: ", (char*) data);
    print_bytes("0x", data, sizeof(data));

    // encryption
    err = twofish_init(&ctx_encrypt, twofish_key_01, TWOFISH_KEY_SIZE);
    printf("err = %d\n", err);
    err = twofish_encrypt(&ctx_encrypt, data, encrypted);
    printf("err = %d\n", err);
    print_bytes("cipher bytes: 0x", encrypted, sizeof(encrypted));

    // decryption
    err = twofish_init(&ctx_decrypt, twofish_key_01, TWOFISH_KEY_SIZE);
    printf("err = %d\n", err);
    err = twofish_decrypt(&ctx_decrypt, encrypted, decrypted);
    printf("err = %d\n", err);
    print_bytes("decrypted bytes: 0x", decrypted, sizeof(decrypted));
    printf("decrypted message: %s\n", decrypted);
}

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
