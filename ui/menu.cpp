#include <ncursesw/curses.h>
#include <string>
#include <vector>
#include <algorithm>
#include <locale.h>

static void draw_menu(const std::vector<std::string>& items, int sel,
                      WINDOW*& win, int& y0, int& x0, int& h, int& w) {
    int H,W; getmaxyx(stdscr,H,W);
    w = 0; for (auto& s: items) w = std::max<int>(w, (int)s.size());
    w += 4; h = (int)items.size() + 2;
    y0 = (H-h)/2; x0 = (W-w)/2;
    if (win) delwin(win);
    win = newwin(h,w,y0,x0);
    box(win,0,0);
    for (int i=0;i<(int)items.size();++i){
        if (i==sel) wattron(win, COLOR_PAIR(2));
        mvwprintw(win, i+1, 2, "%s", items[i].c_str());
        if (i==sel) wattroff(win, COLOR_PAIR(2));
    }
    wrefresh(win);
}

static void draw_menu_titled(const std::string& title,
                             const std::vector<std::string>& items, int sel,
                             WINDOW*& win, int& y0, int& x0, int& h, int& w) {
    int H,W; getmaxyx(stdscr,H,W);
    w = (int)title.size();
    for (auto& s: items) w = std::max<int>(w, (int)s.size());
    w += 4; h = (int)items.size() + 3;
    y0 = (H-h)/2; x0 = (W-w)/2;
    if (win) delwin(win);
    win = newwin(h,w,y0,x0);
    box(win,0,0);
    wattron(win, COLOR_PAIR(3));
    mvwprintw(win,1,(w-(int)title.size())/2,"%s",title.c_str());
    wattroff(win, COLOR_PAIR(3));
    for (int i=0;i<(int)items.size();++i){
        if (i==sel) wattron(win, COLOR_PAIR(2));
        mvwprintw(win, i+2, 2, "%s", items[i].c_str());
        if (i==sel) wattroff(win, COLOR_PAIR(2));
    }
    wrefresh(win);
}

void init_ui(){
    setlocale(LC_ALL,"");
    initscr(); noecho(); cbreak();
    keypad(stdscr, TRUE); curs_set(0);
    if (has_colors()){
        start_color(); use_default_colors();
        init_pair(1, COLOR_WHITE, -1);
        init_pair(2, COLOR_BLACK, COLOR_CYAN);
        init_pair(3, COLOR_YELLOW, -1);
        bkgd(COLOR_PAIR(1));
    }
    refresh();
    mousemask(ALL_MOUSE_EVENTS, nullptr);
}

void shutdown_ui(){ endwin(); }

void message_center(const std::string& title, const std::string& text){
    int H,W; getmaxyx(stdscr,H,W);
    int w = std::max<int>((int)title.size(), (int)text.size()) + 4;
    int h = 5, y0=(H-h)/2, x0=(W-w)/2;
    WINDOW* win = newwin(h,w,y0,x0);
    box(win,0,0);
    wattron(win,COLOR_PAIR(3));
    mvwprintw(win,1,(w-(int)title.size())/2,"%s",title.c_str());
    wattroff(win,COLOR_PAIR(3));
    mvwprintw(win,2,2,"%s",text.c_str());
    wrefresh(win);
    wgetch(win);
    delwin(win); clear(); refresh();
}

int run_menu(const std::vector<std::string>& items){
    int sel=0; WINDOW* win=nullptr; int y0,x0,h,w; MEVENT ev;
    draw_menu(items,sel,win,y0,x0,h,w);
    for(;;){
        int ch=getch();
        if (ch==KEY_UP){ sel=(sel+items.size()-1)%items.size(); draw_menu(items,sel,win,y0,x0,h,w); }
        else if (ch==KEY_DOWN){ sel=(sel+1)%items.size(); draw_menu(items,sel,win,y0,x0,h,w); }
        else if (ch==10 || ch==KEY_ENTER){ delwin(win); clear(); refresh(); return sel; }
        else if (ch==KEY_MOUSE){
            if (getmouse(&ev)==OK && (ev.bstate&BUTTON1_CLICKED)){
                int my=ev.y-y0-1, mx=ev.x-x0;
                if (my>=0 && my<(int)items.size() && mx>=0 && mx<w){ sel=my; delwin(win); clear(); refresh(); return sel; }
            }
        } else if (ch==KEY_RESIZE){ draw_menu(items,sel,win,y0,x0,h,w); }
        else if (ch=='q' || ch==27){ delwin(win); clear(); refresh(); return -1; }
    }
}

int run_menu_titled(const std::string& title, const std::vector<std::string>& items){
    int sel=0; WINDOW* win=nullptr; int y0,x0,h,w; MEVENT ev;
    draw_menu_titled(title,items,sel,win,y0,x0,h,w);
    for(;;){
        int ch=getch();
        if (ch==KEY_UP){ sel=(sel+items.size()-1)%items.size(); draw_menu_titled(title,items,sel,win,y0,x0,h,w); }
        else if (ch==KEY_DOWN){ sel=(sel+1)%items.size(); draw_menu_titled(title,items,sel,win,y0,x0,h,w); }
        else if (ch==10 || ch==KEY_ENTER){ delwin(win); clear(); refresh(); return sel; }
        else if (ch==KEY_MOUSE){
            if (getmouse(&ev)==OK && (ev.bstate&BUTTON1_CLICKED)){
                int my=ev.y-y0-2, mx=ev.x-x0;
                if (my>=0 && my<(int)items.size() && mx>=0 && mx<w){ sel=my; delwin(win); clear(); refresh(); return sel; }
            }
        } else if (ch==KEY_RESIZE){ draw_menu_titled(title,items,sel,win,y0,x0,h,w); }
        else if (ch=='q' || ch==27){ delwin(win); clear(); refresh(); return -1; }
    }
}

int run_submenu(const std::string& title, const std::vector<std::string>& items){
    std::vector<std::string> v=items; if (v.empty() || v.back()!="Volver") v.push_back("Volver");
    int i = run_menu_titled(title,v); if (i<0 || i==(int)v.size()-1) return -1; return i;
}

std::string input_box(const std::string& title, const std::string& prompt, int maxlen){
    int H,W; getmaxyx(stdscr,H,W);
    int w = std::max<int>({(int)title.size(), (int)prompt.size()+maxlen+1, 20}) + 4;
    int h = 6, y0=(H-h)/2, x0=(W-w)/2;
    WINDOW* win = newwin(h,w,y0,x0);
    box(win,0,0);
    wattron(win,COLOR_PAIR(3));
    mvwprintw(win,1,(w-(int)title.size())/2,"%s",title.c_str());
    wattroff(win,COLOR_PAIR(3));
    mvwprintw(win,2,2,"%s",prompt.c_str());
    wrefresh(win);
    echo(); curs_set(1);
    char buf[1024]; int n=std::min(maxlen,(int)sizeof(buf)-1);
    mvwgetnstr(win,3,2,buf,n);
    noecho(); curs_set(0);
    std::string out(buf);
    delwin(win); clear(); refresh();
    return out;
}

bool confirm_box(const std::string& title, const std::string& question){
    std::string t = title + " - " + question;
    int i = run_menu_titled(t, {"Si","No"});
    return i==0;
}

void list_box(const std::string& title, const std::vector<std::string>& lines){
    int H,W; getmaxyx(stdscr,H,W);
    int w=(int)title.size(); for (auto& s: lines) w=std::max<int>(w,(int)s.size());
    w=std::min(W-4,w+4); int vis=std::min(H-4,(int)lines.size()); if (vis<1) vis=1;
    int h=vis+3, y0=(H-h)/2, x0=(W-w)/2; WINDOW* win=newwin(h,w,y0,x0); keypad(win,TRUE);
    int off=0;
    for(;;){
        werase(win); box(win,0,0);
        wattron(win,COLOR_PAIR(3)); mvwprintw(win,1,(w-(int)title.size())/2,"%s",title.c_str()); wattroff(win,COLOR_PAIR(3));
        for (int i=0;i<vis;++i){ int idx=off+i; if(idx>=(int)lines.size()) break; mvwprintw(win,i+2,2,"%.*s",w-4,lines[idx].c_str()); }
        wrefresh(win);
        int ch=wgetch(win);
        if (ch==KEY_UP && off>0) --off;
        else if (ch==KEY_DOWN && off+vis<(int)lines.size()) ++off;
        else if (ch==KEY_PPAGE && off>0) off=std::max(0,off-vis);
        else if (ch==KEY_NPAGE && off+vis<(int)lines.size()) off=std::min((int)lines.size()-vis, off+vis);
        else if (ch==10 || ch==KEY_ENTER || ch==27){ delwin(win); clear(); refresh(); return; }
        else if (ch==KEY_RESIZE){ getmaxyx(stdscr,H,W); w=std::min(W-4,w); y0=(H-h)/2; x0=(W-w)/2; mvwin(win,y0,x0); }
    }
}
