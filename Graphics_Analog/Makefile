
all: 
	perl generateSVG.pl Time3_15.pos > Time3_15.svg
	inkscape --export-type=pdf Time3_15.svg -o Time3_15.pdf
	perl generateSVG.pl Time3_15_debouncing.pos > Time3_15_debouncing.svg  
	inkscape --export-type=pdf Time3_15_debouncing.svg -o Time3_15_debouncing.pdf
	perl generateSVG.pl Time3_15_SetHour.pos > Time3_15_SetHour.svg  
	inkscape --export-type=pdf Time3_15_SetHour.svg -o Time3_15_SetHour.pdf
	perl generateSVG.pl Time3_15_SetMinute.pos > Time3_15_SetMinute.svg  
	inkscape --export-type=pdf Time3_15_SetMinute.svg -o Time3_15_SetMinute.pdf
clean:
	rm *.pdf *.svg
