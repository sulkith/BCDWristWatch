
all: 
	perl generateSVG.pl Time3_15.pos > Time3_15.svg
	inkscape --export-type=pdf Time3_15.svg -o Time3_15.pdf
	perl generateSVG.pl Time3_15_debouncing.pos > Time3_15_debouncing.svg  
	inkscape --export-type=pdf Time3_15_debouncing.svg -o Time3_15_debouncing.pdf
	perl generateSVG.pl Time3_15_SetHour.pos > Time3_15_SetHour.svg  
	inkscape --export-type=pdf Time3_15_SetHour.svg -o Time3_15_SetHour.pdf
	perl generateSVG.pl Time3_15_SetMinute.pos > Time3_15_SetMinute.svg  
	inkscape --export-type=pdf Time3_15_SetMinute.svg -o Time3_15_SetMinute.pdf
	perl generateSVG.pl Date25_Dez.pos > Date25_Dez.svg  
	inkscape --export-type=pdf Date25_Dez.svg -o Date25_Dez.pdf
	perl generateSVG.pl Date25_Dez_debouncing.pos > Date25_Dez_debouncing.svg  
	inkscape --export-type=pdf Date25_Dez_debouncing.svg -o Date25_Dez_debouncing.pdf
	perl generateSVG.pl Date25_Dez_SetDay.pos > Date25_Dez_SetDay.svg  
	inkscape --export-type=pdf Date25_Dez_SetDay.svg -o Date25_Dez_SetDay.pdf
	perl generateSVG.pl Date25_Dez_SetMonth.pos > Date25_Dez_SetMonth.svg  
	inkscape --export-type=pdf Date25_Dez_SetMonth.svg -o Date25_Dez_SetMonth.pdf
	perl generateSVG.pl Date25_Dez_SetYear21.pos > Date25_Dez_SetYear21.svg  
	inkscape --export-type=pdf Date25_Dez_SetYear21.svg -o Date25_Dez_SetYear21.pdf
	perl generateSVG.pl ShowSteps32768.pos > ShowSteps32768.svg  
	inkscape --export-type=pdf ShowSteps32768.svg -o ShowSteps32768.pdf
	perl generateSVG.pl ShowStepsHist_2Days_ago_7500.pos > ShowStepsHist_2Days_ago_7500.svg  
	inkscape --export-type=pdf ShowStepsHist_2Days_ago_7500.svg -o ShowStepsHist_2Days_ago_7500.pdf
	perl generateSVG.pl ShowUBatt_2950mV.pos > ShowUBatt_2950mV.svg  
	inkscape --export-type=pdf ShowUBatt_2950mV.svg -o ShowUBatt_2950mV.pdf	
clean:
	rm *.pdf *.svg
