open COORFILE, "<", $ARGV[0];
@coordinates = <COORFILE>;
close COORFILE;
$numcoor = @coordinates;

open PCBFILE, "<", $ARGV[1];
@pcbarray = <PCBFILE>;
close PCBFILE;
$numpcblines = @pcbarray;

for($i =0; $i<$numcoor;$i++)
{
	if($coordinates[$i] =~ /([A-Za-z0-9]+)\s+([0-9\.]+)\s+([0-9\.]+)\s+([0-9\.]+)/)
	{
		#print $coordinates[$i]."--$1--$2--$3--$4--\n";
		$name = $1;
		$xpos = $2;
		$ypos = $3;
		$ang  = $4;
		print "moving $name to $xpos,$ypos at angle $ang\n";
		for($j=0;$j<$numpcblines;++$j)
		{
			if($pcbarray[$j] =~ /reference $name/)
			{
				print $pcbarray[$j];
				for($k = $j; $k>($j-20); --$k)
				{
					if($pcbarray[$k] =~ /\(module /)
					{
						print $pcbarray[$k];
						print $pcbarray[$k+1];
						if($pcbarray[$k+1] =~ /(\s*\(at ).*/)
						{
							$pcbarray[$k+1] = "$1 $xpos $ypos $ang)\n";
							print $pcbarray[$k+1];
							for(++$k;$k<($j+40);++$k)
							{
								if($pcbarray[$k] =~ /\(module /)
								{
									last;
								}
								if($pcbarray[$k] =~ /\(at /)
								{
									$pcbarray[$k] =~ s/33\)/$ang\)/;
									print "$k $pcbarray[$k]";
								}
							}
							last;
						}
					}
				}
				last;
			}
		}
	}
	else
	{
		print "ERROR parsing line: \"$coordinates[$i]\"";
	}
}

open PCBOUTFILE, ">", "$ARGV[1].out";
for($j=0;$j<$numpcblines;++$j)
{
	print PCBOUTFILE $pcbarray[$j];
}
close PCBOUTFILE;
