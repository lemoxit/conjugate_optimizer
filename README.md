# Conjugate Optimizer implementation (Nolinear)
Using Polak-Ribiere CG method to update $\beta$
## Note
* StateType in class template must support operator +, -, StateType * double, DebugString(), and InnerProduct(StateType)
* You should provide ComputeValue(StateType) and ComputeGradient(StateType) in OptimizeObject

## TODO
* Optimize performance of line search
* Add error handling strategy
