open FILE, "<$ARGV[0]" or die "couldn´t open File";

@lines = <FILE>;
$numLines = @lines;
#					y     m     d     h    min   ew    epc   diff     
#					1		2		3		4		5		6		7		8
if($lines[0] =~ /(\d+)-(\d+)-(\d+)-(\d+)-(\d+)-(\d+)-(\d+)-(\d+)/)
#if($line =~ /([0-9]*)\-([0-9]*)/)
#if($lines[0]=~ /(\d*)/)
{
	my @elem;
	#print "first Measurement $3 $2 $4:$5 Difference was $8\n";
	$basediff = $8;
	$baseepoch = $7;
	for($i=1;$i<$numLines;$i++)
	{
		if($lines[$i] =~ /(\d+)-(\d+)-(\d+)-(\d+)-(\d+)-(\d+)-(\d+)-(\d+)/)
		{
			#print "$i Measurement $3 $2 $4:$5 Difference was $8\n";
			$eleapsed = $7-$baseepoch;
			$diff = $8-$basediff;
			$arr[$i][4] = $7-$baseepoch;
			$arr[$i][5] = $8-$basediff;
			#print "after $eleapsed s there was a difference of $diff s\n";
			$min_correction = 0;
			$hour_correction = 0;
			$day_correction = 0;
			$eleapsed_minutes = ($eleapsed/60);
			$eleapsed_hours = ($eleapsed/(60*60));
			$eleapsed_days = ($eleapsed/(24*60*60));
			$eleapsed_months = ($eleapsed/(30*24*60*60));
			#print "eleapsed: $eleapsed_days d $eleapsed_hours h $eleapsed_minutes m\n";
			if($eleapsed_minutes<$diff)
			{
				#correction every minute needed
				$min_correction = int($diff/$eleapsed_minutes);
				$diff -= $min_correction*$eleapsed_minutes;
			}
			if($eleapsed_hours<$diff)
			{
				#correction every hour needed
				$hour_correction = int($diff/$eleapsed_hours);
				$diff -= $hour_correction*$eleapsed_hours;
			}
			if($eleapsed_days<$diff)
			{
				#correction every hour needed
				$day_correction = int($diff/$eleapsed_days);
				$diff -= $day_correction*$eleapsed_days;
			}
			if($eleapsed_months<$diff)
			{
				#correction every month needed
				$month_correction = int($diff/$eleapsed_months);
				$diff -= $month_correction*$eleapsed_months;
			}
			$diff_per_day = $diff/($eleapsed/(24*60*60));
			print "correction_t EEMEM correction_eep={$min_correction,$hour_correction,$day_correction/*,$month_correction*/}; //remaining Difference per Day: $diff_per_day\n";
			$arr[$i][0] = $min_correction;
			$arr[$i][1] = $hour_correction;
			$arr[$i][2] = $day_correction;
			$arr[$i][3] = $month_correction;
#			$arr[$i][4] = $7-$baseepoch;
#			$arr[$i][5] = $8-$basediff;
		}
	}
	$bestresult = "";
	$bestdiff = 9999999;
	$bestdiffs = 9999999;
	$bestdiffabs = 9999999;
	for($i=1;$i<$numLines;$i++)
	{	
		$sumdiff = 0;
		$min_correction = $arr[$i][0];
		$hour_correction = $arr[$i][1];
		$day_correction = $arr[$i][2];
		$month_correction = $arr[$i][3];
		for($j=1;$j<$numLines;$j++)
		{
			$eleapsed = $arr[$j][4];
			$diff = $arr[$j][5];
			$eleapsed_minutes = ($eleapsed/60);
			$eleapsed_hours = ($eleapsed/(60*60));
			$eleapsed_days = ($eleapsed/(24*60*60));
			$eleapsed_months = ($eleapsed/(30*24*60*60));
			$diff -= $min_correction*$eleapsed_minutes;
			$diff -= $hour_correction*$eleapsed_hours;
			$diff -= $day_correction*$eleapsed_days;
			$diff -= $month_correction*$eleapsed_months;
			$diff_per_day = $diff/($eleapsed_days);
			$sumdiff += abs($diff);
			$sumdiffsigned += $diff;
			print "difference of {$min_correction,$hour_correction,$day_correction/*,$month_correction*/} on Measurement $j $diff\n";
		}
		$avgdiff = $sumdiff/($numLines-1);
		$avgdiffsigned = $sumdiffsigned/($numLines-1);
		print "Average difference of {$min_correction,$hour_correction,$day_correction/*,$month_correction*/} $avgdiffsigned, absolute difference:$avgdiff\n";
		if(abs($avgdiffsigned)<$bestdiff)
		{
			$bestresult = "correction_t EEMEM correction_eep={$min_correction,$hour_correction,$day_correction/*,$month_correction*/}; //remaining Difference per Day: $avgdiffsigned\n";
			$bestdiff = abs($avgdiffsigned);
			$bestdiffs = $avgdiffsigned;
			$bestdiffabs = $avgdiff;
		}
	}
	print "#################\n";
	print "## BEST RESULT ##\n";
	print "#################\n\n";
	print "Average Difference per Day: $bestdiffs\n";
	print "Absolute Average Difference per Day: $bestdiffabs\n";
	print "$bestresult\n"
}
