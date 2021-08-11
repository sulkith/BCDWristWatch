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
L SulkithFootprints:LTC4124 U2
U 1 1 611246F2
P 7950 2950
F 0 "U2" H 7950 3515 50  0000 C CNN
F 1 "LTC4124" H 7950 3424 50  0000 C CNN
F 2 "SulkithFootprints:Analog_LQFN12_LTC4124" H 7950 2950 50  0001 C CNN
F 3 "" H 7950 2950 50  0001 C CNN
	1    7950 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 3050 8600 3050
Wire Wire Line
	8250 3900 8250 3750
Wire Wire Line
	7450 3400 7250 3400
Wire Wire Line
	7250 3400 7250 3900
Wire Wire Line
	7250 3900 8250 3900
Connection ~ 8250 3900
Wire Wire Line
	7400 3000 7450 3000
Wire Wire Line
	7450 2800 7400 2800
Wire Wire Line
	7400 2800 7400 3000
Connection ~ 7400 3000
Connection ~ 7250 3400
Wire Wire Line
	7450 3100 7250 3100
Wire Wire Line
	7250 3100 7250 3300
Wire Wire Line
	7400 3000 7400 3200
Wire Wire Line
	7450 3200 7400 3200
Connection ~ 7400 3200
Wire Wire Line
	7450 3300 7250 3300
Connection ~ 7250 3300
Wire Wire Line
	7250 3300 7250 3400
Wire Wire Line
	8450 2850 8600 2850
Wire Wire Line
	8600 2850 8600 3050
Connection ~ 8600 3050
Wire Wire Line
	8600 3050 8600 3900
Wire Wire Line
	8600 3900 8250 3900
$Comp
L Device:LED D1
U 1 1 61128197
P 8800 3250
F 0 "D1" V 8839 3132 50  0000 R CNN
F 1 "LED" V 8748 3132 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8800 3250 50  0001 C CNN
F 3 "~" H 8800 3250 50  0001 C CNN
	1    8800 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8450 3400 8800 3400
Wire Wire Line
	8800 3100 8800 3050
Wire Wire Line
	8800 3050 8600 3050
$Comp
L Device:Battery_Cell BT1
U 1 1 6112987A
P 7950 4200
F 0 "BT1" H 8068 4296 50  0000 L CNN
F 1 "Battery_Cell" H 8068 4205 50  0000 L CNN
F 2 "SulkithFootprints:SolderPad_2Pin" V 7950 4260 50  0001 C CNN
F 3 "~" V 7950 4260 50  0001 C CNN
	1    7950 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 3800 7650 3800
Wire Wire Line
	7650 3800 7650 3750
Wire Wire Line
	7400 3200 7400 3800
Wire Wire Line
	7650 3800 7650 4600
Wire Wire Line
	7650 4600 7950 4600
Wire Wire Line
	7950 4600 7950 4300
Connection ~ 7650 3800
Wire Wire Line
	7950 4000 7950 3750
$Comp
L Device:C C2
U 1 1 6112A91D
P 6750 3000
F 0 "C2" H 6865 3046 50  0000 L CNN
F 1 "47nF" H 6865 2955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6788 2850 50  0001 C CNN
F 3 "~" H 6750 3000 50  0001 C CNN
	1    6750 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:L L2
U 1 1 6112B2E2
P 6500 3000
F 0 "L2" H 6456 2954 50  0000 R CNN
F 1 "12,6µH" H 6456 3045 50  0000 R CNN
F 2 "SulkithFootprints:solderPad_small" H 6500 3000 50  0001 C CNN
F 3 "~" H 6500 3000 50  0001 C CNN
	1    6500 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	6500 2850 6500 2700
Wire Wire Line
	6500 2700 6750 2700
Wire Wire Line
	6750 2850 6750 2700
Connection ~ 6750 2700
Wire Wire Line
	6750 2700 7450 2700
Wire Wire Line
	6500 3150 6500 4600
Wire Wire Line
	6500 4600 6750 4600
Connection ~ 7650 4600
Wire Wire Line
	6750 3150 6750 4600
Connection ~ 6750 4600
Wire Wire Line
	6750 4600 7650 4600
$Comp
L Device:LED D2
U 1 1 6112D4CF
P 9200 4000
F 0 "D2" V 9239 3882 50  0000 R CNN
F 1 "LED" V 9148 3882 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9200 4000 50  0001 C CNN
F 3 "~" H 9200 4000 50  0001 C CNN
	1    9200 4000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 6112DEEF
P 9200 4400
F 0 "R7" H 9270 4446 50  0000 L CNN
F 1 "R" H 9270 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 9130 4400 50  0001 C CNN
F 3 "~" H 9200 4400 50  0001 C CNN
	1    9200 4400
	1    0    0    -1  
$EndComp
Connection ~ 8800 3050
Wire Wire Line
	9200 4600 7950 4600
Connection ~ 7950 4600
$Comp
L Connector:Conn_01x02_Female J_LED1
U 1 1 611315B5
P 9400 3650
F 0 "J_LED1" H 9428 3626 50  0000 L CNN
F 1 "Conn_01x02_Female" H 9428 3535 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical_SMD_Pin1Left" H 9400 3650 50  0001 C CNN
F 3 "~" H 9400 3650 50  0001 C CNN
	1    9400 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 4600 9200 4550
Wire Wire Line
	9200 4250 9200 4150
Wire Wire Line
	9200 3850 9200 3750
Wire Wire Line
	9200 3050 9200 3650
Wire Wire Line
	8800 3400 8800 3550
Connection ~ 8800 3400
$Comp
L Connector:Conn_01x02_Female J_Power_out1
U 1 1 61135CD9
P 9750 3050
F 0 "J_Power_out1" H 9778 3026 50  0000 L CNN
F 1 "Conn_01x02_Female" H 9778 2935 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical_SMD_Pin1Left" H 9750 3050 50  0001 C CNN
F 3 "~" H 9750 3050 50  0001 C CNN
	1    9750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 3050 9550 3050
Connection ~ 9200 3050
Wire Wire Line
	9550 3150 9550 4600
Wire Wire Line
	9550 4600 9200 4600
Connection ~ 9200 4600
$Comp
L Connector:Conn_01x01_Female J_Bat1
U 1 1 61138519
P 8350 4000
F 0 "J_Bat1" H 8378 4026 50  0000 L CNN
F 1 "Conn_01x01_Female" H 8378 3935 50  0000 L CNN
F 2 "SulkithFootprints:SolderPad" H 8350 4000 50  0001 C CNN
F 3 "~" H 8350 4000 50  0001 C CNN
	1    8350 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 4000 7950 4000
Connection ~ 7950 4000
Wire Wire Line
	8800 3050 9200 3050
$Comp
L Connector:Conn_01x01_Female J_Chg1
U 1 1 61139DD5
P 8800 3750
F 0 "J_Chg1" V 8646 3798 50  0000 L CNN
F 1 "Conn_01x01_Female" V 8737 3798 50  0000 L CNN
F 2 "SulkithFootprints:SolderPad" H 8800 3750 50  0001 C CNN
F 3 "~" H 8800 3750 50  0001 C CNN
	1    8800 3750
	0    1    1    0   
$EndComp
$Comp
L Timer:LTC6993xS6-1 U1
U 1 1 6113B239
P 4200 3150
F 0 "U1" H 4644 3196 50  0000 L CNN
F 1 "LTC6990" H 4644 3105 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TSOT-23-6" H 4200 2750 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/69931234fc.pdf" H 4250 3250 50  0001 C CNN
	1    4200 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 6113BE74
P 3700 3550
F 0 "R4" H 3770 3596 50  0000 L CNN
F 1 "R" H 3770 3505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3630 3550 50  0001 C CNN
F 3 "~" H 3700 3550 50  0001 C CNN
	1    3700 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6113C503
P 3450 3550
F 0 "R1" H 3520 3596 50  0000 L CNN
F 1 "R" H 3520 3505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3380 3550 50  0001 C CNN
F 3 "~" H 3450 3550 50  0001 C CNN
	1    3450 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 6113C9E8
P 3450 3950
F 0 "R2" H 3520 3996 50  0000 L CNN
F 1 "R" H 3520 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3380 3950 50  0001 C CNN
F 3 "~" H 3450 3950 50  0001 C CNN
	1    3450 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 6113CD8C
P 3700 3950
F 0 "R5" H 3770 3996 50  0000 L CNN
F 1 "R" H 3770 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3630 3950 50  0001 C CNN
F 3 "~" H 3700 3950 50  0001 C CNN
	1    3700 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 6113D48D
P 3450 4350
F 0 "R3" H 3520 4396 50  0000 L CNN
F 1 "R" H 3520 4305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3380 4350 50  0001 C CNN
F 3 "~" H 3450 4350 50  0001 C CNN
	1    3450 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 6113D7DE
P 3700 4350
F 0 "R6" H 3770 4396 50  0000 L CNN
F 1 "R" H 3770 4305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3630 4350 50  0001 C CNN
F 3 "~" H 3700 4350 50  0001 C CNN
	1    3700 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 3250 3700 3250
Wire Wire Line
	3450 3700 3450 3750
Wire Wire Line
	3450 3750 3700 3750
Wire Wire Line
	3700 3750 3700 3700
Connection ~ 3450 3750
Wire Wire Line
	3450 3750 3450 3800
Wire Wire Line
	3700 3800 3700 3750
Connection ~ 3700 3750
Wire Wire Line
	3450 4100 3450 4150
Wire Wire Line
	3450 4150 3700 4150
Wire Wire Line
	3700 4150 3700 4100
Connection ~ 3450 4150
Wire Wire Line
	3450 4150 3450 4200
Wire Wire Line
	3700 4200 3700 4150
Connection ~ 3700 4150
Wire Wire Line
	3450 4500 3450 4550
Wire Wire Line
	3450 4550 3700 4550
Wire Wire Line
	3700 4550 3700 4500
Wire Wire Line
	3700 3400 3700 3350
Wire Wire Line
	3450 3400 3450 3350
Wire Wire Line
	3450 3350 3700 3350
Connection ~ 3700 3350
Wire Wire Line
	3700 3350 3700 3250
Wire Wire Line
	3700 4550 3700 4600
Wire Wire Line
	3700 4600 4200 4600
Wire Wire Line
	4200 4600 4200 3450
Connection ~ 3700 4550
Wire Wire Line
	3800 3050 3350 3050
Wire Wire Line
	3350 3050 3350 4600
Wire Wire Line
	3350 4600 3700 4600
Connection ~ 3700 4600
Wire Wire Line
	3800 3150 3650 3150
$Comp
L Device:L L1
U 1 1 6114BC38
P 5450 2900
F 0 "L1" H 5503 2946 50  0000 L CNN
F 1 "L" H 5503 2855 50  0000 L CNN
F 2 "SulkithFootprints:solderPad_small" H 5450 2900 50  0001 C CNN
F 3 "~" H 5450 2900 50  0001 C CNN
	1    5450 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 6114C3F2
P 5200 2900
F 0 "C1" H 5315 2946 50  0000 L CNN
F 1 "C" H 5315 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5238 2750 50  0001 C CNN
F 3 "~" H 5200 2900 50  0001 C CNN
	1    5200 2900
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS138 Q1
U 1 1 6114D27E
P 5100 3400
F 0 "Q1" H 5304 3446 50  0000 L CNN
F 1 "BSS816" H 5304 3355 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5300 3325 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 5100 3400 50  0001 L CNN
	1    5100 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4600 5200 4600
Wire Wire Line
	5200 4600 5200 3600
Connection ~ 4200 4600
Wire Wire Line
	4600 3150 4750 3150
Wire Wire Line
	4750 3150 4750 3400
Wire Wire Line
	4750 3400 4900 3400
Wire Wire Line
	5200 3200 5200 3050
Wire Wire Line
	5200 3050 5450 3050
Connection ~ 5200 3050
Wire Wire Line
	5450 2750 5450 2700
Wire Wire Line
	5450 2700 5200 2700
Wire Wire Line
	5200 2750 5200 2700
Connection ~ 5200 2700
Wire Wire Line
	5200 2700 4200 2700
Wire Wire Line
	3650 2700 3650 3150
Wire Wire Line
	4200 2700 4200 2850
Connection ~ 4200 2700
Wire Wire Line
	4200 2700 3650 2700
$Comp
L Connector:Conn_01x01_Female J_Frq1
U 1 1 6118DF37
P 4750 3600
F 0 "J_Frq1" V 4596 3648 50  0000 L CNN
F 1 "Conn_01x01_Female" V 4687 3648 50  0000 L CNN
F 2 "SulkithFootprints:SolderPad" H 4750 3600 50  0001 C CNN
F 3 "~" H 4750 3600 50  0001 C CNN
	1    4750 3600
	0    1    1    0   
$EndComp
Connection ~ 4750 3400
$Comp
L Connector:Conn_01x02_Female J_Power_in1
U 1 1 6118EE87
P 2450 2800
F 0 "J_Power_in1" H 2342 2475 50  0000 C CNN
F 1 "Conn_01x02_Female" H 2342 2566 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical_SMD_Pin1Left" H 2450 2800 50  0001 C CNN
F 3 "~" H 2450 2800 50  0001 C CNN
	1    2450 2800
	-1   0    0    1   
$EndComp
Wire Wire Line
	2650 2700 3650 2700
Connection ~ 3650 2700
Wire Wire Line
	2650 2800 2650 4600
Wire Wire Line
	2650 4600 3350 4600
Connection ~ 3350 4600
Wire Wire Line
	5200 4600 6500 4600
Connection ~ 5200 4600
Connection ~ 6500 4600
$EndSCHEMATC
