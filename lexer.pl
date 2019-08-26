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
    # This function scans for non alphanumeric tokens, like COMMA, HIFEN, etc.
    # $TEXT is passed by reference, so that the function is able to consume the matching characters from input.
    # Returns the corresponding token if a match is found, otherwise returns an empty token, which can be used in boolean conditions.
    my $text = shift;
    my %symbol_names = (
        "," => "COMMA",
        "+" => "PLUS",
        "*" => "STAR",
        "/" => "SLASH",
        "\\" => "BACKSLASH",
        "-" => "HIFEN",
        '"' => "QUOTE",
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
        "[[" => "SQ_DOUBLE_OPEN",
        "]]" => "SQ_DOUBLE_CLOSE" 
    );
    # APAGAR ISSO, SPECIAL CHARACTERS INSIDE GROUPS: -[]\^$

    if (${$text} =~ /^([,+*\-"(){}])/ #Matches tokens: COMMA, PLUS, STAR, SLASH, BACKSLASH, HIFEN, QUOTE, PAR_OPEN, PAR_CLOSE, CURLY_OPEN, CURLY_CLOSE
        or ${$text} =~ /^([=<>])[^=]/ #Matches tokens: SINGLE_EQUAL, LESSER, GREATER
        or ${$text} =~ /^([=<>][=])/ #Matches tokens: DOUBLE EQUAL, LESSER_THAN, GREATER_THAN
        or ${$text} =~ /^(\[)[^\[]/ #Matches token SQUARE_OPEN
        or ${$text} =~ /^(\])[^\]]/ #Matches token SQUARE_CLOSE
        or ${$text} =~ /^((\[\[)|(\]\]))/ #Matches tokens: SQUARE_DOUBLE_OPEN, SQUARE_DOUBLE_CLOSE
    ){
        my $match = $1;
        my $escaped_match = quotemeta($match); #This is necessary so, in the substitution line below, special characters like / and ( are properly escaped.
        #print("Before \$\$text: '${$text}'\n\$match: '$match'\n\$escaped_match: '$escaped_match'\n");
        ${$text} =~ s/$escaped_match//; #Consumes matching characters from input
        #print("After \$\$text: '${$text}'\n\n");
        return build_token($symbol_names{$match}, "None");
    } 
    return ();
}

sub scan_string {


}


sub skip_whitespace {
    # scan_symbols (\$TEXT)
    # This function receives a string, passed by reference, and consumes an arbitrary amount of leading whitespace.
    my $text = shift;
    ${$text} =~ s/^\s*//;
}

sub get_next_token {
    # get_next_token (\$TEXT)
    my $text_r = shift;
    my %token;
    %token = scan_symbols($text_r);
    return %token if %token;
    die "Deu ruim no input!";
}

sub tokenize_input {
    # tokenize_input ($FILENAME)
    # FUTURAMENTE ESSA FUNCAO VAI SER A INTERFACE COM C++
    my %token;
    my $filename = shift;
    my $text = read_file($filename); #Reads source code from input file.
    $text = "$text "; #Appending a whitespace allows for some cleaner regex while never altering the functionality.
    skip_whitespace(\$text);
    while (length($text)) { #Loops while there are characthers left in the string.
        %token = get_next_token(\$text);
        print (stringify_token(%token), "\n"); #futuramente, trocar esse print e o de baixo por insert numa lista de tokens, que tem que ser retornada para o C++
        skip_whitespace(\$text);
    }
    %token = build_token("EOS", "None");
    print (stringify_token(%token), "\n");
}

my $filename = "input.txt";

print_text(read_file($filename));
tokenize_input($filename);


sub print_text {
    #Deletar essa funcao depois, é só uma helper
    my $output = shift @_;
    print "\n\"\n", $output, "\"\n\n";
}
