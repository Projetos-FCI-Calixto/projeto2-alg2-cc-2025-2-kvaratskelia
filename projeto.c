#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LARGURA 1024
#define MAX_ALTURA 768

int imagem[MAX_ALTURA][MAX_LARGURA];
int largura, altura;

void mostrarAjuda() {
    printf("Uso: ImageEncoder [-? | -m | -f arquivo]\n");
    printf("Codifica imagens binárias em modo manual ou a partir de arquivo PBM.\n\n");
    printf("Argumentos:\n");
    printf("-?          Mostra esta ajuda\n");
    printf("-m          Entrada manual\n");
    printf("-f arquivo  Lê imagem do arquivo PBM (formato P1)\n");
}

int ehUniforme(int x, int y, int w, int h) {
    int cor = imagem[y][x];
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            if (imagem[i][j] != cor) {
                return -1; // não é uniforme
            }
        }
    }
    return cor; // 0 ou 1
}

void codificar(int x, int y, int w, int h) {
    int cor = ehUniforme(x, y, w, h);

    if (cor != -1) { // imagem homogênea
        if (cor == 0) printf("B");
        else printf("P");
        return;
    }

    // imagem mista -> dividir
    printf("X");

    int metadeW = w / 2;
    int metadeH = h / 2;

    int wEsq = metadeW + (w % 2); // esquerda fica com 1 a mais
    int hCima = metadeH + (h % 2); // cima fica com 1 a mais

    // 1º quadrante (superior esquerdo)
    codificar(x, y, wEsq, hCima);
    // 2º quadrante (superior direito)
    if (w - wEsq > 0)
        codificar(x + wEsq, y, w - wEsq, hCima);
    // 3º quadrante (inferior esquerdo)
    if (h - hCima > 0)
        codificar(x, y + hCima, wEsq, h - hCima);
    // 4º quadrante (inferior direito)
    if (w - wEsq > 0 && h - hCima > 0)
        codificar(x + wEsq, y + hCima, w - wEsq, h - hCima);
}

void lerManual() {
    printf("Digite a largura da imagem: ");
    scanf("%d", &largura);
    printf("Digite a altura da imagem: ");
    scanf("%d", &altura);

    printf("Digite os pixels (0=branco, 1=preto):\n");
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            scanf("%d", &imagem[i][j]);
        }
    }
}

void lerArquivo(char nome[]) {
    FILE *arq = fopen(nome, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[200];
    fgets(linha, 200, arq); // lê P1
    if (strncmp(linha, "P1", 2) != 0) {
        printf("Arquivo inválido (deve começar com P1).\n");
        fclose(arq);
        exit(1);
    }

    // Ignorar comentários
    do {
        fgets(linha, 200, arq);
    } while (linha[0] == '#');

    sscanf(linha, "%d %d", &largura, &altura);

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            fscanf(arq, "%d", &imagem[i][j]);
        }
    }

    fclose(arq);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        mostrarAjuda();
        return 0;
    }

    if (strcmp(argv[1], "-?") == 0) {
        mostrarAjuda();
        return 0;
    }

    if (strcmp(argv[1], "-m") == 0) {
        lerManual();
    }
    else if (strcmp(argv[1], "-f") == 0) {
        if (argc < 3) {
            printf("Você precisa informar o nome do arquivo após -f.\n");
            return 1;
        }
        lerArquivo(argv[2]);
    } else {
        mostrarAjuda();
        return 0;
    }

    printf("\nCódigo gerado: ");
    codificar(0, 0, largura, altura);
    printf("\n");

    return 0;
}