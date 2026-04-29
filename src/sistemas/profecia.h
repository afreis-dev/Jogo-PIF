/* ============================================================================
 * profecia.h - INTERFACE DO GERADOR PROCEDURAL DE PROFECIAS
 * ============================================================================
 *
 * Uma profecia é composta por 3 modificadores do tipo
 *   [Elemento] + [Condição] + [Efeito].
 *
 * Ex.: "Fogo | Ao matar -> Explosão em área"
 *
 * O gerador recebe uma SEED (número unsigned int) e produz SEMPRE a mesma
 * profecia pra aquela seed — isso é chamado de "determinismo procedural".
 * Permite debug reproduzível e compartilhar runs com amigos ("joga a seed
 * 4821, a profecia é absurda").
 * ========================================================================== */

#ifndef PROFECIA_H
#define PROFECIA_H

#include "tipos.h"

/* Gera uma profecia completa a partir da seed.
 * Escreve os 3 modificadores em p->mods e guarda a seed em p->seed. */
void profecia_gerar(Profecia *p, unsigned int seed);

/* Desenha a profecia na tela (usado no estado REVELACAO_PROFECIA). */
void profecia_desenhar(const Profecia *p);

/* Funções de lookup: converte enum em texto legível.
 * Úteis pra outros módulos que queiram mostrar nomes (HUD, debug). */
const char *elemento_nome(Elemento e);
const char *condicao_nome(Condicao c);
const char *efeito_nome(Efeito e);

#endif /* PROFECIA_H */
