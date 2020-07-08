#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <math.h>
#define rozmiar 4

static struct termios oldt;
void up(int plansza[rozmiar][rozmiar])
{
    for(int i=0; i<rozmiar; i++)
        for(int j=0; j<rozmiar; j++)
        {
            if(!plansza[j][i])
            {
                for(int k=j+1; k<rozmiar; k++)
                    if(plansza[k][i])
                    {
                        plansza[j][i]=plansza[k][i];
                        plansza[k][i]=0;
                        break;
                    }

            }
        }
}

void przywrocustawienia(void)
{
    tcsetattr(0, TCSANOW, &oldt);
}

void wylaczenter(void)
{
    struct termios newt;


    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    atexit(przywrocustawienia);
}
int wynik(int plansza[rozmiar][rozmiar])
{
    int wyn=0;
    for(int x=0; x<rozmiar; x++)
    {
        for(int y=0; y<rozmiar; y++)
        {
            if(plansza[x][y])
            {
            wyn+=plansza[x][y]*(log2(plansza[x][y])-1);
            }
        }
    }
    return wyn;
}
void mergeup(int plansza[rozmiar][rozmiar])
{
    for(int i=0; i<rozmiar; i++)
    {
        for(int j=0; j<rozmiar; j++)
        {
            if(plansza[j][i]&&plansza[j][i]==plansza[j+1][i])
            {

                plansza[j][i]+=plansza[j+1][i];
                plansza[j+1][i]=0;
            }
        }
    }
}

int liczeniezer(int plansza[rozmiar][rozmiar])
{
    int licznik=0;
    for(int x=0; x<rozmiar; x++)
    {
        for(int y=0; y<rozmiar; y++)
        {
            if(plansza[x][y]==0)
            {
                licznik++;
            }
        }
    }
    return licznik;
}

int sprawdzpare(int plansza[rozmiar][rozmiar])
{
    int licznik=0;
     for(int i=0; i<rozmiar; i++)
    {
        for(int j=0; j<rozmiar; j++)
        {
            if(plansza[j][i]&&plansza[j][i]==plansza[j+1][i])
            {
                licznik++;
            }
        }
    }
    return licznik;
}

void wyswietl(int plansza[rozmiar][rozmiar])
{
    printf("\033[H");
 	printf("2048 %15d punktow\n",wynik(plansza));
 	printf("        ←,↑,→,↓        \n\n");
    for(int x=0; x<rozmiar; x++)
    {
        for(int y=0; y<rozmiar; y++)
        {
            printf("%4d",plansza[x][y]);
            printf(" ");
        }

        printf("\n");
    }
    printf("\n");
}
void obrocplansze(int plansza[rozmiar][rozmiar])
{
    int tmp[rozmiar][rozmiar];
    memcpy(tmp,plansza,rozmiar * sizeof(plansza[0]));
    for(int x=0; x<rozmiar; x++)
    {
        for(int y=0; y<rozmiar; y++)
        {
            plansza[y][x]=tmp[x][rozmiar-1-y];
        }
    }

}

int koniecgry(int plansza[rozmiar][rozmiar])
{
    if(liczeniezer(plansza)>0)
    {
        return 1;
    }
    if(sprawdzpare(plansza)>0)
    {
        return 1;
    }
    obrocplansze(plansza);
    if(sprawdzpare(plansza)>0)
    {
        obrocplansze(plansza);
        obrocplansze(plansza);
        obrocplansze(plansza);
        return 1;
    }
    obrocplansze(plansza);
    obrocplansze(plansza);
    obrocplansze(plansza);
    return 0;

}

void losowa(int plansza[rozmiar][rozmiar])
{
    int wart[2]={2,4};
    int r=rand()%11;
    int k=rand()%rozmiar;
    int j=rand()%rozmiar;
    while(1)
    {
        if(plansza[k][j]==0)
        {
            if(r>9)
            {
            plansza[k][j]=wart[1];
            }
            else
            {
            plansza[k][j]=wart[0];
            }
            break;
        }
        else
        {
            k=rand()%rozmiar;
            j=rand()%rozmiar;
        }
    }

}
void gora(int plansza[rozmiar][rozmiar])
{
    up(plansza);
    mergeup(plansza);
    up(plansza);
}
void dol(int plansza[rozmiar][rozmiar])
{
    obrocplansze(plansza);
    obrocplansze(plansza);
    up(plansza);
    mergeup(plansza);
    up(plansza);
    obrocplansze(plansza);
    obrocplansze(plansza);
}
void lewo(int plansza[rozmiar][rozmiar])
{
    obrocplansze(plansza);
    obrocplansze(plansza);
    obrocplansze(plansza);
    up(plansza);
    mergeup(plansza);
    up(plansza);
    obrocplansze(plansza);
}
void prawo(int plansza[rozmiar][rozmiar])
{
    obrocplansze(plansza);
    up(plansza);
    mergeup(plansza);
    up(plansza);
    obrocplansze(plansza);
    obrocplansze(plansza);
    obrocplansze(plansza);
}
void init(int plansza[rozmiar][rozmiar])
{
    printf("\033[2J");
    printf("\033[H");
    printf("\033[?25l");
    for(int x=0; x<rozmiar; x++)
    {
        for(int y=0; y<rozmiar; y++)
        {
            plansza[x][y]=0;
        }
    }
    losowa(plansza);
    losowa(plansza);
    wyswietl(plansza);
}

int main()
{

    wylaczenter();
    srand(time(NULL));
    int plansza[rozmiar][rozmiar];
    int kopia[rozmiar][rozmiar];
    int c;
    init(plansza);
    memcpy(kopia,plansza,rozmiar* sizeof(plansza[0]));


    while (1)
    {
        c=getchar();
        printf("%d",c);
        switch(c)
        {

        case 97:	// 'a'
        case 104:	// 'h'
        case 68:	// lewa strzalka
            lewo(plansza);
            if (memcmp(kopia,plansza,rozmiar* sizeof(plansza[0])) !=0)
            {
                losowa(plansza);
            }
            memcpy(kopia,plansza,rozmiar* sizeof(plansza[0]));
            break;
        case 100:	// 'd' key
        case 108:	// 'l' key
        case 67:	// prawa strzalka
            prawo(plansza);
            if (memcmp(kopia,plansza,rozmiar* sizeof(plansza[0])) !=0)
            {
                losowa(plansza);
            }
            memcpy(kopia,plansza,rozmiar* sizeof(plansza[0]));
            break;
        case 119:	// 'w' key
        case 107:	// 'k' key
        case 65:	// strzalka w gore
            gora(plansza);
            if (memcmp(kopia,plansza,rozmiar* sizeof(plansza[0])) !=0)
            {
                losowa(plansza);
            }
            memcpy(kopia,plansza,rozmiar* sizeof(plansza[0]));
            break;
        case 115:	// 's' key
        case 106:	// 'j' key
        case 66:	// strzalka w dol
            dol(plansza);
            if (memcmp(kopia,plansza,rozmiar* sizeof(plansza[0])) !=0)
            {
                losowa(plansza);
            }
            memcpy(kopia,plansza,rozmiar* sizeof(plansza[0]));
            break;

        }
        printf("\033[2J");
        printf("\033[H");
        wyswietl(plansza);
        if (koniecgry(plansza)==0)
        {
            printf("koniec");
            break;
        }
    }
    return 0;
}
