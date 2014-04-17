/*
DevSnake 2 - wê¿yk pisany w C++ z wykorzystaniem biblioteki SDL.
Autor: Przemys³aw "Devamors" Lewandowski, devan93@gmail.com

TO DO:
- Dodanie innych bonusów, w tym poruszaj¹cych siê
- Ograniczenie bonusów czasowo
- Ustawienia sterowania
*/
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <conio.h>

using namespace std;

/* 
Funkcja wyœwietlaj¹ca przyciski "TAK" i "NIE"
Przyjmuje nastêpuj¹ce parametry
char s[] - Ci¹g znaków, który bêdzie wyœwietlany jako pytanie.
SDL_Surface * ekran - Powierzchnia g³ówna SDL
TTF_Font *font - Czcionka, której chcemy u¿yæ do wypisania pytania.
SDL_Color kolor - Kolor czcionki u¿ytej w pytaniu.
SDL_Event zdarzenie - Zmienna przechowywuj¹ca zdarzenia
*/
bool YN(char s[], SDL_Surface * ekran, TTF_Font *font, SDL_Color kolor, SDL_Event zdarzenie)
{
	SDL_Surface * tak = IMG_Load("data\\textures\\tak.png");
	SDL_Surface * nie = IMG_Load("data\\textures\\nie.png");
	SDL_Surface * pytanie = TTF_RenderText_Solid(font, s, kolor);
	SDL_Rect pytanieDestination;
	pytanieDestination.x = 220;
	pytanieDestination.y = 100;
	pytanieDestination.w = 200;
	pytanieDestination.h = 40;
	SDL_Rect TakDestination;
	TakDestination.x = 220;
	TakDestination.y = 200;
	TakDestination.w = 80;
	TakDestination.h = 40;
	SDL_Rect NieDestination;
	NieDestination.x = 340;
	NieDestination.y = 200;
	NieDestination.w = 80;
	NieDestination.h = 40;
	int myszkaX, myszkaY;
	short int nChange = 0;
	bool wynik = NULL;
	while(nChange == 0)
	{
		while(SDL_PollEvent(&zdarzenie))
		{
			if(zdarzenie.type == SDL_MOUSEMOTION)
			{
				myszkaX = zdarzenie.motion.x;
				myszkaY = zdarzenie.motion.y;
			}
			if(zdarzenie.type == SDL_MOUSEBUTTONDOWN)
			{
				if(zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= TakDestination.x && myszkaX <= (TakDestination.x + TakDestination.w) 
					&& myszkaY >= TakDestination.y && myszkaY <= (TakDestination.y + TakDestination.h)))
				{
					wynik = true;
					nChange = 1;
				}
				if(zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= NieDestination.x && myszkaX <= (NieDestination.x + NieDestination.w)
					&& myszkaY >= NieDestination.y && myszkaY <= (NieDestination.y + NieDestination.h)))
				{
					wynik = false;
					nChange = 1;
				}
			}
		}
		SDL_FillRect(ekran, NULL, SDL_MapRGBA(ekran->format, 74,74,74, 180));
		SDL_BlitSurface(pytanie, NULL, ekran, &pytanieDestination);
		SDL_BlitSurface(tak, NULL, ekran, &TakDestination);
		SDL_BlitSurface(nie, NULL, ekran, &NieDestination);
		SDL_Flip(ekran);
	}
	SDL_FreeSurface(tak);
	SDL_FreeSurface(nie);
	SDL_FreeSurface(pytanie);
	return wynik;
}

//Deklaracja i inicjalizacja zmiennych
SDL_Surface * ekran = NULL;
SDL_Surface * snake = NULL;
SDL_Surface * mapa = NULL;
SDL_Surface * bonus = NULL;
SDL_Surface * wynik1 = NULL;
SDL_Surface * wynik2 = NULL;
SDL_Surface * icon = IMG_Load("snake.ico");
//SDL_Surface * fpsy = NULL;
SDL_Surface * wynikbox = NULL;
SDL_Surface * pauzabox = NULL;
SDL_Color kolor;
SDL_Event zdarzenie;
SDL_Rect NowaGra;
SDL_Rect Opcje;
SDL_Rect Wyniki;
SDL_Rect Wyjscie;
SDL_Rect SnakeDestination;
SDL_Rect wynik1Destination;
SDL_Rect wynik2Destination;
SDL_Rect MapaDestination;
SDL_Rect BonusDestination;
//SDL_Rect FPSDestination;
SDL_Rect WynikBoxDestination;
SDL_Rect PauzaBoxDestination;
Uint8 * keystate = SDL_GetKeyState(NULL);
int bX,bY,zX,zY,width,height,nWybor,myszkaX, myszkaY, nWynik;
bool bWyjscie = false;
bool sound = true;
bool music = true;
//char sFps[8];
TTF_Font * font;
std::fstream records;
//Uint32 start = 0;
//Uint32 delta = 0;
//Uint32 fps = 0;


//Struktura przechowywuj¹ca rekordy
struct RKD
{
	int liczba;
	string nickname;
	string sWynik;
};

//Klasa w¹¿ od której tworzone s¹ kolejne obiekty - czêœci wê¿a
class waz{
private:
	int wx;					//Wspó³rzêdne czesci weza
	int wy;					
	waz * nastepnyPunkt;	//wskaŸnik do nastepnego punktu
	waz * poprzedniPunkt;	//wskaŸnik do poprzedniego punktu
public:
	//FUNKCJE
	void SetX(int i);
	void SetY(int i);
	int GetX();
	int GetY();
	void Init();			
	void CleanUp();
	void Fill();
	waz * GetNastepnyPunkt();
	waz * GetPoprzedniPunkt();
	void SetNastepnyPunkt(waz * n);
	void SetPoprzedniPunkt(waz * p);
	void AddPunkt();
};
void waz::SetX(int i)
{
	wx = i;
}
void waz::SetY(int i)
{
	wy = i;
}
int waz::GetX()
{
	return wx;
}
int waz::GetY()
{
	return wy;
}
void waz::Init()
{
	nastepnyPunkt = NULL;
}
void waz::CleanUp()
{
	if(nastepnyPunkt != NULL)
	{
		nastepnyPunkt->CleanUp();
		delete nastepnyPunkt;
		nastepnyPunkt = NULL;
	}
}
waz * waz::GetNastepnyPunkt()
{
	return(nastepnyPunkt);
}
waz * waz::GetPoprzedniPunkt()
{
	return(poprzedniPunkt);
}
void waz::SetNastepnyPunkt(waz * n)
{
	nastepnyPunkt = n;
}
void waz::SetPoprzedniPunkt(waz * p)
{
	poprzedniPunkt = p;
}
void waz::Fill()
{
		wx = zX;
		wy = zY;
}
void waz::AddPunkt()
{
	waz * LastPunkt = this;
	while(LastPunkt->nastepnyPunkt != NULL) LastPunkt = LastPunkt->nastepnyPunkt;
	waz * NewPunkt = new waz;
	NewPunkt->Init();
	NewPunkt->Fill();
	NewPunkt->poprzedniPunkt = LastPunkt;
	LastPunkt->nastepnyPunkt = NewPunkt;
}
//Funkcja wyswietlaj¹ca pytanie podane jako argument

//Funkcja wyœwietlaj¹ca textbox i przycisk OK zwracaj¹ca tablicê znaków.
string ShowTextBox()
{
	int j = 0;
	char Chars[30];
	SDL_Surface * TextBox = IMG_Load("data\\textures\\textbox.png");
	SDL_Surface * Name = NULL;
	if(TextBox == NULL)
	{
		fprintf(stderr, "Error while loading texture - textbox %s\n", IMG_GetError());
		exit(1);
	}
	SDL_Surface * ok = IMG_Load("data\\textures\\ok.png");
	SDL_Rect OKDestination;
	OKDestination.x = 278;
	OKDestination.y = 100;
	OKDestination.w = 85;
	OKDestination.h = 50;
	SDL_Rect TextBoxDestination;
	SDL_Rect NameDestination;
	NameDestination.x = 220;
	NameDestination.y = 230;
	TextBoxDestination.x = 120;
	TextBoxDestination.y = 200;
	bool TextBoxFilled = false;
	strcpy_s(Chars, "");
	while(TextBoxFilled == false )
	{
		SDL_BlitSurface(TextBox, NULL, ekran, &TextBoxDestination);
		Name = TTF_RenderText_Solid(font, Chars, kolor);
		SDL_BlitSurface(Name, NULL, ekran, &NameDestination);
		SDL_BlitSurface(ok, NULL, ekran, &OKDestination);
		SDL_Flip(ekran);
		SDL_FillRect(ekran, NULL, SDL_MapRGBA(ekran->format, 74, 74, 74, 128) );
		SDL_FreeSurface(Name);
		while(SDL_PollEvent(&zdarzenie))
		{
			if(zdarzenie.type == SDL_QUIT)
			{
				bWyjscie=true;
				break;
			}
				if(zdarzenie.type == SDL_MOUSEMOTION)
				{
					myszkaX = zdarzenie.motion.x;
					myszkaY = zdarzenie.motion.y;
				}
				if(zdarzenie.type == SDL_MOUSEBUTTONDOWN)
				{
					if(zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= 278 && myszkaX <= 358 
						&& myszkaY >= 100 && myszkaY <= 140))
					{
						TextBoxFilled = true;
					}
				}
				if(zdarzenie.type == SDL_KEYDOWN)
				{
					if(j < 10)
					{
						switch( zdarzenie.key.keysym.sym )
						{
							case SDLK_q : strcat_s(Chars, "Q"); j++; break;
							case SDLK_w : strcat_s(Chars, "W"); j++; break;
							case SDLK_e : strcat_s(Chars, "E"); j++; break;
							case SDLK_r : strcat_s(Chars, "R"); j++; break;
							case SDLK_t : strcat_s(Chars, "T"); j++; break;
							case SDLK_y : strcat_s(Chars, "Y"); j++; break;
							case SDLK_u : strcat_s(Chars, "U"); j++; break;
							case SDLK_i : strcat_s(Chars, "I"); j++; break;
							case SDLK_o : strcat_s(Chars, "O"); j++; break;
							case SDLK_p : strcat_s(Chars, "P"); j++; break;
							case SDLK_a : strcat_s(Chars, "A"); j++; break;
							case SDLK_s : strcat_s(Chars, "S"); j++; break;
							case SDLK_d : strcat_s(Chars, "D"); j++; break;
							case SDLK_f : strcat_s(Chars, "F"); j++; break;
							case SDLK_g : strcat_s(Chars, "G"); j++; break;
							case SDLK_h : strcat_s(Chars, "H"); j++; break;
							case SDLK_j : strcat_s(Chars, "J"); j++; break;
							case SDLK_k : strcat_s(Chars, "K"); j++; break;
							case SDLK_l : strcat_s(Chars, "L"); j++; break;
							case SDLK_z : strcat_s(Chars, "Z"); j++; break;
							case SDLK_x : strcat_s(Chars, "W"); j++; break;
							case SDLK_c : strcat_s(Chars, "C"); j++; break;
							case SDLK_v : strcat_s(Chars, "V"); j++; break;
							case SDLK_b : strcat_s(Chars, "B"); j++; break;
							case SDLK_n : strcat_s(Chars, "N"); j++; break;
							case SDLK_m : strcat_s(Chars, "M"); j++; break;
							case SDLK_BACKSPACE : j--; Chars[j] = NULL; break;
							case SDLK_RETURN : TextBoxFilled = true; break;
						}
					} else
					{
						switch( zdarzenie.key.keysym.sym )
						{
							case SDLK_BACKSPACE : j--; Chars[j] = NULL; break;
							case SDLK_RETURN : TextBoxFilled = true; break;
						}
					}
				} 
		} 
	}
	SDL_FreeSurface(TextBox);
	SDL_FreeSurface(ok);
	return Chars;
}
// Funkcja zapisuj¹ca rekordy do pliku.
void save(RKD tablica[5])
{
	string name;
	int temp;
	string name2;
	int temp2;
	for(int i = 0; i < 5;i++)
	{
		if(nWynik > tablica[i].liczba)
		{
			records.open("data\\records.txt", ios::out);
			if(records.is_open() == true)
			{
				name = tablica[i].nickname;
				temp = tablica[i].liczba;
				tablica[i].liczba = nWynik;
				tablica[i].nickname = ShowTextBox();
				for(int k = i+1; k < 5; k++)
				{
					name2 = tablica[k].nickname;
					temp2 = tablica[k].liczba;
					tablica[k].nickname = name;
					tablica[k].liczba = temp;
					name = name2;
					temp = temp2;
				}
				for(int j = 0; j < 5;j++)
				{
					records << tablica[j].liczba << endl;
					records << tablica[j].nickname << endl;
				}
			}
			records.close();
			records.clear();
			break;
		}
	}
}
//Funkcja odczytuj¹ca rekordy z pliku i przekazuj¹ca je do tablicy struktury RKD
void load(RKD rkd[5])
{
	string rekordziki;
	int c;
	records.open("data\\records.txt", ios::in | ios::out );
	if(records.is_open())
	{
		for(int i = 0; i < 5; i++)
		{
			getline(records, rekordziki);
			c = strtol(rekordziki.c_str(), NULL, 10);
			rkd[i].liczba= c;
			rkd[i].sWynik = rekordziki;
			getline(records, rekordziki);
			rkd[i].nickname = rekordziki;
		}
	}
	records.close();
	records.clear();
}
//====FUNKCJA G£ÓWNA====
int main(int argc, char *argv[] ) 
{
	//INICJALIZACJA SDL I WYPE£NIENIE ZMIENNYCH
	kolor.r=20;
	kolor.g=255;
	kolor.b=20;
	SDL_Init( SDL_INIT_EVERYTHING );	
	if(SDL_Init(SDL_INIT_AUDIO) < 0) exit(1);
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(22050, AUDIO_S16SYS,2,640);
	Mix_Music *mus, *mus2;
	Mix_Chunk *wav, *wav2;
	mus = Mix_LoadMUS("data\\sounds\\all-for-coffee-and-wine.mp3");
	mus2 = Mix_LoadMUS("data\\sounds\\country-road.mp3");
	wav = Mix_LoadWAV("data\\sounds\\eat.wav");
	wav2 = Mix_LoadWAV("data\\sounds\\lose.wav");
	//Ustalenie domyœlnych rozmiarów okna
	width=640;
	height=480;
	//Za³adowanie ikony
	SDL_WM_SetIcon(icon, NULL);
	//Po³o¿enie przyciskow w menu
	//Nowa gra
	NowaGra.x = 220;
	NowaGra.y = 195;
	NowaGra.w = 200;
	NowaGra.h = 50;
	//Opcje
	Opcje.x = 220;
	Opcje.y = 267;
	Opcje.w = 200;
	Opcje.h = 50;
	//Wyniki
	Wyniki.x = 220;
	Wyniki.y = 339;
	Wyniki.w = 200;
	Wyniki.h = 50;
	//Wyjscie
	Wyjscie.x = 220;
	Wyjscie.y = 411;
	Wyjscie.w = 200;
	Wyjscie.h = 50;
/*	
	Okienko wyœwietlanych fpsów
	FPSDestination.x = 530;
	FPSDestination.y = 100;
	FPSDestination.w = 100;
	FPSDestination.h = 50;
*/	
	//INICJALIZACJA SDL_TTF
	if(TTF_Init()!=0)
	{
		fprintf(stderr, "ERROR with Initialization %s\n",TTF_GetError());
		exit(1);
	}
	font=TTF_OpenFont("data\\wydzieranki.ttf", 20);
	if(!font)
	{
		fprintf(stderr, "Error with OpenFont %s\n", TTF_GetError());
		exit(1);
	}
	ekran = SDL_SetVideoMode(width,height,32,SDL_SWSURFACE);
	if(ekran == NULL)
	{
		fprintf(stderr, "ERROR with making screen %s\n", TTF_GetError());
		exit(1);
	}
	SDL_Surface * menu = NULL;
	menu = IMG_Load("data\\textures\\menu.png");
	if(!menu)
	{
		fprintf(stderr, "Error with loading menu texture %s\n", IMG_GetError());
	}
	SDL_WM_SetCaption ("DevSnake 2 0.9b", NULL);
	SDL_Surface * Snowa = IMG_Load("data\\textures\\newgame.png");
	SDL_Surface * Sopcje = IMG_Load("data\\textures\\options.png");
	SDL_Surface * Swyniki = IMG_Load("data\\textures\\scores.png");
	SDL_Surface * Swyjscie = IMG_Load("data\\textures\\exit.png");
//	start = SDL_GetTicks();
	{
		SDL_Surface * logo = NULL;
		logo = IMG_Load("data\\textures\\logo.png");
		if(logo == NULL)
		{
			fprintf(stderr, "Error with loading texture - logo %s\n", IMG_GetError());
		}
		SDL_BlitSurface(logo, NULL, ekran, NULL);
		SDL_Flip(ekran);
		SDL_Delay(1000);
		SDL_FreeSurface(logo);
	}
//====================MENU=========================
	Mix_PlayMusic(mus, 1);
	while(!bWyjscie)
	{
		nWybor = 0;
		//OBSLUGA ZDARZEÑ
		while(SDL_PollEvent( & zdarzenie))
		{
			if(zdarzenie.type == SDL_QUIT)
			{
				bWyjscie=true;
			}
			if(zdarzenie.type == SDL_MOUSEMOTION )
			{
				myszkaX = zdarzenie.motion.x;
				myszkaY = zdarzenie.motion.y;
			}
			if(zdarzenie.type == SDL_MOUSEBUTTONDOWN)
			{
				if( zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= NowaGra.x && myszkaX <= (NowaGra.x + 200) 
					&& myszkaY >= NowaGra.y && myszkaY <= (NowaGra.y + 50) ) )
				{
					nWybor = 1;
				}
				if( zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= Opcje.x && myszkaX <= (Opcje.x + 200)
					&& myszkaY >= Opcje.y && myszkaY <= (Opcje.y + 50) ) )
				{
					nWybor = 2;
					cout << nWybor;
				}
				if( zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= Wyniki.x && myszkaX <= (Wyniki.x + 200)
					&& myszkaY >= Wyniki.y && myszkaY <= (Wyniki.y + 50) ) )
				{
					nWybor = 3;
					cout << nWybor;
				}
				if( zdarzenie.button.button == SDL_BUTTON_LEFT && (myszkaX >= Wyjscie.x && myszkaX <= (Wyjscie.x + 200)
					&& myszkaY >= Wyjscie.y && myszkaY <= (Wyjscie.y + 50) ) )
				{
					nWybor = 4;
				}
			}
		}
		//RYSOWANIE MENU
	/*	delta = SDL_GetTicks() - start;
		start = SDL_GetTicks();
		if(delta != 0)
		{
			fps = 1000 / delta;
		}*/
		//sprintf(sFps, "%d", nWybor);
		//fpsy = TTF_RenderText_Solid(font, sFps, kolor);  
		SDL_BlitSurface(menu, NULL, ekran, NULL);
		SDL_BlitSurface(Snowa, NULL, ekran, &NowaGra);
		SDL_BlitSurface(Sopcje, NULL, ekran, &Opcje);
		SDL_BlitSurface(Swyniki, NULL, ekran, &Wyniki);
		SDL_BlitSurface(Swyjscie, NULL, ekran, &Wyjscie);
		//SDL_BlitSurface(fpsy, NULL, ekran, &FPSDestination);
		SDL_Flip(ekran);
		SDL_FillRect(ekran, NULL, SDL_MapRGB(ekran->format, 255,255,255) );
//===============NOWA GRA==================
		if(nWybor == 1)
		{
			Mix_HaltMusic();
			Mix_PlayMusic(mus2, 0);
			RKD tablica[5];
			char sStan[5];
			wynik1Destination.x = 530;
			wynik1Destination.y = 40;
			wynik2Destination.x = 580;
			wynik2Destination.y = 40;
			WynikBoxDestination.x = 511;
			WynikBoxDestination.y = 0;
//			FPSDestination.x = 530;
//			FPSDestination.y = 100;
//			FPSDestination.w = 100;
//			FPSDestination.h = 50;
			BonusDestination.w=29;
			BonusDestination.h=20;
			MapaDestination.x=0;
			MapaDestination.y=0;
			char sWynik[15] = "Wynik: ";
			int x,y;
			//Konfiguracja generatora liczb pseudolosowych
			bool przegrana = false;
			srand(time(NULL));
			//Ustalenie pocz¹tkowych wartoœci zmiennych
			waz * head = new waz;
			head->SetX(300);
			head->SetY(300);
			head->SetNastepnyPunkt(NULL);
			head->SetPoprzedniPunkt(NULL);
			bX = NULL;
			bY = NULL;
			x=300;
			y=300;
			int pauza = 0;
			int nDir = NULL;
			int nStan = 0;
			nWynik = 0;
			bonus = IMG_Load("data\\textures\\bonus.png");
			if(!bonus)
			{
				fprintf(stderr, "ERROR with loading png file IMG_LOAD: %s\n",IMG_GetError());
				exit(1);
			}
			mapa = SDL_LoadBMP("data\\textures\\grass.bmp");
			if(mapa == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - grass %s\n", IMG_GetError());
				exit(1);
			}
			snake = IMG_Load("data\\textures\\snake.png");
			if(snake == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - snake %s\n", IMG_GetError());
				exit(1);
			}
			wynikbox = IMG_Load("data\\textures\\wynikbox.png");
			if(wynikbox == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - wynikbox %s\n", IMG_GetError());
				exit(1);
			}
			pauzabox = IMG_Load("data\\textures\\pauza.png");
			if(pauzabox == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - pauzabox %s\n", IMG_GetError());
				exit(1);
			}
//			start = SDL_GetTicks();
			//G£ÓWNA PÊTLA GRY
			while(!przegrana)
			{
				//OBSLUGA ZDARZEÑ
				while(SDL_PollEvent( & zdarzenie))
				{
					if(zdarzenie.type == SDL_QUIT)
					{
						bWyjscie=true;
						przegrana = true;
					}
					if(zdarzenie.type == SDL_KEYDOWN )
					{
						switch( zdarzenie.key.keysym.sym)
						{
						case SDLK_RIGHT: if(nDir != 2) {nDir=1;} break;
						case SDLK_LEFT: if(nDir != 1) {nDir=2;} break;
						case SDLK_UP: if(nDir != 4) { nDir=3; } break;
						case SDLK_DOWN: if(nDir != 3) {nDir=4;} break;
							case SDLK_p:	 
								if(pauza == 0) 
								{
									pauza=1;
								} else
									pauza = 0;
								break;
							case SDLK_ESCAPE: przegrana = true; break;
						}
					}
				}
				//Wartoœæ zmiennych x i y jest zmieniana w zale¿noœci od wciœniêtego klawisza
				if(pauza == 0)
				{
					x=head->GetX();
					y=head->GetY();
					if (nDir == 1) { head->SetX(head->GetX() + 3); } else
					if (nDir == 2) { head->SetX(head->GetX() - 3); } else
					if (nDir == 3) { head->SetY(head->GetY() - 3); } else
					if (nDir == 4) { head->SetY(head->GetY() + 3); } 
				}
				//Je¿eli w¹¿ ma wiêcej punktów to nastêpuje przepisanie wartoœci na pozosta³e czêœci wê¿a,
				//by pod¹¿a³y za g³ow¹
				if(pauza == 0)
				{
					if(head->GetNastepnyPunkt() != NULL)
					{
						waz * LastPunkt = head;
						while(LastPunkt->GetNastepnyPunkt() != NULL) LastPunkt = LastPunkt->GetNastepnyPunkt();
						while(LastPunkt->GetPoprzedniPunkt() != head)
						{
							LastPunkt->SetX(LastPunkt->GetPoprzedniPunkt()->GetX());
							LastPunkt->SetY(LastPunkt->GetPoprzedniPunkt()->GetY());
							LastPunkt = LastPunkt->GetPoprzedniPunkt();
						}
						LastPunkt = head->GetNastepnyPunkt();
						LastPunkt->SetX(x);
						LastPunkt->SetY(y);
					}
				}
				//Je¿eli w¹¿ dotyka granic mapy - przegrana
				if (head->GetX() -5 <= 0 || head->GetX() +5 >= 510 || head->GetY() -5 <= 0 || head->GetY() +5 >= 475) 
				{
					if(sound == true)
					{
						Mix_PlayChannel(-1, wav2, 0);
					}
					przegrana=true;
				}
				//Je¿eli w¹¿ zbierze bonus to zmiana stanu, zapisanie pozycji g³owy
				if(head->GetX() >= bX && head->GetX() <= (bX+29) && head->GetY() >= bY && head->GetY() <= (bY+20))
				{
					nStan = 2;
					zX=head->GetX();
					zY=head->GetY();
				}
				//Je¿eli w¹¿ wpadnie sam na siebie to przegrana
				waz * ObecnyPunkt = head;
				while(ObecnyPunkt->GetNastepnyPunkt() != NULL)
				{
					ObecnyPunkt = ObecnyPunkt->GetNastepnyPunkt();
					if(head->GetX() == ObecnyPunkt->GetX() && head->GetY() == ObecnyPunkt->GetY())
					{
						if(sound == true)
						{
							Mix_PlayChannel(-1, wav2, 0);
						}
						przegrana=true;
					}
				} 
				//Bonusy
				//Losowanie pozycji bonusu w granicach mapy
				if(nStan == 0)
				{
					bX = (rand() & 470) + 15;
					bY = (rand() & 440) + 10;
					nStan = 1;
				}
				//Obs³uga sytuacji gdy bonus zostanie zebrany, utworzenie nowej czêœci wê¿a
				if(nStan >= 2)
				{
					nStan++;
					if(nStan <= 12)
					{	
						nWynik++;
						head->AddPunkt();
					}
					if(nStan == 12)
					{
						nStan=0;
						if(sound == true)
						{
							Mix_PlayChannel(-1,wav,0);
						}
					}
				}
				//Przypisanie zmiennych przed rysowaniem sceny
				sprintf_s(sStan, "%d", nWynik);
				wynik1 = TTF_RenderText_Solid(font, sWynik, kolor);
			    wynik2 = TTF_RenderText_Solid(font, sStan, kolor);
				//fpsy = TTF_RenderText_Solid(font, sFps, kolor);
				wynik1Destination.w = wynik1->w;
				wynik1Destination.h = wynik1->h;
				BonusDestination.x = bX;
				BonusDestination.y = bY;
				//Rysowanie sceny
				SDL_BlitSurface(mapa, NULL, ekran, &MapaDestination);
				SDL_BlitSurface(bonus, NULL, ekran, &BonusDestination);
				SDL_BlitSurface(wynikbox, NULL, ekran, &WynikBoxDestination);
				//Wyœwietlanie Snake'a
				waz * WyswietlPunkty = head;
				while(WyswietlPunkty != NULL)
				{
					SnakeDestination.x = WyswietlPunkty->GetX();
					SnakeDestination.y = WyswietlPunkty->GetY();
					SDL_BlitSurface(snake, NULL, ekran, &SnakeDestination);
					WyswietlPunkty = WyswietlPunkty->GetNastepnyPunkt();
				}
				//Liczenie fps
				/*delta = SDL_GetTicks() - start;
				start = SDL_GetTicks();
				if(delta != 0)
				{
					fps = 1000 / delta;
				}
				sprintf(sFps, "%d", fps);
				SDL_BlitSurface(fpsy, NULL, ekran, &FPSDestination); */
				//Wyœwietlanie wyniku
				SDL_BlitSurface(wynik1, NULL, ekran, &wynik1Destination);
				SDL_BlitSurface(wynik2, NULL, ekran, &wynik2Destination);
				if(pauza == 1)
				{
					SDL_BlitSurface(pauzabox, NULL, ekran, &PauzaBoxDestination);
				}
				SDL_Flip(ekran);
				//Czyszczenie bufora ekranu
				SDL_FillRect(ekran, NULL, SDL_MapRGB(ekran->format, 255,255,255) );
				SDL_FreeSurface(wynik1);
				SDL_FreeSurface(wynik2);
			}
			SDL_FreeSurface(snake);
			SDL_FreeSurface(mapa);
			SDL_FreeSurface(bonus);
			SDL_FreeSurface(wynikbox);
			SDL_FreeSurface(pauzabox);
			SDL_FillRect(ekran, NULL, SDL_MapRGB(ekran->format, 255,255,255) );
			head->CleanUp();
			delete head;
			//ODCZYT Z PLIKU
			load(tablica);
			save(tablica);
			Mix_HaltMusic();
			Mix_PlayMusic(mus, 0);
		}
		//OPCJE
		if(nWybor == 2)
		{
			SDL_Surface * OpcjeTlo = NULL;
			OpcjeTlo = IMG_Load("data\\textures\\opcjetlo.png");
			if(OpcjeTlo == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - opcjetlo %s\n", IMG_GetError());
				exit(1);
			}
			SDL_Surface * tick = NULL;
			tick = IMG_Load("data\\textures\\tick.png");
			if(tick == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - tick %s\n", IMG_GetError());
				exit(1);
			}
			SDL_Rect opcjeTloDestination;
			opcjeTloDestination.x = 0;
			opcjeTloDestination.y = 0;
			SDL_Rect soundTick;
			SDL_Rect musicTick;
			soundTick.x = 300;
			soundTick.y = 100;
			musicTick.x = 300;
			musicTick.y = 150;
			SDL_Rect Wroc;
			Wroc.x = 280;
			Wroc.y = 425;
			bool przegrana = false;
			while(!przegrana)
			{
				while(SDL_PollEvent( &zdarzenie ))
				{
					if(zdarzenie.type == SDL_MOUSEMOTION)
					{
						myszkaX = zdarzenie.motion.x;
						myszkaY = zdarzenie.motion.y;
					}
					if(zdarzenie.type == SDL_MOUSEBUTTONDOWN)
					{
						if(myszkaX >= Wroc.x && myszkaX <= (Wroc.x + 80) && myszkaY >= Wroc.y && myszkaY <= (Wroc.y + 44))
						{
							przegrana = true;
						}
						if(myszkaX >= soundTick.x && myszkaX <= (soundTick.x + 30) && myszkaY >= soundTick.y  && myszkaY <= (soundTick.y + 30))
						{
							sound = !sound;
						}
						if(myszkaX >= musicTick.x && myszkaX <= (musicTick.x + 30) && myszkaY >= musicTick.y && myszkaY <= (musicTick.y + 30))
						{
							if(music == true)
							{
								Mix_HaltMusic();
								music = false;
							} else { Mix_PlayMusic(mus, 1); music = true; }
						}
					}
				}
				SDL_BlitSurface(OpcjeTlo, NULL, ekran, &opcjeTloDestination);
				if(sound == true)
				{
					SDL_BlitSurface(tick, NULL, ekran, &soundTick);
				}
				if(music == true)
				{
					SDL_BlitSurface(tick, NULL, ekran, &musicTick);
				}
				SDL_Flip(ekran);
				SDL_FillRect(ekran, NULL, SDL_MapRGB(ekran->format,255,255,255));
			}
		}
		//WYNIKI
		if(nWybor == 3)
		{
			char cWyniki[30];
			bool przegrana = false;
			RKD tablica[5];
			SDL_Surface * WynikiTlo = NULL;
			SDL_Surface *wyniki[5];
			SDL_Surface *rekordzisci[5];
			SDL_Rect wynikiDestination[5];
			SDL_Rect rekordzisciDestination[5];
			SDL_Rect WynikiTloDestination;
			SDL_Rect Wroc; //Rozmiar 80 x 44
			Wroc.x = 280;
			Wroc.y = 425;
			WynikiTloDestination.x = 0;
			WynikiTloDestination.y = 0;
			WynikiTlo = IMG_Load("data\\textures\\tlowyniki.png");
			if(WynikiTlo == NULL)
			{
				fprintf(stderr, "ERROR with loading texture - grass %s\n", IMG_GetError());
				exit(1);
			}
			load(tablica);
			//sprintf(cWyniki, "%d", nWynik);
			//strcpy_s(cWyniki, tablica[0].sWynik.length()+1, tablica[0].sWynik.c_str());
			//wyniki1 = TTF_RenderText_Solid(font, cWyniki, kolor);
			for(int i = 0; i < 5; i++)
			{
				sprintf_s(cWyniki, "%d", tablica[i].liczba);
				wyniki[i] = TTF_RenderText_Solid(font, cWyniki, kolor);
				wynikiDestination[i].x = 410;
				wynikiDestination[i].y = 126 + (60 * i);
				sprintf_s(cWyniki, "%s", tablica[i].nickname.c_str());
				rekordzisci[i] = TTF_RenderText_Solid(font, cWyniki, kolor);
				rekordzisciDestination[i].x = 180;
				rekordzisciDestination[i].y = 126 + (60 * i);
			}
			while(!przegrana)
			{
				while(SDL_PollEvent(&zdarzenie))
				{
					if(zdarzenie.type == SDL_MOUSEMOTION)
					{
						myszkaX = zdarzenie.motion.x;
						myszkaY = zdarzenie.motion.y;
					}
					if(zdarzenie.type == SDL_MOUSEBUTTONDOWN)
					{
						if(myszkaX >= Wroc.x && myszkaX <= (Wroc.x + 80) && myszkaY >= Wroc.y && myszkaY <= (Wroc.y + 44))
						{
							przegrana = true;
						}
					}
				}
				SDL_BlitSurface(WynikiTlo, NULL, ekran, &WynikiTloDestination);
				for(int i = 0; i < 5; i++)
				{
					SDL_BlitSurface(wyniki[i], NULL, ekran, &wynikiDestination[i]);
					SDL_BlitSurface(rekordzisci[i], NULL, ekran, &rekordzisciDestination[i]);
				}
				SDL_Flip(ekran);
				SDL_FillRect(ekran, NULL, SDL_MapRGB(ekran->format, 255,255,255) );
			}
			SDL_FreeSurface(WynikiTlo);
		}
		//WYJSCIE
		if(nWybor == 4)
		{
			bWyjscie = YN("Na pewno chcesz wyjsc?", ekran, font, kolor, zdarzenie);			
		}
	}
	Mix_FreeChunk(wav);
	Mix_FreeChunk(wav2);
	Mix_FreeMusic(mus);
	Mix_Quit();
	SDL_Quit();
	return 0;
}