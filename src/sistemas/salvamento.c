/* ============================================================================
 * salvamento.c - STUB DO SISTEMA DE SAVE/LOAD
 * ============================================================================
 *
 * TODO: Dev 2 implementar este arquivo.
 *
 * Lembrete: o diretorio "saves/" ja existe no repositorio. Mas em outras
 * maquinas pode nao existir. Uma boa pratica e usar _mkdir("saves") no
 * Windows antes de tentar abrir o arquivo (include <direct.h>).
 * ========================================================================== */

#include "salvamento.h"
#include <stdio.h>    /* FILE, fopen, fread, fwrite, fclose */
#include <string.h>   /* memset */

void salvamento_carregar(DadosSalvos *ds) {
    /* TODO: Dev 2 - implementar leitura binaria do arquivo.
     * Por enquanto, zera tudo (como se fosse uma sessao nova). */
    memset(ds, 0, sizeof(DadosSalvos));
}

void salvamento_salvar(const DadosSalvos *ds) {
    (void)ds;
    /* TODO: Dev 2 - implementar escrita binaria no arquivo.
     * Nao esquecer de fechar o FILE com fclose depois do fwrite! */
}
