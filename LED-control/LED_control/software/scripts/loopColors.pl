#/usr/bin/env perl -w

use strict;

my $r = 0;
my $g = 0;
my $b = 0;

while(1){
    if($r){
        if($g){
            if($b){$b++}
        } else  
    }
    print "Colors (rgb): $r $g $b\n";
    system "python varcolor.py",$r,$g,$b;
}
