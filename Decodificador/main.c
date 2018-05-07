#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <math.h>

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos

void funcao(char vetor[], int tamanho);

char separaBitsPalavra(unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1);

void decodificacaoEsteganografia(char vetor[], int tipo, Img pic);

void load(char* name, Img* pic);


// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    //printf("%s\n", argv[1]);

    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load("saida.bmp", &pic);

    printf("Primeiros 10 pixels da imagem:\n");
    for(int i=0; i<8; i++) {
        printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }
    printf("\n\n");

    printf("Primeiros 10 pixels da imagem:\n");
    for(int i=pic.width; i<(pic.width + 6); i++) {
        printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }

    printf("\n\n");

    printf("width = %d\n", pic.width);
    char senhaDescodificada[] = "";
    decodificacaoEsteganografia(senhaDescodificada, 0, pic);

    printf("%s\n", senhaDescodificada);
    printf("width = %d\n", pic.width);
    char senha[10]; // AJUSTAR O TAMANHO CONFORME A SENHA DA IMAGEM
    printf("Digite a senha:");
    scanf(" %[^\n]s", senha);

    printf("width = %d\n", pic.width);

    char msgDecodificada[] = "";
    decodificacaoEsteganografia(msgDecodificada, 1, pic);

    free(pic.img);
}

char separaBitsPalavra(unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1){

    unsigned char letra = 0b00000000;
    *bit8_7 = *bit8_7<<6;
    *bit6_5 = *bit6_5<<4;
    *bit4_3 = *bit4_3<<2;
    *bit2_1 = *bit2_1;

//    printf("bit 8 e 7: %d\nbit 6 e 5: %d\nbit 4 e 3: %d\nbit 2 e 1: %d\n\n\n", bit8_7,bit6_5,bit4_3,bit2_1);

    letra = *bit8_7 | letra;
    letra = *bit6_5 | letra;
    letra = *bit4_3 | letra;
    letra = *bit2_1 | letra;


    return letra;
}


    // Bota a letra no RGB
    // O tipo pode ser 0 ou 1.
    // se for 0 significa q o vetor que ele esta passado é uma senha entao ele vai botar a senha na primeira linha da imagem
    // se for 1 significa q o vetor é uma mensagem entao ele vai botar a msg na segunda linhha da imagem.

void decodificacaoEsteganografia(char vetor[], int tipo, Img pic){

    unsigned int mask = 0b00000011;

    char letra;
    int tamanho = 1;


    int iniciaEm;
    int num_bytes = pic.width * pic.height;

    //printf("width = %d\n", pic.width);
    if(tipo >= 1){
        iniciaEm = pic.width;
        printf("iniciaEm = %d\n", iniciaEm);
    }else{
        iniciaEm = 0;
    }

    for(int i=iniciaEm; i<num_bytes; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;


        //printf("[%d %d %d %d]\n\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        printf("i = %d\n", i);
        printf("[%02X %02X %02X %02X]\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        bit8_7 = pic.img[i].r & mask;
        bit6_5 = pic.img[i].g & mask;
        bit4_3 = pic.img[i].b & mask;
        bit2_1 = pic.img[i+1].r & mask;



        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);


        printf("letra = %c\n", letra);

/*
        //char *str2 = malloc(len + 1 + 1 );
        //strcpy(str2, str);
        char str2[tamanho];// = letra;
        //str2[len + 1] = '\0';

        str2[tamanho] = letra;
        strcpy(vetor,str2);
        printf( "str2 = %s\n", str2 );
        printf("vetor = %s\n", vetor);


        free(str2);
        //vetor[] = letra
*/
        // ADICIONAR A LETRA NA MENSAGEM

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if(letra == '|'){
            printf("IF 1");
            break;
        }


       printf("[%02X %02X %02X %02X]\n", pic.img[i+1].r, pic.img[i+1].g, pic.img[i+2].b,  pic.img[i+2].r);

        bit8_7 = pic.img[i+1].g & mask;
        bit6_5 = pic.img[i+1].b & mask;
        bit4_3 = pic.img[i+2].r & mask;
        bit2_1 = pic.img[i+2].g & mask;

       //printf("[%02X %02X %02X %02X]\n\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        printf("letra = %c\n", letra);

        // ADICIONAR A LETRA NA MENSAGEM
/*
        tamanho = tamanho + 1;

        str2[tamanho];
        str2[tamanho] = letra;
        strcpy(vetor,str2);
        printf("str2 = %s\n", str2);
        printf("vetor = %s\n", vetor);

        free(str2);

*/
        if(letra == '|'){
            printf("IF 2");
            break;
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        printf("[%02X %02X %02X %02X]\n", pic.img[i+2].r, pic.img[i+3].g, pic.img[i+3].b,  pic.img[i+3].r);


        bit8_7 = pic.img[i+2].b & mask;
        bit6_5 = pic.img[i+3].r & mask;
        bit4_3 = pic.img[i+3].g & mask;
        bit2_1 = pic.img[i+3].b & mask;

//        printf("[%02X %02X %02X %02X]\n\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);


        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        printf("letra = %c\n", letra);

        // ADICIONAR A LETRA NA MENSAGEM
  /*      tamanho = tamanho + 1;

        str2[tamanho];
        str2[tamanho] = letra;
        strcpy(vetor,str2);
        printf("str2 = %s\n", str2);
        printf("vetor = %s\n", vetor);

        free(str2);
*/
         if(letra == '|'){
            printf("IF 3");
            break;
        }
    }
    return vetor;
}

void funcao(char vetor[], int tamanho) {
    int i;

    for(i= 0; i<tamanho; i++){
        if(vetor[i] == 'z'){
            vetor[i] = 97;
        }else if(vetor[i] == 'Z'){
            vetor[i] = 65;
        }else if(vetor[i] != ' '){
            vetor[i] = vetor[i] + 1;
        }
    printf("%s", vetor);
    }
}
