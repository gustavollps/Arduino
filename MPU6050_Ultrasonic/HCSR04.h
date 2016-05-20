class HCSR04{
  public:
    HCSR04(int _trigger, int _echo);
    void ISRoutine();
    
  private:
    int trigger;
    int echo;
    int interrupt;
    unsigned long int t_init;
    unsigned long int t_final;
};
