     C
C Subrotina auxiliar utilizada para calcular KL
C
      subroutine calcula_KL( KL, L, E, A, I)
         real KL(n,n), L, E, A, I 
         n = 6
         do 1 j = 1, n
             do 1 k = 1, n
                 KL(j,k) = 0
1        continue
         KL(1,1) = E*A/L 
         KL(4,4) = KL(1,1)
         KL(1,4) = -KL(1,1)
         KL(4,1) = KL(1,4)
         KL(2,2) = 12*E*I/L**3
         KL(5,5) = KL(2,2)
         KL(5,2) = -KL(1,1)
         KL(2,5) = KL(5,2)
         KL(2,3) = 6*E*I/L**2
         KL(3,2) = KL(2,3)
         KL(2,6) = KL(2,3)
         KL(6,2) = KL(2,3)
         KL(3,5) = -KL(2,3)
         KL(5,3) = KL(3,5)
         KL(5,6) = KL(3,5)
         KL(6,5) = KL(3,5)
         KL(3,6) = 2*E*I/L
         KL(6,3) = KL(3,6)
         KL(3,3) = 4*E*I*L
         KL(6,6) = KL(3,3)
         return
      end
C
C
C Subrotina para resolver o sistema linear
      subroutine linresolve( n, A, B )
          real A( n, n ), B( n ), aux
          integer nmaior, i, j, k
          logical sol
          sol = .true.
          i = 1
          do 50 while( sol .and. ( i .le. n ) )
C
C         Escolha do maior pivô
C
              nmaior = i
              do 10 j = i + 1, n
                  if( abs( A( j, i ) ) .gt. abs( A( nmaior, i ) ) ) then
                      nmaior = j
                  endif
10            continue
              if( nmaior .ne. i ) then
                  do 20 j = i, n
                      aux = A( i, j )
                      A( i, j ) = A( nmaior, j )
                      A( nmaior, j ) = aux
20                continue
                  aux = B( i )
                  B( i ) = B( nmaior )
                  B( nmaior ) = aux
              endif
C
C         Fim do pivotamento
C
              if( A( i, i ) .eq. 0 ) then
                  sol = .false.
              else
                  do 40 j = i + 1, n
                      aux = A( j, i )/A( i, i )
                      do 30 k = i, n
                          A( j, k ) = A( j, k ) - A( i, k )*aux
30                    continue
                  B( j ) = B( j ) - B( i )*aux
40                continue
              endif
              i = i + 1
50        continue
          if( sol ) then
              B( n ) = B( n )/A( n, n )
              do 70 i = n - 1, 1, -1
                  aux = 0
                  do 60 j = i + 1, n
                      aux = aux + A( i, j )*B( j )
60                continue
              B( i ) = ( B( i ) - aux )/A( i, i )
70            continue
          endif
      return
      end
