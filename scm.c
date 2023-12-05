#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct dados{
    int lin;
    int col;
    int max;
};

void gerarMatriz(struct dados *, int *);

void quantizarMatriz(struct dados *, int *);

void escrevermatriz(struct dados *, int *, char *);

int main(int argc, char **argv){
    struct dados matriz;

    char ch;
    if (argc!=3){
		printf("Formato: \n\t %s <imagemEntrada.pgm> <imagemSaida.pgm>\n",argv[0]);
		exit(1);
	}
    
    int *p = NULL;
    FILE *fp;

    if ((fp = fopen(argv[1], "r"))==NULL){
    printf("O arquivo não pode ser aberto");
    exit(1);
    }

    if ( (ch = getc(fp))!='P'){
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}

    fseek(fp,1, SEEK_CUR);

    fscanf(fp, "%d %d %d",&matriz.col, &matriz.lin, &matriz.max);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	



    p = (int*) malloc( matriz.lin * matriz.col * sizeof(int));

    srand(time(NULL));

    // criar matriz
    puts("Matriz recebida: ");
    gerarMatriz(&matriz, p);
    
    printf("\n=================\nMatriz quantizada: \n");
    
    quantizarMatriz(&matriz, p);
    
    escrevermatriz(&matriz, p, argv[2]);

    fclose(fp);
    return 0;
}

void gerarMatriz(struct dados *x, int *p){ 
    for (int k=0; k < x->lin * x->col; k++){
        *(p+k) = rand() % x->max;
       
        if (!(k % x->col)) printf("\n");
        
        printf("%5d ",*(p+k));
    }
}

void quantizarMatriz(struct dados *x, int *p){
        for (int k=0; k < x->lin * x->col; k++){
            if (0 < *(p+k) && *(p+k) <= 31){
                *(p+k) = 0;
            } else if (31 < *(p+k) && *(p+k) <= 63){
                *(p+k) = 1;
            } else if (63 < *(p+k) && *(p+k) <= 95){
                *(p+k) = 2;
            } else if (95 < *(p+k) && *(p+k) <= 127){
                *(p+k) = 3;
            } else if (127 < *(p+k) && *(p+k) <= 159){
                *(p+k) = 4;
            } else if (159 < *(p+k) && *(p+k) <= 191){
                *(p+k) = 5;
            } else if (191 < *(p+k) && *(p+k) <= 223){
                *(p+k) = 6;
            } else if (223 < *(p+k) && *(p+k) <= 255){
                *(p+k) = 7;
            } 
            
            if (!(k % x->col)) printf("\n");
            
            printf("%5d ",*(p+k));
        }
}

void escrevermatriz(struct dados *x, int *p, char *filename){
    FILE *fp;
    
    if (!(fp = fopen(filename,"wb"))){
		perror("Erro.");
		exit(1);
	}

    for (int k=0; k < x->lin * x->col; k++){
        fprintf(fp, "%2d", *(p + k) );

    
    }

    fseek( fp, 0, SEEK_END );
    fputs(" stroma", fp);
 
    
    fclose(fp);
}
