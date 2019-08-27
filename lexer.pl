use strict;
use warnings;

sub read_file {
    # read_file ($FILENAME)
    # FILENAME is passed as a string. This function returns the text content of the file.
    my $filename = shift @_;
    my $char_limit = 100000; #Maximum file length supported.
    my $text;
    open (my $file, "<", $filename)
        or die "$0: can't open $filename for reading: $!";
    if (!read ($file, $text, $char_limit) and $!) {
        die "$0: error reading file: $!";
    }
    close ($file)
        or die "$0: can't close $filename for reading: $!";
    return $text;
}

sub parser_error {
    # parser_error ($FILENAME, $LINE, $ERROR_MESSAGE)
    # Prints error message and stops source parsing.
    (my $filename, my $line, my $error_message) = @_;
    print ("{File '$filename', line $line} PARSER ERROR: $error_message");
    exit 1; #Stops parser.
}

sub build_token {
    # build_token ($TYPE, $VALUE)
    (my $type, my $value) = @_;
    my %token = (
        "type"  => $type,
        "value" => $value
    );
    return %token;
}

sub stringify_token {
    # Converts a hash representing a token to a string version in the format: "Token {<TYPE>, <VALUE>}"
    my %token = @_;
    my $type = $token{"type"};
    my $value = $token{"value"};
    return "Token {type: $type; value: $value}";
}

sub scan_symbols {
    # scan_symbols (\$TEXT)
    # This function scans $TEXT for non alphanumeric tokens, like COMMA, HIFEN, etc.
    # $TEXT is passed by reference, so that the function is able to consume the matching characters from input.
    # Returns the corresponding token if a match is found; otherwise, returns an empty hash.
    my $text_r = shift;
    my %symbol_names = (
        "," => "COMMA",
        "+" => "PLUS",
        "*" => "STAR",
        "/" => "SLASH",
        "\\" => "BACKSLASH",
        "-" => "HIFEN",
        "(" => "PAR_OPEN",
        ")" => "PAR_CLOSE",
        "{" => "CURLY_OPEN",
        "}" => "CURLY_CLOSE",
        "=" => "SINGLE_EQUAL",
        "<" => "LESSER",
        ">" => "GREATER",
        "==" => "DOUBLE_EQUAL",
        "<=" => "LESSER_THAN",
        ">=" => "GREATER_THAN",
        "[" => "SQUARE_OPEN",
        "]" => "SQUARE_CLOSE",
    );
    # APAGAR ISSO, SPECIAL CHARACTERS INSIDE GROUPS: -[]\^$

    if (${$text_r} =~ /^([,+*\-(){}])/ #Matches tokens: COMMA, PLUS, STAR, SLASH, BACKSLASH, HIFEN, PAR_OPEN, PAR_CLOSE, CURLY_OPEN, CURLY_CLOSE
        or ${$text_r} =~ /^([=<>])[^=]/ #Matches tokens: SINGLE_EQUAL, LESSER, GREATER
        or ${$text_r} =~ /^([=<>][=])/ #Matches tokens: DOUBLE EQUAL, LESSER_THAN, GREATER_THAN
        or ${$text_r} =~ /^(\[)[^\[]/ #Matches token SQUARE_OPEN
        or ${$text_r} =~ /^(\])[^\]]/ #Matches token SQUARE_CLOSE
    ){
        my $match = $1;
        my $escaped_match = quotemeta($match); #This is necessary so, in the substitution line below, special characters like / and ( are properly escaped.
        #print("Before \$\$text_r: '${$text_r}'\n\$match: '$match'\n\$escaped_match: '$escaped_match'\n");
        ${$text_r} =~ s/$escaped_match//; #Consumes matching characters from input
        #print("After \$\$text_r: '${$text_r}'\n\n");
        return build_token($symbol_names{$match}, "None");
    } 
    return ();
}

sub scan_string {
    # scan_string (\$TEXT)
    # This function scans $TEXT for a string token.
    # $TEXT is passed by reference, so that the function can consume the string characters (including enclosing characters) from input.
    # Returns the corresponding token if a match is found; otherwise, returns an empty hash.
    # If an invalid string is read, returns an ERROR token, which can be treated by 'tokenize_input'.
    my $text_r = shift;
    if (${$text_r} =~ /^(")([^"]*)"/      #String: "foo"
        or ${$text_r} =~ /^(')([^']*)'/   #String: 'foo'
        or ${$text_r} =~ /^(\[\[)([^(\]\])]*)\]\]/ #String: [[foo]]
    ){
        my $enclosing = $1;
        my $match = $2;
        return build_token ("ERROR", "Unexpected newline parsing string literal.") if $match =~ /\n/; #Strings can't contain actual line breaks.
        my $escaped_match = quotemeta($match);
        if ($enclosing ne "[["){
            ${$text_r} =~ s/(["'])$escaped_match\g1//; #Consumes token from input, for formats "foo" and 'foo'.
            $match =~ s/\\([rnt'"\\])/"qq|\\$1|"/gee; #Evaluate special sequences of characters '\n', '\r', and '\t'. This is not done on [[foo]] format, as per Lua specification.
        }
        else {
            ${$text_r} =~ s/\[\[$escaped_match\]\]//; #Consumes token from input, for format [[foo]].
        }
        return build_token("STRING", $match);
    } 

    return ();
}

sub scan_number {
    # scan_number (\$TEXT)
    # This function scans $TEXT for a number token.
    my $text_r = shift;
}


sub skip_whitespace {
    # scan_symbols (\$TEXT, \$LINE)
    # This function receives $TEXT, passed by reference, and consumes an arbitrary amount of leading whitespace.
    # $LINE is incremented for each character \n found.
    (my $text_r, my $line_r) = @_;
    ${$text_r} =~ s/^(\s*)//; #Removes leading whitespace from $text.
    ${$line_r} += ($1 =~ tr/\n//); #Increments $line_r.
}

sub get_next_token {
    # get_next_token (\$TEXT)
    my $text_r = shift;
    my %token;
    %token = scan_symbols($text_r);
    return %token if %token;
    %token = scan_string($text_r);
    return %token if %token;
    return build_token("ERROR", "Unrecognized lexeme.");
}

sub tokenize_input {
    # tokenize_input ($FILENAME)
    # FUTURAMENTE ESSA FUNCAO VAI SER A INTERFACE COM C++
    my %token;
    my $line = 1;
    my $filename = shift;
    my $text = read_file($filename); #Reads source code from input file.
    $text = "$text "; #Appending a whitespace allows for some cleaner regex while never altering the functionality.
    skip_whitespace(\$text, \$line);
    while (length($text)) { #Loops while there are characthers left in the string.
        %token = get_next_token(\$text);
        parser_error($filename, $line, $token{"value"}) if $token{"type"} eq "ERROR";
        print ("Line ", $line, "- ", stringify_token(%token), "\n"); #futuramente, trocar esse print e o de baixo por insert numa lista de tokens, que tem que ser retornada para o C++
        skip_whitespace(\$text, \$line);
    }
    %token = build_token("EOS", "None");
    print ("Line ", $line, "- ", stringify_token(%token), "\n");
}

my $_FILENAME = "input.txt";
my $TESTING = 0;
if (!$TESTING) {
    print_text(read_file($_FILENAME));
    tokenize_input($_FILENAME);
}
else {
    # Do some test
}

sub print_text {
    #Deletar essa funcao depois, é só uma helper
    my $output = shift @_;
    print "\n\"\n", $output, "\"\n\n";
}
