#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>

typedef unsigned int uint;

const short DEFAULT_PAIR = 0;
const short RED_PAIR = 1;
const short GREEN_PAIR = 2;
const short YELLOW_PAIR = 3;
const short BLUE_PAIR = 4;

const unsigned int GUARDAS = 1;
const unsigned int MERCENARIOS = 1;

WINDOW * init();
char escolha_inicial();
void loop(char jogador);
char verificar_vitoria(char tab[9][9]);
bool is_borda(unsigned int x, unsigned int y);
void desenhar_tabuleiro(char tab[9][9]);
void desenhar_linha(char tab[9][9], unsigned int y);
void desenhar_entrelinha();
void desenhar_mensagem(char msg);
void remover_pecas_capturadas(char tab[9][9]);
void ler_movimento(char jogador, unsigned int mov[4]);
bool is_jogada_valida(
    char tab[9][9], char jogador, unsigned int mov[4], char * msg);
bool is_posicao_livre(char pos);
void mover_peca(char tab[9][9], unsigned int mov[4]);
bool testar_captura(
    char tab[9][9], char jogador,
    unsigned int x, unsigned int y, unsigned int captura[2]);
bool is_rei_cercado(char tab[9][9], unsigned int x, unsigned int y);
void capturar_peca(char tab[9][9], char jogador, unsigned int captura[2]);
void desenhar_fim(time_t start, unsigned int n_jog, char fim);
void cleanup();

int main() {
    setlocale(LC_ALL, "");
    init();
    loop(escolha_inicial());
    cleanup();
    return 0;
}

WINDOW * init() {
    WINDOW * w = initscr();
    start_color();
    cbreak();
    noecho();
    init_pair(RED_PAIR,    COLOR_RED,    COLOR_BLACK);
    init_pair(GREEN_PAIR,  COLOR_GREEN,  COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_PAIR,   COLOR_BLUE,   COLOR_BLACK);
    return w;
}

char escolha_inicial() {
    char c;
    int first = 1;
    do {
        move(0, 0);
        addstr("Que jogador fará o primeiro movimento (");
        if(!first)
            color_set(RED_PAIR, 0);
        addstr("M/R");
        if(!first)
            color_set(DEFAULT_PAIR, 0);
        addstr(")? ");
        refresh();
        c = getch();
        first = 0;
    } while(c != 'M' && c != 'R');
    if(c == 'M')
        return MERCENARIOS;
    return GUARDAS;
}

void loop(char jogador) {
    char tab[9][9]= {
        {' ', ' ', ' ', 'M', 'M', 'M', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'M', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'G', ' ', ' ', ' ', ' '},
        {'M', ' ', ' ', ' ', 'G', ' ', ' ', ' ', 'M'},
        {'M', 'M', 'G', 'G', 'R', 'G', 'G', 'M', 'M'},
        {'M', ' ', ' ', ' ', 'G', ' ', ' ', ' ', 'M'},
        {' ', ' ', ' ', ' ', 'G', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'M', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'M', 'M', 'M', ' ', ' ', ' '},
    };
    unsigned int mov[4];
    unsigned int n_jog = 0;
    bool trono = false;
    char msg = 0, fim = 0;
    time_t t0;
    time(&t0);
    while(true) {
        fim = verificar_vitoria(tab);
        clear();
        desenhar_tabuleiro(tab);
        if(fim)
            break;
        move(20, 0);
        desenhar_mensagem(msg);
        refresh();
        remover_pecas_capturadas(tab);
        msg = 0;
        move(21, 0);
        ler_movimento(jogador, mov);
        if(is_jogada_valida(tab, jogador, mov, &msg)) {
            n_jog++;
            mover_peca(tab, mov);
            unsigned int captura[2];
            if(testar_captura(tab, jogador, mov[3], mov[2], captura)) {
                capturar_peca(tab, jogador, captura);
                if(tab[captura[0]][captura[1]] == 'R')
                    fim = 1;
            }
            if(!trono && mov[0] == 4 && mov[1] == 4) {
                tab[4][4] = 'T';
                trono = true;
            }
            jogador = !jogador;
      }
    }
    clear();
    desenhar_tabuleiro(tab);
    move(21, 0);
    desenhar_fim(t0, n_jog, fim);
    refresh();
    getch();
}

char verificar_vitoria(char tab[9][9]) {
    unsigned int x, y;
    bool mercenarios = false;
    for(y = 0; y < 9; y++)
        for(x = 0; x < 9; x++) {
            if(tab[y][x] == 'M')
                mercenarios = true;
            if(tab[y][x] == 'R' && is_borda(x, y))
                return 3;
        }
    if(!mercenarios)
        return 2;
    return 0;
}

bool is_borda(unsigned int x, unsigned int y) {
    return x == 0 || x == 8 || y == 0 || y == 8;
}

void desenhar_tabuleiro(char tab[9][9]) {
    unsigned int x, y;
    move(0, 0);
    addstr("    1   2   3   4   5   6   7   8   9");
    move(1, 0);
    addwstr(L"  ┌");
    for(x = 0; x < 8; x++)
        addwstr(L"───┬");
    addwstr(L"───┐");
    desenhar_linha(tab, 0);
    desenhar_entrelinha();
    for(y = 1; y < 8; y++) {
        desenhar_linha(tab, y);
        desenhar_entrelinha();
    }
    desenhar_linha(tab, 8);
    addwstr(L"\n  └");
    for(x = 0; x < 8; x++)
        addwstr(L"───┴");
    addwstr(L"───┘");
}

void desenhar_linha(char tab[9][9], unsigned int y) {
    unsigned int x;
    move(y * 2 + 2, 0);
    printw("%c ", 'A' + y);
    addwstr(L"│");
    for(x = 0; x < 9; x++) {
        switch(tab[y][x]) {
            case 'M':
                color_set(GREEN_PAIR, 0);
                break;
            case 'G':
                color_set(RED_PAIR, 0);
                break;
            case 'R':
            case 'T':
                color_set(YELLOW_PAIR, 0);
                break;
        }
        printw(" %c", tab[y][x]);
        if(tab[y][x] != ' ')
            color_set(DEFAULT_PAIR, 0);
        addwstr(L" │");
    }
}

void desenhar_entrelinha() {
    unsigned int x;
    addwstr(L"\n  ├");
    for(x = 0; x < 8; x++)
        addwstr(L"───┼");
    addwstr(L"───┤");
}

void desenhar_mensagem(char msg) {
    if(!msg)
        return;
    color_set(RED_PAIR, 0);
    switch(msg) {
        case 1:
            addstr("Coordenadas inválidas.");
            break;
        case 2:
            addstr("Você tem que mexer a peça.");
            break;
        case 3:
            addstr("Não há uma peça nessa coordenada.");
            break;
        case 4:
            addstr("Essa peça não lhe pertence.");
            break;
        case 5:
            addstr("Não pode parar no trono.");
            break;
        case 6:
            addstr("Há uma peça nessa coordenada.");
            break;
        case 7:
            addstr("Não pode passar por outras peças.");
            break;
        case 8:
            addstr("Movimentos devem ser horizontais ou verticais.");
            break;
    }
    color_set(DEFAULT_PAIR, 0);
}

void remover_pecas_capturadas(char tab[9][9]) {
    unsigned int x, y;
    for(y = 0; y < 9; y++)
        for(x = 0; x < 9; x++)
            if(tab[y][x] == 'X')
                tab[y][x]=' ';
}

void ler_movimento(char jogador, unsigned int mov[4]) {
    if(jogador == MERCENARIOS)
        addstr("Mercenario, ");
    else
        addstr("Guarda, ");
    addstr("qual o seu movimento? ");
    refresh();
    nocbreak();
    echo();
    char in[5];
    getnstr(in, 5);
    cbreak();
    noecho();
    mov[0] = toupper(in[0]) - 'A';
    mov[1] = in[1] - '1';
    mov[2] = toupper(in[3]) - 'A';
    mov[3] = in[4] - '1';
}

bool is_jogada_valida(
        char tab[9][9], char jogador, unsigned int mov[4], char * msg) {
    unsigned int y1 = mov[0];
    unsigned int x1 = mov[1];
    unsigned int y2 = mov[2];
    unsigned int x2 = mov[3];
    /* Dentro do tabuleiro. */
    if(x1 < 0 || 8 < x1 || x2 < 0 || 8 < x2
            || y1 < 0 || 8 < y1 || y2 < 0 || 8 < y2) {
        *msg = 1;
        return false;
    }
    /* Parado. */
    if(x1 == x2 && y1 == y2) {
        *msg = 2;
        return false;
    }
    /* Espaço vazio. */
    if(tab[y1][x1] == ' ' || tab[y1][x1] == 'T') {
        *msg = 3;
        return false;
    }
    /* Outro jogador. */
    if((jogador && tab[y1][x1] != 'M') || (!(jogador) && tab[y1][x1] == 'M')) {
        *msg = 4;
        return false;
    }
    /* Trono. */
    if(tab[y2][x2] == 'T') {
        *msg = 5;
        return false;
    }
    /* Lugar ocupado. */
    if(tab[y2][x2] != ' ') {
        *msg = 6;
        return false;
    }
    /* Peças no caminho. */
    unsigned int inicio, fim;
    if(x1 == x2) {
        if(y1 < y2) {
            inicio = y1 + 1;
            fim = y2;
        }
        else {
            inicio = y2;
            fim = y1 - 1;
        }
        for(; inicio < fim; inicio++)
            if(!is_posicao_livre(tab[inicio][x1])) {
                *msg = 7;
                return false;
            }
    }
    else if(y1 == y2) {
        if(x1 < x2) {
            inicio = x1 + 1;
            fim = x2;
        }
        else {
            inicio = x2;
            fim = x1 - 1;
        }
        for(; inicio < fim; inicio++)
            if(!is_posicao_livre(tab[y1][inicio])) {
                *msg = 7;
                return false;
            }
    } else {
        *msg = 8;
        return false;
    }
    return true;
}

bool is_posicao_livre(char pos) {
    return pos == ' ' || pos == 'T';
}


void mover_peca(char tab[9][9], unsigned int mov[4]) {
    unsigned int y1 = mov[0];
    unsigned int x1 = mov[1];
    unsigned int y2 = mov[2];
    unsigned int x2 = mov[3];
    tab[y2][x2] = tab[y1][x1];
    tab[y1][x1] = ' ';
}

bool testar_captura(
        char tab[9][9], char jogador,
        unsigned int x, unsigned int y, unsigned int captura[2]) {
    char amigo, inimigo;
    if(jogador == MERCENARIOS) {
        amigo = 'M';
        inimigo = 'G';
    }
    else {
        amigo = 'G';
        inimigo = 'M';
    }
    if(y < 7 && tab[y + 2][x] == amigo && tab[y + 1][x] == inimigo) {
        captura[0] = y + 1;
        captura[1] = x;
        return true;
    }
    if(y > 1 && tab[y - 2][x] == amigo && tab[y - 1][x] == inimigo) {
        captura[0] = y - 1;
        captura[1] = x;
        return true;
    }
    if(x < 7 && tab[y][x + 2] == amigo && tab[y][x + 1] == inimigo) {
        captura[0] = y;
        captura[1] = x + 1;
        return true;
    }
    if(x > 1 && tab[y][x - 2] == amigo && tab[y][x - 1] == inimigo) {
        captura[0] = y;
        captura[1] = x - 1;
        return true;
    }
    if(jogador != MERCENARIOS)
        return false;
    if(y < 7 && tab[y + 1][x] == 'R' && is_rei_cercado(tab, x, y + 1)) {
        captura[0] = y + 1;
        captura[1] = x;
        return true;
    }
    if(y > 1 && tab[y - 1][x] == 'R' && is_rei_cercado(tab, x, y - 1)) {
        captura[0] = y - 1;
        captura[1] = x;
        return true;
    }
    if(x < 7 && tab[y][x + 1] == 'R' && is_rei_cercado(tab, x + 1, y)) {
        captura[0] = y;
        captura[1] = x + 1;
        return true;
    }
    if(x > 1 && tab[y][x - 1] == 'R' && is_rei_cercado(tab, x - 1, y)) {
        captura[0] = y;
        captura[1] = x - 1;
        return true;
    }
    return false;
}

bool is_rei_cercado(char tab[9][9], unsigned int x, unsigned int y) {
    return
        (tab[y - 1][x] == 'M' || tab[y - 1][x] == 'T')
        && (tab[y + 1][x] == 'M' || tab[y + 1][x] == 'T')
        && (tab[y][x - 1] == 'M' || tab[y][x - 1] == 'T')
        && (tab[y][x + 1] == 'M' || tab[y][x + 1] == 'T');
}

void capturar_peca(char tab[9][9], char jogador, unsigned int captura[2]) {
    tab[captura[0]][captura[1]] = ' ';
}

void desenhar_fim(time_t start, unsigned int n_jog, char fim) {
    time_t end;
    time(&end);
    int h = (int) difftime(end, start);
    int m = (h % 3600) / 60;
    int s = (h % 3600) % 60;
    h = h / 3600;
    printw(
        "Fim do jogo. A partida teve %i movimentos e durou %i:%02i:%02i",
        n_jog, h, m, s);
    switch(fim) {
        case 1:
            addstr(". O rei foi capturado. Vitória dos mercenários.");
            break;
        case 2:
            addstr(". Todos os mercenários foram capturados. Vitória do rei.");
            break;
        case 3:
            addstr(". O rei conseguiu escapar. Vitória do rei.");
            break;
    }
}

void cleanup() {
    nocbreak();
    echo();
    endwin();
}
