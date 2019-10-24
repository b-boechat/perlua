use lualexer;
use strict;
use warnings;

my $FILENAME = shift @ARGV;
if (lualexer::tokenize_input($FILENAME) == 2) {
    print ("Usage: perl $0 <source_code_file>\n");
};
