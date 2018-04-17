#include <iostream>
#include <cstdlib>
#include "alocar.h"

template < typename tipo >
tipo *alocar_vet( tipo *vet, int n )
{
	vet = new tipo[n];
	if( vet == NULL ){
		std::cout << " *** Memória insuficiente *** "<< std::endl;
		return 0;
	}
	return vet;
}

template < typename tipo >
tipo **alocar_mat( tipo **mat, int m , int n )
{
	mat = new tipo*[m];
	if( mat == NULL ){
		std::cout << " *** Memória insuficiente *** " << std::endl;
		return 0;
	}
	for( int i = 0; i < m; i++ ){
		mat[i] = new tipo[n];
		if( mat[i] == NULL ){
			std::cout << " *** Memória insuficiente *** " << std::endl;
			return 0;
		}
	}
	return mat;
}

template < typename tipo >
tipo *libera_vet( int m, tipo *vet )
{
	delete []vet;
	return vet;
}

template < typename tipo >
tipo **libera_mat( int m1, int m2, tipo **mat )
{
	for( int i = 0; i < m1; i++ ){
		delete []mat[i];

	delete []mat;
	return mat;
	}
}
