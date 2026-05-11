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
    /* ========================================================================
     * GUIA PARA SOFIA — implementação da carta escolhida
     * ------------------------------------------------------------------------
     * Esta função recebe o índice (0, 1 ou 2) da carta que o jogador escolheu
     * em `ej->escolhas_upgrade[]` e aplica o efeito no jogador.
     *
     * PASSO 1: validar o índice
     *   Se vier algo fora da faixa [0, CARTAS_POR_ESCOLHA-1] (= [0, 2]),
     *   simplesmente `return;` — proteção contra bug em chamador.
     *
     *     if (indice_escolhido < 0 || indice_escolhido >= CARTAS_POR_ESCOLHA)
     *         return;
     *
     * PASSO 2: pegar a carta escolhida em uma variável local
     *
     *     Carta c = ej->escolhas_upgrade[indice_escolhido];
     *
     *   (`c` é uma cópia — você não precisa modificar a original, só ler.)
     *
     * PASSO 3: `switch (c.tipo)` com um case para cada tipo de carta:
     *
     *   case CARTA_VIDA_UP:
     *       /\* sobe o teto E cura por carta.valor *\/
     *       ej->jogador.vida_maxima += c.valor;
     *       ej->jogador.vida        += c.valor;
     *       /\* opcional: clamp se preferir não passar do teto *\/
     *       break;
     *
     *   case CARTA_VELOCIDADE_UP:
     *       /\* velocidade_movimento é float; carta.valor é int — conversão OK *\/
     *       ej->jogador.velocidade_movimento += (float)c.valor;
     *       break;
     *
     *   case CARTA_DANO_UP:
     *       /\* CUIDADO: NÃO iterar `ej->magias_cabeca` somando dano nos nós.
     *        * A lista contém PROJÉTEIS EM VOO; o bônus se perderia assim que
     *        * eles morressem. O correto é um campo persistente no JOGADOR
     *        * que o sistema de magias da Luísa lê ao CRIAR cada projétil.
     *        *
     *        * Sugestão: adicione `int bonus_dano;` em `Jogador` (tipos.h)
     *        * e inicialize com 0 em `jogador_inicializar()` (jogador.c).
     *        * Aqui ficaria:
     *        *
     *        *     ej->jogador.bonus_dano += c.valor;
     *        *
     *        * Alinhe com a Luísa que ela some `ej->jogador.bonus_dano`
     *        * ao `dano` da nova `Magia` no momento do disparo. *\/
     *       break;
     *
     *   case CARTA_MAIS_MAGIAS:
     *       /\* Depende do contrato com o sistema de magias (Dev 3 / Luísa).
     *        * Hoje `magias_atualizar` ainda é stub e não há helper para
     *        * "adicionar magia ativa". Deixe TODO comentando que precisa
     *        * combinar com ela algo tipo:
     *        *
     *        *     magias_adicionar_ativa(ej, ELEMENTO_FOGO);
     *        *
     *        * Enquanto isso, `break;` vazio — a carta aparece e é
     *        * selecionável, só não tem efeito ainda. *\/
     *       break;
     *
     *   case CARTA_RECARGA_DADO:
     *       /\* O struct `Dado` em tipos.h tem só { faces, ultimo_resultado }.
     *        * Não há campo de "usos restantes" ou "carga". Antes de
     *        * implementar, defina o que "recarregar" significa:
     *        *   - somar usos disponíveis (precisa novo campo em `Dado`)?
     *        *   - liberar um dado bloqueado? etc.
     *        * Quando decidir, o efeito atua em `ej->dados_ativos[]`. *\/
     *       break;
     *
     *   default:
     *       break;  /\* tipo desconhecido — ignora *\/
     *
     * PASSO 4: depois que `cartas_aplicar` voltar, o `atualizar_cartas_upgrade`
     * em main.c se encarrega de avançar a onda. Você não precisa mexer em
     * estado de jogo aqui — só no JOGADOR.
     * ====================================================================== */
    (void)ej;
    (void)indice_escolhido;
}

/* ============================================================================
 * GUIA PARA SOFIA — refatoração e correção desta função
 * ----------------------------------------------------------------------------
 * O que está aqui hoje funciona pra desenhar os retângulos coloridos, MAS:
 *
 * 1) BUG na linha do `if(ej->escolhas_upgrade[0].raridade == 5)` lá embaixo
 *    (no bloco da TERCEIRA carta). Tem dois problemas no mesmo lugar:
 *      - o índice está `[0]` — deveria ser `[2]` (terceira carta);
 *      - usa `if` solto em vez de `else if` — quebra a cadeia da carta 3.
 *    Resultado: a borda lendária da terceira carta nunca acende, e qualquer
 *    raridade 5 da PRIMEIRA carta pinta dourado em CIMA da borda da terceira.
 *
 * 2) REPETIÇÃO: existem três blocos `if/else if` idênticos, um para cada
 *    carta — só muda o índice `[0]` / `[1]` / `[2]` e o `x`. Dá pra colapsar
 *    em UM loop com array de cores e zero copy-paste:
 *
 *      const Color CORES_RARIDADE[6] = { GRAY, GREEN, BLUE, PURPLE, RED, GOLD };
 *      for (int i = 0; i < CARTAS_POR_ESCOLHA; i++) {
 *          Carta c = ej->escolhas_upgrade[i];
 *          int x = INICIO_CARTA_X + (GAP_CARTAS * i);
 *          int y = INICIO_CARTA_Y;
 *          Color borda = (c.raridade >= 0 && c.raridade < 6)
 *                          ? CORES_RARIDADE[c.raridade] : GRAY;
 *          DrawRectangle(x - 2, y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, borda);
 *          DrawRectangle(x,     y,     LARGURA_CARTA,     ALTURA_CARTA,     BLACK);
 *
 *          /\* === o que ainda falta: TEXTO === *\/
 *          DrawText(c.nome,      x + 12, y + 16, 18, WHITE);
 *          DrawText(c.descricao, x + 12, y + 60, 14, LIGHTGRAY);
 *          char rodape[32];
 *          snprintf(rodape, sizeof rodape, "Pressione %d", i + 1);
 *          DrawText(rodape, x + 12, y + ALTURA_CARTA - 36, 18, YELLOW);
 *      }
 *
 *    Lembrete: strings dentro de `DrawText` ficam SEM acento (fonte default
 *    da Raylib é ASCII puro). O `c.nome` e `c.descricao` já vêm sem acento
 *    da TABELA_CARTAS, então tudo certo.
 *
 * 3) Quando o loop estiver pronto, dá pra remover o `(void)ej;` do final —
 *    ele só está aí porque a versão atual não usa `ej` em todos os ramos.
 * ========================================================================== */
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
