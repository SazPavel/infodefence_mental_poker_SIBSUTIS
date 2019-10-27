#include "poker.h"


void set_fg_color(int color)
{
    printf("%s3%d%s", C_BEGIN, color, C_END);
}

void set_bg_color(int color)
{
    printf("%s4%d%s", C_BEGIN, color, C_END);
}

void gotoXY(int x, int y)
{
    printf("%s%d;%d%s", M_BEGIN, y, x, M_END);
}

void print_card(int card, int x, int y)
{
    int i, j, real_card = card - 2;
    if(real_card > 51)
        return;
    set_bg_color(C_WHITE);
    if(real_card < 26) set_fg_color(C_BLACK);
    if(real_card > 25) set_fg_color(C_RED);
    printf("%s", BC_ENTER);
    for(i = y; i < y+9; i++)
    {
        if(i == y)
        {
            gotoXY(x, y);
            printf("l");
            for(j = x; j < x+8; j++)
                printf("r");
            printf("k");
        }else{
            if(i == y+8)
            {
                gotoXY(x, y+8);
                printf("m");
                for(j = x; j < x+8; j++)
                    printf("r");
                printf("j");
            }else{
                gotoXY(x, i);
                printf("x");
                if(i == y + 1)
                {
                    switch(real_card % 13)
                    {
                    case 0:
                        printf("K");
                        break;
                    case 11:
                        printf("J");
                        break;
                    case 12:
                        printf("Q");
                        break;
                    default:
                        printf("%d", real_card%13);
                    }
                    for(j = x+1; j < x+8; j++)
                        printf(" ");
                }else
                if(i == y + 4)
                {
                    gotoXY(x+1, i);
                    for(j = x; j < x+3; j++)
                        printf(" ");
                    if(real_card < 13)
                        printf("%s",SPADE);
                    if(real_card > 12 && real_card < 26)
                        printf("%s",CLUB);
                    if(real_card > 25 && real_card < 39)
                        printf("%s",HEART);
                    if(real_card > 38 && real_card < 52)
                        printf("%s",DIAMOND);
                    for(j = x+4; j < x+8; j++)
                        printf(" ");
                }else
                if(i == y + 7)
                {
                    gotoXY(x+1, i);
                    for(j = x; j < x+7; j++)
                        printf(" ");
                    gotoXY(x+8, i);
                    switch(real_card % 13)
                    {
                    case 0:
                        printf("K");
                        break;
                    case 11:
                        printf("J");
                        break;
                    case 12:
                        printf("Q");
                        break;
                    default:
                        printf("%d", real_card%13);
                    }
                }else
                    for(j = x; j < x+8; j++)
                        printf(" ");


                gotoXY(x+9, i);
                printf("x");
            }
        }
    }
    gotoXY(0, y+10);
    printf("%s", BC_EXIT);
    set_bg_color(C_BLACK);
    set_fg_color(C_WHITE);
}