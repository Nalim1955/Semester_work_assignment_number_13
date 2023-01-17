#include <stdio.h>
#include <stdlib.h> //malloc
#include <ctype.h> //isspace
#include <string.h> //strlen

#define BUFFER_VELKOST_SLOVO 60
#define BUFFER_VELKOST_SLOVO_MIN 2
#define ZDROJOVY_SUB_TEXTU "db_slov.txt"
#define KOPIROVANY_SUB_TEXTU "db_slov_copy.txt"

int pocetZobrazenychSlov(FILE *suborRead_txt, int dlzkaSlov_int); //Funkcia najde vsetky slova podla zadanej dlzky
int kontrolaSuboru(char *filename); //Funkcia skontroluje otovrenie suboru
int minCharDlzka(FILE *suborRead_txt); //Funkcia najde najkratsie slovo v subore
int maxCharDlzka(FILE *suborRead_txt); //Funkcia najde najdlhsie slovo v subore

void kopirovanieDoSuboru(char *filename, char **pArray_chr, int pocetSlov_int, int velkostRetazca_int); //Funkcia na skopirovanie udajov do suboru
void uvolneniePamate(char **pArray_chr, int pocetSlov_int); //Funkcia na uvolnenie pamate

char *malePismo(char *pSlovo_chr, int dlzkaSlov_int); //Funkcia zmeni velke pismena na male pismena

int main(void)
{
    int velkostRetazca_int=0, pocetSlov_int, minChr_int, maxChr_int;
    int i=0, j=0, k, l;
    char **pArray_chr, *pDocasny_chr, slovo_chr[BUFFER_VELKOST_SLOVO], velkostRetazca_chr[BUFFER_VELKOST_SLOVO];

    FILE *suborOpen_txt;

    if(kontrolaSuboru(ZDROJOVY_SUB_TEXTU)) //Subor existuje
    {

        suborOpen_txt = fopen(ZDROJOVY_SUB_TEXTU,"r"); //Otvorenie suboru na citanie

        minChr_int = minCharDlzka(suborOpen_txt); //Zisti minimalnu dlzku slova v ZDROJOVY_SUB_TEXTU
        maxChr_int = maxCharDlzka(suborOpen_txt); //Zisti maximalnu dlzku slova v ZDROJOVY_SUB_TEXTU

        if (minChr_int == 0)
        {
            printf("Program bol ukonceny !\n\nV subore %s sa nenachadzaju ziadne slova.\n\nJe potrebne subor naplnit slovami.\n\n", ZDROJOVY_SUB_TEXTU);
            exit(1);
        }

        printf("Na hladanie rovnakej dlzky slov, zadaj pocet pismien.\nAktualna minimalna dlzka slova je: %d.\nAktualna maximalna dlzka slova je: %d.\n\n", minChr_int, maxChr_int);

        do
        {
            k=0;

            printf("\nProsim zadaj cislo na hladanie rovnakej dlzky slov:");
            scanf("%s", &velkostRetazca_chr);

            while (velkostRetazca_chr[k])
            {
                if (isdigit(velkostRetazca_chr[k]))
                {
                    velkostRetazca_int = atoi(velkostRetazca_chr);
                }
                else
                {
                    velkostRetazca_int = 0;
                    break;
                }

                k++;
            }

        }
        while((velkostRetazca_int < minChr_int) || (velkostRetazca_int > maxChr_int)); //Cyklus sa opakuje pokial su neni splnene podmienky

        pocetSlov_int = pocetZobrazenychSlov(suborOpen_txt, velkostRetazca_int); //Pocet slov podla zadanej dlzky

        if(pocetSlov_int>0)
        {
            //Ak v subore existuju slova pozadovanej dlzky

            if((pArray_chr=(char**)malloc((pocetSlov_int+1)*sizeof(char *))) == NULL)//Alokovanie pamate pre riadky slov
            {
                printf("Nedostatok pamate. Program sa ukoncil.\n");
                free(pArray_chr); //Ak sa pamat nepodari alokovat tak ju vyprazdni
                pArray_chr=NULL;
                exit(1);
            }

            rewind(suborOpen_txt); //Nastavenie ukazovatela na zaciatok suboru

            while(fscanf(suborOpen_txt, "%s", slovo_chr) != EOF) //Cyklus prechadza slovami a ukonci sa EOF - (koniec súboru)
            {

                if ((pArray_chr[i] = (char *) malloc((velkostRetazca_int+1)*sizeof(char))) == NULL) //Alokovanie pamate pre pismena zo slov
                {
                    printf("Nedostatok pamate. Program sa ukoncil.\n");
                    uvolneniePamate(pArray_chr, i); //Ak sa pamat nepodari alokovat | Spusti sa funkcia uvolneniePamate
                    exit(1);
                }

                if(strlen(slovo_chr) == velkostRetazca_int) //Podmienka rovnakej dlzky slova zo suboru a poziadavky uzivatela
                {
                    strcpy(pArray_chr[i],malePismo(slovo_chr, velkostRetazca_int)); //Kopirovanie slova(retazca) do pArray_chr[i] | Funkcia malePismo - zmeni velke

                    i++;
                }

            }

            if ((pDocasny_chr = (char *) malloc((velkostRetazca_int+1)*sizeof(char))) == NULL) //Alokovanie pamate pre porovnavanie slov
            {
                printf("Nedostatok pamate. Program sa ukoncil.\n");
                free(pDocasny_chr); //Ak sa pamat nepodari alokovat tak ju vyprazdni
                pDocasny_chr=NULL;
                exit(1);
            }

            i=j=0;
            for (i = 0; i < pocetSlov_int; i++)
            {
                for (j = i + 1; j < pocetSlov_int; j++)
                {
                    l = strcmp(pArray_chr[i], pArray_chr[j]); //porovna dva retazce a vrati celociselnou hodnotu


                    if (l > 0) // ak je hodnota l vacsia ako 0, zacne sa vymena | abecedne zoradenie
                    {
                        //Pomocov pDocasny_chr sa prechadzuju adresy a usporiadavaju sa abecedne
                        pDocasny_chr = pArray_chr[i];
                        pArray_chr[i] = pArray_chr[j];
                        pArray_chr[j] = pDocasny_chr;
                    }
                }
            }

            printf("\nSlova rovnakej dlzky v subore %s su nasledovne:\n\n", ZDROJOVY_SUB_TEXTU);
            for (i = 0; i < pocetSlov_int; i++)
                printf("%s\n", pArray_chr[i]); //Vypise na obrazovku slova podla kriterii

            printf("\nCelkovy pocet slov rovnakej dlzky v subore %s je: %d\n", ZDROJOVY_SUB_TEXTU, pocetSlov_int);

            if(kontrolaSuboru(KOPIROVANY_SUB_TEXTU)) //Kontrola suboru do ktoreho sa budu kopirovat slova podla kriterii uzivatela
            {
                //Ak subor existuje funkcia kopiruje slova
                kopirovanieDoSuboru(KOPIROVANY_SUB_TEXTU, pArray_chr, pocetSlov_int, velkostRetazca_int);
            }
            else
            {
                //Ak sa subor nenasiel vytvori ho
                kopirovanieDoSuboru(KOPIROVANY_SUB_TEXTU, pArray_chr, pocetSlov_int, velkostRetazca_int);
            }

            uvolneniePamate(pArray_chr, pocetSlov_int); //Funkcia na vyprazdnenie pamate pArray_chr

        }
        else
        {
            //Ak v subore neexistuju slova pozadovanej dlzky
            printf("\nV subore sa nenachadzaju slova pozadovanej dlzky %d.", velkostRetazca_int);
        }


        fclose(suborOpen_txt); //Zatvorenie suboru na citanie

    }
    else //Subor neexistuje
    {
        printf("Pozadovany subor nebolo mozne otvorit. %s", ZDROJOVY_SUB_TEXTU);
    }

    return 0;
}

int pocetZobrazenychSlov(FILE *suborRead_txt, int dlzkaSlov_int) //Funkcia najde vsetky slova podla zadanej dlzky
{
    char slovo_chr[BUFFER_VELKOST_SLOVO];
    int pocitadloSlov_int=0;

    rewind(suborRead_txt); //Nastavenie ukazovatela na zaciatok suboru

    while (fscanf(suborRead_txt, "%s", slovo_chr) != EOF) //Cyklus prechadza slovami a ukonci sa EOF - (koniec súboru)
    {

        if(strlen(slovo_chr) == dlzkaSlov_int)
        {
            pocitadloSlov_int += 1;
        }
    }

    return pocitadloSlov_int;
}

int kontrolaSuboru(char *filename) //Funkcia skontroluje otovrenie suboru
{
    FILE *file;
    if (file = fopen(filename, "r")) //Zisti ci sa da citat
    {
        fclose(file); //Ak sa da citat tak ho zavrie
        return 1;
    }

    return 0;
}

int minCharDlzka(FILE *suborRead_txt) //Funkcia najde najkratsie slovo v subore
{
    int minCharDlzka, dlzkaPomocna;
    char slovo_chr[BUFFER_VELKOST_SLOVO];
    int pocitadloSlov_int=0, i=0;
    char **pString_arr;

    /* Zisti pocet slov v subore */
    rewind(suborRead_txt); //Nastavenie ukazovatela na zaciatok suboru
    while (fscanf(suborRead_txt, "%s", slovo_chr) != EOF)
    {

        if(strlen(slovo_chr)>0)
        {
            pocitadloSlov_int += 1;
        }
    }

    /* Najde najkratsie slovo v subore*/
    if (pocitadloSlov_int > BUFFER_VELKOST_SLOVO_MIN)
    {
        rewind(suborRead_txt); //Nastavenie ukazovatela na zaciatok suboru

        if((pString_arr=(char**)malloc((pocitadloSlov_int+1)*sizeof(char *))) == NULL) //Alokovanie pamate pre pole smernikov - zodpoveda poctu riadkov
        {
            printf("Nedostatok pamate. Program ukoncil.\n");
            free(pString_arr); //Ak sa pamat nepodari alokovat tak ju vyprazdni
            pString_arr=NULL;
            exit(1);
        }

        while (fscanf(suborRead_txt, "%s", slovo_chr) != EOF)
        {
            if ((pString_arr[i] = (char *) malloc((strlen(slovo_chr)+1)*sizeof(char))) == NULL) //Alokovanie pamate pre jednotlive vektory pola smerníkov - pismena zo slov
            {
                printf("Nedostatok pamate. Program sa ukoncil.\n");
                uvolneniePamate(pString_arr, i); //Ak sa pamat nepodari alokovat tak ju vyprazdni
                exit(1);
            }

            strcpy(pString_arr[i],slovo_chr); //Kopirovanie slova(retazca) do pString_arr[i]

            i+=1;
        }

        minCharDlzka = strlen(pString_arr[0]); //Nulty zaznam nastaveny ako najmensia dlzka

        for (i = 1; i < pocitadloSlov_int; i++)
        {
            dlzkaPomocna = strlen(pString_arr[i]);

            if( minCharDlzka > dlzkaPomocna)

                if (BUFFER_VELKOST_SLOVO_MIN <= dlzkaPomocna) //Podmienka ze dlzkaPomocna nesmie byt mensia ako BUFFER_VELKOST_SLOVO_MIN
                {
                    minCharDlzka = strlen(pString_arr[i]);

                }
        }

        uvolneniePamate(pString_arr, pocitadloSlov_int); //Funkcia na vyprazdnenie pamate pArray_chr
    }
    else
    {
        minCharDlzka = 0;
    }


    return minCharDlzka;
}

int maxCharDlzka(FILE *suborRead_txt) //Funkcia najde najdlhsie slovo v subore
{
    int maxCharDlzka, dlzkaPomocna;
    char slovo_chr[BUFFER_VELKOST_SLOVO];
    int pocitadloSlov_int=0, i=0;
    char **pString_arr;

    /* Zisti pocet slov v subore */
    rewind(suborRead_txt); //Nastavenie ukazovatela na zaciatok suboru
    while (fscanf(suborRead_txt, "%s", slovo_chr) != EOF)
    {

        if(strlen(slovo_chr)>0)
        {
            pocitadloSlov_int += 1;
        }
    }

    /* Najde najdlhsie slovo v subore*/
    if (pocitadloSlov_int > BUFFER_VELKOST_SLOVO_MIN)
    {
        rewind(suborRead_txt); //Nastavenie ukazovatela na zaciatok suboru



        if((pString_arr=(char**)malloc((pocitadloSlov_int+1)*sizeof(char *))) == NULL) //Alokovanie pamate pre pole smernikov - zodpoveda poctu riadkov
        {
            printf("Nedostatok pamate. Program ukoncil.\n");
            free(pString_arr);
            pString_arr=NULL;
            exit(1);
        }

        while (fscanf(suborRead_txt, "%s", slovo_chr) != EOF)
        {
            if ((pString_arr[i] = (char *) malloc((strlen(slovo_chr)+1)*sizeof(char))) == NULL) //Alokovanie pamate pre jednotlive vektory pola smerníkov - pismena zo slov
            {
                printf("Nedostatok pamate. Program sa ukoncil.\n");
                uvolneniePamate(pString_arr, i);
                exit(1);
            }

            strcpy(pString_arr[i],slovo_chr);

            i+=1;
        }

        maxCharDlzka = strlen(pString_arr[0]); //Nulty zaznam nastaveny ako najvacsia dlzka

        for (i = 1; i < pocitadloSlov_int; i++)
        {
            dlzkaPomocna = strlen(pString_arr[i]);

            if( maxCharDlzka < dlzkaPomocna)

                if (BUFFER_VELKOST_SLOVO >= dlzkaPomocna)
                {
                    maxCharDlzka = strlen(pString_arr[i]);

                }
        }

        uvolneniePamate(pString_arr, pocitadloSlov_int);
    }
    else
    {
        maxCharDlzka = 0;
    }

    return maxCharDlzka;
}

void kopirovanieDoSuboru(char *filename, char **pArray_chr, int pocetSlov_int, int velkostRetazca_int) //Funkcia na skopirovanie udajov do suboru
{
    int i = 0;

    FILE *suborWrite_txt;

    suborWrite_txt = fopen(filename, "w"); //Otorenie suboru na zapis slov

    fprintf(suborWrite_txt, "***** Skopirovane slova zo suboru: %s *****\n", ZDROJOVY_SUB_TEXTU);

    for (i; i < pocetSlov_int; i++)
        fprintf(suborWrite_txt, "%s\n", pArray_chr[i]); //Zapis slov do riadkov z pArray_chr

    fprintf(suborWrite_txt, "***** Celkovy pocet skopirovanych slov je: %d o velkosti: %d *****\n", pocetSlov_int, velkostRetazca_int);

    fclose(suborWrite_txt); //Zatvorenie suboru po zapise
}

void uvolneniePamate(char **pArray_chr, int pocetSlov_int) //Funkcia na uvolnenie papate
{
    int i;

    for(i=0; i<pocetSlov_int; i++)
    {
        free(pArray_chr[i]); //Uvolnenie pamate pre jednotlive vektory pola
    }

    free(pArray_chr); //Uvolnenie pamate pre pole smernikov na vektory
    pArray_chr=NULL;
}

char *malePismo(char *pSlovo_chr, int dlzkaSlov_int) //Funkcia zmeni velke pismena na male pismena
{
    char *pZmenaPismena;
    int i = 0;

    if ((pZmenaPismena = (char*)calloc(strlen(pSlovo_chr) + 1, sizeof(char))) == NULL)
    {
        printf("Nedostatok pamate. Program sa ukoncil.\n");
        free(pZmenaPismena);
        pZmenaPismena=NULL;
        exit(1);
    }

    do
    {
        pZmenaPismena[i] = pSlovo_chr[i];
        i++;
    }
    while(pSlovo_chr[i] != '\0');

    for(i=0; i<=dlzkaSlov_int; i++)
    {
        pZmenaPismena[i] = tolower(pZmenaPismena[i]);
    }

    return pZmenaPismena;
}
