#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#ifndef __NCURSES_H__
#define __NCURSES_H__

/* #define SINGLE */

#define _MAX_X 10
#define _MAX_Y 10
#define ON 1
#define OFF 0
#define TRUE 1
#define FALSE 0

#define CHAR_NONE 0
#define CHAR_WALL 1

// 構造体定義
typedef struct _position {
	int x;
	int y;
} POSITION;

typedef struct _block {
	int type;
	int rotate;
	POSITION pos;
} BLOCK;

// グローバル変数
char board[_MAX_Y][_MAX_X]; // 盤面
BLOCK current; // 現在のブロック
POSITION tile[7][4] = {
	{{-1, 0}, {0, 0}, { 1,  0}, { 2,  0}}, // STICK
	{{0, -1}, {0, 0}, { 1,  0}, { 2,  0}}, // L1
	{{0, -1}, {0, 0}, {-1,  0}, {-2,  0}}, // L2
	{{-1, 0}, {0, 0}, { 0,  1}, { 1,  1}}, // KEY1
	{{1,  0}, {0, 0}, { 0,  1}, {-1,  1}}, // KEY2
	{{-1, 0}, {0, 0}, { 0,  1}, {-1,  1}}, // SQUARE
	{{-1, 0}, {0, 0}, { 0, -1}, { 1,  0}}  // T
};

// プロトタイプ宣言
static void init(void);
static int put_block(int on_off_flg );
static int is_puttable( const int x, const int y );
static int rotate_block(void);
static int init_board(void);
static void delete_lines(void);
static void delete_line(void);
static void game_over(void);
static void put_char( int y, int x );

#endif //__NCURSES_H__
