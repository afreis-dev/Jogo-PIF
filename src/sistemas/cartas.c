#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartas.h"

#define INICIO_CARTA_X  160
#define INICIO_CARTA_Y  200
#define LARGURA_CARTA   240
#define ALTURA_CARTA    320
#define GAP_CARTAS      360


typedef struct {
    char *nome;
    char *descricao;
    int valor;
} DadosCarta;

DadosCarta TABELA_CARTAS[CARTA_TOTAL][6] = {
    
    {
        { "Dano nvl comum",    "Aumenta o dano das suas magias.\n+%d de dano.",  10 },
        { "Dano nvl incomum",    "Aumenta o dano das suas magias.\n+%d de dano.",  15 },
        { "Dano nvl raro",   "Aumenta o dano das suas magias.\n+%d de dano.",  20 },
        { "Dano nvl epico",   "Aumenta o dano das suas magias.\n+%d de dano.",  25 },
        { "Dano nvl mitico",   "Aumenta o dano das suas magias.\n+%d de dano.",  30 },
        { "Dano nvl lendario",  "Aumenta o dano das suas magias.\n+%d de dano.",  35 },
    },
    
    {
        { "Vida nvl comum",    "Aumenta seu HP maximo.\n+%d de HP.",  15 },
        { "Vida nvl incomum",    "Aumenta seu HP maximo.\n+%d de HP.",  20 },
        { "Vida nvl raro",   "Aumenta seu HP maximo.\n+%d de HP.",  30 },
        { "Vida nvl epico",   "Aumenta seu HP maximo.\n+%d de HP.",  35 },
        { "Vida nvl mitico",   "Aumenta seu HP maximo.\n+%d de HP.",  40 },
        { "Vida nvl lendario",  "Aumenta seu HP maximo.\n+%d de HP.",  50 },
    },
    
    {
        { "Velocidade nvl comum",    "Aumenta sua velocidade de movimento.\n+%d de velocidade.",   5 },
        { "Velocidade nvl incomum",    "Aumenta sua velocidade de movimento.\n+%d de velocidade.",   10 },
        { "Velocidade raro",   "Aumenta sua velocidade de movimento.\n+%d de velocidade.",  15 },
        { "Velocidade epico",   "Aumenta sua velocidade de movimento.\n+%d de velocidade.",  20 },
        { "Velocidade mitico",   "Aumenta sua velocidade de movimento.\n+%d de velocidade.",  25 },
        { "Velocidade lendario",  "Aumenta sua velocidade de movimento.\n+%d de velocidade.",  18 },
    },
    
    {
        { "Magia nvl comum",    "Adiciona mais uma magia ativa.\n+%d magia.",   1 },
        { "Magia nvl incomum",    "Adiciona mais uma magia ativa.\n+%d magia.",   2 },
        { "Magia nvl raro",   "Adiciona mais magias ativas.\n+%d magias.",    3 },
        { "Magia nvl epico",   "Adiciona mais magias ativas.\n+%d magias.",    4 },
        { "Magia nvl mitico",   "Adiciona mais magias ativas.\n+%d magias.",    5 },
        { "Magia nvl lendario",  "Adiciona mais magias ativas.\n+%d magias.",    6 },
    },
    
    {
        { "Recarregar Dado",    "Devolve um dado para voce.\n+%d dado.",    1 },
        { "Recarregar 2 Dados", "Devolve dois dados para voce.\n+%d dados.", 2 },
        { "Recarregar 3 Dados", "Devolve tres dados para voce.\n+%d dados.", 3 },
        { "Recarregar 4 Dados", "Devolve tres dados para voce.\n+%d dados.", 4 },
        { "Recarregar 5 Dados", "Devolve tres dados para voce.\n+%d dados.", 5 },
        { "Recarregar 6 Dados", "Devolve tres dados para voce.\n+%d dados.", 6 },
    },
};

void cartas_gerar_escolhas(EstadoJogo *ej) {

    for (int i = 0; i < CARTAS_POR_ESCOLHA; i++) {
        int tipo    = rand() % CARTA_TOTAL;
        int sorteio = rand() % 100;
        int raridade;

        if (sorteio < 55)      raridade = 0;  
        else if (sorteio < 75) raridade = 1;  
        else if (sorteio < 85) raridade = 2;  
        else if (sorteio < 93) raridade = 3;  
        else if (sorteio < 98) raridade = 4;  
        else  raridade = 5;  
        

        ej->escolhas_upgrade[i].tipo     = tipo;
        ej->escolhas_upgrade[i].raridade = raridade;
        ej->escolhas_upgrade[i].valor    = TABELA_CARTAS[tipo][raridade].valor;

        strncpy(ej->escolhas_upgrade[i].nome, TABELA_CARTAS[tipo][raridade].nome, 64);

        snprintf(ej->escolhas_upgrade[i].descricao, 256, TABELA_CARTAS[tipo][raridade].descricao, TABELA_CARTAS[tipo][raridade].valor);
    
    }
}


void cartas_aplicar(EstadoJogo *ej, int indice_escolhido) {
    (void)ej;
    (void)indice_escolhido; 
}

void cartas_desenhar_ui(const EstadoJogo *ej) {

    if(ej->escolhas_upgrade[0].raridade == 0){
        DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GRAY);
    }
    else if(ej->escolhas_upgrade[0].raridade == 1){
        DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GREEN);
    }
    else if(ej->escolhas_upgrade[0].raridade == 2){
        DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    }
    else if(ej->escolhas_upgrade[0].raridade == 3){
        DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, PURPLE);
    }
    else if(ej->escolhas_upgrade[0].raridade == 4){
        DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, RED);
    }
    else if(ej->escolhas_upgrade[0].raridade == 5){
        DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GOLD);
    }
    DrawRectangle(INICIO_CARTA_X , INICIO_CARTA_Y , LARGURA_CARTA, ALTURA_CARTA, BLACK);
    
    

    if(ej->escolhas_upgrade[1].raridade == 0){
        DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GRAY);
    }
    else if (ej->escolhas_upgrade[1].raridade == 1){
        DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GREEN);
    }
    else if (ej->escolhas_upgrade[1].raridade == 2){
        DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    }
    else if (ej->escolhas_upgrade[1].raridade == 3){
        DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, PURPLE);
    }
    else if (ej->escolhas_upgrade[1].raridade == 4){
        DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, RED);
    }
    else if(ej->escolhas_upgrade[1].raridade == 5){
        DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GOLD);
    }
    DrawRectangle(INICIO_CARTA_X + GAP_CARTAS , (INICIO_CARTA_Y), LARGURA_CARTA, ALTURA_CARTA, BLACK);



    if(ej->escolhas_upgrade[2].raridade == 0){
        DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GRAY);
    }
    else if(ej->escolhas_upgrade[2].raridade == 1){
        DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GREEN);
    }
    else if(ej->escolhas_upgrade[2].raridade == 2){
        DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    }
    else if(ej->escolhas_upgrade[2].raridade == 3){
        DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, PURPLE);
    }
    else if(ej->escolhas_upgrade[2].raridade == 4){
        DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, RED);
    }
    if(ej->escolhas_upgrade[0].raridade == 5){
        DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, GOLD);
    }
    DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) , (INICIO_CARTA_Y), LARGURA_CARTA, ALTURA_CARTA, BLACK);

    (void)ej; 
}
