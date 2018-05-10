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

void descriptografica(char vetor[], int tamanho);

char separaBitsPalavra(unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1);

void decodificacaoEsteganografia(char vetor[], int tipo, Img pic);

int contaTamanho(int tipo, Img pic);

void juntaLetra(char letra, char vetor[], int tamanho);

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

    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load("saida.bmp", &pic);

    int verificador = 0;

    int tamanhoSenha = contaTamanho(0, pic);
    char senhaDescodificada[tamanhoSenha];
    decodificacaoEsteganografia(senhaDescodificada, 0, pic);

    char senha[tamanhoSenha];

    do{
        printf("Digite a senha:");
        scanf(" %[^\n]s", senha);

        if(strcmp(senha, senhaDescodificada) == 0){
            verificador = 1;
        }else{
            printf("\nSenha incorreta! Digite novamente\n\n");
        }
    }while (verificador < 1);

    int tamanhoMsg = contaTamanho(1, pic);
    char msgDecodificada[tamanhoMsg];
    decodificacaoEsteganografia(msgDecodificada, 1, pic);

    descriptografica(msgDecodificada, tamanhoMsg);

    printf("\nMENSAGEM: %s\n", msgDecodificada);

    free(pic.img);
}

char separaBitsPalavra(unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1){

    unsigned char letra = 0b00000000;
    *bit8_7 = *bit8_7<<6;
    *bit6_5 = *bit6_5<<4;
    *bit4_3 = *bit4_3<<2;
    *bit2_1 = *bit2_1;

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
    int tamanho = 0;


    int iniciaEm;
    int num_bytes = pic.width * pic.height;

    if(tipo >= 1){
        iniciaEm = pic.width;
    }else{
        iniciaEm = 0;
    }

    for(int i=iniciaEm; i<num_bytes; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;

        bit8_7 = pic.img[i].r & mask;
        bit6_5 = pic.img[i].g & mask;
        bit4_3 = pic.img[i].b & mask;
        bit2_1 = pic.img[i+1].r & mask;

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(letra == '|'){
            vetor[tamanho] = '\0';
            break;
        }

        vetor[tamanho] = letra;
        tamanho = tamanho +1;

        bit8_7 = pic.img[i+1].g & mask;
        bit6_5 = pic.img[i+1].b & mask;
        bit4_3 = pic.img[i+2].r & mask;
        bit2_1 = pic.img[i+2].g & mask;

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(letra == '|'){
            vetor[tamanho] = '\0';
            break;
        }

        vetor[tamanho] = letra;
        tamanho = tamanho +1;

        bit8_7 = pic.img[i+2].b & mask;
        bit6_5 = pic.img[i+3].r & mask;
        bit4_3 = pic.img[i+3].g & mask;
        bit2_1 = pic.img[i+3].b & mask;

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

         if(letra == '|'){
            vetor[tamanho] = '\0';
            break;
        }

        vetor[tamanho] = letra;
        tamanho = tamanho +1;
    }
}


int contaTamanho(int tipo, Img pic){

    unsigned int mask = 0b00000011;

    char letra;
    int tamanho = 0;

    int iniciaEm;
    int num_bytes = pic.width * pic.height;

    if(tipo >= 1){
        iniciaEm = pic.width;
    }else{
        iniciaEm = 0;
    }

    for(int i=iniciaEm; i<num_bytes; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;

        bit8_7 = pic.img[i].r & mask;
        bit6_5 = pic.img[i].g & mask;
        bit4_3 = pic.img[i].b & mask;
        bit2_1 = pic.img[i+1].r & mask;

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(letra == '|'){
            return tamanho;
            break;
        }

        tamanho = tamanho +1;

        bit8_7 = pic.img[i+1].g & mask;
        bit6_5 = pic.img[i+1].b & mask;
        bit4_3 = pic.img[i+2].r & mask;
        bit2_1 = pic.img[i+2].g & mask;

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(letra == '|'){
            return tamanho;
            break;
        }

        tamanho = tamanho +1;

        bit8_7 = pic.img[i+2].b & mask;
        bit6_5 = pic.img[i+3].r & mask;
        bit4_3 = pic.img[i+3].g & mask;
        bit2_1 = pic.img[i+3].b & mask;

        letra = separaBitsPalavra(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

         if(letra == '|'){
            return tamanho;
            break;
        }

        tamanho = tamanho +1;
    }
    return 0;
}

void descriptografica(char vetor[], int tamanho) {
    int i;

    for(i= 0; i<tamanho; i++){
        if(vetor[i] == 'a'){
            vetor[i] = 122;
        }else if(vetor[i] == 'A'){
            vetor[i] = 90;
        }else if(vetor[i] == '|'){
            break;
        }else if(vetor[i] != ' '){
            vetor[i] = vetor[i] - 1;
        }
    }
}
