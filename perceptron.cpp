/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
////////////////////////////////////////

Perceptron v1.12
Autor: Sebastian Dadej
Rok: 2012

///////////////////////////////////////
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/


#include <cstdio>
#include <cstdlib>
#include <cstring>

struct neuron_
{
	int prog;
	float *waga;
};

char neuron( struct neuron_ n, int il_dendrytow, char sygnaly[])
{
	float suma=0;
	for(int i=0; i<il_dendrytow; i++ )
	{
		suma=suma+sygnaly[i] * n.waga[i];
	}
	if( suma >= (float)n.prog) return 1; else return 0;
	
}

float powieksz(int li_prob, int li_jedynek, float waga, float wspolczynnik )
{
      return waga + (waga*(float)li_jedynek/(float)li_prob) * wspolczynnik;
}

void zeruj(int x[], int ilosc)
{
     int i;
     for(i=0; i<ilosc; i++)
     {
              x[i]=0;
     }
}
 
int main()
{
	int i, i2, j,k,l;
	FILE *plik;
	char buffor[100];
	char buffor2[100];
	int czas=80;

	//Wczytanie danych OTWARCIE PLIKU
	plik=fopen("perceptron.cfg", "r");

	//ile jest neuronów
	fgets(buffor, 100, plik);
	int il_neuronow=atol(buffor);
	printf("il_neuronow: %d \n", il_neuronow);

	struct neuron_ neuroN[il_neuronow];

	//ile jest dendrytow
	fgets(buffor, 100, plik);
	int il_dendrytow=atol(buffor);
	printf("il_dendrytow: %d\n", il_dendrytow);

	//pobieranie progów
	fgets(buffor, 100, plik);

	char *token;
	token = strtok (buffor," ,.-"); //Pozycja startowa
	for(i=0; i<il_neuronow && token != NULL; i++)
	{
  	neuroN[i].prog=atol(token);
	printf("neuroN[%d].prog: %d\n", i,neuroN[i].prog);
  	token = strtok (NULL, " ,.-"); //kolejny token
	}
	printf("\n");


	//pobieranie wag
	for(i=0; i<il_neuronow; i++)
	{
		neuroN[i].waga=new float[il_dendrytow];
		fgets(buffor, 100, plik);
		token = strtok (buffor," ,.-"); //Pozycja startowa
		for(j=0; j<il_dendrytow && token != NULL; j++)
		{
  		neuroN[i].waga[j]=(float)atol(token);
		printf("neuroN[%d].waga[%d]: %f\n", i, j, neuroN[i].waga[j]);
  		token = strtok (NULL, " ,.-"); //kolejny token
		}
		printf("\n");
	}
	fclose(plik);

	//Pobieranie sygnałów OTWARCIE PLIKU
	char tab_sygnaly[il_dendrytow][czas];
	plik=fopen("perceptron.ini", "r");
	int licz=0;;

	for(i=0; i<il_dendrytow; i++)
	{
		fgets(buffor, 100, plik);
		for(j=0; j<czas; j++) 
		{
			if( !(buffor[j]==49 || buffor[j]==48) ) 
				{printf("Blad w pliku perceptron.ini\n"); printf("Linia: %d, Kolumna: %d //%d\n", j, i, buffor[j]); return 0;} 
			tab_sygnaly[i][j]=buffor[j]-48; 
			printf("ts[%d][%d]:%d ", i, j, tab_sygnaly[i][j]); 
			if(licz%6==0) printf("\n"); 
			licz++;
		}
		printf("\n\n\n"); 
	}
	fclose(plik);

	//SYMULACZJA 
	char sygnaly[il_dendrytow];
	char wyjscie[il_neuronow][czas+2]; //Zbieranie wyników
	
	int li_prob=5;
  int li_wystapien[il_dendrytow]; //k - ilosc dendrytow 
  zeruj(li_wystapien, il_dendrytow);

	for(i=0; i<czas; i++)
	{	
		for(k=0; k<il_dendrytow; k++) 
        {
             sygnaly[k]=tab_sygnaly[k][i]; //przygotowanie sygnalów
		         if(tab_sygnaly[k][i]==1) li_wystapien[k]++;                  
        }
        
		//Wzmacnianie wag
		if(i!=0 && i%5==0) 
		{
    	for(j=0; j<il_neuronow; j++) 
      	for(k=0; k<il_dendrytow; k++) neuroN[j].waga[k]=powieksz(li_prob, li_wystapien[k], neuroN[j].waga[k], 0.01);
          
        //wyswietlanie
        for(j=0; j<il_neuronow; j++) 
        {      
        	for(k=0; k<il_dendrytow; k++) printf("N%d W%d %.3f ", j, k, neuroN[j].waga[k]); 
        	printf("\n");
        }

			zeruj(li_wystapien, il_dendrytow);
   }
      
		for(j=0; j<il_neuronow; j++) wyjscie[j][i]=48+neuron( neuroN[j], il_dendrytow, sygnaly );

		printf("Czas %d", i);
		printf("\n");

		if(il_dendrytow==5 && il_neuronow==3)
		{
		printf("[%d] --------(%3d)-----&&&                \n", sygnaly[0], neuroN[0].waga[0] );
		printf("[%d] --------(%3d)----&   &    %d         \n", sygnaly[1], neuroN[0].waga[1], neuroN[0].prog );
		printf("[%d] --------(%3d)---&  *  &===========> %d\n", sygnaly[2], neuroN[0].waga[2], wyjscie[0][i]-48 );
		printf("[%d] --------(%3d)----&   &               \n", sygnaly[3], neuroN[0].waga[3] );	
		printf("[%d] --------(%3d)-----&&&                \n", sygnaly[4], neuroN[0].waga[4] );		
		printf("\n\n");
		printf("[%d] --------(%3d)-----&&&               \n", sygnaly[0], neuroN[1].waga[0] );
		printf("[%d] --------(%3d)----&   &    %d         \n", sygnaly[1], neuroN[1].waga[1], neuroN[1].prog );
		printf("[%d] --------(%3d)---&  *  &===========> %d\n", sygnaly[2], neuroN[1].waga[2], wyjscie[1][i]-48 );
		printf("[%d] --------(%3d)----&   &               \n", sygnaly[3], neuroN[1].waga[3] );	
		printf("[%d] --------(%3d)-----&&&                \n", sygnaly[4], neuroN[1].waga[4] );
		printf("\n\n");
		printf("[%d] --------(%3d)-----&&&                \n", sygnaly[0], neuroN[2].waga[0] );
		printf("[%d] --------(%3d)----&   &    %d         \n", sygnaly[1], neuroN[2].waga[1], neuroN[2].prog );
		printf("[%d] --------(%3d)---&  *  &===========> %d\n", sygnaly[2], neuroN[2].waga[2], wyjscie[2][i]-48 );
		printf("[%d] --------(%3d)----&   &               \n", sygnaly[3], neuroN[2].waga[3] );	
		printf("[%d] --------(%3d)-----&&&                \n", sygnaly[4], neuroN[2].waga[4] );
		
		system("pause");
		printf("\n");
		
		}
		else
		{
		for(j=0; j<il_neuronow; j++)
		{
			for(l=0; l<il_dendrytow; l++)
			{				
				if(l==il_dendrytow/2-1 ) 
				{ printf("[%d] ---(%7.3f)   & &  %d       \n",sygnaly[l], neuroN[j].waga[l], neuroN[j].prog ); continue; }
				if(l==il_dendrytow/2   ) 
				{ printf("[%d] ---(%7.3f)--& * &=====> %d \n",sygnaly[l], neuroN[j].waga[l], wyjscie[j][i]-48 );  continue; }
				if(l==il_dendrytow/2+1 )
				{ printf("[%d] ---(%7.3f)   & &           \n",sygnaly[l], neuroN[j].waga[l] );	continue; }
				 else printf("[%d] ---(%7.3f)                 \n", sygnaly[l], neuroN[j].waga[l] );
			}
			printf("\n\n");
		}

		system("pause");
		printf("\n");
		}

	}

	//dodanie znaku nowej lini i konca strinku
	for(j=0; j<il_neuronow; j++)
	{
		wyjscie[j][czas]='\n'; wyjscie[j][czas+1]='\0';
	}
	
	//ZAPIS WYNIKU
	plik=fopen("perceptron.wy","w");
	for(i=0; i<il_neuronow; i++ ) fputs( wyjscie[i], plik);
	fclose(plik);
	
	for(i=0; i<il_neuronow; i++) delete[] neuroN[i].waga;
	
	system("pause");
	return 0;
}
