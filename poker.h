#ifndef POKER_H
#define POKER_H
#include <string.h>
#include <time.h>
#include "ciphers.h"
#include "infodef.h"

#define BC_ENTER    "\E(0"
#define BC_EXIT     "\E(B"
#define M_BEGIN     "\E["
#define M_END       "H"
#define C_BEGIN     "\E["
#define C_END       "m"
#define SPADE       "\xE2\x99\xA0"
#define CLUB        "\xE2\x99\xA3"
#define HEART       "\xE2\x99\xA5"
#define DIAMOND     "\xE2\x99\xA6"
#define C_BLACK     0
#define C_RED       1
#define C_WHITE     7

void print_card(int card, int x, int y);
void gotoXY(int x, int y);
void set_bg_color(int color);
void set_fg_color(int color);
void poker_sign_generate(int_least64_t p, int_least64_t *c, int_least64_t *d);
void poker_cards_generate(int *cards, int size);
void poker_cards_crypt(int *cards, int size, int_least64_t c, int_least64_t p);
void poker_cards_rand(int *cards, int size);
void poker_save_key(int_least64_t c, int_least64_t d, char *filename);
void poker_load_key(int_least64_t *c, int_least64_t *d, char *filename);
void poker_save_prime(int_least64_t p);
void poker_load_prime(int_least64_t *p);
void poker_save_cards(int *cards, int size, char *filename);
void poker_load_cards(int *cards, int size, char *filename);
#endif