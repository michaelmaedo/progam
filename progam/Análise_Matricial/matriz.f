      subroutine soma_mat( A, mA, nA, B, mB, nB )
          dimension A( mA, nA ), B( mB, nB )
          if( ( mA .eq. mB ) .and. ( nA .eq. nB ) ) then
              do 1 i = 1, mA
                  do 1 j = 1, nA
                      A(i,j) = A(i,j) + B(i,j)
1             continue
          endif
          return
      end

      subroutine sub_mat( A, mA, nA, B, mB, nB )
          dimension A( mA, nA ), B( mB, nB )
          if( ( mA .eq. mB ) .and. ( nA .eq. nB ) ) then
              do 2 i = 1, mA
                  do 2 j = 1, nA
                      A(i,j) = A(i,j) - B(i,j)
2             continue
          endif
          return
      end

      subroutine prod_mat( C, A, mA, nA, B, mB, nB )
          dimension A( mA, nA ), B( mB, nB ), C( mA, nB)
          if( nA .eq. mB ) then
              do 3 i = 1, mA
                  do 3 j = 1, nB
                      C( i, j ) = 0
                      do 3 k = 1, nA
                          C( i, j ) = C( i, j ) + 
     + A( i, k )*B( k, j )
3             continue
          endif
      return
      end

      real function trans_mat( A, mA, nA)
          dimension A( mA, nA )
          do 4 i = 1, mA
              do 4 j = 1, nA
                  A( i, j ) = A( j, i )
4         continue
      return
      end
