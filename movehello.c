#include <ncurses.h>

static const char message[] = "Hello, world!";
enum { key_escape = 27 }; //отдельный тип перечисление (enum), задающий набор всех возможных целочисленных значений переменной этого типа.

static void show_message(int x, int y)
// будет выдавать сообщение в указанном месте экрана получая координаты через параметр
{
    move(y, x); // меняет расположение корсора в порядке (y, x), а не (x, y) 
    addstr(message); // просто выводят строку символов на экран.
    refresh(); // делает то что написано в описании
}
static void hide_message(int x, int y)
// печатает в соответствующем месте колличестко пробелов
{
    int i;
    move(y, x);
    for(i = 0; i < sizeof(message)-1; i++)
        addch(' '); // выводит один символ в текущую позицию курсора с заданными атрибутами.
    refresh(); 
}
static void check(int *coord, int max)
{
    if(*coord < 0)
        *coord = 0;
    else
    if(*coord > 0)
        *coord = max;
}
static void
move_message(int *x, int *y, int mx, int my, int dx, int dy)
// перемещает в новую позицию экрана, одновременн изменяя переменные в которых храниятся координаты, для этого ей передаются адреса этих переменных
{
    hide_message(*x, *y);
    *x += dx;
    check(x, mx);
    *y += dy;
    check(y, my);
    show_message(*x, *y);
}
static void handle_resize(int *x, int *y, int *mx, int *my)
// обработка события изменения терминала
{
    int row, col;
    getmaxyx(stdscr, row, col);//получает максимально возможное количество строк и столбцов
    *mx = col - sizeof(message)+1;
    *my = row -1;
    hide_message(*x,*y);
    check(x, *mx);
    check(y, *my);
    show_message(*x, *y);
}
int main()
{
    int row, col, x, y, max_x, max_y, key;
    initscr();
    cbreak();//  Ctrl-C, Ctrl-D продолжат работать как обычно, в противном случае нужна ф-я raw()
    keypad(stdscr, 1); // поручить библиотеке обработку escape-последовательностей стрелками, если поставить 0- выключить, стр 235 второго тома.
    noecho(); // включить автоматическое отображение вводимых символов на экране
    curs_set(0);
    getmaxyx(stdscr, row, col);
    x = (col-(sizeof(message)-1))/2;
    y = row/2;
    max_x = col - sizeof(message)+1;
    max_y = row -1;
    show_message(x, y);
    while((key = getch()) != key_escape) {
        switch (key)
        {
        case KEY_UP:
            move_message(&x, &y, max_x, max_y, 0, -1);
            break;
        case KEY_DOWN:
            move_message(&x, &y, max_x, max_y, 0, 1)        ;
            break;
        case KEY_RIGHT:
            move_message(&x, &y, max_x, max_y, 1, 0);
            break;
        case KEY_RESIZE:
            handle_resize(&x, &y, &max_x, &max_y);
            break;
        }
    }
    endwin();
    return 0;
}

// кампиляция
// gcc -Wall -lm -g movehello.c -lncurses -o movehello