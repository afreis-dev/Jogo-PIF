/*
 * Arquivo: profecia.c
 * Propósito: gerar profecias determinísticas a partir de uma seed.
 * Quem depende dele: core, HUD e qualquer módulo que precise ler o estado da run.
 */

#include "augur/regras.h"

#include <stdio.h>

static const char* NOMES_ELEMENTOS[] = {
    "Fogo",
    "Gelo",
    "Relâmpago",
    "Veneno",
    "Arcano",
    "Sombra"
};

static const char* NOMES_CONDICOES[] = {
    "Ao matar",
    "Ao tomar dano",
    "A cada 10s",
    "Em combo ativo",
    "Ao rolar dado",
    "Ao curar",
    "Na primeira hit",
    "Abaixo de 30% de vida"
};

static const char* NOMES_EFEITOS[] = {
    "Explode em área",
    "Cura o jogador",
    "Duplica projétil",
    "Congela o alvo",
    "Dropa dado",
    "+2 no próximo roll",
    "Dano triplo",
    "Inflama passivo",
    "Reduz recarga",
    "Invoca aliado temporário"
};

static unsigned int avancar_estado_aleatorio(unsigned int* estado)
{
    *estado = (*estado * 1664525u) + 1013904223u;
    return *estado;
}

static int sortear_indice(unsigned int* estado, int quantidade)
{
    return (int)(avancar_estado_aleatorio(estado) % (unsigned int)quantidade);
}

static bool modificador_igual(const ModificadorProfecia* primeiro, const ModificadorProfecia* segundo)
{
    return primeiro->elemento == segundo->elemento &&
           primeiro->condicao == segundo->condicao &&
           primeiro->efeito == segundo->efeito;
}

static void preencher_texto_modificador(ModificadorProfecia* modificador)
{
    snprintf(modificador->texto_resumido, sizeof(modificador->texto_resumido),
             "%s · %s · %s",
             NOMES_ELEMENTOS[modificador->elemento],
             NOMES_CONDICOES[modificador->condicao],
             NOMES_EFEITOS[modificador->efeito]);
}

Profecia gerar_profecia(unsigned int semente)
{
    Profecia profecia;
    unsigned int estado = semente;

    profecia.semente = semente;

    for (int indice_modificador = 0; indice_modificador < QUANTIDADE_MODIFICADORES_PROFECIA; ++indice_modificador) {
        ModificadorProfecia candidato;
        bool repetiu;

        do {
            candidato.elemento = (ElementoProfecia)sortear_indice(
                &estado, (int)(sizeof(NOMES_ELEMENTOS) / sizeof(NOMES_ELEMENTOS[0])));
            candidato.condicao = (CondicaoProfecia)sortear_indice(
                &estado, (int)(sizeof(NOMES_CONDICOES) / sizeof(NOMES_CONDICOES[0])));
            candidato.efeito = (EfeitoProfecia)sortear_indice(
                &estado, (int)(sizeof(NOMES_EFEITOS) / sizeof(NOMES_EFEITOS[0])));

            preencher_texto_modificador(&candidato);
            repetiu = false;

            for (int indice_existente = 0; indice_existente < indice_modificador; ++indice_existente) {
                if (modificador_igual(&candidato, &profecia.modificadores[indice_existente])) {
                    repetiu = true;
                    break;
                }
            }
        } while (repetiu);

        profecia.modificadores[indice_modificador] = candidato;
    }

    return profecia;
}
