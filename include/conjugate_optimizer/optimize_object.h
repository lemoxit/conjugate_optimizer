
template <class StateType>
class OptimizeObject {
 public:
  using ObjectValue = double;
  using DirectionType = StateType;
  using GradientType = StateType;
  using InitialGuessType = StateType;

  explicit OptimizeObject(const InitialGuessType& initial_guess)
      : state_(initial_guess) {}
  virtual ~OptimizeObject() = default;

  inline const StateType state() const { return state_; }
  inline const ObjectValue value() const { return value_; }
  inline const GradientType gradient() const { return gradient_; }
  void;

 protected:
  virtual const GradientType ComputeGradient() const = 0;
  virtual const ObjectValue ComputeValue() const = 0;
  StateType state_;
  ObjectValue value_;
  GradientType gradient_;
};