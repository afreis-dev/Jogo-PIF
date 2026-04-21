/* ============================================================================
 * tipos.h - CONTRATO ENTRE OS DEVS
 * ============================================================================
 *
 * Este e o arquivo mais importante do projeto. Aqui ficam TODAS as structs,
 * enums e constantes que os 3 devs compartilham. Se voce e o Dev 2 ou Dev 3
 * lendo isso: nao escreva suas proprias versoes das structs - use as daqui.
 *
 * POR QUE TUDO EM UM ARQUIVO SO?
 *   - Evita dependencias circulares (um header que inclui outro que inclui...).
 *   - Qualquer modulo so precisa dar #include "tipos.h" pra ver tudo.
 *   - E o "contrato": quando esse arquivo esta estavel, os 3 podem codar em
 *     paralelo sem esperar ninguem.
 *
 * REGRA: Mudou alguma struct aqui? AVISE O GRUPO antes de commitar. Todo
 * mundo depende desse arquivo.
 * ========================================================================== */

#ifndef TIPOS_H
#define TIPOS_H

#include "raylib.h"   /* Vector2, Rectangle, Color vem dela */
#include <stdbool.h>  /* bool, true, false */
#include <stddef.h>   /* NULL - usado nas cabecas das listas encadeadas */

/* ============================================================================
 * CONSTANTES GLOBAIS
 * --------------------------------------------------------------------------
 * Qualquer "numero magico" que varios arquivos usam vira #define aqui.
 * Assim muda em um lugar so.
 * ========================================================================== */
#define LARGURA_TELA      1280
#define ALTURA_TELA       720
#define FPS_ALVO          60

#define MAX_PROJETEIS     256   /* teto de seguranca pra lista de magias */
#define MAX_INIMIGOS      128   /* teto de seguranca pra lista de inimigos */
#define CARTAS_POR_ESCOLHA 3    /* quantas cartas aparecem entre ondas */
#define MAX_DADOS_JOGADOR 2     /* quantos dados o jogador leva por run */

/* ============================================================================
 * ENUMS - "Int com nome"
 * --------------------------------------------------------------------------
 * Enum = tipo que e internamente um int, mas com nomes legiveis. Em vez de
 * escrever "if (estado == 2)", voce escreve "if (estado == COMBATE)".
 * Bem mais facil de ler e dificil de errar.
 * ========================================================================== */

/* Estados da maquina de estados do jogo.
 * Main.c tem um switch que decide o que rodar baseado nesse valor. */
typedef enum {
    ESTADO_MENU,                /* tela inicial */
    ESTADO_REVELACAO_PROFECIA,  /* mostra os 3 modificadores sorteados */
    ESTADO_COMBATE,             /* wave rolando, inimigos spawnando */
    ESTADO_CARTAS_UPGRADE,      /* entre ondas: jogador escolhe upgrade */
    ESTADO_GAME_OVER,           /* morreu, mostra score e seed */
    ESTADO_SAIR                 /* sinaliza main pra fechar a janela */
} EstadoAtual;

/* 6 elementos do jogo. Ordem importa: e usada pra indexar tabelas de nomes.
 * ELEMENTO_TOTAL no final e truque comum: vira a contagem automaticamente. */
typedef enum {
    ELEMENTO_FOGO,
    ELEMENTO_GELO,
    ELEMENTO_RELAMPAGO,
    ELEMENTO_VENENO,
    ELEMENTO_ARCANO,
    ELEMENTO_SOMBRA,
    ELEMENTO_TOTAL
} Elemento;

/* Tipos de inimigos. Dev 3 usa isso pra decidir IA e sprite. */
typedef enum {
    INIMIGO_CORPO_A_CORPO,
    INIMIGO_A_DISTANCIA,
    INIMIGO_ELITE,
    INIMIGO_CHEFE
} TipoInimigo;

/* Gatilhos das profecias. "Quando X acontece, dispare o efeito".
 * Dev 3 precisa checar essas condicoes no loop de combate. */
typedef enum {
    COND_AO_ACERTAR,        /* toda vez que magia acerta inimigo */
    COND_AO_MATAR,          /* quando inimigo morre */
    COND_VIDA_ABAIXO_50,    /* enquanto HP do jogador < 50% */
    COND_VIDA_ACIMA_80,     /* enquanto HP do jogador > 80% */
    COND_A_CADA_5S,         /* timer interno: dispara a cada 5 segundos */
    COND_EM_CRITICO,        /* quando magia acerta um critico */
    COND_NO_DASH,           /* quando jogador usa dash (futuro) */
    COND_COMBO_X3,          /* combo de 3 kills em sequencia */
    COND_INICIO_ONDA,       /* no comeco de cada onda */
    COND_AO_RECEBER_DANO,   /* quando jogador toma hit */
    COND_TOTAL
} Condicao;

/* Efeitos que podem ser disparados pelas profecias. */
typedef enum {
    EF_EXPLOSAO,            /* dano em area */
    EF_RAIO_EM_CADEIA,      /* pula entre inimigos */
    EF_CURA,                /* recupera HP */
    EF_ESCUDO,              /* bloqueia um hit */
    EF_VELOCIDADE_MAIS,     /* buff de speed */
    EF_DANO_MAIS,           /* buff de dano */
    EF_GERA_ORBE,           /* spawna orbe girando */
    EF_VENENO,              /* DoT */
    EF_TELEPORTA,           /* teleporte curto */
    EF_MULTIPLICA,          /* dobra proximo projetil */
    EF_LENTIDAO,            /* slow nos inimigos */
    EF_ROUBO_DE_VIDA,       /* lifesteal */
    EF_TOTAL
} Efeito;

/* Tipos de cartas de upgrade. Dev 2 popula e aplica. */
typedef enum {
    CARTA_DANO_UP,
    CARTA_VIDA_UP,
    CARTA_VELOCIDADE_UP,
    CARTA_MAIS_MAGIAS,
    CARTA_RECARGA_DADO,     /* devolve um dado pro jogador */
    CARTA_TOTAL
} TipoCarta;


/* ============================================================================
 * STRUCTS DO JOGO
 * --------------------------------------------------------------------------
 * Toda entidade importante tem sua propria struct. Passamos elas por ponteiro
 * nas funcoes (Jogador *j), o que e mais rapido que copiar e permite que a
 * funcao modifique os campos.
 * ========================================================================== */

/* -------------------- JOGADOR -------------------- */
typedef struct {
    Vector2 posicao;        /* x, y na tela. Vector2 e do Raylib. */
    Vector2 velocidade;     /* usado pra mover de forma suave */
    float   raio;           /* pra colisao circular */
    int     vida;           /* HP atual */
    int     vida_maxima;    /* HP teto */
    float   velocidade_movimento;  /* pixels por segundo */
    int     biomassa;       /* moeda da meta-progressao */
} Jogador;


/* -------------------- MAGIAS (LISTA ENCADEADA) --------------------
 * POR QUE LISTA ENCADEADA?
 *   - Projeteis nascem e morrem o tempo todo (spawn e free constante).
 *   - Quantidade varia: ora 2, ora 150.
 *   - Lista encadeada atende o REQUISITO OBRIGATORIO do PIF de listas.
 *   - Cada magia vira um "no" alocado com malloc e liberado com free.
 * ---------------------------------------------------------------- */

/* Dados de UMA magia/projetil individual. */
typedef struct {
    Vector2  posicao;
    Vector2  velocidade;    /* direcao e velocidade do projetil */
    float    dano;
    float    tempo_de_vida; /* em segundos; projetil some quando chega a 0 */
    Elemento elemento;
    bool     viva;          /* se false, sera removida no proximo frame */
} Magia;

/* No da lista encadeada. Cada no carrega uma Magia e aponta pro proximo. */
typedef struct MagiaNo {
    Magia           dados;
    struct MagiaNo *proxima;  /* NULL = fim da lista */
} MagiaNo;


/* -------------------- INIMIGOS (LISTA ENCADEADA) --------------------
 * Mesma logica das magias: spawn/despawn constante, quantidade variavel.
 * ------------------------------------------------------------------ */
typedef struct {
    Vector2     posicao;
    Vector2     velocidade;
    float       raio;
    int         vida;
    int         vida_maxima;
    float       dano;                /* dano que causa ao encostar */
    float       velocidade_movimento;
    TipoInimigo tipo;
    int         recompensa_biomassa; /* quantas biomassas dropa ao morrer */
    bool        vivo;
} Inimigo;

typedef struct InimigoNo {
    Inimigo            dados;
    struct InimigoNo  *proximo;
} InimigoNo;


/* -------------------- ONDA (WAVE) --------------------
 * Uma onda e um grupo de inimigos que spawnam por X segundos.
 * Dev 3 implementa a logica de spawn e escala de dificuldade.
 * ---------------------------------------------------- */
typedef struct {
    int   numero;                 /* 1, 2, 3... fica mais dificil a cada numero */
    int   inimigos_para_spawnar;  /* total previsto pra essa onda */
    int   inimigos_restantes;     /* quantos ainda faltam spawnar */
    float tempo_entre_spawns;     /* em segundos */
    float timer_spawn;            /* acumulador interno */
    bool  completa;               /* true = onda acabou, ir pra tela de cartas */
} Onda;


/* -------------------- PROFECIA (O CORACAO DO JOGO) --------------------
 * Uma profecia e composta por 3 modificadores independentes. Cada um
 * combina [Elemento] + [Condicao] + [Efeito], por exemplo:
 *   "Fogo | Ao matar -> Explosao"
 *
 * Com 6 elementos x 10 condicoes x 12 efeitos = 720 combinacoes por modificador.
 * 3 modificadores = 720^3 = ~370 milhoes de combinacoes teoricas.
 *
 * Como e gerado proceduralmente a partir de uma seed (unsigned int), duas
 * runs com a mesma seed produzem a mesma profecia - reprodutivel pra debug
 * e pra compartilhar runs interessantes com amigos.
 * -------------------------------------------------------------------- */
typedef struct {
    Elemento elemento;
    Condicao condicao;
    Efeito   efeito;
} Modificador;

typedef struct {
    Modificador mods[3];       /* sempre 3 modificadores por profecia */
    unsigned int seed;         /* guardada pra debug e replay */
} Profecia;


/* -------------------- CARTAS E DADOS (DEV 2) --------------------
 * Cartas de upgrade que aparecem entre ondas. Dados sao rolados pra
 * modificar o valor das cartas.
 * --------------------------------------------------------------- */
typedef struct {
    TipoCarta tipo;
    int       raridade;          /* 0=comum, 1=rara, 2=lendaria */
    int       valor;             /* ex.: +10 de dano */
    char      nome[64];
    char      descricao[256];
} Carta;

typedef struct {
    int faces;                   /* d6 = 6, d20 = 20 */
    int ultimo_resultado;        /* pra mostrar na tela apos rolar */
} Dado;


/* -------------------- DADOS SALVOS (DEV 2) --------------------
 * Persistem entre runs. Dev 2 salva/carrega de saves/biomassa.dat
 * usando fwrite/fread (REQUISITO OBRIGATORIO do PIF: arquivo).
 * ------------------------------------------------------------- */
typedef struct {
    int  biomassa_total;            /* moeda acumulada em todas as runs */
    int  runs_completadas;
    int  melhor_onda;               /* maior onda alcancada ate hoje */
    int  profecias_desbloqueadas[20]; /* MATRIZ - requisito obrigatorio */
    char nome_jogador[32];
} DadosSalvos;


/* ============================================================================
 * ESTADO DO JOGO (STRUCT RAIZ)
 * --------------------------------------------------------------------------
 * Essa e a struct mae. Ela carrega absolutamente tudo do estado atual.
 * Passamos ponteiro dela (EstadoJogo *ej) pra todas as funcoes do jogo.
 *
 * POR QUE USAR ESSA STRUCT RAIZ?
 *   - Zero variaveis globais espalhadas pelo codigo.
 *   - Toda funcao ve o contexto inteiro e pode modificar o que precisa.
 *   - Facil de salvar/carregar: so serializa essa struct.
 *   - Fica obvio quem depende de que.
 * ========================================================================== */
typedef struct {
    /* --- Maquina de estados --- */
    EstadoAtual estado_atual;
    EstadoAtual proximo_estado;   /* buffer de transicao pra trocar entre frames */

    /* --- Entidades principais --- */
    Jogador   jogador;
    Profecia  profecia;
    Onda      onda_atual;
    DadosSalvos salvamento;

    /* --- Listas encadeadas (cabecas) ---
     * Comecam em NULL = lista vazia. Dev 3 adiciona nos com malloc. */
    MagiaNo   *magias_cabeca;
    InimigoNo *inimigos_cabeca;

    /* --- Opcoes de upgrade mostradas no estado CARTAS_UPGRADE ---
     * MATRIZ de 3 cartas (requisito obrigatorio). */
    Carta     escolhas_upgrade[CARTAS_POR_ESCOLHA];

    /* --- Dados que o jogador escolheu levar nessa run --- */
    Dado      dados_ativos[MAX_DADOS_JOGADOR];

    /* --- Tempo e frames --- */
    float     delta_tempo;        /* segundos desde o ultimo frame */
    float     tempo_total;        /* tempo acumulado da run */
    int       contador_frames;

    /* --- Debug --- */
    bool      modo_debug;         /* F1 alterna; mostra FPS e info extra */
} EstadoJogo;

#endif /* TIPOS_H */
