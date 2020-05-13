/* © João Loureiro & Luís Borges
       COMO, T2 - FEUP 2019
       Prof. Manuel Ricardo
                              */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int lessthan0;
    int lessthan10;
    int lessthan20;
    int lessthan30;
    int lessthan40;
    int lessthan50;
    int lessthan60;
    int total;
} intervals;

void initIntervals(intervals *aux){
    
    printf("\n***************************************************\n");
    printf("*                                                 *\n");
    printf("*                                                 *\n");
    printf("*              TOOL FOR RADIO MOBILE:             *\n");
    printf("*   CUMULATIVE COVERAGE DISTRIBUTION CALCULATOR   *\n");
    printf("*                  COMO-FEUP 2019                 *\n");
    printf("*                                                 *\n");
    printf("*                                                 *\n");
    printf("***************************************************\n\n");
    
    aux->lessthan0  = 0;
    aux->lessthan10 = 0;
    aux->lessthan20 = 0;
    aux->lessthan30 = 0;
    aux->lessthan40 = 0;
    aux->lessthan50 = 0;
    aux->lessthan60 = 0;
    aux->total=0;
}// end of initIntervals

void updateCounters(intervals *aux, double Rx){
    if(Rx < 0.0){
        aux->lessthan0++;}
    if(Rx < 10.0){
        aux->lessthan10++;}
    if(Rx < 20.0){
        aux->lessthan20++;}
    if(Rx < 30.0){
        aux->lessthan30++;}
    if(Rx < 40.0){
        aux->lessthan40++;}
    if(Rx < 50.0){
        aux->lessthan50++;}
    if(Rx < 60.0){
        aux->lessthan60++;}
}//end of updateCounters

double StringToDouble(char* buffer){
    int aux1 = 0;
    double aux2 = 0.0;
    char partial[3];
    partial[0] = buffer[23];
    partial[1] = buffer[24];
    partial[2] = '\0';
    aux1 = atoi(partial);
    partial[0] = buffer[26];
    partial[1] = '\0';
    aux2 = atoi(partial)*0.1;
    aux2 = aux1 + aux2;
    
    if(buffer[21] == 45)
        aux2 = -aux2;
    
    return aux2;
}//end of StringToDouble

void printResults(intervals *aux, int *BestUnit, int counter){
    double hold = 0.0;
    int i = 0;
    
    printf("\nResults for the Cumulative Distribution of Radio Coverage:\n\n");
    hold = 100*(double)(aux->lessthan0)/(double)(aux->total);
    printf("Rx < -120 dBm: %.4lf%%\n", hold);
    hold = 100*(double)(aux->lessthan10)/(double)(aux->total);
    printf("Rx < -110 dBm: %.4lf%%\n", hold);
    hold = 100*(double)(aux->lessthan20)/(double)(aux->total);
    printf("Rx < -100 dBm: %.4lf%%\n", hold);
    hold = 100*(double)(aux->lessthan30)/(double)(aux->total);
    printf("Rx <  -90 dBm: %.4lf%%\n", hold);
    hold = 100*(double)(aux->lessthan40)/(double)(aux->total);
    printf("Rx <  -80 dBm: %.4lf%%\n", hold);
    hold = 100*(double)(aux->lessthan50)/(double)(aux->total);
    printf("Rx <  -70 dBm: %.4lf%%\n", hold);
    hold = 100*(double)(aux->lessthan60)/(double)(aux->total);
    printf("Rx <  -60 dBm: %.4lf%%\n\n", hold);
    
    printf("Conclusions:\n");
    hold = 100*(double)(aux->lessthan0)/(double)(aux->total);
    printf("\nArea without coverage (Rx < -120 dBm): %.4lf %%\n", hold);
    hold = 100*(double)(aux->lessthan10)/(double)(aux->total);
    printf("Area below good coverage (Rx < -110 dBm): %.4lf %%\n", hold);
    hold = 100*(double)(aux->lessthan60)/(double)(aux->total) - 100*(double)(aux->lessthan10)/(double)(aux->total);
    printf("Area with good coverage (-60 dBm > Rx > -110 dBm): %.4lf %%\n", hold);
    
    for(i=2;i < counter;i++){
        hold = 100*(double)BestUnit[i]/(double)aux->total;
        printf("\nThe site/station nr. %d is the Best Unit for %d different locations (%.4lf%%).", i, BestUnit[i], hold);
    }
    hold = 100*(double)BestUnit[0]/(double)aux->total;
    printf("\n%d units don't consider any of the stations as their Best Unit (%.4lf%%).\n", BestUnit[0], hold);
}//end of printResults

int StringToInt(char *buffer){
    char a = buffer[strlen(buffer) - 3];
    return a - '0';
}//end of StringToInt

int main(){
    FILE *f;
    char filename[20];
    char buffer[100];
    int flag = 0;
    int counter = 0;
    double Rx = 0.0;
    intervals resultados;
    int *BestUnit = (int *)calloc(1, sizeof(int));
    int unit = 0;
    
    initIntervals(&resultados);
    
    printf("Insert the name of the Radio Mobile exported file: ");
    scanf("%s",filename);
    
    f = fopen(filename, "r");
    
    while(f == NULL){
        printf("Can't find such file, please try again!\n");
        printf("Insert the name of the Radio Mobile exported file: ");
        scanf("%s",filename);
        f = fopen(filename, "r");
    }
    
    printf("\nYour Units:\n\n");
    
    while(fgets(buffer, 100, f)){
        if(flag){
            Rx = StringToDouble(buffer);
            updateCounters(&resultados, Rx);
            resultados.total++;
            unit = StringToInt(buffer);
            BestUnit[unit]++;
        }//end of if
        
        if(buffer[0] == 'L'){//significa que estamos na ultima linha antes de comecarem os valores
          flag++;
          BestUnit = (int *)realloc(BestUnit, sizeof(int)*(counter)); //no INITIAL.txt o counter vai ser 5, ou seja, criar um vetor BestUnit com 5*4bytes, 5 posiçoes, posicao 0 é os q n consideraram nada, posicao 1 é nada (mobile), posiçao 2 3 4 sao as estaçoes
        }
        else if(buffer[0] != 'L' && flag == 0){
            printf("%s\n", buffer); 
            counter++;
        }
    } //end of while
    printResults(&resultados, BestUnit, counter);
    free(BestUnit);
    return 0;
}//end of main
