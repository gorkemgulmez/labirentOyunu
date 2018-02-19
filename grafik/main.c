//Gorkem Gulmez 160202076 - Huseyin Firat Albayrak 160202024
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define GENISLIK 640
#define YUKSEKLIK 400
#define ENFAZLA 20
#define BEKLE 150

struct stack {
	short x, y;
	struct stack* geri;
};

struct matris {
	short yol;
	struct matris* ileri;
};

typedef struct stack  stack;
typedef struct matris matris;

SDL_Window *window=NULL;
SDL_Renderer *renderer=NULL;

void matrisOlustur(int boyut, matris *kokMatris)
{
	int i;
	matris* m = kokMatris;
	matris *yeni;
	for(i=0;i<boyut*boyut;i++) {
			yeni = (matris *) malloc(sizeof(matris));
			m->ileri = yeni;

			m->ileri->yol = rand()%2;
			m = m->ileri;
	}
	m->ileri = NULL;
}

int yiginKontrol(stack* son, int x, int y)
{
	stack* tm;
	for(tm = son; tm->x!=-1; tm = tm->geri) {
		if(tm->x == x && tm->y == y)
			return 0;
	}

	return 1;
}

short getMatris(int boyut, matris *kokMatris, int y, int x)
{
    int i;
	int git = y * boyut + x;
	matris *m = kokMatris;
	//istenilen elemana git
	for(i=0; i<git; i++, m=m->ileri);

	return m->ileri->yol;
}

void setMatris(int boyut, matris *kokMatris, int y, int x, short degisim)
{
    int i;
	int git = y * boyut + x;
	matris *m = kokMatris;
	//istenilen elemana git
	for(i=0; i<git; i++, m=m->ileri);
	m->ileri->yol = degisim;
}

void yolCiz(int boyut, matris *kokMatris, int y, int x)
{
	int genislik = 200/(boyut/1.5);
	SDL_Rect kare = {15, 15, genislik, genislik};
	matris* m;
	int i=0,k=0;

	for(m = kokMatris; m->ileri != NULL; m=m->ileri) {
		if(m->ileri->yol == 0){
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &kare);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &kare);
		}
		if(y*boyut+x == k) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderFillRect(renderer, &kare);
		}
		i++;
		k++;
		kare.x += genislik+2;
		if(i==boyut) {
			kare.y += genislik+2;
			kare.x = 15;
			i=0;
		}
	}

	SDL_RenderPresent(renderer);

}

stack* yolBul(int boyut, matris *kokMatris, int x, int y, int x1, int y1)
{
	stack* bas = (stack*) malloc(sizeof(stack));
	stack* son = (stack*) malloc(sizeof(stack));
	stack* s;

	bas->x = -1;
	son->x = x;
	son->y = y;
	son->geri = bas;

	yolCiz(boyut, kokMatris, son->y, son->x);
	SDL_Delay(BEKLE);
	while(x!=x1 || y!=y1) {
		//bir yol sec(yukarı asagi sol sag) (x-1 x+1 y-1 y+1)
			//yol kenarı asmamalı ve gittigi stackde 1 olmalı ve bir onceki gidilmis stack olmamali
				//gidersen baglı listenin sonuna ekle
				//gidemezsen geri don ve baglı listenin son elemanını sil
					//eger bas pointerina donuluyorsa null return et

		//eski versiyonda: && (son->geri->x != x+1 || son->geri->y != y)

		if(x+1<boyut && getMatris(boyut, kokMatris, y, x+1) == 1 && yiginKontrol(son, x+1, y))  {
			x++;
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(y+1<boyut && getMatris(boyut, kokMatris, y+1, x) == 1 && yiginKontrol(son, x, y+1)) {
			y++;
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(x-1>=0 && getMatris(boyut, kokMatris, y, x-1) == 1 && yiginKontrol(son, x-1, y)) {
			x--;
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(y-1>=0 && getMatris(boyut, kokMatris, y-1, x) == 1 && yiginKontrol(son, x, y-1)) {
			y--;
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else {

			//eger basa donuyorsa cıkıs yoktur
			if(son->geri->x == -1) return NULL;


			//geri don
			setMatris(boyut, kokMatris, y, x, 0);
			x = son->geri->x;
			y = son->geri->y;

			stack *temp = son;
			son = son->geri;
			free(temp);
		}
		yolCiz(boyut, kokMatris, son->y, son->x);
		SDL_Delay(BEKLE);
	}

	return son;
}

void yoluCikar(int boyut, matris *kokMatris, stack *son)
{
	int i;
	matris *m = kokMatris;
	for(i=0; i<boyut*boyut; i++, m=m->ileri) {
		m->ileri->yol = 0;
	}

	stack* s;
	for(s=son; s->x!=-1; s=s->geri) {
		setMatris(boyut, kokMatris, s->y, s->x, 1);
	}
}

void ekranBirCiz(SDL_Rect yukari, SDL_Rect asagi, SDL_Rect gonder)
{
	int noktalar = 3;
	SDL_Point ustok[3] = { {yukari.x+3, yukari.y+yukari.h-3}, {yukari.x+yukari.w/2, yukari.y+3}, {yukari.x+yukari.w-3, yukari.y+yukari.h-3} };
	SDL_Point altok[3] = { {asagi.x+3, asagi.y+3}, {asagi.x+asagi.w/2, asagi.y+asagi.h-3}, {asagi.x+asagi.w-3, asagi.y+3} };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderDrawRect(renderer, &yukari);
	SDL_RenderDrawRect(renderer, &asagi);
	SDL_RenderDrawRect(renderer, &gonder);
	SDL_RenderDrawLines(renderer, ustok, noktalar);
	SDL_RenderDrawLines(renderer, altok, noktalar);

	SDL_RenderPresent(renderer);
}

void ekranBirYazi(SDL_Rect y_sayi1, SDL_Rect y_gonder1, int sdl_sayi1)
{
	TTF_Font* Xenotron = TTF_OpenFont("Xenotron.ttf", 10);
	SDL_Color Black = {0, 0, 0};
	char sayistr[3];

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &y_sayi1);
	sprintf(sayistr, "%d", sdl_sayi1);
	SDL_Surface* surface = TTF_RenderText_Solid(Xenotron, sayistr, Black);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &y_sayi1);
	surface = TTF_RenderText_Solid(Xenotron, "Gonder", Black);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &y_gonder1);
	SDL_Rect bilgi1 = {(GENISLIK-290)/2, 75, 290, 30};
	surface = TTF_RenderText_Solid(Xenotron, "Matris Boyutunu Seciniz", Black);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &bilgi1);

	SDL_RenderPresent(renderer);
}

void ekranIkiCiz(SDL_Rect yukari, SDL_Rect asagi, SDL_Rect yukari2, SDL_Rect asagi2, SDL_Rect gonder)
{
    int noktalar = 3;
	SDL_Point ustok[3] = { {yukari.x+3, yukari.y+yukari.h-3}, {yukari.x+(yukari.w/2), yukari.y+3}, {yukari.x+yukari.w-3, yukari.y+yukari.h-3} };
	SDL_Point altok[3] = { {asagi.x+3, asagi.y+3}, {asagi.x+asagi.w/2, asagi.y+asagi.h-3}, {asagi.x+asagi.w-3, asagi.y+3} };
	SDL_Point ustok2[3] = { {yukari2.x+3, yukari2.y+yukari2.h-3}, {yukari2.x+(yukari2.w/2), yukari2.y+3}, {yukari2.x+yukari2.w-3, yukari2.y+yukari2.h-3} };
	SDL_Point altok2[3] = { {asagi2.x+3, asagi2.y+3}, {asagi2.x+asagi2.w/2, asagi2.y+asagi2.h-3}, {asagi2.x+asagi2.w-3, asagi2.y+3} };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderDrawRect(renderer, &yukari);
	SDL_RenderDrawRect(renderer, &asagi);
	SDL_RenderDrawRect(renderer, &yukari2);
	SDL_RenderDrawRect(renderer, &asagi2);
	SDL_RenderDrawRect(renderer, &gonder);
	SDL_RenderDrawLines(renderer, ustok, noktalar);
	SDL_RenderDrawLines(renderer, altok, noktalar);
	SDL_RenderDrawLines(renderer, ustok2, noktalar);
	SDL_RenderDrawLines(renderer, altok2, noktalar);

	SDL_RenderPresent(renderer);
}

void ekranIkiYazi(SDL_Rect y_sayi2, SDL_Rect y_sayi3, SDL_Rect y_gonder2, int sdl_sayi2, int sdl_sayi3, int cikis)
{
	TTF_Font* Xenotron = TTF_OpenFont("Xenotron.ttf", 10);
	SDL_Color Black = {0, 0, 0};
	SDL_Rect giris_cikis = {460, 20, 75, 25};
	char sayistr2[3];

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &y_sayi2);
	SDL_RenderFillRect(renderer, &y_sayi3);
	SDL_RenderFillRect(renderer, &giris_cikis);

	sprintf(sayistr2, "%d", sdl_sayi2);
	SDL_Surface* surface = TTF_RenderText_Solid(Xenotron, sayistr2, Black);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &y_sayi2);
	sprintf(sayistr2, "%d", sdl_sayi3);
	surface = TTF_RenderText_Solid(Xenotron, sayistr2, Black);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &y_sayi3);

	surface = TTF_RenderText_Solid(Xenotron, "Gonder", Black);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &y_gonder2);

	if(!cikis) {
		surface = TTF_RenderText_Solid(Xenotron, "Giris", Black);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, texture, NULL, &giris_cikis);
		return ;
	}

	surface = TTF_RenderText_Solid(Xenotron, "Cikis", Black);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &giris_cikis);
}

void matrisCiz(int boyut, matris *kokMatris)
{
	int genislik = 200/(boyut/1.5);
	SDL_Rect kare = {15, 15, genislik, genislik};
	matris* m;
	int i=0;

	for(m = kokMatris; m->ileri != NULL; m=m->ileri) {
		if(m->ileri->yol == 0){
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &kare);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &kare);
		}
		i++;
		kare.x += genislik+2;
		if(i==boyut) {
			kare.y += genislik+2;
			kare.x = 15;
			i=0;
		}
	}

	SDL_RenderPresent(renderer);
}

int kenardanGir(int boyut, matris* kokMatris, int x, int y)
{
	if((x!=0 && x!=boyut-1) && (y!=0 && y!=boyut-1) || (getMatris(boyut, kokMatris, y, x) != 1))
		return 0;

	return 1;
}

void cikisYok(SDL_Rect gonder2)
{
	TTF_Font* Xenotron = TTF_OpenFont("Xenotron.ttf", 10);
	SDL_Color Black = {0, 0, 0};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &gonder2);
	gonder2.w +=20;
	gonder2.y -=10;
	SDL_Surface *surface = TTF_RenderText_Solid(Xenotron, "Cikis Yok", Black);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &gonder2);
	SDL_RenderPresent(renderer);

}

int main(int argc, char *argv[])
{
	///matris olusturma
	matris *root = (matris *) malloc(sizeof(matris));
	root->ileri = NULL;
	int boyut, sdl_sayi1=2, sdl_sayi2=0, sdl_sayi3=0, sdl_sayi4=0, sdl_sayi5=0;
	SDL_Event eventS;
	srand(time(NULL));

	//ekran olusturma
	window = SDL_CreateWindow("Labirent Oyunu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GENISLIK, YUKSEKLIK, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	TTF_Init();

	//ekran1
	SDL_Rect yukari1 = {GENISLIK/2-25, 120, 25, 25};
	SDL_Rect asagi1 = {yukari1.x, yukari1.y+70, yukari1.w, yukari1.h};
	SDL_Rect gonder1= {yukari1.x-yukari1.w, 250, 100, 50};
	SDL_Rect y_sayi1 = {yukari1.x, (yukari1.y+asagi1.y)/2, yukari1.w, yukari1.h};
	SDL_Rect y_gonder1= {gonder1.x, gonder1.y, gonder1.w, gonder1.h};
	//ekran2 ve 3
	SDL_Rect yukari2 = {450, 70, 25, 25};
	SDL_Rect asagi2 = {yukari2.x, yukari2.y+70, yukari2.w, yukari2.h};
	SDL_Rect y_sayi2 = {yukari2.x, (yukari2.y+asagi2.y)/2, yukari2.w, yukari2.h};
	SDL_Rect yukari3 = {550, yukari2.y, 25, 25};
	SDL_Rect asagi3 = {yukari3.x, yukari3.y+70, yukari3.w, yukari3.h};
	SDL_Rect y_sayi3 = {yukari3.x, (yukari3.y+asagi3.y)/2, yukari3.w, yukari3.h};

	SDL_Rect gonder2 = {465, 250, 100, 50};
	SDL_Rect y_gonder2 = {gonder2.x, gonder2.y, gonder2.w, gonder2.h};

	//bool flagleri
	int b_Running = 1, b_ekranbiraktif=1, b_ekranikiaktif=0, b_ekranucaktif=0;

	ekranBirCiz(yukari1, asagi1, gonder1);
	ekranBirYazi(y_sayi1, y_gonder1, sdl_sayi1);

	while(b_Running) {
		//eger bir yere tiklandiysa
		while(SDL_PollEvent(&eventS) != 0) {
			//carpiya basinca cik
			if(eventS.type == SDL_QUIT) {
				b_Running=0;
			}

			//Mouse un sol tusuna basıldıysa
			if(eventS.type == SDL_MOUSEBUTTONDOWN && eventS.button.button == SDL_BUTTON_LEFT) {
				//mouseun tiklandigi koordinatlar
				int x = eventS.button.x;
				int y = eventS.button.y;

				//printf("Matrisin boyutunu giriniz: ");
				if(b_ekranbiraktif && (x>yukari1.x && y>yukari1.y && x<(yukari1.x+yukari1.w) && y<(yukari1.y+yukari1.h)) && sdl_sayi1<ENFAZLA) {
					sdl_sayi1++;
					ekranBirYazi(y_sayi1, y_gonder1, sdl_sayi1);
					ekranBirCiz(yukari1, asagi1, gonder1);
				}

				else if(b_ekranbiraktif && (x>asagi1.x && y>asagi1.y && x<(asagi1.x+asagi1.w) && y<(asagi1.y+asagi1.h)) && sdl_sayi1>2) {
					sdl_sayi1--;
					ekranBirYazi(y_sayi1, y_gonder1, sdl_sayi1);
					ekranBirCiz(yukari1, asagi1, gonder1);
				}
				else if(b_ekranbiraktif && (x>gonder1.x && y>gonder1.y && x<(gonder1.x+gonder1.w) && y<(gonder1.y+gonder1.h))) {
					boyut = sdl_sayi1;
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderClear(renderer);
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi2, sdl_sayi3,0);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					b_ekranbiraktif=0;
					b_ekranikiaktif=1;

					matrisOlustur(boyut, root);
					matrisCiz(boyut, root);
				}

				//giris cikis boyutlarini giriniz
				else if(b_ekranikiaktif && (x>yukari2.x && y>yukari2.y && x<(yukari2.x+yukari2.w) && y<(yukari2.y+yukari2.h)) && sdl_sayi2<boyut-1) {
					sdl_sayi2++;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi2, sdl_sayi3,0);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi3, sdl_sayi2);
				}

				else if(b_ekranikiaktif && (x>asagi2.x && y>asagi2.y && x<(asagi2.x+asagi2.w) && y<(asagi2.y+asagi2.h)) && sdl_sayi2>0) {
					sdl_sayi2--;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi2, sdl_sayi3,0);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi3, sdl_sayi2);
				}

				else if(b_ekranikiaktif && (x>yukari3.x && y>yukari3.y && x<(yukari3.x+yukari3.w) && y<(yukari3.y+yukari3.h)) && sdl_sayi3<boyut-1) {
					sdl_sayi3++;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi2, sdl_sayi3,0);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi3, sdl_sayi2);
				}

				else if(b_ekranikiaktif && (x>asagi3.x && y>asagi3.y && x<(asagi3.x+asagi3.w) && y<(asagi3.y+asagi3.h)) && sdl_sayi3>0) {
					sdl_sayi3--;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi2, sdl_sayi3,0);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi3, sdl_sayi2);
				}

				else if(b_ekranikiaktif && (x>gonder2.x && y>gonder2.y && x<(gonder2.x+gonder2.w) && y<(gonder2.y+gonder2.h)) && kenardanGir(boyut, root, sdl_sayi2, sdl_sayi3)) {
					b_ekranikiaktif =0;
					b_ekranucaktif =1;
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderClear(renderer);
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi4, sdl_sayi5,1);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					matrisCiz(boyut, root);
				}

				else if(b_ekranucaktif && (x>yukari2.x && y>yukari2.y && x<(yukari2.x+yukari2.w) && y<(yukari2.y+yukari2.h)) && sdl_sayi4<boyut-1) {
					sdl_sayi4++;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi4, sdl_sayi5,1);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi5, sdl_sayi4);
				}

				else if(b_ekranucaktif && (x>asagi2.x && y>asagi2.y && x<(asagi2.x+asagi2.w) && y<(asagi2.y+asagi2.h)) && sdl_sayi4>0) {
					sdl_sayi4--;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi4, sdl_sayi5,1);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi5, sdl_sayi4);
				}

				else if(b_ekranucaktif && (x>yukari3.x && y>yukari3.y && x<(yukari3.x+yukari3.w) && y<(yukari3.y+yukari3.h)) && sdl_sayi5<boyut-1) {
					sdl_sayi5++;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi4, sdl_sayi5,1);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi5, sdl_sayi4);
				}

				else if(b_ekranucaktif && (x>asagi3.x && y>asagi3.y && x<(asagi3.x+asagi3.w) && y<(asagi3.y+asagi3.h)) && sdl_sayi5>0) {
					sdl_sayi5--;
					ekranIkiYazi(y_sayi2, y_sayi3, y_gonder2, sdl_sayi4, sdl_sayi5,1);
					ekranIkiCiz(yukari2, asagi2, yukari3, asagi3, gonder2);
					yolCiz(boyut, root, sdl_sayi5, sdl_sayi4);
				}

				else if(b_ekranucaktif && (x>gonder2.x && y>gonder2.y && x<(gonder2.x+gonder2.w) && y<(gonder2.y+gonder2.h)) && kenardanGir(boyut, root, sdl_sayi4, sdl_sayi5)) {
					///yol bulma
					stack* son = yolBul(boyut, root, sdl_sayi2, sdl_sayi3, sdl_sayi4, sdl_sayi5);
					if(son == NULL) {
                        b_ekranucaktif=0;
						cikisYok(gonder2);
					}
					else {
						yoluCikar(boyut, root, son);
						matrisCiz(boyut, root);
					}
				}
			}
		}
	}

	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
