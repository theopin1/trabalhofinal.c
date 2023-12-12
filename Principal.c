#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct dados{
    int lin;
    int col;
    int max;
    unsigned char *data;
    unsigned char *data2;
};

void lerMatriz(struct dados *, char *);

void quantizarMatriz(struct dados *, int *);

void escrevermatriz(struct dados *, char *);

void matrizSCM(struct dados *, struct dados *);

int main(int argc, char **argv){
    struct dados matriz, matrizsuav;
    
    
    if (argc!=5){
		printf("Formato: \n\t %s quantização suavização <imagemEntrada.pgm> <imagemEntrada2.pgm> <imagemSaida.pgm>\n",argv[0]);
		exit(1);
	}
    
    DIR *d;
    struct dirent *dir;
    d = opendir(argv[1]);
    if (d){
         while ((dir = readdir(d)) != NULL)
        {
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue; // Ignora os diretórios . e ..
            }
            char *caminho1 = malloc(strlen(dir->d_name) +  strlen(argv[3]) + 2);

			strcpy(caminho1, argv[3]);
			strcat(caminho1, "/");
			strcat(caminho1, dir->d_name);

			printf("%s\n", dir->d_name);

	
            lerMatriz(&matriz, caminho1);

            suavizarMatriz(&matriz, &matrizsuav, argv[2]);
    
            quantizarMatriz(&matriz, argv[1]);

            quantizarMatriz(&matrizsuav, argv[1]);

            matrizSCM(&matriz, &matrizsuav);
    
            escrevermatriz(&matriz, argv[4]);

            free(caminho1);
    
        }
        closedir(d);


    }
    return 0;
}

void lerMatriz(struct dados *x, char *filename){ 
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

    fseek(fp,1, SEEK_CUR);

    fscanf(fp, "%d %d %d",&x->col, &x->lin, &x->max);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	
    
    x->data = (unsigned char*) malloc(x->col * x->lin * sizeof(unsigned char));

    puts("Lendo imagem PGM (dados em binário)");
    fread(x->data, sizeof(unsigned char), x->col * x->lin, fp);
    
        
    fclose(fp);
}

void quantizarMatriz(struct dados *x, int *nivel){
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

void escrevermatriz(struct dados *x, char *filename){
    FILE *fp;
    
    if (!(fp = fopen(filename,"ab"))){
		perror("Erro.");
		exit(1);
	}

    for (int k=0; k < x->lin * x->col; k++){
        fprintf(fp, "%2d", x->data[k] );
        fputc(',', fp);
    
    }

    fseek( fp, 0, SEEK_END );
    fputs(" stroma\n", fp);
 
    
    fclose(fp);
}

void matrizSCM(struct dados *x, struct dados *y){
    
    for(int i = 0; i  < x->col * x->lin; i++){
        x->data2[i] = 0;
    }
    
    for(int i = 0; i  < x->col * x->lin; i++){
        a = x->data[i];
        b = y->data[i];
        x->data2[a * x->col + b] += 1;
    }

}
