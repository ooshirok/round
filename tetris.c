#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <ncurses.h>

#include "tetris.h"

int main( int argc, char *argv[] )
{

	// 変数宣言
	int dx = 0; // 移動量
	int dy = 0; // 移動量
	int tmp_x = 0; // 移動後の位置
	int tmp_y = 0; // 移動後の位置
	int ret;
	int wait_cnt = 0;
	int down_flg = FALSE;
	int rotate_flg = FALSE;
	char c = 0;

	current.type   = 3;
	current.rotate = 1;
	current.pos.x  = 5;
	current.pos.y  = 1;

	init();

	mvprintw(21, 0, "h: LEFT, l: RIGHT, j: DOWN, k: ROTATE, q: QUIT");

	init_board(); // 盤面初期化

	attrset(COLOR_PAIR(current.type + 1)); // カラー切替え
	ret = put_block( ON ); // 最初のブロック配置

	for(;;)
	{
		mvprintw(22, 0, " %2d %2d ", current.pos.x, current.pos.y);

		// キー入力
		c=(char)getch();

		if((char*)strchr("hjklq", c)==NULL){
			c='X';
			wait_cnt++;
			if (wait_cnt % 8 == 0)
			{
				// 自動下降
				dx = 0; dy = 1;
				down_flg = TRUE;
			}
			//continue;
		}

		if(c=='q') break;
		if(c=='l') { // 右
			dx = 1; dy = 0;
		}
		if(c=='h') { // 左
			dx = -1; dy = 0;
		}
		if(c=='j') { // 下
			dx = 0; dy = 1;
			down_flg = TRUE;
		}
		if(c=='k') { // 回転
			dx = 0; dy = 0;
			current.rotate++;
			rotate_flg = TRUE;
		}

		// いったん消して
		ret = put_block( OFF );

		if ( TRUE == is_puttable(current.pos.x + dx, current.pos.y + dy) ) { 
			// 置けるなら、移動後の座標に置く
			current.pos.x = current.pos.x + dx;
			current.pos.y = current.pos.y + dy;
			ret = put_block( ON );
		}
		else
		{
			// 置けないなら、再表示
			ret = put_block( ON );
			if ( rotate_flg == TRUE )
			{
				// 回転中だった場合は回転を戻す
				current.rotate--;
			}
			if ( down_flg == TRUE )
			{
				// 下降中だった場合は次のブロックを
				current.type = (int) (6.0 * rand() / (RAND_MAX + 1.0)); // 0-6
				current.rotate = 1;
				current.pos.x = 5;
				current.pos.y = 1;
				attrset(COLOR_PAIR(current.type + 1)); // カラー切替え
				//ret = put_block( ON );
				if ( TRUE != is_puttable(current.pos.x, current.pos.y) )
				{
					game_over();
					break;
				}
			}
		}
		rotate_flg = FALSE;
		down_flg = FALSE;
		dx = 0;
		dy = 0;

		// 列削除の判定
		//delete_lines();
	}

	endwin();

}

/*
 * init
 */
static void init(void)
{
	// 初期設定
	setlocale( LC_ALL, "" );
	initscr();
	cbreak();
	noecho();
	start_color();
	init_pair(99,COLOR_WHITE  , COLOR_WHITE);
	init_pair(1,COLOR_BLACK  , COLOR_WHITE);
	init_pair(2,COLOR_RED    , COLOR_WHITE);
	init_pair(3,COLOR_GREEN  , COLOR_WHITE);
	init_pair(4,COLOR_YELLOW , COLOR_WHITE);
	init_pair(5,COLOR_BLUE   , COLOR_WHITE);
	init_pair(6,COLOR_MAGENTA, COLOR_WHITE);
	init_pair(7,COLOR_CYAN   , COLOR_WHITE);
	wtimeout(stdscr, 100); // getch() でブロックされないように
}

/*
 * game_over
 */
static void game_over(void)
{
	mvprintw(21, 0, "==============");

	int x, y;
	for ( y=0; y < _MAX_Y; y++)
	{
		for ( x=0; x < _MAX_X; x++)
		{
			board[y][x] = CHAR_WALL;
			put_char(y, x);
		}
	}
	sleep(1);
}

/*
 * delete_lines
 */
static void delete_lines(void)
{
	int x, y, j;
	int non_block_flg = FALSE;

	for ( y = _MAX_Y; y > 0; y--)
	{
		non_block_flg = FALSE;

		// 横1列のチェック
		for ( x=1; x<_MAX_X; x++)
		{
			if ( board[y][x] != CHAR_WALL )
			{
				non_block_flg = TRUE;
			}
		}

		if ( non_block_flg == FALSE )
		{
			// 列の削除して、
			/*
			for ( x=1; x<_MAX_X; x++)
			{
				board[y][x] = CHAR_NONE;
			}
			*/
			// 1段下へずらす
			for ( j = y; j > 0; j-- )
			{
				for ( x=1; x<_MAX_X; x++)
				{
					board[j][x] = board[j-1][x];
				}
			}
		}
	}
}

/*
 * put_block
 */
static int put_block(int on_off_flg)
{
	int i;
	int _x, _y;
	char c;

	if ( on_off_flg == ON ) {
		c = CHAR_WALL; // "#"
	} else {
		c = CHAR_NONE; // " "(space)
	}

	for ( i=0; i<4; i++)
	{
		_x = current.pos.x + tile[current.type][i].x;
		_y = current.pos.y + tile[current.type][i].y;
		board[_y][_x] = c; // ブロック
		//mvprintw(_y, _x, "%c", board[_y][_x]);
		put_char( _y, _x );
	}

	return 0;
}

/*
 * is_puttable
 */
static int is_puttable( const int x, const int y )
{
	int i;
	int _x, _y;
	char c;

	for ( i=0; i<4; i++)
	{
		_x = x + tile[current.type][i].x;
		_y = y + tile[current.type][i].y;

		// debug
		//mvprintw(24+i, 0, "i:%d, x:%02d, y:%02d, _x:%02d, _y:%02d, brd[%02d][%02d]:%02d", i, x, y, _x, _y, _y, _x, board[_y][_x] );

		if (board[_y][_x] == CHAR_WALL) // 壁、床、ブロックに衝突
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * rotate_block
 */
static int rotate_block(void)
{
/*
 x' = cosB -sinB  x
 y' = sinB  cosB  y
 B = pi/2  (反時計回り90度)の場合、
	x' = -y
	y' = x
 B = -pi/2 (時計回り90度)の場合、
	x' = y
	y' = -x
*/
	return 0;
}

/*
 * init_board
 */
static int init_board(void)
{
	int x, y;
	for ( y=0; y<_MAX_Y; y++)
	{
		for ( x=0; x<_MAX_X; x++)
		{
			if ( x == 0 || x == _MAX_X - 1 || y == _MAX_Y - 1)
			{
				board[y][x] = CHAR_WALL; // "#" 壁 or 床
				attrset(COLOR_PAIR(0)); // 白地に黒色
			}
			else
			{
				board[y][x] = CHAR_NONE; // " " 空き
				attrset(COLOR_PAIR(99)); // 白地に白色
			}
			//mvprintw(y, x, "%c", board[y][x]);
			put_char( y, x );
		}
	}
}

/*
 * put_char
 */
static void put_char( int y, int x )
{
#ifdef SINGLE
	if ( board[y][x] == CHAR_WALL )
	{
		mvprintw(y, x, "%s", "#");
	}
	else
	{
		mvprintw(y, x, "%s", " ");
	}
#else
	if ( board[y][x] == CHAR_WALL )
	{
		mvprintw(y, x * 2, "%s", "■");
	}
	else
	{
		mvprintw(y, x * 2, "%s", "　");
	}
#endif
}

