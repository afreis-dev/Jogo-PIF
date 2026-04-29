
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

DadosCarta TABELA_CARTAS[CARTA_TOTAL][3] = {
    
    {
        { "Dano nvl comum",    "Aumenta o dano das suas magias.\n+%d de dano.",  10 },
        { "Dano nvl raro",   "Aumenta o dano das suas magias.\n+%d de dano.",  20 },
        { "Dano nvl lendario",  "Aumenta o dano das suas magias.\n+%d de dano.",  35 },
    },
    
    {
        { "Vida nvl comum",    "Aumenta seu HP maximo.\n+%d de HP.",  15 },
        { "Vida nvl raro",   "Aumenta seu HP maximo.\n+%d de HP.",  30 },
        { "Vida nvl lendario",  "Aumenta seu HP maximo.\n+%d de HP.",  50 },
    },
    
    {
        { "Velocidade nvl comum",    "Aumenta sua velocidade de movimento.\n+%d de velocidade.",   5 },
        { "Velocidade raro",   "Aumenta sua velocidade de movimento.\n+%d de velocidade.",  10 },
        { "Velocidade lendario",  "Aumenta sua velocidade de movimento.\n+%d de velocidade.",  18 },
    },
    
    {
        { "Magia nvl comum",    "Adiciona mais uma magia ativa.\n+%d magia.",   1 },
        { "Magia nvl raro",   "Adiciona mais magias ativas.\n+%d magias.",    2 },
        { "Magia nvl lendario",  "Adiciona mais magias ativas.\n+%d magias.",    3 },
    },
    
    {
        { "Recarregar Dado",    "Devolve um dado para voce.\n+%d dado.",    1 },
        { "Recarregar 2 Dados", "Devolve dois dados para voce.\n+%d dados.", 2 },
        { "Recarregar 3 Dados", "Devolve tres dados para voce.\n+%d dados.", 3 },
    },
};

void cartas_gerar_escolhas(EstadoJogo *ej) {

    for (int i = 0; i < CARTAS_POR_ESCOLHA; i++) {
        int tipo    = rand() % CARTA_TOTAL;
        int sorteio = rand() % 10;
        int raridade;

        if (sorteio < 6)      raridade = 0;  
        else if (sorteio < 9) raridade = 1;  
        else                  raridade = 2;  

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

    DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    DrawRectangle(INICIO_CARTA_X , INICIO_CARTA_Y , LARGURA_CARTA, ALTURA_CARTA, BLACK);
    

    DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    DrawRectangle(INICIO_CARTA_X + GAP_CARTAS , (INICIO_CARTA_Y), LARGURA_CARTA, ALTURA_CARTA, BLACK);

    DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) , (INICIO_CARTA_Y), LARGURA_CARTA, ALTURA_CARTA, BLACK);

    (void)ej; 
}
