#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

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

void separaNumero(char vetor, unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1);

void funcaoPrincipal(char vetor[], int num_pixels, Img pic);

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
    char vetor[] = "Vamos fazer uma casa em cima da casa da sua mae";
    int tamanho = strlen(vetor);

    int num_pixels = (tamanho + 0) * 4;

    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);


    funcao(vetor, tamanho);

    printf("Primeiros 10 pixels da imagem:\n");
    for(int i=0; i<10; i++) {
        printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }
    printf("\n");

    funcaoPrincipal(vetor, num_pixels, pic);

    printf("Primeiros 10 pixels da imagem:\n");
    for(int i=0; i<10; i++) {
        printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }
    printf("\n");

    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(pic.img);
}
    // FUNCIONANDO
    // Falta fazer uma função q altera a letra dentro do laço de RGB e controlar o tamanho
    // Pensar em um jeito do laço nao andar 4 (talvez uma verificação do i com break pra sair do laço) ;


    // Preapara a letra para passar pro RGB.

    //for(int i=0; i<tamanho;i++){

void separaNumero(char vetor, unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1){

    unsigned int mask2 = 0b00000011;
    int aux;
    aux = vetor;

    unsigned char palavra = aux;
    *bit8_7 = palavra>>6;
    *bit6_5 = palavra>>4;
    *bit4_3 = palavra>>2;
    *bit2_1 = palavra;

    //printf("bit 8 e 7: %d\nbit 6 e 5: %d\nbit 4 e 3: %d\nbit 2 e 1: %d\n\n\n", bit8_7,bit6_5,bit4_3,bit2_1);

    *bit8_7 = *bit8_7 & mask2;
    *bit6_5 = *bit6_5 & mask2;
    *bit4_3 = *bit4_3 & mask2;
    *bit2_1 = *bit2_1 & mask2;

    //printf("bit 8 e 7: %d\nbit 6 e 5: %d\nbit 4 e 3: %d\nbit 2 e 1: %d\n\n\n", bit8_7,bit6_5,bit4_3,bit2_1);
}


    // Bota a letra no RGB

void funcaoPrincipal(char vetor[], int num_pixels, Img pic){

    int j = 0; //contador da palavra a cada laco 3 letras

    unsigned int mask = 0b11111100;

    for(int i=1; i<num_pixels; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;

        //printf("[%d %d %d %d]\n\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        pic.img[i].r = pic.img[i].r & mask;
        pic.img[i].g = pic.img[i].g & mask;
        pic.img[i].b = pic.img[i].b & mask;
        pic.img[i+1].r = pic.img[i+1].r & mask;

        //printf("[%d %d %d %d]\n\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        separaNumero(vetor[j], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i].r = pic.img[i].r | bit8_7;
        pic.img[i].g = pic.img[i].g | bit6_5;
        pic.img[i].b = pic.img[i].b | bit4_3;
        pic.img[i+1].r = pic.img[i+1].r | bit2_1;

        //printf("[%d %d %d %d]\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        if((i+2) == num_pixels){
            break;
        }

        pic.img[i+1].g = pic.img[i+1].r & mask;
        pic.img[i+1].b = pic.img[i+1].r & mask;
        pic.img[i+2].r = pic.img[i+2].r & mask;
        pic.img[i+2].g = pic.img[i+2].r & mask;

        separaNumero(vetor[j+1], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i+1].r = pic.img[i+1].r | bit8_7;
        pic.img[i+1].g = pic.img[i+1].g | bit6_5;
        pic.img[i+2].b = pic.img[i+2].b | bit4_3;
        pic.img[i+2].r = pic.img[i+2].r | bit2_1;


        if((i+3) == num_pixels){
            break;
        }

        pic.img[i+2].b = pic.img[i+2].r & mask;
        pic.img[i+3].r = pic.img[i+3].r & mask;
        pic.img[i+3].g = pic.img[i+3].r & mask;
        pic.img[i+3].b = pic.img[i+3].r & mask;

        separaNumero(vetor[j+2], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i+2].r = pic.img[i+2].r | bit8_7;
        pic.img[i+3].g = pic.img[i+3].g | bit6_5;
        pic.img[i+3].b = pic.img[i+3].b | bit4_3;
        pic.img[i+3].r = pic.img[i+3].r | bit2_1;

        j = j + 3;
}
/*
    printf("Primeiros 10 pixels da imagem:\n");
    for(int i=0; i<10; i++) {
        printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }
    printf("\n");

    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(pic.img);
*/
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
    }
    //printf("%s", vetor);
}
