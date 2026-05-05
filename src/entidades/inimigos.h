/* ============================================================================
 * inimigos.h - INTERFACE DA ENGINE DE INIMIGOS
 * ============================================================================
 *
 * RESPONSABILIDADE: Arthur (Dev 1) — engine.
 *
 * Esta camada lida com lista encadeada, push-out físico, render genérico
 * e dispatch de IA. Os DADOS dos tipos (vida, dano, IA específica) vivem
 * em src/entidades/inimigos_tipos.c, que a Luísa (Dev 3) edita.
 *
 * QUEM CHAMA O QUÊ:
 *   - main.c chama atualizar/desenhar/liberar uma vez por frame.
 *   - cronograma.c chama inimigos_spawnar_em quando a timeline pede.
 *   - colisao.c marca .vivo = false e .vida -= dano (não remove da lista).
 *
 * COMO A LUÍSA ADICIONA UM TIPO NOVO: vai em inimigos_tipos.c. NÃO precisa
 * mexer aqui.
 * ============================================================================ */

#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "tipos.h"

/* Dispatch IA + movimento + push-out + remoção dos mortos. Uma chamada por
 * frame durante o combate. */
void inimigos_atualizar(EstadoJogo *ej);

/* Render circular usando cor da tabela de parâmetros. */
void inimigos_desenhar(const EstadoJogo *ej);

/* Libera todos os nós da lista. Chamada pelo main na finalização e quando
 * uma run termina (game over / vitória). */
void inimigos_liberar_tudo(EstadoJogo *ej);

/* Cria um inimigo na posição dada com os stats do tipo (lê PARAMETROS_INIMIGO).
 * Usada pelo cronograma e por testes. Ignora silenciosamente se a lista
 * estourar MAX_INIMIGOS. */
void inimigos_spawnar_em(EstadoJogo *ej, Vector2 posicao, TipoInimigo tipo);

#endif /* INIMIGOS_H */
