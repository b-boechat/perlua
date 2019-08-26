use strict;
use warnings;

# Essa parte do codigo vai ser modificada quando tiver o API.
my $filename = shift @ARGV;
my $char_limit = 100000;


sub read_file {
    # read_file ($FILENAME)
    # FILENAME is passed as a string. This function returns the text content of the file.
    my $filename = shift @_;
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
        print("Before \$\$text: '${$text}'\n\$match: '$match'\n\$escaped_match: '$escaped_match'\n");
        ${$text} =~ s/$escaped_match//; #Consumes matching characters from input
        print("After \$\$text: '${$text}'\n\n");
        return build_token($symbol_names{$match}, "None");
    } 
    return build_token("EOS", "None"); #MODIFICAR ISSO, EH PRA RETORNAR FALSE OU NIL PRA PODER USAR COMO BOOLEAN
}

my $text_test = "]]]";
my $text_corr = "$text_test ";
my %token = scan_symbols(\$text_corr);
print (stringify_token(%token), "\n");
while ($token{"type"} ne "EOS") {
    %token = scan_symbols(\$text_corr);
    print (stringify_token(%token), "\n");
}


sub print_text {
    #Deletar essa funcao depois, é só uma helper
    my $output = shift @_;
    print "\n\"\n", $output, "\"\n";
}
