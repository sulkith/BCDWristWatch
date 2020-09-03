#!/bin/perl
open COORFILE, "<", $ARGV[0];
@coordinates = <COORFILE>;
close COORFILE;
$numcoor = @coordinates;

print '<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" width="310" height="310" viewBox="0 0 310 310">
<g id="PCB">
<circle cx="155" cy="155" r="155" stroke="green" stroke-width="0" fill="green" />
</g>
<g id="LEDs">\n';

for($i =0; $i<$numcoor;$i++)
{
	if($coordinates[$i] =~ /([A-Za-z0-9#*_]+)\s+([0-9\.]+)\s+([0-9\.]+)\s+([0-9\.]+)/)
	{
		#print $coordinates[$i]."--$1--$2--$3--$4--\n";
		$name = $1;
		$xpos = $2;
		$xpos = $xpos*10;
		$ypos = $3;
		$ypos = $ypos*10;
		$ang  = $4;
		$ang = 90-$ang;
		$color = "30%,30%,30%";
		if (index($name, "#") != -1) {
			$color = "100%,0%,0%";
		}
		if (index($name, "*") != -1) {
			$color = "100%,0%,0%";
		}
		print "<rect x=\"-5\" y=\"-10\" width=\"10\" height=\"20\" style=\"fill:rgb($color);fill-opacity:1;stroke:none;\" transform=\"translate($xpos,$ypos) rotate($ang)\"/>\n";
		if (index($name, "*") != -1) {
			print "<rect x=\"-5\" y=\"-10\" width=\"10\" height=\"20\" style=\"fill:rgb(100%,80%,80%);fill-opacity:1;stroke:none;\" transform=\"translate($xpos,$ypos) rotate($ang)\">
			<animate attributeName=\"opacity\" values=\"0;1;0\" dur=\"0.5s\" repeatCount=\"indefinite\" /></rect>
			\n";
		}
	}
	else
	{
		#print "ERROR parsing line: \"$coordinates[$i]\"";
	}
}

print '
<rect x="-25" y="-25" width="50" height="50" style="fill:rgb(0%,0%,0%);fill-opacity:1;stroke:none;" transform="translate(155,155)rotate(45)"/>
<rect x="-10" y="-10" width="20" height="20" style="fill:rgb(0%,0%,0%);fill-opacity:1;stroke:none;" transform="translate(115,195)rotate(0)"/>

</g>
</svg>';

#<rect x="-7.5" y="-15" width="15" height="30" style="fill:rgb(30%,30%,30%);fill-opacity:1;stroke:none;" transform="translate(175,190)rotate(45)"/>
#<line x1="155" y1="0" x2="155" y2="310" style="stroke:rgb(255,0,0);stroke-width:2" />
