#include <iostream>
#include <cstdlib>

using namespace std;

int **soma_mati(int **A, int mA, int nA, int **B, int mB, int nB)
{
	if((mA != mB) || (nA != nB)){
		cout << "Dimensões incorretas para somar as matrizes" << endl;
		exit(1);
	}
	else{
		for(int i = 0; i < mA; i++)
			for(int j = 0; j < nA; j++)
				A[i][j] = A[i][j] + B[i][j];
	}
	return (A);
}

float **soma_matf(float **A, int mA, int nA, float **B, int mB, int nB)
{
	if((mA != mB) || (nA != nB)){
		cout << "Dimensões incorretas para somar as matrizes" << endl;
		exit(1);
	}
	else{
		for(int i = 0; i < mA; i++)
			for(int j = 0; j < nA; j++)
				A[i][j] = A[i][j] + B[i][j];
	}
	return (A);
}

int **sub_mati(int **A, int mA, int nA, int **B, int mB, int nB)
{
	if((mA != mB) || (nA != nB)){
		cout << "Dimensões incorretas para subtrair as matrizes" << endl;
		exit(1);
	}
	else{
		for(int i = 0; i < mA; i++)
			for(int j = 0; j < nA; j++)
				A[i][j] = A[i][j] - B[i][j];
	}
	return (A);
}

float **sub_matf(float **A, int mA, int nA, float **B, int mB, int nB)
{
	if((mA != mB) || (nA != nB)){
		cout << "Dimensões incorretas para subtrair as matrizes" << endl;
		exit(1);
	}
	else{
		for(int i = 0; i < mA; i++)
			for(int j = 0; j < nA; j++)
				A[i][j] = A[i][j] - B[i][j];
	}
	return (A);
}

int **prod_mati(int **A, int mA, int nA, int **B, int mB, int nB)
{
	int **R;

	R = (int **)malloc(sizeof(int *)*mA);
	if(R == NULL){
		cout << "*** Memória insuficiente ***" << endl;
		return 0;
	}
	for(int i = 0; i < mA; i++){
		R[i] = (int *)malloc(sizeof(int)*nB);
		if(R[i] == NULL){
			cout << "*** Memória insuficiete ***" << endl;
			return 0;
		}
	}

	if(nA != mB){
		cout << "Dimensões incorretas para multiplicar as matrizes" << endl;
		exit(1);
	}
	else{
		for(int i = 0; i < mA; i++)
			for(int j = 0; j < nB; j++){
				R[i][j] = 0;
				for(int k = 0; k < nA; k++){
					R[i][j] = R[i][j] + A[i][k]*B[k][j];
				}
			}
	}
	return (R);
}

float **prod_matf(float **A, int mA, int nA, float **B, int mB, int nB)
{
	float **R;

	R = (float **)malloc(sizeof(float *)*mA);
	if(R == NULL){
		cout << "*** Memória insuficiente ***" << endl;
		return 0;
	}
	for(int i = 0; i < mA; i++){
		R[i] = (float *)malloc(sizeof(float)*nB);
		if(R[i] == NULL){
			cout << "*** Memória insuficiete ***" << endl;
			return 0;
		}
	}

	if(nA != mB){
		cout << "Dimensões incorretas para multiplicar as matrizes" << endl;
		exit(1);
	}
	else{
		for(int i = 0; i < mA; i++)
			for(int j = 0; j < nB; j++){
				R[i][j] = 0;
				for(int k = 0; k < nA; k++){
					R[i][j] = R[i][j] + A[i][k]*B[k][j];
				}
			}
	}
	return (R);
}

int **trans_mati(int **A, int mA, int nA)
{
	int **At;
	At = (int **)malloc(sizeof(int *)*nA);
	if(At == NULL){
		cout << "*** Memória insuficiente ***" << endl;
		return 0;
	}
	for(int i = 0; i < nA; i++){
		At[i] = (int *)malloc(sizeof(int)*mA);
		if(At[i] == NULL){
			cout << "*** Memória insuficiete ***" << endl;
			return 0;
		}
	}
	
		for(int i = 0; i < nA; i++)
			for(int j = 0; j < mA; j++)
					At[i][j] = A[j][i];

	return (At);
}

float **trans_matf(float **A, int mA, int nA)
{
	float **At;
	At = (float **)malloc(sizeof(float *)*nA);
	if(At == NULL){
		cout << "*** Memória insuficiente ***" << endl;
		return 0;
	}
	for(int i = 0; i < nA; i++){
		At[i] = (float *)malloc(sizeof(float)*mA);
		if(At[i] == NULL){
			cout << "*** Memória insuficiete ***" << endl;
			return 0;
		}
	}
		for(int i = 0; i < nA; i++)
			for(int j = 0; j < mA; j++)
					At[i][j] = A[j][i];

	return (At);
}
