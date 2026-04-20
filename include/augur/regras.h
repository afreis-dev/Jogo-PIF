/*
 * Arquivo: regras.h
 * Propósito: expor funções puras e helpers estáveis de regras do jogo.
 * Quem depende deste arquivo: core, stubs e futuros sistemas de combate.
 */

#ifndef AUGUR_REGRAS_H
#define AUGUR_REGRAS_H

#include <stdbool.h>

#include "augur/tipos.h"

Profecia gerar_profecia(unsigned int semente);
bool colide_circulos(float centro_ax, float centro_ay, float raio_a,
                     float centro_bx, float centro_by, float raio_b);
bool colide_retangulos(float retangulo_ax, float retangulo_ay, float largura_a, float altura_a,
                       float retangulo_bx, float retangulo_by, float largura_b, float altura_b);
void limitar_jogador_na_tela(Jogador* jogador, int largura_tela, int altura_tela);

#endif
