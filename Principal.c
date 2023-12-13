
//********************************************
// Aluno: Théo Pinheiro Alcântara
// Matricula: 20231045050173
// Avaliação 04: Trabalho final
// 04.505.23 − 2023.2 − Prof. Daniel Ferreira
// Compilado: gcc
//********************************************

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct dados {
    int lin;
    int col;
    int max;
    int tipo;
    unsigned char *data;
    unsigned char *data2;
};

void cabecalhoCSV(char *, int);
void lerMatriz(struct dados *, struct dados *, char *);
void suavizarMatriz(struct dados *, struct dados *, int );
void quantizarMatriz(struct dados *, int);
void matrizSCM(struct dados *, struct dados *, int);
void escrevermatriz(struct dados *, char *, int , char);

int main(int argc, char **argv) {
    struct dados matriz, matrizsuav;


    if (argc != 5) {
        printf("Formato: \n\t %s quantização suavização <imagemEntrada.pgm> <imagemEntrada2.pgm> <imagemSaida.pgm>\n", argv[0]);
        exit(1);
    }

    cabecalhoCSV(argv[4], atoi(argv[2]));
    
    DIR *d;
    struct dirent *dir;
    d = opendir(argv[3]);
    if (!d) {
        perror("Erro ao abrir o diretório");
        exit(1);
    }

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue; // Ignora os diretórios . e ..
        }
        char *caminho1 = malloc(strlen(dir->d_name) + strlen(argv[3]) + 3);
        strcpy(caminho1, argv[3]);
        strcat(caminho1, "/");
        strcat(caminho1, dir->d_name);

        char *nomeArquivo =  dir->d_name;
        char primeiroCaractere = nomeArquivo[0];

        printf("Processando: %s\n", caminho1);

        lerMatriz(&matriz, &matrizsuav, caminho1);
        
        suavizarMatriz(&matriz, &matrizsuav, atoi(argv[1]));

        //Quantizar a matriz original
        quantizarMatriz(&matriz, atoi(argv[2]));

        //quantizar a matriz borrada
        quantizarMatriz(&matrizsuav, atoi(argv[2]));

        //Fazer a coocorrência
        matrizSCM(&matriz, &matrizsuav, atoi(argv[2]));

        

        escrevermatriz(&matriz, argv[4], atoi(argv[2]), primeiroCaractere);

        free(caminho1);

          
    }
    
    closedir(d);
    free(matriz.data);
    free(matrizsuav.data);
    free(matriz.data2);

    return 0;
}

void lerMatriz(struct dados *x, struct dados *y, char *filename){ 
    //Leitura do arquivo PGM
    
    char ch;
    FILE *fp;
    

    if ((fp = fopen(filename, "r"))==NULL){
    printf("O arquivo não pode ser aberto");
    exit(1);
    }

    if ( (ch = getc(fp))!='P'){
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}

    x->tipo = getc(fp)-48;

    fseek(fp,1, SEEK_CUR);

    while((ch=getc(fp))=='#'){
		while( (ch=getc(fp))!='\n');
	}


    fscanf(fp, "%d %d %d",&x->col, &x->lin, &x->max);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	
    
    x->data = (unsigned char*) malloc(x->col * x->lin * sizeof(unsigned char));
    y->data = (unsigned char*) malloc(x->col * x->lin * sizeof(unsigned char));
    y->lin = x->lin;
    y->col = x->col;
    y->max = x->max;

    puts("Lendo imagem PGM (dados em binário)");
    fread(x->data, sizeof(unsigned char), x->col * x->lin, fp);
    
        
    fclose(fp);
}

void quantizarMatriz(struct dados *x, int nivel){
    x->data2 = (unsigned char*) malloc(nivel *nivel * sizeof(unsigned char));
    
    float intv = (float)(x->max + 1)/nivel;
    int m = 0;
    for(int i = 0; i  < x->col * x->lin; i++){
        while(m <= nivel){
            if((x->data[i]) >= (intv*m) && (x->data[i]) < (intv*(m+1))){
                x->data[i] = m;
                break;
            }
            m++;
        }
        m = 0;
    }
        
        
}

void escrevermatriz(struct dados *x, char *filename, int n, char c){
    FILE *fp;
    
    if (!(fp = fopen(filename,"ab"))){
		perror("Erro.");
		exit(1);
	}

    for (int k=0; k < n * n; k++){
        fprintf(fp, "%d", x->data2[k] );
        fputc(',', fp);
    
    }

    fseek( fp, 0, SEEK_END );
    
    if(c == '0'){
        fputs("epithelium\n", fp);
    } else if(c == '1'){
        fputs("stroma\n", fp);
    }
    
    fclose(fp);
}

void matrizSCM(struct dados *x, struct dados *y, int n){
    int a, b;
    
    for(int i = 0; i  <  x->lin * x->col; i++){
        a = x->data[i];
        b = y->data[i];
        x->data2[a * n + b] += 1;
    }

}

void suavizarMatriz(struct dados *x, struct dados *y, int n) {
    // Fazer a média
    for (int i = 0; i < x->lin; i++) {
        for (int j = 0; j < x->col; j++) {
            int soma = 0;
            
            for (int z = -n/2; z <= n/2; z++) {
                for (int h = -n/2; h <= n/2; h++) {
                    int linha = i + z;
                    int coluna = j + h;

                    if (linha >= 0 && linha < x->lin && coluna >= 0 && coluna < x->col) {
                        soma += x->data[linha * x->col + coluna];
                    }
                } 
            }

            y->data[i * x->col + j] = soma / (n * n);
        }
    }
}

void cabecalhoCSV(char *filename, int n) {
    FILE *fp;

    if (!(fp = fopen(filename, "a+"))) {
        perror("Erro.");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);  

    
        for (int i = 0; i <= n * n; i++) {
            fprintf(fp, "%d", i);
           if (i < n * n) {
                fputc(',', fp);
            }
        }
        fprintf(fp, "\n");

    fclose(fp);
}
