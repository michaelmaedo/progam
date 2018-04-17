/***************************************************************************************************************************************
*                         Programa desenvolvido para a análide de estruturas de pórticos planos                                        *
*                                                                                                                                      *
* Obs1: A linguagem C++ armazena dados em vetores a partir de V[0] e M[0][0] para matrizes. Logo, a barra 1 será a barra i+1 quando i=0.
* Obs2: Nas função ler_dados1 e ler_dados2 foi utilizada passagem de parâmetros por referência, pois desse modo alteraram-se os seus    * valores armazenados na memória e não simplesmente na função em questão.
**************************************************************************************************************************************/

//Inclusão dos cabeçalhos
//Cabeçalhos padrões
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <fstream>
#include <unistd.h>

//Cabeçalhos arquivos .header
#include "alocar.cpp"
#include "matriz.h"

//Declaração de constantes
#define max 150
#define n 6

using namespace std;

//typedef para tornar o programa mais portável, sendo que barra será um sinônimo para o tipo elemento
typedef struct elemento barra;

//Declaração de registros para as barras
struct elemento{
	int noinicio, nofim;
	float E, Area, Iz, comp, co, se, h, alpha;
};

//Protótipo das funções
void ler_dados1( ifstream&, ofstream&, int *,int *,int *,int *,int *, int *, int * );
void ler_dados2( ifstream&, ofstream& , float *, float *, float *, float *, barra *, int, int );
void ler_dados3(ifstream &, ofstream &, float*, float *, float *, float *, float *, float *, int *,
 int , int , int , int , int, int );
float **calcula_KL( float**, float *, float *, float *, float * );
float **rotacao( float **, float *, float *);
float **calcula_Rig( ifstream&, ofstream&, int, int, int, barra *, float ** );
float  *calcula_Alep( float , float , float , float, float, float, float, float, float, float, float );
float *monta_ANE( int, float *, float *, float *, float *, barra *, float *, float*, float* );
void vinculacao( int *,float *, float *, float **, int, int );
void resultados(ofstream &, ofstream &, barra *,  float *, float **, int , int, float *, 
float *, float *, float *, float *, float * );

extern "C"{
	void linresolve_(int *, float *, float *);
}

int main()
{
	//Limpa a tela para iniciar o programa
	system( "clear" );

	//Imprime na tela
	cout << "********************************************************************************\n";
	cout << "*                           Programa de Portico Plano                          *\n";
	cout << "********************************************************************************" << endl;
	cout << '\n'
		 << "Desenvolvido por: Michael A. Maedo          RA: 810495\n"
		 << "                  Herman Piccinin Pagotto   RA: 810312\n"
		 << "                  Ricardo V. Sartori        RA: 810355" << endl;

	/*Declaração de variáveis: Dados relativos à estruturas, como o n⁰ de nós, n⁰ de barras, n⁰ de nos com carga, 
    n⁰ de barras com carga, n⁰ de nos com vinculação, conectividade das barras, coordenadas dos nos e propriedades das barras*/
	int Nnos, TresNnos, Nbarras, Nnocar, Nbarcar, Nnovinc, Nnoapel, Nnorec, *noinicio, *nofim, *vinc;
	float *x, *y, **Rig, *ANE, *px1, *px2, *py1, *py2, *Rig2, *te, *ti, *Rec;
	char arq_entrada[max], arq_saida[max];
	barra *elemento;

	cout << "\nEntre com o nome do arquivo de entrada de dados: ";
	cin.getline( arq_entrada, max );

	//Construtor de ifstream abre o arquivo de entrada
	ifstream entrada( arq_entrada, ios::in );
	if( !entrada ){
		cerr << "Arquivo não pode ser aberto" << endl;
		exit (1);
	}

	cout << "Entre com o nome do arquivo de saída de dados: ";	
	cin.getline( arq_saida, max );

	//Construtor de ofstream abre o arquivo de saída de dados, criando um arquivo de saída caso não exista
	ofstream saida( arq_saida, ios::out );
	if( !saida ){
		cerr << "Arquivo de saída não pode ser aberto" << endl;
		exit (1);
	}

	//Função para ler os dados inteiros da estrutura
	ler_dados1( entrada, saida, &Nnos, &Nbarras, &Nnocar, &Nbarcar, &Nnovinc, &Nnoapel, &Nnorec );

	//Verificação da dimensão
	if( Nnos <= 0 ){
		cout << "Dimensão inválida" << endl
		<< "Tecle enter para continuar..." << endl;
		cin.get();
		return (NULL);
	}

	ofstream saida_2( "plot", ios::out );
	if( !saida ){
		cerr << "Arquivo de saída não pode ser aberto" << endl;
		exit (1);
	}
	TresNnos = 3*Nnos;

	//Alocação dinâmicamente de memória
	x = alocar_vet( x, Nnos );
	y = alocar_vet( y, Nnos );
	Rig = alocar_mat( Rig, 3*Nnos, 3*Nnos );
	elemento = alocar_vet( elemento, Nbarras );
	ANE = alocar_vet( ANE, TresNnos );
	px1 = alocar_vet( px1, Nbarras );
	px2 = alocar_vet( px2, Nbarras );
	py1 = alocar_vet( py1, Nbarras );
	py2 = alocar_vet( py2, Nbarras );
	vinc = alocar_vet( vinc, TresNnos );
	Rig2 = alocar_vet( Rig2, TresNnos*TresNnos);
	te = alocar_vet( te, Nbarras );
	ti = alocar_vet( ti, Nbarras );
	Rec = alocar_vet( Rec, TresNnos );

	//Função para ler os dados reais da estrutura
	ler_dados2( entrada, saida, x, y, te, ti , elemento, Nnos, Nbarras );

	//Função para calcular a matriz de rigidez não restringida da estrutura
	Rig = calcula_Rig( entrada, saida, Nnos,Nnoapel, Nbarras, elemento, Rig );

	ler_dados3( entrada, saida, ANE, px1, px2, py1, py2, Rec, vinc, TresNnos, Nbarras, Nnocar, Nbarcar, Nnovinc, Nnorec );
	//Função para o vetor de forças nodais
	ANE = monta_ANE( Nbarras, px1, px2, py1, py2, elemento, ANE, te, ti );

	//Função para vincular os nós necessários
	vinculacao( vinc, ANE, Rec, Rig, TresNnos, Nnorec );

	//Função para resolver o sistema linear
	Rig = trans_matf(Rig, TresNnos, TresNnos);
	for( int i = 0; i < TresNnos; i ++ ){
		for( int j = 0; j < TresNnos; j++){
			Rig2[ TresNnos*i + j ] = Rig[i][j];
		}
	}
	linresolve_( &TresNnos, Rig2, ANE );

	//Função para a saída de dados
	resultados( saida, saida_2, elemento, ANE, Rig, Nnos, Nbarras, px1, px2, py1, py2, te, ti );
	cout << "\n\t*** Operação realizada com sucesso ***\n\nPressione enter para finalizar..." << endl;
	cin.get();

	libera_vet( Nnos, x );
	libera_vet( Nnos, y );
	delete []elemento;
	libera_mat( TresNnos, TresNnos, Rig );

	execlp("gnuplot", "gnuplot", "./plot", NULL);
	//Destruidor de ifstream  e ofstream fecha o arquivo 
        return 0;
}

void ler_dados1( ifstream &entrada, ofstream &saida, int *Nnos, int *Nbarras, int *Nnocar, 
int *Nbarcar, int *Nnovinc, int *Nnoapel, int *Nnorec )
{
	//Declaração de variáveis locais
	const int espaco1 = 60, espaco2 = 60;
	char  titulo1[max];

	//Da primeira linha até a décima quinta linha, copia-se as linhas do arquivo de entrada para o arquivo de saída
	for( int i = 1; i <= 15; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	entrada.getline(titulo1, max);
	saida << setiosflags(ios::left) << titulo1 << endl;

	entrada.get(titulo1, espaco1);
	entrada >> *Nnos;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nnos << endl;

	entrada.getline(titulo1, NULL);
	entrada.get(titulo1, espaco1);
	entrada >> *Nbarras;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nbarras << endl;

	entrada.getline(titulo1, NULL);
	entrada.get(titulo1, espaco1);
	entrada >> *Nnocar;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nnocar << endl;

	entrada.getline(titulo1, NULL);
	entrada.get(titulo1, espaco1);
	entrada >> *Nbarcar;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nbarcar << endl;

	entrada.getline(titulo1, NULL);
	entrada.get(titulo1, espaco1);
	entrada >> *Nnovinc;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nnovinc << endl;
	
	entrada.getline(titulo1, NULL);
	entrada.get(titulo1, espaco1);
	entrada >> *Nnoapel;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nnoapel << endl;
	
	entrada.getline(titulo1, NULL);
	entrada.get(titulo1, espaco1);
	entrada >> *Nnorec;
	saida << setiosflags(ios::left) << setw(espaco2) << titulo1 << *Nnorec << endl;
}

void ler_dados2(ifstream &entrada, ofstream &saida, float *x, float *y, float *te, float *ti, 
barra *elemento, int Nnos, int Nbarras )
{
	int aux;
	float dx, dy;
	char titulo1[max];

	entrada.getline(titulo1, NULL);
	for(int i = 1; i <= 8; i++){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for(int i = 0; i < Nnos; i++){
		entrada >> aux >> x[i] >> y[i];
		saida << setiosflags(ios::right) << setw(15) << aux << setw(15) << x[i] << setw(15) << y[i] << endl;
	}

	entrada.getline(titulo1, NULL);
	for(int i = 1; i <= 7; i++){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for(int i = 0; i < Nbarras; i++){

		//Le no arquivo de entrada os dados da barra i+1
		entrada >> aux >> elemento[i].noinicio >> elemento[i].nofim >> elemento[i].E >> elemento[i].Area >> elemento[i].Iz;
		int j = elemento[i].noinicio, k = elemento[i].nofim;
		dx = x[k-1] - x[j-1];
		dy = y[k-1] - y[j-1];

		//Determinação do comprimento, do seno e cosseno da barra i+1;
		elemento[i].comp = sqrt(pow(dx,2) + pow(dy,2));
		elemento[i].co = dx/elemento[i].comp;
		elemento[i].se = dy/elemento[i].comp;

		//Imprime no arquivo de saida os dados relativos a barra i+1
		saida << setiosflags(ios::right) << setw(18) << aux << setw(15) << elemento[i].noinicio << setw(15) 
                << elemento[i].nofim << setw(15) << elemento[i].E << setw(15) << elemento[i].Area << setw(15)
                << elemento[i].Iz << endl;

	}

	entrada.getline( titulo1, NULL );	
	for(int i = 1; i <= 7; i++){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for( int i = 0; i < Nbarras; i++ ){
		entrada >> aux >> elemento[i].h >> elemento[i].alpha >> te[ i ] >> ti[ i ];
		saida << setiosflags(ios::right) << setw(18) << aux << setw(15) << elemento[i].h << setw(15) 
                << elemento[i].alpha << setw(15) << te[ i ] << setw(15) << ti[i] <<  endl;
		elemento[ i ].h = elemento[ i ].h/100;
	}
}

void ler_dados3(ifstream &entrada, ofstream &saida, float *ANE, float *px1, float *px2, float *py1, float *py2, float *Rec, 
   int *vinc, int TresNnos, int Nbarras, int Nnocar, int Nbarcar, int Nnovinc, int Nnorec )
{
	int aux;
	float dx, dy;
	char titulo1[max];

	for( int i = 1; i <= 8; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}
	for( int i = 0; i < TresNnos; i++ ){
		ANE[ i ] = 0;
		vinc[ i ] = 0;
		Rec[ i ] = 0;
	}

	for( int i = 0; i < Nnocar; i++ ){
		entrada >> aux;
		entrada >> ANE[ 3*aux - 3 ] >> ANE[ 3*aux - 2 ] >> ANE[ 3*aux - 1 ];
		saida << setiosflags(ios::left) << setw(15) << aux << setw(20) << ANE[ 3*aux - 3 ] << setw(20) 
                <<  ANE[ 3*aux - 2 ] << setw(20) <<  ANE[ 3*aux - 1 ] << endl;
	}

	for( int i = 1; i <= 8; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}
	for( int i = 0; i < Nbarras; i++ )
		px1[ i ] = px2[ i ] = py1[ i ] = py2[ i ] = 0;

	for( int i = 0; i < Nbarcar; i++ ){
		entrada >> aux;
		entrada >> px1[ aux - 1 ] >> px2[ aux - 1 ] >> py1[ aux - 1 ] >> py2[ aux - 1 ];
		saida << setiosflags(ios::right) << setw(18) << aux << setw(18) << px1[ aux - 1 ] << setw(18) 
                <<  px2[ aux - 1 ] << setw(18) <<  py1[ aux - 1 ] << setw(18) << py2[ aux - 1] << endl;
	}

	//Lê no arquivo de entrada os dados relativos as vinculações e imprime no arquivo de saída
	for( int i = 1; i <= 8; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for( int i = 0; i < Nnovinc; i++ ){
		entrada >> aux;
		entrada >> vinc[ 3*aux - 3 ] >> vinc[ 3*aux - 2 ] >> vinc[ 3*aux - 1];
		saida << setiosflags(ios:: right) << setw(15) << aux << setw(15) << vinc[ 3*aux - 3 ] << setw(15) << vinc[ 3*aux - 2 ] 
                << setw(15) << vinc[ 3*aux - 1 ] << endl;
	}

	for( int i = 1; i <= 8; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for( int i = 0; i < Nnorec; i++ ){
		entrada >> aux;
		entrada >> Rec[ 3*aux - 3 ] >> Rec[ 3*aux - 2 ] >> Rec[ 3*aux - 1];
		saida << setiosflags(ios:: right) << setw(15) << aux << setw(15) << Rec[ 3*aux - 3 ] << setw(15) << Rec[ 3*aux - 2 ] 
                << setw(15) << Rec[ 3*aux - 1 ] << endl;
	}
}

float **calcula_KL(float **KL, float L, float E, float A, float I)
{
	for(int j = 0; j < n; j++)
		for(int k = 0; k < n; k++) KL[j][k] = 0;

	KL[0][0] = KL[3][3] = E*A/L;
	KL[3][0] = KL[0][3] = -KL[0][0];
	KL[1][1] = KL[4][4] = 12*E*I/pow(L,3);
	KL[1][4] = KL[4][1] = -KL[1][1];
	KL[1][2] = KL[2][1] = KL[1][5] = KL[5][1] = 6*E*I/pow(L,2);
	KL[2][4] = KL[4][2] = KL[4][5] = KL[5][4] = -KL[1][2];
	KL[2][5] = KL[5][2] = 2*E*I/L;
	KL[2][2] = KL[5][5] = 4*E*I/L;

	return (KL);
}

float **rotacao(float **rot, float co, float se)
{
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)	rot[i][j] = 0;

	rot[0][0] = rot[1][1] = rot[3][3] = rot[4][4] = co;
	rot[0][1] = rot[3][4]= se;
	rot[1][0] = rot[4][3] = -se;
	rot[2][2] = rot[5][5] = 1;

	return(rot);
}

float **calcula_Rig( ifstream &entrada, ofstream &saida, int Nnos, int Nnoapel, int Nbarras, barra *elemento, float **Rig )
{
	//Variáveis locais
	char titulo1[max];
	int aux;
	float **KL, **rot, **KG, kx, ky, kz;

	//Alocação de memória para as matrizes KL, rot e KG
	KL = alocar_mat( KL, n, n );
	rot = alocar_mat( rot, n, n );
	KG = alocar_mat( KG, n, n );

	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) Rig[i][j] = 0;

	for(int i = 0; i < Nbarras; i++){
		//Calcula a matriz de rigidez local da barra i+1
		KL = calcula_KL( KL, elemento[i].comp, elemento[i].E, elemento[i].Area, elemento[i].Iz);

		//Calcula a matriz de rotação da barra i+1
		rot = rotacao(rot, elemento[i].co, elemento[i].se);

		//Determinação da matriz global da barra i+1
		KG = prod_matf(trans_matf(rot, n, n), n, n, KL, n, n);
		KG = prod_matf(KG, n, n, rot, n, n);

		//Insere os elementos da matriz de rigidez global da barras i+1 na matriz de rigidez da estrutura
		int j = elemento[i].noinicio;
		int k = elemento[i].nofim;
		for(int a = 0; a < 6; a++){
			if(a < 3){
				for(int b = 0; b < 6; b++){
					if(b < 3)
						Rig[3*(j - 1) + a][3*(j - 1) + b] = Rig[3*(j - 1) + a][3*(j - 1) + b] + KG[a][b];
					else
						Rig[3*(j - 1) + a][3*(k - 2) + b] = Rig[3*(j - 1) + a][3*(k - 2) + b] + KG[a][b];
				}
			}
			else{
				for(int b = 0; b < 6; b++){
					if(b < 3)
						Rig[3*(k - 2) + a][3*(j - 1) + b] = Rig[3*(k - 2) + a][3*(j - 1) + b] + KG[a][b];
					else
						Rig[3*(k - 2) + a][3*(k - 2) + b] = Rig[3*(k - 2) + a][3*(k - 2) + b] + KG[a][b];
				}
			}
		}
	}

	for(int i = 1; i <= 8; i++){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for( int i = 0; i < Nnoapel; i++ ){
		entrada >> aux >> kx >> ky >> kz;
		Rig[ 3*aux - 3 ][ 3*aux - 3 ] = Rig[ 3*aux - 3 ][3*aux - 3 ] + kx;
		Rig[ 3*aux - 2 ][ 3*aux - 2 ] = Rig[ 3*aux - 2 ][3*aux - 2 ] + ky;
		Rig[ 3*aux - 1 ][ 3*aux - 1 ] = Rig[ 3*aux - 1 ][3*aux - 1 ] + kz;
		saida << setiosflags(ios::right) << setw(15) << aux << setw(15) << kx << setw(15) 
                << ky << setw(15) <<  kz << endl;
	}

	libera_mat( n, n, KL );
	libera_mat( n, n, rot );
	libera_mat( n ,n, KG );
	return Rig;
}

float **calcula_Alep( float px1, float px2, float py1, float py2, float l, float alpha, 
float E, float A, float I, float te, float ti, float h )
{
	float **Alep = alocar_mat( Alep, n, 1);
	Alep[ 0 ][ 0 ] = -( px2 + 2*px1 )*l/6 + ( alpha*( ( te + ti )/2 )*E*A );
	Alep[ 1 ][ 0 ] = -( 3*py2 + 7*py1 )*l/20;
	Alep[ 2 ][ 0 ] = -( 2*py2 + 3*py1 )*pow( l , 2 )/60 + ( alpha*( ti - te )*E*I/h );
	Alep[ 3 ][ 0 ] = -( 2*px2 + px1 )*l/6 - ( alpha*( ( te + ti )/2 )*E*A );
	Alep[ 4 ][ 0 ]= -( 7*py2 + 3*py1 )*l/20;
	Alep[ 5 ][ 0 ] = ( 3*py2 + 2*py1 )*pow( l , 2 )/60 - ( alpha*( ti - te )*E*I/h );
	return Alep;
}

float *monta_ANE( int Nbarras, float *px1, float *px2, float *py1, float *py2, barra *elemento, 
float *ANE, float *te, float *ti )
{
	int j, k;
	float **rot = alocar_mat( rot, n , n ), **Agep = alocar_mat( Agep, n, 1 ), **Alep = alocar_mat( Alep, n , 1 );

	for(int i = 0; i < Nbarras; i++ ){
		Alep = calcula_Alep( px1[ i ], px2[ i ], py1[ i ], py2[ i ], elemento[ i ].comp, 
elemento[ i ].alpha, elemento[i].E, elemento[i].Area, elemento[i].Iz, te[ i ], ti[ i ], elemento[ i ].h );
		rot = rotacao( rot, elemento[ i ].co, elemento[ i ].se );
		Agep = prod_matf( trans_matf( rot, n, n ), n, n, Alep, n, 1);
		j = elemento[ i ].noinicio;
		k = elemento[ i ].nofim;
		ANE[ 3*j - 3 ] = ANE[ 3*j - 3 ] - Agep[ 0 ][ 0 ];
		ANE[ 3*j - 2 ] = ANE[ 3*j - 2 ] - Agep[ 1 ][ 0 ];
		ANE[ 3*j - 1 ] = ANE[ 3*j - 1 ] - Agep[ 2 ][ 0 ];
		ANE[ 3*k - 3 ] = ANE[ 3*k - 3 ] - Agep[ 3 ][ 0 ];
		ANE[ 3*k - 2 ] = ANE[ 3*k - 2 ] - Agep[ 4 ][ 0 ];
		ANE[ 3*k - 1 ] = ANE[ 3*k - 1 ] - Agep[ 5 ][ 0 ];
	}
	return ANE;
}

void vinculacao(int *vinc, float *ANE, float *Rec, float **Rig, int TresNnos, int Nnorec )
{
	for( int i = 0; i < TresNnos; i++ ){
		if( vinc[ i ] == 1 ){
			for( int j = 0; j < TresNnos; j++ ){
                                ANE[ j ] = ANE[ j ] - Rig[ j ][ i ]* Rec[ i ];
				Rig[ i ][ j ] = Rig[ j ][ i ] = 0;
			}
                        Rig[ i ][ i ] = 1;
			ANE[ i ] = Rec[ i ];
		}
	}
}

void resultados( ofstream &saida, ofstream &saida_2, barra *elemento,  float *ANE, float **Rig, int Nnos, int Nbarras, float *px1, float *px2,
    float *py1, float *py2, float *te, float *ti )
{
	int aux = 0;
	float **uG, **uL, **Af, **KL, **rot, **Alep, a, b, c, x, x1, x2, M;
	uG = alocar_mat( uG, n, n );
	uL = alocar_mat( uL, n, n );
	Af = alocar_mat( Af, n, n );
	KL = alocar_mat( KL, n, n );
	rot = alocar_mat( rot, n, n );
	Alep = alocar_mat( Alep, n, n );

	saida << "\n\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
    saida << "\tDESLOCAMENTOS NODAIS\n\n\tnum. no      desloc. x      desloc. y      rotacao z" ;
    saida << "\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n";
	for(int i = 0; i < Nnos; i++){
            aux = i + 1;
			saida << setiosflags(ios::right) << setw(15) << aux << setiosflags(ios::scientific) << setprecision(4) 
            << setw(15) << setiosflags(ios::internal) << ANE[ 3*aux - 3 ] << setw(15) << ANE[ 3*aux - 2 ] << setw(15) 
           << ANE[ 3*aux - 1 ]<<  endl;
	}

	saida << "\n\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
    saida << "\tESFORÇOS FINAIS\n\n\tnum. barra      num.no       F. normal(kN)     F. cortante(kN)     M. fletor(kN*m)" ;
    saida << "\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";

	for( int i = 0; i < Nbarras; i++ )
	{
		int j = elemento[i].noinicio;
		int k = elemento[i].nofim;

		uG[ 0 ][ 0 ] = ANE[ 3*j - 3 ];
        uG[ 1 ][ 0 ] = ANE[ 3*j - 2 ];
		uG[ 2 ][ 0 ] = ANE[ 3*j - 1 ];
		uG[ 3 ][ 0 ] = ANE[ 3*k - 3 ];
        uG[ 4 ][ 0 ] = ANE[ 3*k - 2 ];
		uG[ 5 ][ 0 ] = ANE[ 3*k - 1 ];

		rot = rotacao( rot, elemento[i].co, elemento[i].se );
		uL = prod_matf( rot, n, n, uG, n, 1 );
		KL = calcula_KL( KL, elemento[i].comp, elemento[i].E, elemento[i].Area, elemento[i].Iz );
		Af = prod_matf( KL, n, n, uL, n, 1 );
		Alep = calcula_Alep( px1[ i ], px2[ i ], py1[ i ], py2[ i ], elemento[ i ].comp,
		elemento[ i ].alpha, elemento[ i ].E, elemento[ i ].Area, elemento[ i ].Iz, te[ i ], ti[ i ], elemento[ i ].h );
		Af = soma_matf( Af, n, 1, Alep, n, 1 );
		aux = i + 1;
		saida << endl << setiosflags(ios::right) << setw(15) << aux << setw(15) << j
        << setiosflags(ios::scientific) << setprecision(4) 
        << setw(20) << setiosflags(ios::internal) << -Af[ 0 ][ 0 ] << setw(20) << Af[ 1 ][ 0 ] << setw(20) 
        << -Af[ 2 ][ 0 ] <<  endl;

		saida << setiosflags(ios::right) << setw(30) << k
        << setiosflags(ios::scientific) << setprecision(4) 
        << setw(20) << setiosflags(ios::internal) << Af[ 3 ][ 0 ] << setw(20) << -Af[ 4 ][ 0 ] << setw(20) 
        << Af[ 5 ][ 0 ] <<  endl;

		a = ( py2[ i ] - py1[ i ] )/(2*elemento[ i ].comp);
		b = py1[ i ];
		c = Af[ 1 ][ 0 ];

		saida_2 << "reset" << endl
		<< "M(x) = (" << py2[ i ] - py1[ i ] << "*x**3)/(6*" << elemento[ i ].comp << ")+(" << py1[ i ]
		<< "*x**2)/2+" << Af[ 1 ][ 0 ] << "*x-" << Af[ 2 ][ 0 ] << endl;
		saida_2 << "set title \"Momento fletor (kN.m)\"" << endl << "set xrange [ 0 :" << elemento[ i ].comp << "] noreverse" << endl
		<<"set xlabel \"x(m)\"" << endl  << "set ylabel \"M(kN.m)\"" << endl;  
		if( fabs( Af[ 2 ][ 0 ] ) != fabs( Af[ 5 ][ 0 ] ) ){
			if( ( py1[ i ] == py2[ i ] ) && ( py1[ i ] != 0 ) ){
				x = -c/b;
				if( ( x > 0 ) && ( x <= elemento[ i ].comp ) ){ 
					M = ( py2[ i ] - py1[ i ] )*pow( x, 3 )/( 6*elemento[ i ].comp ) + 
					( py1[ i ]*pow( x, 2 ) )/2 + Af[ 1 ][ 0 ]*x - Af[ 2 ][ 0 ];
					if( x <= elemento[ i ].comp/5 )
						saida_2 << "set grid\nset xtics " << setprecision( 3 ) << x << endl << "set ytics " << M <<  endl;
					else
						saida_2 << "set grid\nset xtics " << setprecision(7) << x << endl << "set ytics " << M <<  endl;

					saida_2 << "set yrange [] reverse" << endl;
					saida_2 << "set term postscript enhanced color; set output \"Momento fletor da barra " 
    					<< aux << ".eps\"" << endl << "plot M(x) with boxes 1 " << endl << endl;
				}				
				else{
					saida_2 << "set yrange [] reverse" << endl
					<< "set term postscript enhanced color; set output \"Momento fletor da barra " 
	    				<< aux << ".eps\"" << endl << "plot M(x) with boxes 1 " << endl << endl;
				}
			}
			else{
				x1 = ( -b + sqrt( pow( b, 2 ) - 4*a*c ) )/( 2*a ); 
				x2 = ( -b - sqrt( pow( b, 2 ) - 4*a*c ) )/( 2*a );
				if( ( x1 > 0 ) && ( x1 <= elemento[ i ].comp ) ){
					x = x1;
					M = ( py2[ i ] - py1[ i ] )*pow( x, 3 )/( 6*elemento[ i ].comp ) + 
					( py1[ i ]*pow( x, 2 ) )/2 + Af[ 1 ][ 0 ]*x - Af[ 2 ][ 0 ];
					if( x <= elemento[ i ].comp/5 )
						saida_2 << "set grid\nset xtics " << setprecision( 3 ) << x << endl << "set ytics " << M <<  endl;
					else
						saida_2 << "set grid\nset xtics " << setprecision(7) << x << endl << "set ytics " << M <<  endl;

					saida_2 << "set yrange [] reverse" << endl;
					saida_2 << "set term postscript enhanced color; set output \"Momento fletor da barra " 
	    				<< aux << ".eps\"" << endl << "plot M(x) with boxes 1 " << endl << endl;
				}
				else{
					if( ( x2 >= 0 ) && ( x2 <= elemento[ i ].comp ) ){
						x = x2;
						M = ( py2[ i ] - py1[ i ] )*pow( x, 3 )/( 6*elemento[ i ].comp ) + 
						( py1[ i ]*pow( x, 2 ) )/2 + Af[ 1 ][ 0 ]*x - Af[ 2 ][ 0 ];
						if( x <= elemento[ i ].comp/5 )
						saida_2 << "set grid\nset xtics " << setprecision( 3 ) << x << endl << "set ytics " << M <<  endl;
						else
							saida_2 << "set grid\nset xtics " << setprecision(7) << x << endl << "set ytics " << M <<  endl;

						saida_2 << "set yrange [] reverse" << endl;
						saida_2 << "set term postscript enhanced color; set output \"Momento fletor da barra " 
	    					<< aux << ".eps\"" << endl << "plot M(x) with boxes 1 " << endl << endl;
					}
					else{
						saida_2 << "set yrange [] reverse" << endl
						<< "set term postscript enhanced color; set output \"Momento fletor da barra " 
	    					<< aux << ".eps\"" << endl << "plot M(x) with boxes 1 " << endl << endl;
					}
				}
			}
		}
		else{
			if( ( Af[ 2 ][ 0 ] == 0 ) && ( Af[ 5 ][ 0 ] == 0 ) ){
				saida_2 << "set yrange [" << -5 << ":" << 5 << "]  reverse" << endl;
				saida_2 << "set term postscript enhanced color; set output \"Momento fletor da barra " 
    			<< aux << ".eps\"" << endl << "plot M(x) with boxes 2 " << endl << endl;
			}
			else{
				saida_2 << "set yrange [" << -Af[ 2 ][ 0 ] << ":" << Af[ 5 ][ 0 ] << "]  reverse" << endl;
				saida_2 << "set term postscript enhanced color; set output \"Momento fletor da barra " 
    			<< aux << ".eps\"" << endl << "plot M(x) with boxes 2 " << endl << endl;
			}
		}


		saida_2 << "reset" << endl;
		saida_2 << "N(x) = -(" << Af[ 0 ][ 0 ]<<  "+"<< px1[ i ] << "*x)" << endl;
		saida_2 << "set title \"Forca axial (kN)\"" << endl 
		<< "set xlabel \"x(m)\"" << endl  << "set ylabel \"N(kN)\"" << endl; 
		if( fabs( Af[ 0 ][ 0 ] ) != fabs( Af[ 3 ][ 0 ] ) ){
			saida_2 <<  "set xrange [ 0 : " << elemento[ i ].comp <<" ]" << endl << "set zeroaxis 7" << endl << 
			"set yrange [] noreverse" << endl;
			saida_2 << "set term postscript enhanced color; set output \"Força axial da barra " 
    		<< aux << ".eps\"" << endl << "plot N(x) with boxes 2 " << endl << endl;
		}
		else{
			if( ( Af[ 0 ][ 0 ] == 0 ) && ( Af[ 3 ][ 0 ] == 0 ) ){
				saida_2 << "set yrange [" << -5 << ":" << 5 << "]  noreverse" << endl;
				saida_2 << "set term postscript enhanced color; set output \"Força axial da barra " 
    			<< aux << ".eps\"" << endl << "plot N(x) with boxes 2 " << endl << endl;
			}
			else{
				saida_2 << "set yrange [" << -Af[ 0 ][ 0 ] << ":" << Af[ 0 ][ 0 ] << "]  noreverse" << endl;
				saida_2 << "set term postscript enhanced color; set output \"Força axial da barra " 
    			<< aux << ".eps\"" << endl << "plot N(x) with boxes 2 " << endl << endl;
			}
		}

		saida_2 << "reset" << endl;
		saida_2 << "V(x) = " << py2[ i ] - py1[ i ] << "*x**2/" << 2*elemento[ i ].comp << "+" << py1[ i ] << "*x+" << Af[ 1 ][ 0 ] << endl;
		saida_2 << "set title \"Forca cortante (kN)\"" << endl 
		<< "set xlabel \"x(m)\"" << endl  << "set ylabel \"V(kN)\"" << endl ;
		if( fabs( Af[ 1 ][ 0 ] ) != fabs( Af[ 4 ][ 0 ] ) ){
  			saida_2 << "set xrange [ 0 : " << elemento[ i ].comp <<" ]" << endl << "set zeroaxis 7" << endl 
			<<  "set yrange [] noreverse" << endl;
			saida_2 << "set term postscript enhanced color; set output \"Força cortante da barra "
    		<< aux << ".eps\"" << endl << "plot V(x) with boxes 3 " << endl << endl;
		}
		else{
			if( ( Af[ 1 ][ 0 ] == 0 ) && ( Af[ 4 ][ 0 ] == 0 ) ){
				saida_2 <<  "set yrange [" << -5 << ":" <<  5  << "] noreverse" << endl;
				saida_2 << "set term postscript enhanced color; set output \"Força cortante da barra "
    			<< aux << ".eps\"" << endl << "plot V(x) with boxes 3 " << endl << endl;
			}
			else{
				saida_2 <<  "set yrange [" << -Af[ 1 ][ 0 ] << ":" << Af[ 1 ][ 0 ]  << "] noreverse" << endl;
				saida_2 << "set term postscript enhanced color; set output \"Força cortante da barra "
    			<< aux << ".eps\"" << endl << "plot V(x) with boxes 3 " << endl << endl;
			}
		}
	}
}
