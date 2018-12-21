# BCDWristWatch
## First PCB Version
The First PCB Version was a desaster. The PCBs arrived very fast, but the QFN Package had the wrong size.<br>
ATMEL only produces QFN32 in 7x7mm. Therefore the design of the PCB hat to be changed to the smaller package,<br> 
but that gave me the Chance to shrink the Design to a smaller form factor.<br>
The part which keeps the PCB from getting smaller is the Coin Cell Battery (CR2032).<br>
On the PCBs there was the order number printed on the Front side, which would be visble in the final assembly.<br>
## Second PCB Version
The second PCB Version was designed from scratch. But the Front was the side with the Battery, so hopefully the Order Number would not be printed on the visible side of the PCB. This worked out very well for the second Version of the PCBs.
### PCB1
since the PCBs finally arrived. it was possible to assemble all the parts.<br>
The first PCB was assembled with some Cables soldered to the Testpins on the Backside of the Board.<br>
And the Battery case was also not assembled.<br>
Here is a Picture of the uncleaned but already soldered PCB<br>
![picture of PCB1](Pictures/PCB1_F1.jpg)

### PCB2
The PCB2 was assembled and cleaned as a First Prototype with the Housing. <br>
Here aretwo Pictures of the PCB one without the LEDs turned on and one with the LEDs turned on. <br>
Brightness was set in the Program to 100/255 (see software void showLEDs, constant perc=100) 
![picture of PCB2 switched off](Pictures/PCB2_F1_off.jpg)
![picture of PCB2 switched on](Pictures/PCB2_F1_on.jpg)

## Housing
The Housing was designed in OpenSCAD. <br>
a very helpfull reference for the OpenSCAD syntax was<br>
https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Transformations

### Housing 1
The Housing was printed by the TOOM Printing Service as SLS in PLA. <br>
Sadly all the surfaces are not really smooth, therefore the Buttons are working very bad.<br>

### Plans for Housing 2
The next housing should be produced via SLA with transparent Resin, so no glass for Protection is needed, 
since the resin could be used.
