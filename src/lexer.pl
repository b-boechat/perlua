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
    # build_token ($TYPE, $VALUE, $LINE)
    (my $type, my $value, my $line) = @_;
    my %token = (
        "type"  => $type,
        "value" => $value,
        "line" => $line
    );
    return %token;
}

sub stringify_token {
    # Converts a hash representing a token to a string version in the format: "Token {<TYPE>, <VALUE>}"
    my %token = @_;
    my $type = $token{"type"};
    my $value = $token{"value"};
    my $line = $token{"line"};
    return "Line $line- Token {type: $type; value: $value}";
}

sub scan_symbols {
    # scan_symbols (\$TEXT, $LINE)
    # This function scans $TEXT for non alphanumeric tokens, like COMMA, HIFEN, etc.
    # $TEXT is passed by reference, so that the function is able to consume the matching characters from input.
    # Returns the corresponding token if a match is found; otherwise, returns an empty hash.
    (my $text_r, my $line) = @_;
    my %symbol_names = (
        "," => "COMMA",
        "+" => "PLUS",
        "*" => "STAR",
        "|" => "VERTICAL_BAR",
        "%" => "PERCENT_SIGN",
        "#" => "HASH_SIGN",
        "&" => "AMPERSEND",
        "^" => "CIRCUMFLEX",
        ";" => "SEMICOLON",
        "(" => "PAR_OPEN",
        ")" => "PAR_CLOSE",
        "{" => "CURLY_OPEN",
        "}" => "CURLY_CLOSE",
        "/" => "SLASH",
        "-" => "HIFEN",
        ":" => "COLON",
        "//" => "DOUBLE_SLASH",
        "::" => "DOUBLE_COLON",
        "=" => "SINGLE_EQUAL",
        "~" => "TIL",
        "==" => "DOUBLE_EQUAL",
        "~=" => "NOT_EQUAL",
        "<=" => "LESS_EQUAL",
        ">=" => "GREATER_EQUAL",
        "<" => "LESS",
        ">" => "GREATER",
        "<<" => "DOUBLE_LESS",
        ">>" => "DOUBLE_GREATER",
        "[" => "SQUARE_OPEN",
        "]" => "SQUARE_CLOSE",
        "." => "DOT",
        ".." => "DOUBLE_DOT",
        "..." => "TRIPLE_DOT",
    ); # APAGAR ISSO, REGEX SPECIAL CHARACTERS INSIDE GROUPS: -[]\^$

    if (${$text_r} =~ /^([,+*|%#&\^;(){}])/ #Matches tokens: COMMA, PLUS, STAR, VERTICAL_BAR, PERCENT_SIGN, HASH_SIGN, AMPERSAND, CIRCUMFLEX, SEMICOLON, PAR_OPEN, PAR_CLOSE, CURLY_OPEN, CURLY_CLOSE
        or ${$text_r} =~ /^(\/)[^\/]/ #Matches token SLASH
        or ${$text_r} =~ /^(\-)[^\-]/ #Matches token HIFEN, COLON
        or ${$text_r} =~ /^(:)[^:]/ #Matches token COLON
        or ${$text_r} =~ /^(\:\:)/ #Matches token DOUBLE_COLON
        or ${$text_r} =~ /^(\/\/)/ #Matches token DOUBLE_SLASH
        or ${$text_r} =~ /^([=~])[^=]/ #Matches tokens: SINGLE_EQUAL, TIL
        or ${$text_r} =~ /^([=~<>]=)/ #Matches tokens: DOUBLE EQUAL, NOT_EQUAL, LESS_EQUAL, GREATER_EQUAL
        or ${$text_r} =~ /^(<)[^<]/ #Matches token LESS (supoe que o LESS_EQUAL JA FOI ACHADO)
        or ${$text_r} =~ /^(>)[^>]/ #Matches token GREATER (supoe que o GREATER_EQUAL JA FOI ACHADO)
        or ${$text_r} =~ /^(<<|>>)/ #Matches tokens DOUBLE_LESS, DOUBLE_GREATER
        or ${$text_r} =~ /^(\[)[^\[]/ #Matches token SQUARE_OPEN
        or ${$text_r} =~ /^(\])[^\]]/ #Matches token SQUARE_CLOSE
        or ${$text_r} =~ /^(\.)[^\.]/ #Matches token DOT
        or ${$text_r} =~ /^(\.\.)[^\.]/ #Matches token DOUBLE_DOT
        or ${$text_r} =~ /^(\.\.\.)/ #Matches token TRIPLE_DOT
    ){
        my $match = $1;
        my $escaped_match = quotemeta($match); #This is necessary so, in the substitution line below, special characters like / and ( are properly escaped.
        ${$text_r} =~ s/$escaped_match//; #Consumes matching characters from input
        #print("After \$\$text_r: '${$text_r}'\n\n");
        return build_token($symbol_names{$match}, "None", $line);
    } 
    return ();
}

sub scan_string {
    # scan_string (\$TEXT, $LINE)
    # This function scans $TEXT for a string token.
    # $TEXT is passed by reference, so that the function can consume the string characters (including enclosing characters) from input.
    # Returns the corresponding token if a match is found; otherwise, returns an empty hash.
    # If an invalid string is read, returns an ERROR token, which can be treated by 'tokenize_input'.
    (my $text_r, my $line) = @_;
    if (${$text_r} =~ /^(")([^"]*)"/      #String: "foo"
        or ${$text_r} =~ /^(')([^']*)'/   #String: 'foo'
        or ${$text_r} =~ /^(\[\[)([^(\]\])]*)\]\]/ #String: [[foo]]
    ){
        my $enclosing = $1;
        my $match = $2;
        return build_token("ERROR", "Unexpected newline parsing string literal.", $line) if $match =~ /\n/; #Strings can't contain actual line breaks.
        my $escaped_match = quotemeta($match);
        if ($enclosing ne "[["){
            ${$text_r} =~ s/(["'])$escaped_match\g1//; #Consumes token from input, for formats "foo" and 'foo'.
            $match =~ s/\\([rnt'"\\])/"qq|\\$1|"/gee; #Evaluate special sequences of characters '\n', '\r', and '\t'. This is not done on [[foo]] format, as per Lua specification.
        }
        else {
            ${$text_r} =~ s/\[\[$escaped_match\]\]//; #Consumes token from input, for format [[foo]].
        }
        return build_token("STRING", $match, $line);
    } 

    return ();
}

sub scan_number {
    # scan_number (\$TEXT, $LINE)
    # This function scans $TEXT for a number token.
    (my $text_r, my $line) = @_;
    if (${$text_r} =~ /^(([0-9]+\.?[0-9]*)([eE][-+]?[0-9]+)?)/){   #TA COM BUG PERMITINDO 2e SEM NENHUM NUMERO DEPOIS!
        my $match = $1;
        my $escaped_match = quotemeta($match);
        #print("Before \$text_r: '${$text_r}'\n\$match: '$match'\n"); #DEBUG TIRAR DEPOIS
        ${$text_r} =~ s/$escaped_match//; #Consumes matching characters from input 
            return build_token("NUMBER", $match+0, $line); #Perl allows for implicit string to number conversion.
    }
     return ();
}

sub scan_identifier {
    # scan_identifer (\$TEXT, $LINE)
    # This functions scans $TEXT for an keyword or identifier.
    my %keywords = (
        "and" => "KW_AND",
        "false" => "KW_FALSE",
        "local" => "KW_LOCAL",
        "then" => "KW_THEN",
        "break" => "KW_BREAK",
        "for" => "KW_FOR",
        "nil" => "KW_NIL",
        "true" => "KW_TRUE",
        "do" => "KW_DO",
        "function" => "KW_FUNCTION",
        "not" => "KW_NOT",
        "until" => "KW_UNTIL",
        "else" => "KW_ELSE",
        "goto" => "KW_GOTO",
        "or" => "KW_OR",
        "while" => "KW_WHILE",
        "elseif" => "KW_ELSEIF",
        "if" => "KW_IF",
        "repeat" => "KW_REPEAT",
        "end" => "KW_END",
        "in" => "KW_IN",
        "return" => "KW_RETURN",
    );
    (my $text_r, my $line) = @_;
    if (${$text_r} =~ /^([a-zA-Z_][0-9a-zA-Z_]*)[^0-9a-zA-Z_]/){ #Matches a valid identifier or keyword (contains only alphanumeric characters, not starting with digits).
        my $match = $1;
        ${$text_r} =~ s/$match//; #Consumes matching characters from input
        if (exists($keywords{$match})) { #Token is a keyword.
            return build_token($keywords{$match}, "None", $line);
        }
        #Token is not a keyword, so it's interpreted as an identifier.
        return build_token("IDENTIFIER", $match, $line);
    }
    return ();
}

sub skip_whitespace {
    # skip_whitespace (\$TEXT, \$LINE)
    # This function receives $TEXT, passed by reference, and consumes an arbitrary amount of leading whitespace.
    # $LINE is incremented for each character \n found.
    # Returns the amount of leading whitespace skipped. 
    (my $text_r, my $line_r) = @_;
    ${$text_r} =~ s/^(\s*)//; #Removes leading whitespace from $text.
    my $match = $1;
    ${$line_r} += ($match =~ tr/\n//); #Increments $line_r.
    return length($match);
}

sub skip_comment {
    # skip_comment (\$TEXT, \$LINE)
    # Returns 1 if at least one comment section was skipped, 0 otherwise. 
    (my $text_r, my $line_r) = @_;
    my $had_comment = 0;
    # A loop is used so sequences of comments can be properly skipped.
    #print ("Text before skipping: \'\n${$text_r}\n'\n\n");
    while (${$text_r} =~ /^(\-\-\[\[.*\]\])/s #Matches multi-line comment of the form --[[comment ]]
            or ${$text_r} =~ /^(\-\-.*)/ #Matches single-line comment of the of the form --coment
    ){
    #print ("In this?\nThen \$match = \'$1\'\n\n");
    $had_comment = 1;
    my $match = $1;
    ${$line_r} += ($match =~ tr/\n//); #Increments $line_r.
    my $escaped_match = quotemeta($match); #This is necessary so, in the substitution line below, special characters like / and ( are properly escaped.
    ${$text_r} =~ s/$escaped_match//; #Consumes matching characters from input
    }
    return $had_comment;
}

sub skip_meaningless {
    # skip_comment (\$TEXT, \$LINE)
    (my $text_r, my $line_r) = @_;
    my $skipped = 1;
    while ($skipped) {
        $skipped = skip_comment($text_r, $line_r) + skip_whitespace($text_r, $line_r);
    }
}

sub get_next_token {
    # get_next_token (\$TEXT, $LINE)
    (my $text_r, my $line) = @_;
    my %token;
    %token = scan_symbols($text_r, $line);
    return %token if %token;
    %token = scan_string($text_r, $line);
    return %token if %token;
    %token = scan_number($text_r, $line);
    return %token if %token;
    %token = scan_identifier($text_r, $line);
    return %token if %token;
    return build_token("ERROR", "Unrecognized lexeme.", $line);
}

sub tokenize_input {
    # tokenize_input ($FILENAME)
    # FUTURAMENTE ESSA FUNCAO VAI SER A INTERFACE COM C++
    my %token;
    my $line = 1;
    my $filename = shift;
    my $text = read_file($filename); #Reads source code from input file.
    $text = "$text "; #Appending a whitespace allows for some cleaner regex while never altering the functionality.
    skip_meaningless(\$text, \$line);
    while (length($text)) { #Loops while there are characthers left in the string.
        %token = get_next_token(\$text, $line);
        parser_error($filename, $token{"line"}, $token{"value"}) if $token{"type"} eq "ERROR";
        print (stringify_token(%token), "\n"); #futuramente, trocar esse print e o de baixo por insert numa lista de tokens, que tem que ser retornada para o C++
        skip_meaningless(\$text, \$line);
    }
    %token = build_token("EOS", "None", $line);
    print (stringify_token(%token), "\n");
    print ("\nScanning of '$filename' sucessfully completed.\n");
}

############################# EVERYTHING BELOW IS JUST FOR TESTING, NOT INCLUDED IN FINAL CODE


my $TESTING = 0;
if (!$TESTING) {
    my $_FILENAME = shift @ARGV;
    if (!$_FILENAME){
        $_FILENAME = "input.txt";
    }
    print_text(read_file($_FILENAME));
    tokenize_input($_FILENAME);
}
else {
    my $_FILENAME = shift @ARGV;
    print_text(read_file($_FILENAME));
    tokenize_input($_FILENAME);
    # Do some test
}

sub print_text {
    #Deletar essa funcao depois, é só uma helper
    my $text = shift @_;
    print ("\"$text\"\n\n");
}
