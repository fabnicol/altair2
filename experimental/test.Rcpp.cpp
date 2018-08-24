
#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]

int fibCpp(int n) 
            {
              if (n == 0) return(0);
              if (n == 1) return(1);
              return(fibCpp(n - 1) + fibCpp(n - 2));

            }
            

