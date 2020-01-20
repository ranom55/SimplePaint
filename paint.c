#include <ncurses.h>

#define BKGD 0
#define BLUE 1
#define RED 2
#define GREEN 3
#define YELLOW 4
#define LIM_COLOR 4

WINDOW* help;

void drawHelp()
{
    int ch;
    touchwin(help);
    wrefresh(help);

    while (ch = getch() != '\n');

    touchwin(stdscr);
    wrefresh(stdscr);
}

int main()
{   
    int ch;
    short color = 1;
    MEVENT mouse_event;

    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    start_color();

    keypad(stdscr, TRUE);
    curs_set(0);

    if (!can_change_color)
    {
        printw("Your terminal does not support color customization.\n");
        napms(3000);
        endwin();
        return 1;
    }

    init_color(BKGD, 0, 0, 0);
    init_color(BLUE, 0, 0, 1000);
    init_color(RED, 1000, 0, 0);
    init_color(GREEN, 0, 1000, 0);
    init_color(YELLOW, 1000, 1000, 0);

    init_pair(BLUE, COLOR_WHITE, BLUE);
    init_pair(BKGD, COLOR_WHITE, BKGD);
    init_pair(RED, RED, RED);
    init_pair(GREEN, GREEN, GREEN);
    init_pair(YELLOW, YELLOW, YELLOW);

    mvhline(getmaxy(stdscr) / 2 - 3, getmaxx(stdscr) / 2 - 40, ACS_HLINE, 80);
    mvhline(getmaxy(stdscr) / 2 + 3, getmaxx(stdscr) / 2 - 40, ACS_HLINE, 80);

    mvvline(getmaxy(stdscr) / 2 - 3, getmaxx(stdscr) / 2 - 40, ACS_VLINE, 7);
    mvvline(getmaxy(stdscr) / 2 - 3, getmaxx(stdscr) / 2 + 40, ACS_VLINE, 7);

    mvaddstr(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - 15, "GNU PAINTER BY RULYAN SCHIAVO");
    mvaddstr(getmaxy(stdscr) / 2 + 3, getmaxx(stdscr) / 2 - 12, "PRESS ENTER TO CONTINUE");

    while (1)
    {
        ch = getch();

        if (ch == '\n')
        {
            break;
        }
        if (ch == KEY_F(4))
        {
            endwin();
            return 0;
        }
    }
    clear();
    refresh();

    #ifdef NCURSES_MOUSE_VERSION
        mousemask(ALL_MOUSE_EVENTS, NULL);
    #else
        printw("Neither your terminal does not have support for the mouse or your ncurses version didnt\n");
        napms(3000);
        endwin()
        return 1;
    #endif
    help = newwin(20, 60, getmaxy(stdscr) / 2 - 10, getmaxx(stdscr) / 2 - 30);

    if (help == NULL)
    {
        endwin();
        return 1;
    }

    wbkgd(help, COLOR_PAIR(BLUE));
    mvwaddstr(help, 2, 10, "C-change color");
    mvwaddstr(help, 4, 10, "F2-clear the screen");
    mvwaddstr(help, 6, 10, "S-save to draw.dat");
    mvwaddstr(help, 8, 10, "R-restore from draw.dat");
    mvwaddstr(help, 10, 10, "F4-exit");
    mvwaddstr(help, 12, 10, "F5-help");
    mvwaddstr(help, 14, 10, "Enter-exit help");
    box(help, ACS_VLINE, ACS_HLINE);

    while (1)
    {
        getmouse(&mouse_event);
        ch = getch();

        if (mouse_event.bstate == BUTTON1_PRESSED)
        {
            attrset(COLOR_PAIR(color));
            wmove(stdscr, mouse_event.y, mouse_event.x);
            printw(" ");
            refresh();
            attroff(COLOR_PAIR(color));
        }
        if (mouse_event.bstate == BUTTON3_PRESSED)
        {
            attrset(COLOR_PAIR(BKGD));
            move(mouse_event.y, mouse_event.x);
            printw(" ");
            refresh();
            attroff(COLOR_PAIR(BKGD));
        }
        switch (ch)
        {
            case KEY_F(5):
                scr_dump("temp.dat");
                drawHelp();
                scr_restore("temp.dat");
                refresh();
                break;

            case KEY_F(2):
                clear();
                refresh();
                break;
            
            case KEY_F(4):
                endwin();
                return 0;
                break;

            case 'c':
                ++color;
                if (color > LIM_COLOR) color = 1;
                break;

            case 's':
            case 'S':
                scr_dump("draw.dat");
                break;

            case 'r':
            case 'R':
                scr_restore("draw.dat");
                refresh();
        }
    }
    endwin();
    return 0;
}
