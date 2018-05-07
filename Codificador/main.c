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

void separaBitsPalavra(char letra ,unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1);

void funcaoPrincipal(char vetor[], int tipo, float num_bytes, Img pic);

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
    char senha[] = "";
    printf("Digite a senha:");
    scanf(" %[^\n]s", senha);
    //scanf("%s", &senha);

    // pega o tamanho da senha e cria outro tamanho somando mais 1 por causa do simbolo
    float tamanhoSenha = strlen(senha);
    int tamanhoSenhaComSimb = tamanhoSenha +1;

    // cria o array com o tamanho com o simb ja incluido
    char senhaComSimb[tamanhoSenhaComSimb];
    strcpy(senhaComSimb, senha);

    //////////////////////////////////////////////////////////////////////////////////

    char mensagem[] = "";
    printf("Digite a mensagem:");
    scanf(" %[^\n]s", mensagem); // Pega mesagem completa incluindo espaços
    //scanf("%s", &mensagem);
    // pega o tamanho da mensagem cria outro tamanho somando mais 1 por causa do simbolo
    float tamanhoMensagem = strlen(mensagem);
    int tamanhoMsgComSimb = tamanhoMensagem +1;


    // Cria um novo vetor com o vetor antigo mais 1 espaço (estava bugando a imagem se tentava concatenar).
    char msgComSimb[tamanhoMsgComSimb];
    strcpy(msgComSimb, mensagem);


    // Calcula o numero de bytes que o laço vai ter q percorrer
    float num_bytesSenha = ((tamanhoSenha+1) * 4) / 3;
    float num_bytes = ((tamanhoMensagem+1) * 4) / 3;

    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);


    //funcao(vetor, tamanho);

    //printf("Primeiros 10 pixels da imagem:\n");
    //for(int i=0; i<8; i++) {
      //  printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    //}
    //printf("\n");


    // Adiciona o simbolo de parada a senha e a mensagem
    char sinal[] = "|"; // ASCIII 124 - simbolo de parada
    strcat(senhaComSimb, sinal);

    //char sinal2[] = "|"; // ASCIII 124 - simbolo de parada
    strcat(msgComSimb, sinal);

    printf("%s\n", senhaComSimb);
    printf("%s\n", msgComSimb);

    funcaoPrincipal(senhaComSimb, 0, ceil(num_bytesSenha), pic); // Arredonda pra cima o tamanho, valor 0 significa que é uma senha
    funcaoPrincipal(msgComSimb, 1, ceil(num_bytes), pic); // Arredonda pra cima o tamanho, valor 1 significa que é uma msg.

    //printf("Primeiros 10 pixels da imagem:\n");
    //for(int i=0; i<8; i++) {
      //  printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    //}
    //printf("\n");

    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(pic.img);
}
    // FUNCIONANDO
    // Falta fazer uma função q altera a letra dentro do laço de RGB e controlar o tamanho
    // Pensar em um jeito do laço nao andar 4 (talvez uma verificação do i com break pra sair do laço) ;


    // Preapara a letra para passar pro RGB.

void separaBitsPalavra(char letra, unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1){

    unsigned int mask2 = 0b00000011;
    int aux;
    aux = letra;

    printf("aux = %d\n", aux);

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
    // O tipo pode ser 0 ou 1.
    // se for 0 significa q o vetor que ele esta passado é uma senha entao ele vai botar a senha na primeira linha da imagem
    // se for 1 significa q o vetor é uma mensagem entao ele vai botar a msg na segunda linhha da imagem.

void funcaoPrincipal(char vetor[], int tipo, float num_bytes, Img pic){

    //printf("num_pixels = %f\n", num_pixels);

    int j = 0; //contador da palavra a cada laco 3 letras

    unsigned int mask = 0b11111100;

    int num_bytes2;
    int iniciaEm;

    if(tipo >= 1){
        num_bytes2 = num_bytes + pic.width; //SEGUNDA LINHA
        iniciaEm = pic.width;
    }else{
        num_bytes2 = num_bytes;
        iniciaEm = 0;
    }

    for(int i=iniciaEm; i<num_bytes2; i= i+4){

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

        separaBitsPalavra(vetor[j], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

       //  printf("bit 8 e 7: %d      bit 6 e 5: %d\nbit 4 e 3: %d      bit 2 e 1: %d\n\n\n", bit8_7,bit6_5,bit4_3,bit2_1);

        pic.img[i].r = pic.img[i].r | bit8_7;
        pic.img[i].g = pic.img[i].g | bit6_5;
        pic.img[i].b = pic.img[i].b | bit4_3;
        pic.img[i+1].r = pic.img[i+1].r | bit2_1;

        //printf("[%d %d %d %d]\n", pic.img[i].r, pic.img[i].g, pic.img[i].b,  pic.img[i+1].r);

        int num1 = i + 2;

        printf("num1 = %d\n", num1);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(num1 >= num_bytes2){
            printf("IF 1");
            break;
        }

        pic.img[i+1].g = pic.img[i+1].g & mask;
        pic.img[i+1].b = pic.img[i+1].b & mask;
        pic.img[i+2].r = pic.img[i+2].r & mask;
        pic.img[i+2].g = pic.img[i+2].g & mask;

        separaBitsPalavra(vetor[j+1], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

       // printf("bit 8 e 7: %d      bit 6 e 5: %d\nbit 4 e 3: %d      bit 2 e 1: %d\n\n\n", bit8_7,bit6_5,bit4_3,bit2_1);

        pic.img[i+1].g = pic.img[i+1].g | bit8_7;
        pic.img[i+1].b = pic.img[i+1].b | bit6_5;
        pic.img[i+2].r = pic.img[i+2].r | bit4_3;
        pic.img[i+2].g = pic.img[i+2].g | bit2_1;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        num1 = i + 3;
        printf("num1 = %d\n", num1);
        if(num1 >= num_bytes2 ){
            printf("IF 2");
            break;

        }

        pic.img[i+2].b = pic.img[i+2].b & mask;
        pic.img[i+3].r = pic.img[i+3].r & mask;
        pic.img[i+3].g = pic.img[i+3].g & mask;
        pic.img[i+3].b = pic.img[i+3].b & mask;

        separaBitsPalavra(vetor[j+2], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i+2].b = pic.img[i+2].b | bit8_7;
        pic.img[i+3].r = pic.img[i+3].r | bit6_5;
        pic.img[i+3].g = pic.img[i+3].g | bit4_3;
        pic.img[i+3].b = pic.img[i+3].b | bit2_1;

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
    printf("%s", vetor);
    }
}
