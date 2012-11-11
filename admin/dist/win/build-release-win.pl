#!/usr/bin/perl
#
# brief: This script builds a release build from scratch and packages it
#        into an installer. You need %QTDIR% and %VSDIR% set in your environment
#
#        Must be run from trunk.
#
# pre-requisites: perl, svn, tar, bzip2, 
#                 Inno Setup 5 (install to default location or set %ISDIR%)
#

use File::Path;
use File::Copy;
use File::Find;


########################################################################

# copy the input file to a temp location and use that instead
copy('Last.fm.iss', 'Last.fm.tmp.iss') or die;
my $ISSFILE = 'Last.fm.tmp.iss';

my $QTDIR = quotemeta( $ENV{'QTDIR'} or die $! );
my $VSDIR = quotemeta( $ENV{'VSDIR'} or die $! );
#$QTDIR =~ s/\\/\\\\/g; #double escape \s for the shell :(
#$VSDIR =~ s/\\/\\\\/g;

########################################################################
header( "Building release and installer for Last.fm-$VERSION" );

########################################################################

header( "Substituting strings in various files" );
    #sub findVersionFiles()
    #{
    #    if ($_ =~ /\.rc$/i || $_ =~ /\.manifest$/i || $_ =~ /\.iss$/i)
    #    {
    #        push( @versionFiles, $File::Find::name );
    #    }
    #}
	#find( \&findVersionFiles, "." );
	
	push( @versionFiles, $ISSFILE );

    system( 'perl -pi".bak" -e "s/\%QTDIR\%/' . $QTDIR . '/g" ' . $ISSFILE );
    system( 'perl -pi".bak" -e "s/\%VSDIR\%/' . $VSDIR . '/g" ' . $ISSFILE );

header( "Translations" );
    system( "lrelease  -removeidentical ../../../Last.fm.pro" );

header( "Installer" );
    #my $ISDIR = $ENV{'ISDIR'} or "c:\\Program Files\\Inno Setup 5";
    #$ISDIR =~ s/\\/\//g;
    #run( "$ISDIR\\iscc.exe", "$ISSFILE" ) or die $!;
	
	my $output = qx/"c:\/Program Files (x86)\/Inno Setup 5\/iscc.exe" $ISSFILE/  or die $!;
	print $output;
	my @lines = split(/\n/, $output);
	my $installer = @lines[-1..-1];
	print $installer;

#header( "Building symbolstore" );
#    dumpSyms( "bin" );
#    dumpSyms( "$ENV{QTDIR}/lib" );
#    chdir( "build/syms" );
#    run( "tar", "cjf", "../../dist/Last.fm-$VERSION-win.symbols.tar.bz2", "." );

header( "Signing" );
	system( 'signtool sign /a /d "Last.fm Scrobbler" /du http://www.last.fm/download ' . $installer );

	
header( "done!" );
    print "To upload the symbols, issue the following command:\n";
    print "       perl dist\\build-release-win.pl --upload";



########################################################################
sub header
{
    print "\n==> $_[0]\n";
}

sub run
{
    return system( @_ ) == 0;
}
########################################################################
