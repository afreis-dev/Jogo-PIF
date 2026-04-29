/* ============================================================================
 * profecia.c - IMPLEMENTAÇÃO DO GERADOR DE PROFECIAS
 * ============================================================================
 *
 * CONCEITOS IMPORTANTES USADOS AQUI:
 *   - MATRIZES de strings (requisito obrigatório de PIF): tabelas fixas com
 *     os nomes de cada enum. Consulta direta por índice.
 *   - Gerador pseudoaleatório determinístico via srand(seed) + rand().
 *     Mesma seed = mesma profecia toda vez.
 *   - "static const" nas tabelas: "static" = visível só nesse arquivo;
 *     "const" = somente leitura. Protege contra modificação acidental.
 *
 * ATENÇÃO: as strings das tabelas abaixo VÃO PARA O DrawText() em
 * profecia_desenhar. Como a fonte default do Raylib é ASCII puro, as
 * strings ficam SEM acentos ("Relampago", "Explosao") — se acentuar aqui,
 * vira quadradinho na tela. Os comentários, sim, podem ter acento.
 * ========================================================================== */

#include "profecia.h"
#include "raylib.h"
#include <stdlib.h>   /* srand, rand */
#include <stdio.h>    /* snprintf */


/* ============================================================================
 * TABELAS DE NOMES
 * --------------------------------------------------------------------------
 * MATRIZES (arrays) de strings. Cada índice corresponde ao valor do enum.
 * Ordem tem que bater com a ordem dos enums em tipos.h.
 *
 * Exemplo: nomes_elementos[ELEMENTO_FOGO] = "Fogo"
 *
 * Strings sem acento de propósito (vão pra DrawText, fonte ASCII).
 * ========================================================================== */

static const char *nomes_elementos[ELEMENTO_TOTAL] = {
    "Fogo",
    "Gelo",
    "Relampago",
    "Veneno",
    "Arcano",
    "Sombra"
};

static const char *nomes_condicoes[COND_TOTAL] = {
    "Ao acertar",
    "Ao matar",
    "Vida abaixo de 50%",
    "Vida acima de 80%",
    "A cada 5 segundos",
    "Em acerto critico",
    "No dash",
    "Combo x3",
    "Inicio da onda",
    "Ao receber dano"
};

static const char *nomes_efeitos[EF_TOTAL] = {
    "Explosao em area",
    "Raio em cadeia",
    "Cura",
    "Escudo temporario",
    "Aumenta velocidade",
    "Aumenta dano",
    "Gera orbe girante",
    "Aplica veneno",
    "Teleporta o jogador",
    "Multiplica projetil",
    "Lentidao nos inimigos",
    "Roubo de vida"
};


/* ============================================================================
 * GERAÇÃO DA PROFECIA
 * --------------------------------------------------------------------------
 * srand(seed) reinicia o gerador aleatório com um valor fixo. A partir daí,
 * cada chamada de rand() produz a mesma sequência de números. É por isso
 * que "mesma seed = mesma profecia".
 *
 * rand() % N retorna um valor entre 0 e N-1, que cai exatamente no range
 * de um enum. Pequena imperfeição: "modulo bias" — números baixos têm
 * probabilidade ligeiramente maior. Pra um projeto acadêmico, irrelevante.
 * ========================================================================== */
void profecia_gerar(Profecia *p, unsigned int seed) {
    p->seed = seed;
    srand(seed);

    /* Sorteia os 3 modificadores */
    for (int i = 0; i < 3; i++) {
        p->mods[i].elemento = (Elemento)(rand() % ELEMENTO_TOTAL);
        p->mods[i].condicao = (Condicao)(rand() % COND_TOTAL);
        p->mods[i].efeito   = (Efeito)  (rand() % EF_TOTAL);
    }
}


/* ============================================================================
 * LOOKUPS
 * --------------------------------------------------------------------------
 * Recebem um valor de enum e retornam a string correspondente.
 * Evitam switch gigante em todo lugar.
 * ========================================================================== */
const char *elemento_nome(Elemento e) {
    if (e < 0 || e >= ELEMENTO_TOTAL) return "???";
    return nomes_elementos[e];
}

const char *condicao_nome(Condicao c) {
    if (c < 0 || c >= COND_TOTAL) return "???";
    return nomes_condicoes[c];
}

const char *efeito_nome(Efeito e) {
    if (e < 0 || e >= EF_TOTAL) return "???";
    return nomes_efeitos[e];
}


/* ============================================================================
 * DESENHO
 * --------------------------------------------------------------------------
 * Mostra a profecia formatada na tela. Cada modificador em uma linha,
 * dourado pra dar destaque.
 *
 * snprintf é uma versão "segura" do sprintf: nunca escreve além do tamanho
 * do buffer, evitando buffer overflow.
 * ========================================================================== */
void profecia_desenhar(const Profecia *p) {
    DrawText("PROFECIA", LARGURA_TELA / 2 - 130, 80, 56, GOLD);

    /* Mostra seed pequena abaixo do título, pra identificar a run */
    char buffer_seed[64];
    snprintf(buffer_seed, sizeof(buffer_seed), "Seed: %u", p->seed);
    DrawText(buffer_seed, LARGURA_TELA / 2 - 60, 160, 18, GRAY);

    /* Desenha os 3 modificadores. Cada um numerado com I, II, III
     * pra dar um toque "misterioso". */
    const char *numerais[3] = { "I", "II", "III" };

    for (int i = 0; i < 3; i++) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s.  %s  |  %s  ->  %s",
                 numerais[i],
                 elemento_nome(p->mods[i].elemento),
                 condicao_nome(p->mods[i].condicao),
                 efeito_nome(p->mods[i].efeito));

        /* Espaçamento vertical: cada linha 60 pixels abaixo da anterior. */
        int y = 240 + i * 70;
        DrawText(buffer, 150, y, 26, WHITE);
    }
}
