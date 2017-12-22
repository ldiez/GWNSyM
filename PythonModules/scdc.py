from gpkit.shortcuts import *
from math import exp
from math import factorial as facto
from math import log
from numpy import *

class SingleAssoc (Model):
    """
    Model of the single association problem with traffic demand constraints

    min sum_{ij} x_{ij}
    s.t. 
        x_{ij} * log_2 ( P_{ij} / (a + 1/b * sum P_{ik} sum x_{mk} ))<= D_i;
        sum_i x_{ij} <= b \forall j %capacity constraint
        x_{ij} <= b %% it is implicit in the previous constraint

    Arguments
    ---------
    I : [][]numpy matrix
    Interference matrix. It has N rows where N is the number of users.


    P : [][]numpy matrix
    Received power matrix of size NxM where N is the number of users 
    and M the number of access elements
    A : float
    Noise power
    D : [float]
    List od traffic demands of size N  where N is the number of users
    """

    def setup(self, Inter, Power, Demand, Noise):

        size = Inter.shape
        nu = size[0]
        nae = size[1]
       
        N = Vec(nu, "Noise", Noise);
        Floor = Var("Floor", 1e-12);
        x = Vec(nu, "x")
        t = Vec(nu, "t")
        D = Vec(nu, "D", Demand)

        constraints = [t >= Floor, x >= Floor,
        t >= self.Exp(log(2) * D / x)]

        for u in range(0, nu):
            aux = squeeze(Inter[u])
            I = Vec (nae, "I" + str(u + 1), aux)
            constraints += [1 / t[u] >= (N[u] + sum(I * x)) / Power[u]];
        obj = sum(x)            
        return (obj, constraints)


    def Exp (self, Var):
        """
        Compute the exponential by using the expansion in Taylor
        series
        Arguments
        ---------
        Var: variable to compute
        """
        return 1 + Var + (Var ** 2) / 2 + \
            (Var ** 3) / facto(3) + \
            (Var ** 4) / facto(4) + \
            (Var ** 5) / facto(5) + \
            (Var ** 6) / facto(6) + \
            (Var ** 7) / facto(7) + \
            (Var ** 8) / facto(8) + \
            (Var ** 9) / facto(9) + \
            (Var ** 10) / facto(10) + \
            (Var ** 11) / facto(11) + \
            (Var ** 12) / facto(12) + \
            (Var ** 13) / facto(13) + \
            (Var ** 14) / facto(14) + \
            (Var ** 15) / facto(15) + \
            (Var ** 16) / facto(16) + \
            (Var ** 17) / facto(17) + \
            (Var ** 18) / facto(18) + \
            (Var ** 19) / facto(19) + \
            (Var ** 20) / facto(20)
   
def Solve (Power, Demand, Inter, Noise):
    m = SingleAssoc(Inter=array(Inter), Power=Power, Demand=Demand, Noise=Noise);
    sol = m.solve(verbosity=0)
    x = sol('x')
    
#    print x
    return x.tolist()

if __name__ == "__main__":
    print ("Main, call 'Solve' to perform optimization")

    
