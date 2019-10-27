#include "poker.h"

#define LENGTH 30
#define LOG 1

void poker_sign_generate(int_least64_t p, int_least64_t *c, int_least64_t *d)
{
    int_least64_t am[3], bm[3];
    do
    {
        randombytes(c, sizeof(*c));
        *c = fabs(*c % (int_least64_t)(p-2)) + 2;
        am[0] = p; am[1] = 1; am[2] = 0;
        bm[0] = *c; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);
    if(am[2] < 0)
        am[2] += p;
    *d = am[2];
}

void poker_cards_generate(int *cards, int size)
{
    int i = 0;
    for(;i < size; i++)
    {
        cards[i] = i + 2;
    }

}

void poker_cards_crypt(int *cards, int size, int_least64_t c, int_least64_t p)
{
    int i = 0;
    for(;i < size; i++)
    {
        cards[i] = modpow(cards[i], c, p);
    }
}

void poker_cards_rand(int *cards, int size)
{
    int i = 0;
    srand(time(0));
    for(; i < size; i++)
    {
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        int tmp = cards[j];
        cards[j] = cards[i];
        cards[i] = tmp;
    }
}

void poker_save_key(int_least64_t c, int_least64_t d, char *filename)
{
    FILE *fout = file_open(filename, "w");
    fprintf(fout, "%"PRId64" %"PRId64"\n", c, d);
    fclose(fout);
}

void poker_load_key(int_least64_t *c, int_least64_t *d, char *filename)
{
    FILE *fout = file_open(filename, "r");
    fscanf(fout, "%"PRId64" %"PRId64, c, d);
    fclose(fout);
}

void poker_save_prime(int_least64_t p)
{
    FILE *fout = file_open("tmp/poker_prime", "w");
    fprintf(fout, "%"PRId64"\n", p);
    fclose(fout);
}

void poker_load_prime(int_least64_t *p)
{
    FILE *fout = file_open("tmp/poker_prime", "r");
    fscanf(fout, "%"PRId64, p);
    fclose(fout);
}

void poker_save_cards(int *cards, int size, char *filename)
{
    FILE *fout = file_open(filename, "w");
    int i = 0;
    for(; i < size; i++)
    {
        fprintf(fout, "%d ", cards[i]);
    }
    fclose(fout);
}

void poker_load_cards(int *cards, int size, char *filename)
{
    FILE *fout = file_open(filename, "r");
    int i = 0;
    for(; i < size; i++)
    {
        fscanf(fout, "%d", &cards[i]);
    }
    fclose(fout);
}

int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "Rus");
    int i, j, size, num_of_player, player, temp;
    char filename[LENGTH], tmp_filename[LENGTH];    
    int_least64_t p, g, c, d;
    int new_size = 2, start = 0, new_cards[new_size];
    if(argc < 4)
    {
        printf("example: ./poker 1 num_of_players num_of_cards\n./poker 2 num_of_players player\n");
        exit(0);
    }
#if LOG
    FILE *fout = file_open("tmp/log", "a");
    int l;
#endif
    sscanf(argv[1], "%d", &temp);
    sscanf(argv[3], "%d", &size);
    int cards[size];
    switch(temp)
    {
    case 1:
        sscanf(argv[2], "%d", &num_of_player);
        if(num_of_player > size)
        {
            printf("num_of_players > size\n");
            exit(-1);
        }
        //generate cards
        poker_cards_generate(cards, size);
#if LOG
        fprintf(fout, "%s\n", "original cards:");
        for(l = 0; l < size; l++)
            fprintf(fout, "%d ", cards[l]);
        fprintf(fout, "\n");
#endif

        //generate prime
        prime_safe_generate(&p, &g, 1e9, 256);
        poker_save_prime(p);

#if LOG
        fprintf(fout, "%s %"PRId64"\n", "prime:", p);
#endif
        poker_save_cards(cards, size, "tmp/cards");
        poker_save_cards(cards, size, "tmp/nocryptcards");

        //generate c d
        for(i = 0; i < num_of_player; i++)
        {
            memset(filename, '\0', LENGTH);
            poker_sign_generate(p-1, &c, &d);
#if LOG
            fprintf(fout, "%s %d %s %"PRId64 " %s %"PRId64"\n", "player:", i+1, "c:", c, "d:", d);
#endif
            sprintf(filename, "%s%d%s", "tmp", i+1, "/keys");
            poker_save_key(c, d, filename);
        }

        //crypt cards
        for(i = 0; i < num_of_player; i++)
        {
            poker_load_cards(cards, size, "tmp/cards");
            memset(filename, '\0', LENGTH);
            sprintf(filename, "%s%d%s", "tmp", i+1, "/keys");
            poker_load_key(&c, &d, filename);
            poker_cards_crypt(cards, size, c, p);
#if LOG
            fprintf(fout, "%d %s\n", i+1, "player crypt cards");
            for(l = 0; l < size; l++)
                fprintf(fout, "%d ", cards[l]);
            fprintf(fout, "\n\n");
#endif
            poker_cards_rand(cards, size);
#if LOG
            fprintf(fout, "%d %s\n", i+1, "player shuffle cards");
            for(l = 0; l < size; l++)
                fprintf(fout, "%d ", cards[l]);
            fprintf(fout, "\n\n");
#endif
            poker_save_cards(cards, size, "tmp/cards");
        }
        poker_load_cards(cards, size, "tmp/cards");
        
        //separation cards
        for(i = 0; i < num_of_player; i++)
        {
            memset(filename, '\0', LENGTH);
            sprintf(filename, "%s%d%s", "tmp", i+1, "/cards_crypt");
#if LOG
            fprintf(fout, "%d %s\n", i+1, "player deck");
            for(l = start; l < start+new_size; l++)
                fprintf(fout, "%d ", cards[l]);
            fprintf(fout, "\n\n");
#endif
            poker_save_cards(&cards[start], new_size, filename);
            
            if(i != num_of_player - 1)
                start += new_size;
        }

        poker_save_cards(&cards[start], size-start, "tmp/cards");
        break;

    case 2:
        //decrypt cards
        sscanf(argv[2], "%d", &num_of_player);
        sscanf(argv[3], "%d", &player);
        memset(filename, '\0', LENGTH);
        sprintf(filename, "%s%d%s", "tmp", player, "/cards_crypt");
        poker_load_prime(&p);
        for(j = 0 ; j < num_of_player; j++)
        {
            if(j+1 != player)
            {
                poker_load_cards(new_cards, new_size, filename);
                memset(tmp_filename, '\0', LENGTH);
                sprintf(tmp_filename, "%s%d%s", "tmp", j+1, "/keys");
                poker_load_key(&c, &d, tmp_filename);
                poker_cards_crypt(new_cards, new_size, d, p);
#if LOG
                fprintf(fout, "%d %s %d %s\n", j+1, "player decrypt ", player, "deck");
                for(l = 0; l < new_size; l++)
                    fprintf(fout, "%d ", new_cards[l]);
                fprintf(fout, "\n\n");
#endif
                memset(filename, '\0', LENGTH);
                sprintf(filename, "%s%d%s", "tmp", player, "/cards_temp_crypt");
                poker_save_cards(new_cards, new_size, filename);
            }
        }
        poker_load_cards(new_cards, new_size, filename);
        memset(tmp_filename, '\0', LENGTH);
        sprintf(tmp_filename, "%s%d%s", "tmp", player, "/keys");
        poker_load_key(&c, &d, tmp_filename);
        poker_cards_crypt(new_cards, new_size, d, p);
#if LOG
                fprintf(fout, "%d %s\n", player, "player decrypt deck");
                for(l = 0; l < new_size; l++)
                    fprintf(fout, "%d ", new_cards[l]);
                fprintf(fout, "\n\n");
#endif
        system("clear");
        for(j = 0; j < new_size; j++)
            print_card(new_cards[j], j*12 + 1, 1);
        break;
    }
    exit(0);
}
