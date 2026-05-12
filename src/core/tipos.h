/* ============================================================================
 * tipos.h - CONTRATO ENTRE OS DEVS
 * ============================================================================
 *
 * Este é o arquivo mais importante do projeto. Aqui ficam TODAS as structs,
 * enums e constantes que os 3 devs compartilham. Se você é o Dev 2 ou Dev 3
 * lendo isso: não escreva suas próprias versões das structs — use as daqui.
 *
 * POR QUE TUDO EM UM ARQUIVO SÓ?
 *   - Evita dependências circulares (um header que inclui outro que inclui...).
 *   - Qualquer módulo só precisa dar #include "tipos.h" pra ver tudo.
 *   - É o "contrato": quando esse arquivo está estável, os 3 podem codar em
 *     paralelo sem esperar ninguém.
 *
 * REGRA: Mudou alguma struct aqui? AVISE O GRUPO antes de commitar. Todo
 * mundo depende desse arquivo.
 * ========================================================================== */

#ifndef TIPOS_H
#define TIPOS_H

#include "raylib.h"   /* Vector2, Rectangle, Color vêm dela */
#include <stdbool.h>  /* bool, true, false */
#include <stddef.h>   /* NULL — usado nas cabeças das listas encadeadas */

/* ============================================================================
 * CONSTANTES GLOBAIS
 * --------------------------------------------------------------------------
 * Qualquer "número mágico" que vários arquivos usam vira #define aqui.
 * Assim muda em um lugar só.
 * ========================================================================== */
#define LARGURA_TELA      1280
#define ALTURA_TELA       720
#define FPS_ALVO          60

#define MAX_PROJETEIS     256   /* teto de segurança pra lista de magias */
#define MAX_INIMIGOS      128   /* teto de segurança pra lista de inimigos */
#define MAX_OBSTACULOS     40   /* teto de obstáculos do mapa por run */
#define CARTAS_POR_ESCOLHA 3    /* quantas cartas aparecem entre ondas */
#define MAX_DADOS_JOGADOR 2     /* quantos dados o jogador leva por run */

/* ============================================================================
 * ENUMS — "Int com nome"
 * --------------------------------------------------------------------------
 * Enum = tipo que é internamente um int, mas com nomes legíveis. Em vez de
 * escrever "if (estado == 2)", você escreve "if (estado == COMBATE)".
 * Bem mais fácil de ler e difícil de errar.
 * ========================================================================== */

/* Estados da máquina de estados do jogo.
 * Main.c tem um switch que decide o que rodar baseado nesse valor. */
typedef enum {
    ESTADO_MENU,                /* tela inicial */
    ESTADO_REVELACAO_PROFECIA,  /* mostra os 3 modificadores sorteados */
    ESTADO_COMBATE,             /* wave rolando, inimigos spawnando */
    ESTADO_CARTAS_UPGRADE,      /* entre ondas: jogador escolhe upgrade */
    ESTADO_GAME_OVER,           /* morreu, mostra score e seed */
    ESTADO_SAIR                 /* sinaliza main pra fechar a janela */
} EstadoAtual;

/* 6 elementos do jogo. Ordem importa: é usada pra indexar tabelas de nomes.
 * ELEMENTO_TOTAL no final é truque comum: vira a contagem automaticamente. */
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
 * Dev 3 precisa checar essas condições no loop de combate. */
typedef enum {
    COND_AO_ACERTAR,        /* toda vez que magia acerta inimigo */
    COND_AO_MATAR,          /* quando inimigo morre */
    COND_VIDA_ABAIXO_50,    /* enquanto HP do jogador < 50% */
    COND_VIDA_ACIMA_80,     /* enquanto HP do jogador > 80% */
    COND_A_CADA_5S,         /* timer interno: dispara a cada 5 segundos */
    COND_EM_CRITICO,        /* quando magia acerta um crítico */
    COND_NO_DASH,           /* quando jogador usa dash (futuro) */
    COND_COMBO_X3,          /* combo de 3 kills em sequência */
    COND_INICIO_ONDA,       /* no começo de cada onda */
    COND_AO_RECEBER_DANO,   /* quando jogador toma hit */
    COND_TOTAL
} Condicao;

/* Efeitos que podem ser disparados pelas profecias. */
typedef enum {
    EF_EXPLOSAO,            /* dano em área */
    EF_RAIO_EM_CADEIA,      /* pula entre inimigos */
    EF_CURA,                /* recupera HP */
    EF_ESCUDO,              /* bloqueia um hit */
    EF_VELOCIDADE_MAIS,     /* buff de speed */
    EF_DANO_MAIS,           /* buff de dano */
    EF_GERA_ORBE,           /* spawna orbe girando */
    EF_VENENO,              /* DoT */
    EF_TELEPORTA,           /* teleporte curto */
    EF_MULTIPLICA,          /* dobra próximo projétil */
    EF_LENTIDAO,            /* slow nos inimigos */
    EF_ROUBO_DE_VIDA,       /* lifesteal */
    EF_TOTAL
} Efeito;

/* Tipos de obstáculo do mapa. Cada tipo desenha de um jeito (árvore tem
 * tronco + copa, pedra é um círculo cinza com volume) e tem range de raio
 * próprio. Luísa decide a lista final na implementação. */
typedef enum {
    OBSTACULO_ARVORE,
    OBSTACULO_PEDRA,
    OBSTACULO_TIPO_TOTAL
} TipoObstaculo;

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
 * Toda entidade importante tem sua própria struct. Passamos elas por ponteiro
 * nas funções (Jogador *j), o que é mais rápido que copiar e permite que a
 * função modifique os campos.
 * ========================================================================== */

/* -------------------- JOGADOR -------------------- */
typedef struct {
    Vector2 posicao;        /* x, y na tela. Vector2 é do Raylib. */
    Vector2 velocidade;     /* usado pra mover de forma suave */
    float   raio;           /* pra colisão circular */
    int     vida;           /* HP atual */
    int     vida_maxima;    /* HP teto */
    float   velocidade_movimento;  /* pixels por segundo */
    int     biomassa;  
    int bonus_dano;     
} Jogador;


/* -------------------- MAGIAS (LISTA ENCADEADA) --------------------
 * POR QUE LISTA ENCADEADA?
 *   - Projéteis nascem e morrem o tempo todo (spawn e free constante).
 *   - Quantidade varia: ora 2, ora 150.
 *   - Lista encadeada atende o REQUISITO OBRIGATÓRIO de PIF de listas.
 *   - Cada magia vira um "nó" alocado com malloc e liberado com free.
 * ---------------------------------------------------------------- */

/* Dados de UMA magia/projétil individual. */
typedef struct {
    Vector2  posicao;
    Vector2  velocidade;    /* direção e velocidade do projétil */
    float    dano;
    float    tempo_de_vida; /* em segundos; projétil some quando chega a 0 */
    Elemento elemento;
    bool     viva;          /* se false, será removida no próximo frame */
} Magia;

/* Nó da lista encadeada. Cada nó carrega uma Magia e aponta pro próximo. */
typedef struct MagiaNo {
    Magia           dados;
    struct MagiaNo *proxima;  /* NULL = fim da lista */
} MagiaNo;


/* -------------------- INIMIGOS (LISTA ENCADEADA) --------------------
 * Mesma lógica das magias: spawn/despawn constante, quantidade variável.
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
 * Uma onda é um grupo de inimigos que spawnam por X segundos.
 * Dev 3 implementa a lógica de spawn e escala de dificuldade.
 * ---------------------------------------------------- */
typedef struct {
    int   numero;                 /* 1, 2, 3... fica mais difícil a cada número */
    int   inimigos_para_spawnar;  /* total previsto pra essa onda */
    int   inimigos_restantes;     /* quantos ainda faltam spawnar */
    float tempo_entre_spawns;     /* em segundos */
    float timer_spawn;            /* acumulador interno */
    bool  completa;               /* true = onda acabou, ir pra tela de cartas */
} Onda;


/* -------------------- PROFECIA (O CORAÇÃO DO JOGO) --------------------
 * Uma profecia é composta por 3 modificadores independentes. Cada um
 * combina [Elemento] + [Condição] + [Efeito], por exemplo:
 *   "Fogo | Ao matar -> Explosão"
 *
 * Com 6 elementos × 10 condições × 12 efeitos = 720 combinações por modificador.
 * 3 modificadores = 720^3 = ~370 milhões de combinações teóricas.
 *
 * Como é gerado proceduralmente a partir de uma seed (unsigned int), duas
 * runs com a mesma seed produzem a mesma profecia — reprodutível pra debug
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


/* -------------------- OBSTÁCULOS DO MAPA (DEV 3) --------------------
 * Objetos fixos no mundo (árvores, pedras...). Bloqueiam jogador e inimigos
 * (push-out) mas não causam dano. Layout deve ser gerado UMA vez no início
 * da run, determinístico a partir da seed da profecia (mesma seed = mesmo
 * mapa, pra debug/replay).
 *
 * Como são fixos durante a run (não spawnam nem somem em tempo de combate),
 * usamos um ARRAY simples no EstadoJogo com qtd_obstaculos marcando quantos
 * slots estão preenchidos. Não precisa de lista encadeada aqui.
 * ----------------------------------------------------------------- */
typedef struct {
    Vector2       posicao;
    float         raio;
    TipoObstaculo tipo;
} Obstaculo;


/* -------------------- CARTAS E DADOS (DEV 2) --------------------
 * Cartas de upgrade que aparecem entre ondas. Dados são rolados pra
 * modificar o valor das cartas.
 * --------------------------------------------------------------- */
typedef struct {
    TipoCarta tipo;
    int       raridade;          /* 0=comum, 1=rara, 2=lendária */
    int       valor;             /* ex.: +10 de dano */
    char      nome[64];
    char      descricao[256];
} Carta;

typedef struct {
    int faces;                   /* d6 = 6, d20 = 20 */
    int ultimo_resultado;        /* pra mostrar na tela após rolar */
} Dado;


/* -------------------- DADOS SALVOS (DEV 2) --------------------
 * Persistem entre runs. Dev 2 salva/carrega de saves/biomassa.dat
 * usando fwrite/fread (REQUISITO OBRIGATÓRIO de PIF: arquivo).
 * ------------------------------------------------------------- */
typedef struct {
    int  biomassa_total;            /* moeda acumulada em todas as runs */
    int  runs_completadas;
    int  melhor_onda;               /* maior onda alcançada até hoje */
    int  profecias_desbloqueadas[20]; /* MATRIZ — requisito obrigatório */
    char nome_jogador[32];
} DadosSalvos;


/* ============================================================================
 * ESTADO DO JOGO (STRUCT RAIZ)
 * --------------------------------------------------------------------------
 * Essa é a struct mãe. Ela carrega absolutamente tudo do estado atual.
 * Passamos ponteiro dela (EstadoJogo *ej) pra todas as funções do jogo.
 *
 * POR QUE USAR ESSA STRUCT RAIZ?
 *   - Zero variáveis globais espalhadas pelo código.
 *   - Toda função vê o contexto inteiro e pode modificar o que precisa.
 *   - Fácil de salvar/carregar: só serializa essa struct.
 *   - Fica óbvio quem depende de quê.
 * ========================================================================== */
typedef struct {
    /* --- Máquina de estados --- */
    EstadoAtual estado_atual;
    EstadoAtual proximo_estado;   /* buffer de transição pra trocar entre frames */

    /* --- Entidades principais --- */
    Jogador   jogador;
    Profecia  profecia;
    Onda      onda_atual;
    DadosSalvos salvamento;

    /* --- Camera 2D ---
     * O jogador vive em um mundo infinito (sem bordas). A câmera segue o
     * jogador: camera.target = jogador.posicao. offset é o ponto da tela onde
     * o target aparece (centro da tela = player centralizado). Tudo que for
     * desenhado dentro de BeginMode2D(camera)/EndMode2D é interpretado em
     * coordenadas de mundo; o que fica fora é coord de tela (HUD, menus). */
    Camera2D  camera;

    /* --- Listas encadeadas (cabeças) ---
     * Começam em NULL = lista vazia. Dev 3 adiciona nós com malloc. */
    MagiaNo   *magias_cabeca;
    InimigoNo *inimigos_cabeca;

    /* --- Obstáculos do mapa ---
     * Array fixo populado uma vez no início da run (determinístico a partir
     * da seed da profecia). qtd_obstaculos é quantos slots estão realmente
     * preenchidos. Bloqueiam o jogador e os inimigos. */
    Obstaculo obstaculos[MAX_OBSTACULOS];
    int       qtd_obstaculos;

    /* --- Opções de upgrade mostradas no estado CARTAS_UPGRADE ---
     * MATRIZ de 3 cartas (requisito obrigatório). */
    Carta     escolhas_upgrade[CARTAS_POR_ESCOLHA];

    /* --- Dados que o jogador escolheu levar nessa run --- */
    Dado      dados_ativos[MAX_DADOS_JOGADOR];

    /* --- Tempo e frames --- */
    float     delta_tempo;        /* segundos desde o último frame */
    float     tempo_total;        /* tempo acumulado da run */
    int       contador_frames;

    /* --- Debug --- */
    bool      modo_debug;         /* F1 alterna; mostra FPS e info extra */
} EstadoJogo;

#endif /* TIPOS_H */
