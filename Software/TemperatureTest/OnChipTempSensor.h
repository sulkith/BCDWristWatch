class OnChipTempSensor
{
	enum 
	{
		idle,
		startADC,
		startConversion,
		waiting
	}State;
	uint16_t temp;
public:
	OnChipTempSensor():temp(0){};
	void loop()
	{
		switch(State)
		{
			case idle:
				break;
			case startADC:
				ACSR=0x00;
				ADCSRA = 0x80;   
				State = startConversion;
				break;
			case startConversion:
				ADMUX= 0xC0 + 8; //select 1.1 vref + channel 8 
				ADCSRA |= 0x40;  //init conversion 
				State = waiting;
			case waiting:
				if((ADCSRA & 0x40) == 0)
				{
					ACSR=0x80;
					ADCSRA = 0x00;
					temp = ADC;
					State = idle;
				}
				break;	
		}
	};
	void trigger()
	{
		State = startADC;
	};
	uint16_t get()
	{
		return temp;
	};
};
