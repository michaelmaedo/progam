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

//Cabeçalhos arquivos .header
#include "alocar.cpp"
#include "matriz.h"
#include "cppsubrotinas.h"

//Declaração de constantes
#define max 150
#define n 6

using namespace std;

//typedef para tornar o programa mais portável, sendo que barra será um sinônimo para o tipo elemento
typedef struct elemento barra;

//Declaração de registros para as barras
struct elemento{
	int noinicio, nofim;
	float E, Area, Iz, comp, co, se;
};

void ler_dados1( ifstream &entrada, ofstream &saida, int *Nnos, int *Nbarras, int *Nnocar, int *Nbarcar, int *Nnovinc )
{
	//Declaração de variáveis locais
	const int espaco1 = 40, espaco2 = 60;
	char  titulo1[max];

	//Da primeira linha até a sétima, copia-se as linhas do arquivo de entrada para o arquivo de saída
	for( int i = 1; i <= 11; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << '\n';
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
}

void ler_dados2(ifstream &entrada, ofstream &saida, float *x, float *y, barra *elemento, float *ANE, float *px1, float *px2, float *py1, float *py2, int *vinc, int Nnos, int Nbarras, int Nnocar, int Nbarcar, int Nnovinc )
{
	int aux;
	float dx, dy;
	char titulo1[max];

	entrada.getline(titulo1, NULL);
	for(int i = 1; i < 5; i++){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for(int i = 0; i < Nnos; i++){
		entrada >> aux >> x[i] >> y[i];
		saida << setiosflags(ios::left) << setw(11) << ' ' << aux << setw(15) << ' ' << x[i] << setw(15) << ' ' << y[i] << endl;
	}

	entrada.getline(titulo1, NULL);
	for(int i = 1; i < 5; i++){
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
		saida << setiosflags(ios::left) << setw(12) << ' ' << aux << setw(19) << ' ' << elemento[i].noinicio << setw(15) << ' ' << elemento[i].nofim << setw(15) << ' ' << elemento[i].E << setw(14) << ' ' << elemento[i].Area << setw(15) << ' ' << elemento[i].Iz << endl;

	}
	//Le no arquivo de entrada dados relativo as forças e imprime no arquivo de saida
	for( int i = 1; i < 9; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}
	for( int i = 0; i < 3*Nnos; i++ ){
		ANE[ i ] = 0;
		vinc[ i ] = 0;
	}

	for( int i = 0; i < Nnocar; i++ ){
		entrada >> aux;
		entrada >> ANE[ 3*aux - 3 ] >> ANE[ 3*aux - 2 ] >> ANE[ 3*aux - 1 ];
		saida << setiosflags(ios::left) << setw(8) << ' ' << aux << setw(20) << ' ' << ANE[ 3*aux - 3 ] << setw(20) << ' ' <<  ANE[ 3*aux - 2 ] << setw(20) << ' ' <<  ANE[ 3*aux - 1 ] << endl;
	}

	for( int i = 1; i < 9; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}
	for( int i = 0; i < Nbarras; i++ ){
		px1[ i ] = 0;
		px2[ i ] = 0;
		py1[ i ] = 0;
		py2[ i ] = 0;
	}

	for( int i = 0; i < Nbarcar; i++ ){
		entrada >> aux;
		entrada >> px1[ aux - 1 ] >> px2[ aux - 1 ] >> py1[ aux - 1 ] >> py2[ aux - 1 ];
		saida << setiosflags(ios::left) << setw(8) << ' ' << aux << setw(15) << ' ' << px1[ aux - 1 ] << setw(15) << ' ' <<  px2[ aux - 1 ] << setw(15) << ' ' <<  py1[ aux - 1 ] << setw(15) << ' ' << py2[ aux - 1] << endl;
	}

	//Lê no arquivo de entrada os dados relativos as vinculações e imprime no arquivo de saída
	for( int i = 1; i < 9; i++ ){
		entrada.getline(titulo1, max);
		saida << setiosflags(ios::left) << titulo1 << endl;
	}

	for( int i = 0; i < Nnovinc; i++ ){
		entrada >> aux;
		entrada >> vinc[ 3*aux - 3 ] >> vinc[ 3*aux - 2 ] >> vinc[ 3*aux - 1];
		saida << setiosflags(ios:: right) << setw(15) << aux << setw(15) << vinc[ 3*aux - 3 ] << setw(15) << vinc[ 3*aux - 2 ] << setw(15) << vinc[ 3*aux - 1 ] << endl;
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

float **calcula_Rig(int Nnos, int Nbarras, barra *elemento, float **Rig)
{
	//Variáveis locais
	float **KL, **rot, **KG;

	//Alocação de memória para as matrizes KL, rot e KG
	KL = alocar_mat( KL, n, n );
	rot = alocar_mat( rot, n, n );
	KG = alocar_mat( KG, n, n );

	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) Rig[i][j] = 0;

	for(int i = 0; i < Nbarras; i++){
		//Calcula a matriz de rigidez local da barra i+1
		KL = calcula_KL(KL, elemento[i].comp, elemento[i].E, elemento[i].Area, elemento[i].Iz);

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
	libera_mat( n, n, KL );
	libera_mat( n, n, rot );
	libera_mat( n ,n, KG );
	return (Rig);
}

float **calcula_Alep( float px1, float px2, float py1, float py2, int l)
{
	float **Alep = alocar_mat( Alep, n, 1);
	Alep[ 0 ][ 0 ] = -( px2 + 2*px1 )*l/6;
	Alep[ 1 ][ 0 ] = -( 3*py2 + 7*py1 )*l/20;
	Alep[ 2 ][ 0 ] = -( 2*py2 + 3*py1 )*pow( l , 2 )/60;
	Alep[ 3 ][ 0 ] = -( 2*px2 + px1 )*l/6;
	Alep[ 4 ][ 0 ]= -( 7*py2 + 3*py1 )*l/20;
	Alep[ 5 ][ 0 ] = ( 3*py2 + 2*py1 )*pow( l , 2 )/60;
	return Alep;
}

float *monta_ANE( int Nbarras, float *px1, float *px2, float *py1, float *py2, barra *elemento, float *ANE )
{
	int j, k;
	float **rot = alocar_mat( rot, n , n ), **Agep = alocar_mat( Agep, n, 1 ), **Alep = alocar_mat( Alep, n , 1 );

	for(int i = 0; i < Nbarras; i++ ){
		Alep = calcula_Alep( px1[ i ], px2[ i ], py1[ i ], py2[ i ], elemento[ i ].comp);
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

void vinculacao(int *vinc, float *ANE, float **Rig, int Nnos)
{
	for( int i = 0; i < 3*Nnos; i++ ){
		if( vinc[ i ] == 1 ){
			ANE[ i ] = 0;
			for( int j = 0; j < 3*Nnos; j++ ){
				Rig[ i ][ j ] = Rig[ j ][ i ] = 0;
				Rig[ i ][ i ] = 1;
			}
		}
	}
}

void resolve( ofstream &saida, const int &m, float *B, float **A )
{
	int nmaior, i = 0, j, k;
	float aux;
	bool sol = true;

	while( sol && ( i < m ) ){
		nmaior = i;
		for( j = i + 1; j < m; j++ ){
			if( fabs( A[j][i] ) > fabs( A[nmaior][i] ) )
				nmaior = j;
		}
		if( nmaior != i){
			for( j = i; j < m; j++ ){
				aux = A[i][j];
				A[i][j] = A[nmaior][j];
				A[nmaior][j] = aux;
			}
			aux = B[i];
			B[i] = B[nmaior];
			B[nmaior] = aux;
		}
		if( A[i][i] == 0){
			cout << "sistema sem solução\n";
			sol = false;
		}
		else{
			for( j = i + 1; j < m; j++ ){
				aux = A[j][i]/A[i][i];
				for( k = i; k < m; k++ )
					A[j][k] = A[j][k] - A[i][k]*aux;
				B[j] = B[j] - B[i]*aux;
			}
		}
	i = i + 1;
	}
	if( sol ){
		B[m - 1] = B[m - 1]/A[m - 1][m - 1];
		for( i = m - 2; i > 0; i-- ){
			aux = 0;
			for( j = i + 1; j < m;j++ )
				aux = aux + A[i][j]*B[j];
		B[i] = ( B[i] - aux)/A[i][i];
		}
	}
}

void saida_dados(ofstream &saida, float *ANE, float **Rig, int Nnos)
{
	int tam = 3*Nnos;
	saida << "\n\n\t------Esforços nodais------\n" ;
	for(int i = 0; i < tam; i++)
			saida << setiosflags(ios::scientific) << setprecision(4) << setw(18) << setiosflags(ios::internal) << ANE[i] << endl;

	saida << "\n\n\t------Matriz de Rigidez------\n";
	for(int i = 0;i < tam; i++){
		saida << "\n";
		for(int j = 0; j < tam; j++ )
			saida << setiosflags(ios::scientific) << setprecision(4) << setw(18) << setiosflags(ios::internal) << Rig[i][j];
	}
}
