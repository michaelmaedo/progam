//Biblioteca para as funções utilizadas no programa

typedef struct elemento barra;

//Declaração de registros para as barras
struct elemento{
	int noinicio, nofim;
	float E, Area, Iz, comp, co, se;
};

//Protótipo das funções
void ler_dados1( ifstream&, ofstream&, int *,int *,int *,int *,int * );
void ler_dados2( ifstream&, ofstream& , float *, float *, barra *, float *, float *, float *, float *, float *, int *, int, int , int , int, int );
float **calcula_KL( float**, float *, float *, float *, float * );
float **rotacao( float **, float *, float *);
float **calcula_Rig( int, int, barra *, float ** );
void saida_dados( ofstream&, float *,float **, int );
float  *calcula_Alep( float , float , float , float );
float *monta_ANE( int, float *, float *, float *, float *, barra *, float *);
void vinculacao(int *, float *, float **, int );
void resolve( ofstream&, const int &, float *, float **);
