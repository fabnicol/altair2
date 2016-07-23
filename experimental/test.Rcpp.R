library(Rcpp)
cppFunction('int fibCpp0(int n) 
            {
              if (n == 0) return(0);
              if (n == 1) return(1);
              return(fibCpp0(n - 1) + fibCpp0(n - 2));

            }
            
            ')
fibCpp0(20)

sourceCpp("../test.Rcpp.cpp")

library(microbenchmark)
microbenchmark(fibCpp0(20), fibCpp(20), times=500)
