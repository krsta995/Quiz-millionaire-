//Jovan Krstevski NRT-86/17
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  //koristi se za fju tolower()
#include <time.h>  //za fju time(NULL)
#include <conio.h> //za sistemsku fju koja pri svakom prikazivanju pitanja brise sve prethodno sa ekrana
#define MAX_DUZINA_PITANJA 500
#define MAX_DUZINA_ODGOVOR 50
#define BROJ_PITANJA 7
#define DUZINA_IMENA 20

int tacnih = 0;
int brPokusaja = 3;
int nijeIzabraoPolaPola=1;
int nijeIzabraoZamensko=1;

char ime[DUZINA_IMENA]; 

struct pitanje{                 //struktura pitanja koja citamo iz datoteke
    char pitanje[MAX_DUZINA_PITANJA];  // pitanje
    char odg_a[MAX_DUZINA_ODGOVOR]; //odgovor a
    char odg_b[MAX_DUZINA_ODGOVOR]; //odg b
    char odg_c[MAX_DUZINA_ODGOVOR]; //odg c
    char odg_d[MAX_DUZINA_ODGOVOR]; //odg d
    char tacno;  //tacan odg
};
struct pitanje zamensko = {  //zamensko pitanje
    "Koliko je dugacak maraton, a koliko polumaraton_",
    "42km 22km",
    "52km 26km",
    "42km 21km",
    "41km 20.5km",
    'a'
};

struct pitanje* spisak_pitanja;    //niz struktura pitanja

void procitaj_pitanja();  //fja koja cita pitanja iz datoteke
void pitaj(struct pitanje p); //fja koja ispisuje i obradjuje unos pitanja
char* odgovor_od_slova(char slovo, struct pitanje* p); //pomocna fja koja za zadato slovo vraca string tog odgovora
char nasumicnoSlovo(char izuzetak); //fja koja vraca nasumicno slovo

int main() {  //glavna funkcija
    int i;

    //najosnovnije informacije o kvizu
    printf("\nDobrodosli u igru milioner!\nPred vama je 7 pitanja iz razlicitih oblasti! Imate 3 mogucnosti na gresku, i 2 pomoci!");
    printf("\nNa raspolaganju su vam pomoc pola pola i zamensko pitanje!\n");
    printf("\nDa bi ste iskoristili pola-pola pritisnite p, za zamensko pitanje z");
    printf("\nSvaku pomoc mozete iskoristiti samo jedanput!\n\n\n");


    puts("Unesite vase ime: "); 
    gets(ime);

    ime[0]=toupper(ime[0]);  //pri ispisu imena vraca prvo slovo kao veliko

    procitaj_pitanja();  //cita pitanja
    for(i=0; i<BROJ_PITANJA; i++) { //for petlja ide od 0 do broja pitanja
        pitaj(spisak_pitanja[i]); //ispisuje pitanja
    }

    printf("\nCestitamo %s! Uspeli ste da prodjete kviz!\n\nBroj tacnih odgovora: %d", ime, tacnih); //ispis broja tacnih odgovora ukoliko je korisnik zavrsio test bez  greska

    return 0;
}

void procitaj_pitanja(){  //fja koja otvara i cita datoteku sa pitanjima
    FILE *fptr;
    int i;

    if((fptr = fopen("pitanja.txt", "r"))==NULL) {
        printf("\nGreska prilikom otvaranja datoteke za citanje?");  
        exit(1);
    }

    if((spisak_pitanja = (struct pitanje*)malloc(BROJ_PITANJA*sizeof(struct pitanje)))==NULL) {
        printf("\nGreska prilikom dinamicke dodele niza!");
        exit(1);
    }

    for(i=0; i<BROJ_PITANJA; i++){ //upis u memoriju pitanja koja su procitana iz datoteke
        fgets(spisak_pitanja[i].pitanje, MAX_DUZINA_PITANJA, fptr); 
        fgets(spisak_pitanja[i].odg_a, MAX_DUZINA_ODGOVOR, fptr);
        fgets(spisak_pitanja[i].odg_b, MAX_DUZINA_ODGOVOR, fptr);
        fgets(spisak_pitanja[i].odg_c, MAX_DUZINA_ODGOVOR, fptr);
        fgets(spisak_pitanja[i].odg_d, MAX_DUZINA_ODGOVOR, fptr);
        spisak_pitanja[i].tacno  = fgetc(fptr);
        fgetc(fptr);
        fgetc(fptr);
    }
 }

void pitaj(struct pitanje p){
    system("cls");  //fja cisti ekran nakon svakog ucitanog pitanja radi preglednosti
    char odgovor;
    printf("\nPitanje: %s", p.pitanje);
    printf("\nA: %s", p.odg_a);
    printf("\nB: %s", p.odg_b);
    printf("\nC: %s", p.odg_c);
    printf("\nD: %s", p.odg_d);
    do{
        printf("\n\nUnesite odgovor: ");
        fflush(stdin);
        odgovor = getchar();
        odgovor = tolower(odgovor); //svaki unos korisnka pretvara u malo slovo

        if(odgovor == 'p' && nijeIzabraoPolaPola){
             break;
        } else if(odgovor=='p') {
            puts("Nemate vise pravo da iskoristite pomoc pola-pola! Probajte drugu opciju!");
            continue;
        }

        if(odgovor == 'z' && nijeIzabraoZamensko){ 
            break;
        } else if(odgovor == 'z'){
            puts("Nemate vise pravo da iskoristite pomoc zamenskog pitanja! Probajte drugu opciju!");
            continue;
        }
        if(odgovor=='a' || odgovor =='b' || odgovor =='c' ||odgovor=='d'){break;}else{
            puts("Uneli ste neodgovarajucu opciju");
        }

    }while(1);

    if(odgovor == 'p' && nijeIzabraoPolaPola) {  //pola-pola
        char nasumicnoSl  = nasumicnoSlovo(p.tacno);
        puts("Izabrali ste pomoc pola-pola!");
        printf("\n%c: %s ", toupper(p.tacno), odgovor_od_slova(p.tacno, &p));
        printf("\n%c: %s ", toupper(nasumicnoSl), odgovor_od_slova(nasumicnoSl, &p));
        fflush(stdin);
        odgovor = getchar();
        nijeIzabraoPolaPola = 0;
    }else if(odgovor == 'z' && nijeIzabraoZamensko){
        p=zamensko;
        puts("Izabrali ste pomoc zamenkso pitnje!");
        printf("\nPitanje: %s", p.pitanje);
        printf("\nA: %s", p.odg_a);
        printf("\nB: %s", p.odg_b);
        printf("\nC: %s", p.odg_c);
        printf("\nD: %s", p.odg_d);
        printf("\n");
        nijeIzabraoZamensko=0;
    
        do{
            odgovor = getchar();
            odgovor = tolower(odgovor);
        }while(odgovor!='a'&&odgovor!='b'&&odgovor!='c'&&odgovor!='d');

    }

    if(odgovor==p.tacno) {
        printf("\nTacan odgovor!\n");
        tacnih++;
    }
    else{
        printf("\nOdgvor je netacan!\n");
        brPokusaja--;
    }

    if(brPokusaja==0) {
        printf("\nIskoristili ste sve mogucnosti na gresku! ");
        printf("\nBroj tacnih odgovora: %d", tacnih);
        printf("\n\nKRAJ IGRE!!!\n\n");
        
        exit(1);       
    }

    puts("Unesite enter da nastavite dalje!");
    fflush(stdin);
    getchar();
}

char* odgovor_od_slova(char slovo, struct pitanje* p){
    switch(slovo) {
        case 'a':
            return p->odg_a;
        case 'b':
            return p->odg_b;
        case 'c':
            return p->odg_c;
        case 'd':
            return p->odg_d;
    }
}

char nasumicnoSlovo(char izuzetak){
    int r;
    char slovo;

    srand(time(NULL));

    do{
        r = rand()%4;
        slovo = 97+r;
    }while(slovo == izuzetak);
    
    return slovo;
}
