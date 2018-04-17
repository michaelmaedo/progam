//nome: alocar.h
//função: alocar memória dinâmicamente

//Gabartiro para o vetor
template < typename tipo >
//alocar memória dinâmicamente para vetores
tipo *alocar_vet( tipo *, int );

//Gabarito para a matriz
template < typename tipo >
//alocar memória dinâmicamente para matrizes
tipo **alocar_mat( tipo **, int ,int );

template < typename tipo >
//libera memória de vet
tipo *libera_vet( int , tipo * );

template < typename tipo >
//libera memória de mat
tipo **libera_mat( int , int , tipo ** );
