# Trab_SB_Esteganografia

# 1 Introdução

Atualmente a criptografia é amplamente empregada quando se deseja enviar uma mensagem confidencial. Isso é feito em praticamente todas as 
aplicações que envolvem trocas de dados, como navegadores, email, mensagens instantâneas, etc. Porém, há uma outra forma de ocultar 
informação, denominada esteganografia. Ela consiste em aplicar técnicas capazes de ocultar informações dentro de outros arquivos como 
imagens, músicas ou vídeos. O objetivo deste trabalho é praticar a manipulação de memória em baixo nível, implementando uma técnica 
simples de esteganografia em imagens.



# 2 Funcionamento

Deverão ser criados dois programas:

Codificador: lê uma imagem, codifica uma mensagem nele, grava um novo arquivo com o resultado
Decodificador: lê a imagem com as informações ocultas, decodifica e exibe a mensagem na tela

Uma imagem é geralmente representada por uma matriz de pontos (pixels) onde cada cor é definida por 3 componentes: vermelho (R), verde (G)
e azul (B). Cada uma dessas componentes usualmente é codificada em um byte, o que produz 3 bytes por pixel (24 bits) - ou seja, 16 milhões
de possíveis cores. Em outras palavras, as intensidades (R, G, B) variam de 0 a 255, onde 0 é escuro e 255 é claro.

# 2.1 Codificador

O codificador deverá:
- Ler a imagem de entrada
- Solicitar uma senha qualquer
- Solicitar a mensagem a ser armazenada
- Aplicar alguma técnica simples de criptografia, a fim de substituir os caracteres originais da imagem por outros. A técnica escolhida deverá permitir a volta aos caracteres originais, se a mesma senha for aplicada na mensagem criptografada.
- Utilizar alguma técnica de esteganografia, incorporar a mensagem na imagem original
- Salvar o arquivo resultante em formato BMP (formato suportado pela biblioteca SOIL)

# 2.2 Decodificador

O decodificador deverá:
- Ler a imagem com as informações ocultas em formato BMP
- Solicitar a senha (mesma utilizada na etapa de codificação)
- Recuperar os caracteres a partir do conteúdo da imagem
- Reverter a criptografia e gerar a mensagem original, exibindo ela na tela

# 3 Implementando Esteganografia

Há diversas técnicas para realizar o processo de esteganografia em imagens, com graus diferentes de complexidade. Uma técnica simples é
a alteração dos bits menos significativos de cada pixel. Essa técnica consiste em substituir um ou mais bits menos significativos de 
cada componente de cor de um pixel por uma parte da mensagem. Como temos 8 bits para R, G e B, isso em geral altera muito pouco a cor 
de cada pixel.


Por exemplo, se trocarmos os últimos dois bits de um pixel 100% vermelho (11111111) para 11111101, isso implica em alterar o valor da 
componente vermelha de 255 para 253, o que é praticamente imperceptível a olho nu. Por exemplo: Para representar uma palavra de 3 letras
(24 bits = 8 cada letra) são necessários 4 pixels. 
