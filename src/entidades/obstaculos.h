/* ============================================================================
 * obstaculos.h - INTERFACE DO MODULO DE OBSTACULOS (SANDBOX)
 * ============================================================================
 *
 * Existe SO na branch test/sandbox-integracao. Adiciona arvores e pedras
 * espalhadas pelo mundo, com layout deterministico a partir da seed da run.
 *
 * Bloqueiam o jogador (push-out) e os inimigos (push-out). Nao causam dano.
 *
 * Quando a Luisa fizer o sistema de bioma oficial, este modulo provavelmente
 * vai virar parte de "mapa" / "bioma".
 * ========================================================================== */

#ifndef OBSTACULOS_H
#define OBSTACULOS_H

#include "tipos.h"

/* Gera o layout aleatorio de obstaculos a partir da seed.
 * Mesma seed -> mesmo mapa. Substitui qualquer layout anterior. */
void obstaculos_gerar(EstadoJogo *ej, unsigned int seed);

/* Desenha todos os obstaculos. DEVE ser chamado DENTRO de BeginMode2D
 * (eles vivem em coordenadas de mundo). */
void obstaculos_desenhar(const EstadoJogo *ej);

/* Empurra o jogador pra fora de qualquer obstaculo que ele esteja sobrepondo.
 * Chamada apos jogador_atualizar. */
void obstaculos_resolver_jogador(EstadoJogo *ej);

/* Mesmo padrao, mas pra cada inimigo da lista encadeada. */
void obstaculos_resolver_inimigos(EstadoJogo *ej);

#endif /* OBSTACULOS_H */
