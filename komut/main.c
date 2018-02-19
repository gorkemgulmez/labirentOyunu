#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void olustur(int boyut, short labirent[][boyut])
{
	int i,j;
	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			labirent[i][j] = rand()%2;
		}
	}

}

void matrisDonusum(int boyut, short labirent[][boyut], matris *kokMatris)
{
	//uretilmis matrisi baglı liste haline getirir
	int i,j;
	matris* m = kokMatris;
	matris *yeni;
	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			yeni = (matris *) malloc(sizeof(matris));
			m->ileri = yeni;

			m->ileri->yol = labirent[i][j];
			m = m->ileri;
		}
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
	printf("Degisim: %d, Yol: %d\n", degisim, m->ileri->yol);
}

stack* yolBul(int boyut, matris *kokMatris)
{
	int x,y,x1,y1;// x,y giris x1,y1 cikis noktaları

	//kullaniciya programi anlatma
	printf("Sirayla giris ve cikis noktalarini belirleyeceksiniz.\nEger bu noktalar matrisin kenarinda olmazsa ya da girilen nokta 1 degilse koordinatlari yeniden girmeniz istenecek\nEger giridiniz sayi 0 dan kucuk ya da matris boyutundan buyuk olursa program noktayi kendisi atayacaktir\n");
	do {
		printf("Baslamak icin bir kenar secin.\n");
		printf("X[0-%d] araligi-Varsayilan: 0-: ", boyut-1);
		scanf("%d", &x);
		if(x<0 || x>=boyut)
			x = 0;

		printf("Y[0-%d] araligi-Varsayilan: 0-: ", boyut-1);
		scanf("%d", &y);
		if(y<0 || y>=boyut)
			y = 0;
	}while((x!=0 && x!=boyut-1) && (y!=0 && y!=boyut-1) || getMatris(boyut, kokMatris, y, x) != 1);
	do {
		printf("Cikis icin bir kenar secin.\n");
		printf("X[0-%d] araligi-Varsayilan: %d-: ", boyut-1, boyut-1);
		scanf("%d", &x1);
		if(x1<0 || x1>=boyut)
			x1 = boyut-1;

		printf("Y[0-%d] araligi-Varsayilan: %d-: ", boyut-1, boyut-1);
		scanf("%d", &y1);
		if(y1<0 || y1>=boyut)
			y1 = boyut-1;
	}while((x1!=0 && x1!=boyut-1) && (y1!=0 && y1!=boyut-1) || getMatris(boyut, kokMatris, y1, x1) != 1);

	printf("Girilen koordinatlar: %d,%d\n", x,y);
	printf("Cikis koordinatlari : %d,%d\n", x1,y1);

	stack* bas = (stack*) malloc(sizeof(stack));
	stack* son = (stack*) malloc(sizeof(stack));
	stack* s;

	bas->x = -1;
	son->x = x;
	son->y = y;
	son->geri = bas;

	while(x!=x1 || y!=y1) {
		//bir yol sec(yukarı asagi sol sag) (x-1 x+1 y-1 y+1)
			//yol kenarı asmamalı ve gittigi stackde 1 olmalı ve bir onceki gidilmis stack olmamali
				//gidersen baglı listenin sonuna ekle
				//gidemezsen geri don ve baglı listenin son elemanını sil
					//eger bas pointerina donuluyorsa null return et

		//eski versiyonda: && (son->geri->x != x+1 || son->geri->y != y)
		if(x+1<boyut && getMatris(boyut, kokMatris, y, x+1) == 1 && yiginKontrol(son, x+1, y))  {
			x++;
			printf("x+1: x:%dy:%d\n", x, y);
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(y+1<boyut && getMatris(boyut, kokMatris, y+1, x) == 1 && yiginKontrol(son, x, y+1)) {
			y++;
			printf("y+1: x:%dy:%d\n", x, y);
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(x-1>=0 && getMatris(boyut, kokMatris, y, x-1) == 1 && yiginKontrol(son, x-1, y)) {
			x--;
			printf("x-1: x:%dy:%d\n", x, y);
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(y-1>=0 && getMatris(boyut, kokMatris, y-1, x) == 1 && yiginKontrol(son, x, y-1)) {
			y--;
			printf("y-1: x:%dy:%d\n", x, y);
			s = (stack*) malloc(sizeof(stack));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else {
			printf("Geri donuluyor: x:%d, y:%d\n", x, y);

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
	}

	return son;
}

void yazdir(int boyut, matris *kokMatris)
{
	int i=0;
	matris* m;
	for(m = kokMatris; m->ileri != NULL; m = m->ileri) {
		printf("%d ", m->ileri->yol);
		i++;
		if(i==boyut) {
			printf("\n");
			i=0;
		}
	}
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

int main()
{
	///matris olusturma
	matris *root = (matris *) malloc(sizeof(matris));
	root->ileri = NULL;
	int boyut;
	printf("Matrisin boyutunu giriniz: ");
	scanf("%d", &boyut);
	short labirent[boyut][boyut];
	srand(time(NULL));
	olustur(boyut, labirent);
	matrisDonusum(boyut, labirent, root);
	///yazdirma
	printf("Labirent\n");
	yazdir(boyut, root);
	///yol bulma
	stack* son = yolBul(boyut, root);
	if(son == NULL) {
		printf("Cikis yok\n");
	}
	else {
		printf("Ulasilan yol\n");
		yoluCikar(boyut, root, son);
		yazdir(boyut, root);
	}
}
