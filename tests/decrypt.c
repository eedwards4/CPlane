#include <stdio.h>
#include <string.h>

/*
Program that takes a string ( CIPHERTEXT ) in uppercase and assigns values to letters in alphabet.
The program shifts the numbers corresponding to letters and outputs the current string.
It will do this 26 times allowing the user to see all possible outcomes.
*/

void decrypt(char cipher[], int shift);

int main() {
    char cipherText[100];
    
    // Input the cipher text uppercase
    printf("Enter the cipher text: ");
    fgets(cipherText, sizeof(cipherText), stdin);

    // Loop through all possible shift values (1 to 25)
    for (int shift = 1; shift <= 25; ++shift) {
        printf("Key %d: ", shift);
        decrypt(cipherText, shift);
    }

    return 0;
}

void decrypt(char cipher[], int shift) {

    int k;
    char decrypted[100];


    for (k = 0; cipher[k] != '\0'; ++k) {
        if (cipher[k] >= 'A' && cipher[k] <= 'Z') {
            decrypted[k] = ((cipher[k] - 'A' - shift + 26) % 26) + 'A';
        } else {
            decrypted[k] = cipher[k];
        }
    }

    decrypted[k] = '\0';
    printf("%s\n", decrypted);
}
