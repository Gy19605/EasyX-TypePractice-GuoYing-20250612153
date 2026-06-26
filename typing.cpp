#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define USER_FILE "user.txt"
#define SCORE_FILE "score.txt"
#define KEYREC_FILE "keyrecord.txt"

// 个人标识
const char NAME[] = "郭颖";
const char ID[] = "20250612153";

typedef struct User {
    char acc[20];
    char pwd[20];
} User;

typedef struct Score {
    char acc[20];
    int speed;
    int right;
    int total;
    long long t;
} Score;

int keyCnt[256] = { 0 };
char curUser[20] = "";
int setTime = 60;

void drawTitle(const char* t) {
    settextcolor(WHITE);
    settextstyle(25, 0, "宋体");
    char s[100];
    sprintf(s, "%s | 姓名:%s 学号:%s", t, NAME, ID);
    outtextxy(20, 10, s);
}

int checkUser(char* a, char* p) {
    FILE* f = fopen(USER_FILE, "r");
    if (!f) return -1;
    User u;
    while (fscanf(f, "%s %s", u.acc, u.pwd) == 2) {
        if (strcmp(u.acc, a) == 0 && strcmp(u.pwd, p) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int regUser(char* a, char* p) {
    if (strlen(a) == 0 || strlen(p) == 0) return 0;
    FILE* f = fopen(USER_FILE, "a");
    if (!f) return 0;
    fprintf(f, "%s %s\n", a, p);
    fclose(f);
    return 1;
}

void saveScore(int sp, int r, int tot) {
    FILE* f = fopen(SCORE_FILE, "a");
    if (!f) return;
    Score sc;
    strcpy(sc.acc, curUser);
    sc.speed = sp;
    sc.right = r;
    sc.total = tot;
    sc.t = (long long)time(NULL);
    fprintf(f, "%s %d %d %d %lld\n", sc.acc, sc.speed, sc.right, sc.total, sc.t);
    fclose(f);
}

void saveKeyRec() {
    FILE* f = fopen(KEYREC_FILE, "w");
    for (int i = 0; i < 256; i++) {
        if (keyCnt[i] > 0)
            fprintf(f, "%c:%d\n", i, keyCnt[i]);
    }
    fclose(f);
}

void loginPage() {
    cleardevice();
    drawTitle("登录界面");
    char acc[20] = "", pwd[20] = "";
    int posA = 0, posP = 0;
    bool isAcc = true;
    settextstyle(20, 0, "宋体");
    while (1) {
        rectangle(50, 80, 300, 120);
        rectangle(50, 150, 300, 190);
        settextcolor(YELLOW);
        outtextxy(50, 82, "账号:");
        outtextxy(50, 152, "密码:");
        char buf[30];
        sprintf(buf, "%s", acc);
        outtextxy(120, 82, buf);
        sprintf(buf, "%s", pwd);
        outtextxy(120, 152, buf);
        settextcolor(GREEN);
        outtextxy(50, 220, "回车确认 | Tab切换输入 | ESC返回");
        if (_kbhit()) {
            char k = _getch();
            if (k == 27) return;
            if (k == '\t') isAcc = !isAcc;
            if (k == '\r') {
                int res = checkUser(acc, pwd);
                if (res == 1) {
                    strcpy(curUser, acc);
                    return;
                }
                else {
                    outtextxy(50, 260, "账号密码错误!");
                    Sleep(800);
                    cleardevice();
                    drawTitle("登录界面");
                    posA = posP = 0;
                    memset(acc, 0, sizeof(acc));
                    memset(pwd, 0, sizeof(pwd));
                }
            }
            if (k == 8) {
                if (isAcc && posA > 0) acc[--posA] = 0;
                if (!isAcc && posP > 0) pwd[--posP] = 0;
            }
            if ((k >= '0' && k <= '9') || (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')) {
                if (isAcc && posA < 19) acc[posA++] = k;
                if (!isAcc && posP < 19) pwd[posP++] = k;
            }
        }
    }
}

void regPage() {
    cleardevice();
    drawTitle("注册界面");
    char acc[20] = "", pwd[20] = "";
    int posA = 0, posP = 0;
    bool isAcc = true;
    settextstyle(20, 0, "宋体");
    while (1) {
        rectangle(50, 80, 300, 120);
        rectangle(50, 150, 300, 190);
        settextcolor(YELLOW);
        outtextxy(50, 82, "新账号:");
        outtextxy(50, 152, "新密码:");
        char buf[30];
        sprintf(buf, "%s", acc);
        outtextxy(130, 82, buf);
        sprintf(buf, "%s", pwd);
        outtextxy(130, 152, buf);
        settextcolor(GREEN);
        outtextxy(50, 220, "回车注册 | Tab切换 | ESC返回");
        char k = _getch();
        if (k == 27) return;
        if (k == '\t') isAcc = !isAcc;
        if (k == '\r') {
            if (regUser(acc, pwd)) {
                outtextxy(50, 260, "注册成功!");
                Sleep(1000);
                return;
            }
            else outtextxy(50, 260, "注册失败!");
        }
        if (k == 8) {
            if (isAcc && posA > 0) acc[--posA] = 0;
            if (!isAcc && posP > 0) pwd[--posP] = 0;
        }
        if ((k >= '0' && k <= '9') || (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')) {
            if (isAcc && posA < 19) acc[posA++] = k;
            if (!isAcc && posP < 19) pwd[posP++] = k;
        }
    }
}

void settingPage() {
    cleardevice();
    drawTitle("练习设置");
    settextstyle(20, 0, "宋体");
    char s[50];
    sprintf(s, "当前练习时长：%d 秒", setTime);
    outtextxy(30, 80, s);
    outtextxy(30, 120, "按+增加时长，按-减少，ESC返回");
    while (1) {
        if (_kbhit()) {
            char k = _getch();
            if (k == 27) return;
            if (k == '+') setTime += 10;
            if (k == '-' && setTime > 10) setTime -= 10;
            cleardevice();
            drawTitle("练习设置");
            sprintf(s, "当前练习时长：%d 秒", setTime);
            outtextxy(30, 80, s);
            outtextxy(30, 120, "按+增加时长，按-减少，ESC返回");
        }
    }
}

void showHistory() {
    cleardevice();
    drawTitle("历史成绩记录");
    settextstyle(18, 0, "宋体");
    FILE* f = fopen(SCORE_FILE, "r");
    if (!f) {
        outtextxy(20, 80, "暂无成绩记录");
        _getch();
        return;
    }
    Score sc;
    int y = 80;
    outtextxy(20, 50, "账号 速度 正确数 总字符 时间");
    while (fscanf(f, "%s %d %d %d %lld", sc.acc, &sc.speed, &sc.right, &sc.total, &sc.t) == 5) {
        char buf[100];
        sprintf(buf, "%s %d %d %d", sc.acc, sc.speed, sc.right, sc.total);
        outtextxy(20, y, buf);
        y += 25;
        if (y > 450) break;
    }
    fclose(f);
    outtextxy(20, y + 20, "按任意键返回");
    _getch();
}

void showKeyHabit() {
    cleardevice();
    drawTitle("个人按键习惯统计");
    settextstyle(18, 0, "宋体");
    FILE* f = fopen(KEYREC_FILE, "r");
    if (!f) {
        outtextxy(20, 80, "暂无按键数据");
        _getch();
        return;
    }
    char ch; int cnt, y = 80;
    while (fscanf(f, "%c:%d\n", &ch, &cnt) == 2) {
        char buf[50];
        sprintf(buf, "按键%c：敲击%d次", ch, cnt);
        outtextxy(20, y, buf);
        y += 25;
        if (y > 450) break;
    }
    fclose(f);
    outtextxy(20, y + 20, "按任意键返回");
    _getch();
}

void typingGame() {
    cleardevice();
    drawTitle("打字练习");
    memset(keyCnt, 0, sizeof(keyCnt));
    srand((unsigned)time(NULL));
    char target[200] = "", input[200] = "";
    int tPos = 0, iPos = 0, right = 0, total = 0;
    for (int i = 0; i < 150; i++)
        target[i] = 'a' + rand() % 26;
    target[150] = 0;
    clock_t start = clock();
    int run = 1;
    settextstyle(22, 0, "宋体");
    while (run) {
        double pass = 1.0 * (clock() - start) / CLOCKS_PER_SEC;
        if (pass >= setTime) run = 0;
        rectangle(20, 80, 600, 130);
        rectangle(20, 160, 600, 210);
        settextcolor(WHITE);
        outtextxy(20, 85, "目标：");
        outtextxy(80, 85, target + tPos);
        outtextxy(20, 165, "输入：");
        for (int i = 0; i < iPos; i++) {
            if (input[i] == target[tPos + i])
                settextcolor(GREEN);
            else
                settextcolor(RED);
            char c[2];
            c[0] = input[i];
            c[1] = 0;
            outtextxy(80 + i * 14, 165, c);
            keyCnt[(unsigned char)input[i]]++;
        }
        settextcolor(YELLOW);
        char info[100];
        int speed = pass > 0 ? (int)(right / pass * 60) : 0;
        sprintf(info, "速度:%d 正确:%d 总输入:%d 剩余%.1fs", speed, right, total, setTime - pass);
        outtextxy(20, 250, info);
        if (_kbhit()) {
            char k = _getch();
            if (k == 27) { run = 0; break; }
            if (k == 8 && iPos > 0) {
                iPos--; total--;
            }
            else if ((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')) {
                input[iPos] = k;
                if (k == target[tPos + iPos]) right++;
                iPos++; total++;
                input[iPos] = 0;
            }
            if (iPos >= 150) {
                saveScore(speed, right, total);
                saveKeyRec();
                break;
            }
        }
    }
    saveKeyRec();
    outtextxy(20, 300, "练习结束，按任意键返回");
    _getch();
}

void mainMenu() {
    initgraph(650, 500);
    setbkcolor(BLACK);
    cleardevice();
    while (1) {
        cleardevice();
        drawTitle("主菜单");
        settextstyle(22, 0, "宋体");
        settextcolor(WHITE);
        outtextxy(50, 80, "1 - 用户注册");
        outtextxy(50, 120, "2 - 用户登录");
        outtextxy(50, 160, "3 - 开始打字练习(需登录)");
        outtextxy(50, 200, "4 - 练习设置");
        outtextxy(50, 240, "5 - 查看历史成绩");
        outtextxy(50, 280, "6 - 个人按键习惯统计");
        outtextxy(50, 340, "0 - 退出程序");
        char userTip[80];
        if (strlen(curUser) > 0)
            sprintf(userTip, "当前登录账号：%s", curUser);
        else
            sprintf(userTip, "未登录");
        settextcolor(CYAN);
        outtextxy(50, 400, userTip);
        char k = _getch();
        switch (k) {
        case '1': regPage(); break;
        case '2': loginPage(); break;
        case '3':
            if (strlen(curUser) == 0) {
                cleardevice(); drawTitle("提示");
                outtextxy(50, 150, "请先登录账号！");
                _getch();
            }
            else typingGame();
            break;
        case '4': settingPage(); break;
        case '5': showHistory(); break;
        case '6': showKeyHabit(); break;
        case '0': closegraph(); return;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
