#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void suavizarMatriz(struct dados *x, struct dados *y, int *n){
   int matriz[x->lin * x->col];
   
   // fazer a média
   for (int i = 0; i < x->lin; i++) {
        for (int j = 0; j < x->col; j++) {
            int soma = 0;    
            
            for (int z = -n/2; z <= n/2; z++){
                for (int h = -n/2; h <= n/2; h++){

                    int linha = i + z;
                    int coluna = j + h;

                
                    if (linha >= 0 && linha < x->lin && coluna >= 0 && coluna < x->col) {
                        soma += matriz[linha * x->col + coluna];
                    }

                } 
            }
     
            y->data[i * x->col + j] = soma/ (n * n);
        }
       
    }
}
