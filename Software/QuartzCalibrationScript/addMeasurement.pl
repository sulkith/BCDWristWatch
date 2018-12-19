use Time::Local;
print "Press any key, when the minute counts 1 up\n";
<STDIN>;
($sec2,$min2,$hour2,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
print "Which Time is now shown [hh-mm]\n";
$timeWatch = <STDIN>;
if($timeWatch =~ /(\d\d)[: -](\d\d)/)
{
	print "Clock shows $1:$2\n";
	$timeWatch= "$1-$2";
	$hour=$1;
	$minute=$2;
}

($sec2,$min2,$hour2,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
$epocClock = timelocal(0,$minute,$hour,$mday,$mon,1900+$year);
$epoc = timelocal($sec2,$min2,$hour2,$mday,$mon,1900+$year);

$difference=$epoc-$epocClock;


open FILE,">>$ARGV[0]" || die("This file will not open!");
print FILE "$year-$mon-$mday-$timeWatch-$epocClock-$epoc-$difference\n";
print "$year-$mon-$mday-$timeWatch-$epocClock-$epoc-$difference\n";
close FILE;
