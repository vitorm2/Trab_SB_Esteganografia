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

void criptografia(char vetor[], int tamanho);

void separaBitsPalavra(char letra ,unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1);

void esteganografia(char vetor[], int tipo, float num_bytes, Img pic);

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
    load(argv[1], &pic);

    char senha[pic.width]; //Tamanho maximo da senha corresponde a um linha
    printf("Digite a senha:");
    scanf(" %[^\n]s", senha);

    // pega o tamanho da senha e cria outro tamanho somando mais 1 por causa do simbolo
    float tamanhoSenha = strlen(senha);
    int tamanhoSenhaComSimb = tamanhoSenha +1;

    // cria o array com o tamanho com o simb ja incluido
    char senhaComSimb[tamanhoSenhaComSimb];
    strcpy(senhaComSimb, senha);

    //////////////////////////////////////////////////////////////////////////////////

    char mensagem[(pic.width * pic.height) - pic.width]; // tamnaho maixmo da mensagem é a partir da segunda linha ate o final da imagem.
    printf("Digite a mensagem:");
    scanf(" %[^\n]s", mensagem); // Pega mesagem completa incluindo espaços

    // pega o tamanho da mensagem cria outro tamanho somando mais 1 por causa do simbolo
    float tamanhoMensagem = strlen(mensagem);
    int tamanhoMsgComSimb = tamanhoMensagem +1;

    // Cria um novo vetor com o vetor antigo mais 1 espaço (estava bugando a imagem se tentava concatenar).
    char msgComSimb[tamanhoMsgComSimb];
    strcpy(msgComSimb, mensagem);

    // Calcula o numero de bytes que o laço vai ter q percorrer
    float num_bytesSenha = ((tamanhoSenha+1) * 4) / 3;
    float num_bytes = ((tamanhoMensagem+1) * 4) / 3;

    // Adiciona o simbolo de parada a senha e a mensagem
    char sinal[] = "|"; // ASCIII 124 - simbolo de parada
    strcat(senhaComSimb, sinal);

    strcat(msgComSimb, sinal);

    // Criptografa a mensagem
    criptografia(msgComSimb, tamanhoMsgComSimb);

    esteganografia(senhaComSimb, 0, ceil(num_bytesSenha), pic); // Arredonda pra cima o tamanho, valor 0 significa que é uma senha
    esteganografia(msgComSimb, 1, ceil(num_bytes), pic); // Arredonda pra cima o tamanho, valor 1 significa que é uma msg.

    //Salva a imagem
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(senha);
    free(mensagem);
    free(senhaComSimb);
    free(msgComSimb);
    free(pic.img);

    return 1;
}

    // Preapara a letra para passar pro RGB.

void separaBitsPalavra(char letra, unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1){

    unsigned int mask2 = 0b00000011;
    int aux;
    aux = letra;

    //printf("aux = %c\n", aux);

    unsigned char palavra = aux;
    *bit8_7 = palavra>>6;
    *bit6_5 = palavra>>4;
    *bit4_3 = palavra>>2;
    *bit2_1 = palavra;

    *bit8_7 = *bit8_7 & mask2;
    *bit6_5 = *bit6_5 & mask2;
    *bit4_3 = *bit4_3 & mask2;
    *bit2_1 = *bit2_1 & mask2;
}


// Bota a letra no RGB
// O tipo pode ser 0 ou 1.
// se for 0 significa q o vetor que ele esta passado é uma senha entao ele vai botar a senha na primeira linha da imagem
// se for 1 significa q o vetor é uma mensagem entao ele vai botar a msg na segunda linhha da imagem.

void esteganografia(char vetor[], int tipo, float num_bytes, Img pic){

    int j = 0; //contador da palavra a cada laco 3 letras

    unsigned int mask = 0b11111100; // Mascara para limpar os 2 primeiros bits do rgb

    int num_bytes2;
    int iniciaEm;

    if(tipo >= 1){
        num_bytes2 = num_bytes + pic.width;
        iniciaEm = pic.width; // Começa segunda linha
    }else{
        num_bytes2 = num_bytes;
        iniciaEm = 0; // Começa primeira linha
    }

    for(int i=iniciaEm; i<num_bytes2; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;


        int num1 = i + 1;

        if(num1 >= num_bytes2){
            break;
        }

        pic.img[i].r = pic.img[i].r & mask;
        pic.img[i].g = pic.img[i].g & mask;
        pic.img[i].b = pic.img[i].b & mask;
        pic.img[i+1].r = pic.img[i+1].r & mask;

        separaBitsPalavra(vetor[j], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i].r = pic.img[i].r | bit8_7;
        pic.img[i].g = pic.img[i].g | bit6_5;
        pic.img[i].b = pic.img[i].b | bit4_3;
        pic.img[i+1].r = pic.img[i+1].r | bit2_1;

        num1 = i + 2;

        if(num1 >= num_bytes2){
            break;
        }

        pic.img[i+1].g = pic.img[i+1].g & mask;
        pic.img[i+1].b = pic.img[i+1].b & mask;
        pic.img[i+2].r = pic.img[i+2].r & mask;
        pic.img[i+2].g = pic.img[i+2].g & mask;

        separaBitsPalavra(vetor[j+1], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i+1].g = pic.img[i+1].g | bit8_7;
        pic.img[i+1].b = pic.img[i+1].b | bit6_5;
        pic.img[i+2].r = pic.img[i+2].r | bit4_3;
        pic.img[i+2].g = pic.img[i+2].g | bit2_1;

        num1 = i + 3;

        if(num1 >= num_bytes2 ){
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
}


// A criptografia é simples apenas troca a letra atual pela proxima do alfabeto.

void criptografia(char vetor[], int tamanho) {
    int i;

    for(i= 0; i<tamanho; i++){
        if(vetor[i] == 'z'){
            vetor[i] = 97;
        }else if(vetor[i] == 'Z'){
            vetor[i] = 65;
        }else if(vetor[i] == '|'){
            break;
        }else if(vetor[i] != ' '){
            vetor[i] = vetor[i] + 1;
        }
    }
}
