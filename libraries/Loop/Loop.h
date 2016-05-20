class Loop{
  public:
    Loop(int _rate);
    void set_Loop_Rate(int _rate);
    bool ok();
    unsigned int get_Frequency();
    void enable_Frequency_Monitor();
    void disable_Frequency_Monitor();    
    void Debug_Frequency();    
    
  private:
    float rate;
    float period_us;
    unsigned long int base_time;
    unsigned long int time;
    bool frequency_monitor;
    bool first_loop;
    unsigned int counter_temp;
    unsigned int counter;
    unsigned long int last_loop;    
};
