# Conjugate Optimizer implementation (Nolinear)
Using Polak-Ribiere CG method to update $\beta$
## Note
* StateType in class template must support operator +, -,  double * StateType and StateType * double
* You should provide ComputeValue(StateType) and ComputeGradient(StateType), InnerProduct(State,State)
* DebugString() may not work for your own StateType

## TODO
* Optimize performance of line search
* Add error handling strategy