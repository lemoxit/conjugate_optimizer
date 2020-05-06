# Conjugate Optimizer implementation (Nolinear)
Using Polak-Ribiere CG method to update $\beta$
## Note
* State is defined as StateType<N>, which has std::array<double, N> as state variables, N is the dimension of state
* You should provide ComputeValue(StateType) and ComputeGradient(StateType) in OptimizeObject

## TODO
* Optimize performance of line search
* Add error handling strategy
