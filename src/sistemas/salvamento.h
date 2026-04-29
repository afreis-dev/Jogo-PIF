/* ============================================================================
 * salvamento.h - INTERFACE DO SISTEMA DE SAVE/LOAD
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 2
 *
 * Atende o REQUISITO OBRIGATORIO do PIF: leitura e escrita em arquivo.
 *
 * O que salva: struct DadosSalvos (definida em tipos.h) em formato BINARIO.
 * Arquivo: saves/biomassa.dat
 *
 * Por que binario e nao texto?
 *   - Mais simples de implementar (fwrite/fread de uma so vez).
 *   - Menor no disco.
 *   - Desvantagem: nao da pra abrir no bloco de notas. Pra debug, adicionar
 *     uma funcao de "imprimir salvamento" que printa no terminal.
 *
 * COMO IMPLEMENTAR:
 *   Salvar:  FILE *f = fopen("saves/biomassa.dat", "wb");
 *            fwrite(ds, sizeof(DadosSalvos), 1, f);
 *            fclose(f);
 *
 *   Carregar: FILE *f = fopen("saves/biomassa.dat", "rb");
 *             if (f) { fread(ds, sizeof(DadosSalvos), 1, f); fclose(f); }
 *             else   { // arquivo nao existe, usar valores padrao
 * ========================================================================== */

#ifndef SALVAMENTO_H
#define SALVAMENTO_H

#include "tipos.h"

/* Le saves/biomassa.dat e preenche ds. Se o arquivo nao existir, zera tudo. */
void salvamento_carregar(DadosSalvos *ds);

/* Grava ds em saves/biomassa.dat. Chamada ao fechar o jogo. */
void salvamento_salvar(const DadosSalvos *ds);

#endif /* SALVAMENTO_H */
