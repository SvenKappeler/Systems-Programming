#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void encrypt(char str[]);
void decrypt(char str[]);

int k = 5;

int main(){
    char cmd1[] = "encrypt", cmd2[] = "decrypt", cmd3[] = "exit";
    char input[80];
    char d[] = "()\n";
    char e[] = "\n";
    char *convert;

    fgets(input, 80, stdin);

    //Breaks up 'input' whenver a 'd' occurs
    char *command = strtok(input, d);

    // Parse for Encrypt
    if((strcmp(command, cmd1)) == 0){
        convert = strtok(NULL, d);
        encrypt(convert);
    }
    // Parse for Decrypt 
    else if((strcmp(command, cmd2)) == 0){
        convert = strtok(NULL, e);
        decrypt(convert);
    }
    // Parse for Exit Command
    else if((strcmp(command, cmd3)) == 0){
        return 0; 
    }
    // Catch for incorrect commands
    else{
        printf("Error, please enter the command correctly\n");
        main();
    }
}

void encrypt(char str[]){
    // Iterate through the String
    for(int i = 0; i < strlen(str); i++){
        // If the resulting ASCII will be less than 128
        if( (str[i] + k) < 128 ){
            printf("%c", str[i] + k);
        }
        // Restart going through the ASCII
        else{
            printf("%c", str[i] - 124);
        }
    }
    printf("\n");
    main();
}

void decrypt(char str[]){
    for( int i = 0; i < (strlen(str) - 1); i++){
        if( (str[i] - k) >= 0 ){
            printf("%c", str[i] - k);
        }
        else{
            printf("%c", str[i] + 124);
        }
    }
    printf("\n");
    main();
}