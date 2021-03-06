EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Regulator_Switching:TPS62203DBV U2
U 1 1 608D6EF5
P 7150 2850
F 0 "U2" H 7150 3275 50  0000 C CNN
F 1 "TPS62203DBV" H 7150 3184 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 7200 2700 50  0001 L CIN
F 3 "http://www.ti.com/lit/ds/symlink/tps62201.pdf" H 7150 2950 50  0001 C CNN
	1    7150 2850
	1    0    0    -1  
$EndComp
$Comp
L Battery_Management:LTC4054ES5-4.2 U1
U 1 1 608D76A5
P 4450 2550
F 0 "U1" H 4894 2546 50  0000 L CNN
F 1 "LTC4054ES5-4.2" H 4894 2455 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TSOT-23-5" H 4450 2050 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/405442xf.pdf" H 4450 2450 50  0001 C CNN
	1    4450 2550
	1    0    0    -1  
$EndComp
Text GLabel 4050 2550 0    50   Input ~ 0
CHG_State
Text GLabel 4450 2250 1    50   Input ~ 0
CHG+
$Comp
L Device:R R1
U 1 1 608DCB4C
P 3900 2950
F 0 "R1" H 3970 2996 50  0000 L CNN
F 1 "10k" H 3970 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3830 2950 50  0001 C CNN
F 3 "~" H 3900 2950 50  0001 C CNN
	1    3900 2950
	1    0    0    -1  
$EndComp
Text GLabel 4850 2550 2    50   Input ~ 0
BAT+
Text GLabel 6300 2650 0    50   Input ~ 0
BAT+
$Comp
L Device:L L1
U 1 1 608DD422
P 7700 2650
F 0 "L1" V 7519 2650 50  0000 C CNN
F 1 "10uH" V 7610 2650 50  0000 C CNN
F 2 "Inductor_SMD:L_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7700 2650 50  0001 C CNN
F 3 "~" H 7700 2650 50  0001 C CNN
	1    7700 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	6700 2650 6700 2850
Wire Wire Line
	6700 2850 6850 2850
Connection ~ 6700 2650
Wire Wire Line
	6700 2650 6850 2650
Wire Wire Line
	4050 2650 3900 2650
Wire Wire Line
	3900 2650 3900 2800
Wire Wire Line
	3900 3100 3900 3200
Wire Wire Line
	3900 3200 4450 3200
Wire Wire Line
	4450 3200 4450 2950
Wire Wire Line
	7450 2650 7550 2650
Wire Wire Line
	7450 2850 7950 2850
Wire Wire Line
	7950 2850 7950 2650
Wire Wire Line
	7950 2650 7850 2650
Wire Wire Line
	7950 2650 8200 2650
Connection ~ 7950 2650
$Comp
L power:GND #PWR0101
U 1 1 608DE303
P 4450 3200
F 0 "#PWR0101" H 4450 2950 50  0001 C CNN
F 1 "GND" H 4455 3027 50  0000 C CNN
F 2 "" H 4450 3200 50  0001 C CNN
F 3 "" H 4450 3200 50  0001 C CNN
	1    4450 3200
	1    0    0    -1  
$EndComp
Connection ~ 4450 3200
$Comp
L power:GND #PWR0102
U 1 1 608DE550
P 7150 3250
F 0 "#PWR0102" H 7150 3000 50  0001 C CNN
F 1 "GND" H 7155 3077 50  0000 C CNN
F 2 "" H 7150 3250 50  0001 C CNN
F 3 "" H 7150 3250 50  0001 C CNN
	1    7150 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 3050 7150 3200
$Comp
L Device:C C2
U 1 1 608DF44A
P 8200 2950
F 0 "C2" H 8315 2996 50  0000 L CNN
F 1 "C" H 8315 2905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 8238 2800 50  0001 C CNN
F 3 "~" H 8200 2950 50  0001 C CNN
	1    8200 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 608DF9F7
P 8550 2950
F 0 "C3" H 8665 2996 50  0000 L CNN
F 1 "C" H 8665 2905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 8588 2800 50  0001 C CNN
F 3 "~" H 8550 2950 50  0001 C CNN
	1    8550 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 608DFDF4
P 6500 2950
F 0 "C1" H 6615 2996 50  0000 L CNN
F 1 "C" H 6615 2905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6538 2800 50  0001 C CNN
F 3 "~" H 6500 2950 50  0001 C CNN
	1    6500 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2650 6500 2650
Wire Wire Line
	6500 2800 6500 2650
Connection ~ 6500 2650
Wire Wire Line
	6500 2650 6700 2650
Wire Wire Line
	6500 3100 6500 3200
Wire Wire Line
	6500 3200 7150 3200
Connection ~ 7150 3200
Wire Wire Line
	7150 3200 7150 3250
Wire Wire Line
	7150 3200 8200 3200
Wire Wire Line
	8200 3200 8200 3100
Wire Wire Line
	8200 3200 8550 3200
Wire Wire Line
	8550 3200 8550 3100
Connection ~ 8200 3200
Wire Wire Line
	8200 2800 8200 2650
Wire Wire Line
	8200 2650 8550 2650
Wire Wire Line
	8550 2650 8550 2800
Connection ~ 8200 2650
Text GLabel 6300 3200 0    50   Input ~ 0
BAT-
Wire Wire Line
	6300 3200 6500 3200
Connection ~ 6500 3200
Text GLabel 4850 3200 2    50   Input ~ 0
BAT-
Wire Wire Line
	4450 3200 4850 3200
Text GLabel 8550 2650 2    50   Input ~ 0
PowerOut
$Comp
L Connector:Conn_01x02_Female J_Bat_Cur1
U 1 1 608E4C84
P 6050 1500
F 0 "J_Bat_Cur1" V 5988 1312 50  0000 R CNN
F 1 "Conn_01x02_Female" V 5897 1312 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6050 1500 50  0001 C CNN
F 3 "~" H 6050 1500 50  0001 C CNN
	1    6050 1500
	0    -1   -1   0   
$EndComp
$Comp
L Device:Battery_Cell BT1
U 1 1 608E5CB8
P 6350 1900
F 0 "BT1" H 6468 1996 50  0000 L CNN
F 1 "Battery_Cell" H 6468 1905 50  0000 L CNN
F 2 "SulkithKicadFootprints:SolderPad_2Pin" V 6350 1960 50  0001 C CNN
F 3 "~" V 6350 1960 50  0001 C CNN
	1    6350 1900
	1    0    0    -1  
$EndComp
Text GLabel 5850 1700 0    50   Input ~ 0
BAT+
Text GLabel 5850 2000 0    50   Input ~ 0
BAT-
Wire Wire Line
	5850 1700 6050 1700
Wire Wire Line
	6150 1700 6350 1700
Wire Wire Line
	5850 2000 6350 2000
$Comp
L Connector:Conn_01x02_Female J_ChgIn1
U 1 1 608EA35C
P 2300 2800
F 0 "J_ChgIn1" H 2192 2475 50  0000 C CNN
F 1 "Conn_01x02_Female" H 2192 2566 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2300 2800 50  0001 C CNN
F 3 "~" H 2300 2800 50  0001 C CNN
	1    2300 2800
	-1   0    0    1   
$EndComp
Text GLabel 2500 2700 2    50   Input ~ 0
CHG+
$Comp
L power:GND #PWR0103
U 1 1 608EB1C3
P 2500 2800
F 0 "#PWR0103" H 2500 2550 50  0001 C CNN
F 1 "GND" H 2505 2627 50  0000 C CNN
F 2 "" H 2500 2800 50  0001 C CNN
F 3 "" H 2500 2800 50  0001 C CNN
	1    2500 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female BatDummy1
U 1 1 608EB516
P 10000 2900
F 0 "BatDummy1" H 9892 2575 50  0000 C CNN
F 1 "Conn_01x02_Female" H 9892 2666 50  0000 C CNN
F 2 "SulkithKicadFootprints:CR2032_Dummy" H 10000 2900 50  0001 C CNN
F 3 "~" H 10000 2900 50  0001 C CNN
	1    10000 2900
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 608EBE68
P 10200 2900
F 0 "#PWR0104" H 10200 2650 50  0001 C CNN
F 1 "GND" H 10205 2727 50  0000 C CNN
F 2 "" H 10200 2900 50  0001 C CNN
F 3 "" H 10200 2900 50  0001 C CNN
	1    10200 2900
	1    0    0    -1  
$EndComp
Text GLabel 10200 2800 2    50   Input ~ 0
PowerOut
$Comp
L Connector:Conn_01x01_Female GND1
U 1 1 608EEAD8
P 4450 5150
F 0 "GND1" H 4478 5176 50  0000 L CNN
F 1 "Conn_01x01_Female" H 4478 5085 50  0000 L CNN
F 2 "SulkithKicadFootprints:SolderPad" H 4450 5150 50  0001 C CNN
F 3 "~" H 4450 5150 50  0001 C CNN
	1    4450 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 608EF4E3
P 4250 5150
F 0 "#PWR0105" H 4250 4900 50  0001 C CNN
F 1 "GND" H 4255 4977 50  0000 C CNN
F 2 "" H 4250 5150 50  0001 C CNN
F 3 "" H 4250 5150 50  0001 C CNN
	1    4250 5150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Female J_ChgStat1
U 1 1 608F13D8
P 4450 5000
F 0 "J_ChgStat1" H 4478 5026 50  0000 L CNN
F 1 "Conn_01x01_Female" H 4478 4935 50  0000 L CNN
F 2 "SulkithKicadFootprints:SolderPad" H 4450 5000 50  0001 C CNN
F 3 "~" H 4450 5000 50  0001 C CNN
	1    4450 5000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Female J_Bat+1
U 1 1 608F16B8
P 4450 4850
F 0 "J_Bat+1" H 4478 4876 50  0000 L CNN
F 1 "Conn_01x01_Female" H 4478 4785 50  0000 L CNN
F 2 "SulkithKicadFootprints:SolderPad" H 4450 4850 50  0001 C CNN
F 3 "~" H 4450 4850 50  0001 C CNN
	1    4450 4850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Female J_PowOut1
U 1 1 608F18A9
P 4450 4700
F 0 "J_PowOut1" H 4478 4726 50  0000 L CNN
F 1 "Conn_01x01_Female" H 4478 4635 50  0000 L CNN
F 2 "SulkithKicadFootprints:SolderPad" H 4450 4700 50  0001 C CNN
F 3 "~" H 4450 4700 50  0001 C CNN
	1    4450 4700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Female J_CHG+1
U 1 1 608F1AFA
P 4450 4550
F 0 "J_CHG+1" H 4478 4576 50  0000 L CNN
F 1 "Conn_01x01_Female" H 4478 4485 50  0000 L CNN
F 2 "SulkithKicadFootprints:SolderPad" H 4450 4550 50  0001 C CNN
F 3 "~" H 4450 4550 50  0001 C CNN
	1    4450 4550
	1    0    0    -1  
$EndComp
Text GLabel 4250 5000 0    50   Input ~ 0
CHG_State
Text GLabel 4250 4850 0    50   Input ~ 0
BAT+
Text GLabel 4250 4700 0    50   Input ~ 0
PowerOut
Text GLabel 4250 4550 0    50   Input ~ 0
CHG+
$EndSCHEMATC
