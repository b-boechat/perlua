package lualexer;

use 5.026001;
use strict;
use warnings;
use Carp;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use lualexer ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = qw (
    return_two
    read_file
    parser_error
    build_token
    stringify_token
    append_token
    scan_symbols
    scan_string
    scan_number
    scan_identifier
    skip_whitespace
    skip_comment
    skip_meaningless
    get_next_token
    tokenize_input
);

our @EXPORT = qw(
	
);

# Lexer functions below ==========

sub return_two {
    print 2;
    return 2;
}


sub read_file {
    # read_file ($FILENAME)
    # FILENAME is passed as a string. This function returns the text content of the file.
    my $filename = shift @_;
    my $char_limit = 100000000; #Maximum file length supported.
    my $text;
    open (my $file, "<", $filename)
        or die "Fatal Error: Can't open $filename for reading: $!\n";
    if (!read ($file, $text, $char_limit) and $!) {
        die "Fatal Error: Error reading file: $!\n";
    }
    close ($file)
        or die "Fatal Error: can't close $filename for reading: $!\n";
    return $text;
}

sub parser_error {
    # parser_error ($FILENAME, $LINE, $ERROR_MESSAGE)
    # Prints error message and stops source parsing.
    (my $filename, my $line, my $error_message) = @_;
    print ("{File '$filename', line $line} Parser Error: $error_message\n");
    return "1";
}

sub build_token {
    # build_token ($TYPE, $VALUE, $LINE)
    # Returns a hash table representing a language token with the given parameters type, value and line.
    (my $type, my $value, my $line) = @_;
    my %token = (
        "type"  => $type,
        "value" => $value,
        "line" => $line
    );
    return %token;
}

#REMOVER FUTURAMENTE
sub stringify_token {
    # stringify_token (%TOKEN)
    # Converts a hash representing a token to a string version in the format: "Token {<TYPE>, <VALUE>}"
    my %token = @_;
    my $type = $token{"type"};
    my $value = $token{"value"};
    my $line = $token{"line"};
    return "Line $line- Token {type: $type; value: $value}";
}

sub append_token {
    # codify_token ($CODIFIED_TOKENS, \%TOKEN)
    # DOC DOC DOC
    (my $c_tokens, my $token_r) = @_;
    my $type = $token_r->{type};
    my $value = $token_r->{value};
    my $line = $token_r->{line};
    $c_tokens = $c_tokens.$type."\003".$value."\003".$line."\003";
    $c_tokens = $c_tokens."\0" if $type eq "EOS";
    return $c_tokens;
}



sub scan_symbols {
    # scan_symbols (\$TEXT, $LINE)
    # This function scans $TEXT for non alphanumeric tokens, like COMMA, HIFEN, etc. If one is found, it is consumed and returned.
    (my $text_r, my $line) = @_;
    # Here all symbol tokens are defined.
    my %symbol_names = (
        "," => "COMMA",
        "+" => "PLUS",
        "*" => "STAR",
        "|" => "VERTICAL_BAR",
        "%" => "PERCENT",
        "#" => "HASH",
        "&" => "AMPERSEND",
        "^" => "CIRCUMFLEX",
        ";" => "SEMICOLON",
        "(" => "PAR_OPEN",
        ")" => "PAR_CLOSE",
        "{" => "CURLY_OPEN",
        "}" => "CURLY_CLOSE",
        "/" => "SLASH",
        "-" => "MINUS",
        ":" => "COLON",
        "//" => "DOUBLE_SLASH",
        "::" => "DOUBLE_COLON",
        "=" => "SINGLE_EQUAL",
        "~" => "TIL",
        "==" => "DOUBLE_EQUAL",
        "~=" => "NOT_EQUAL",
        "<=" => "LESSER_EQUAL",
        ">=" => "GREATER_EQUAL",
        "<" => "LESSER",
        ">" => "GREATER",
        "<<" => "DOUBLE_LESSER",
        ">>" => "DOUBLE_GREATER",
        "[" => "SQUARE_OPEN",
        "]" => "SQUARE_CLOSE",
        "." => "DOT",
        ".." => "DOUBLE_DOT",
        "..." => "TRIPLE_DOT",
    ); 
    if (${$text_r} =~ /^([,+*|%#&\^;(){}])/ #Matches tokens: COMMA, PLUS, STAR, VERTICAL_BAR, PERCENT, HASH, AMPERSEND, CIRCUMFLEX, SEMICOLON, PAR_OPEN, PAR_CLOSE, CURLY_OPEN, CURLY_CLOSE
        or ${$text_r} =~ /^(\/)[^\/]/ #Matches token SLASH
        or ${$text_r} =~ /^(\-)[^\-]/ #Matches token MINUS
        or ${$text_r} =~ /^(:)[^:]/ #Matches token COLON
        or ${$text_r} =~ /^(\:\:)/ #Matches token DOUBLE_COLON
        or ${$text_r} =~ /^(\/\/)/ #Matches token DOUBLE_SLASH
        or ${$text_r} =~ /^([=~])[^=]/ #Matches tokens: SINGLE_EQUAL, TIL
        or ${$text_r} =~ /^([=~<>]=)/ #Matches tokens: DOUBLE EQUAL, NOT_EQUAL, LESSER_EQUAL, GREATER_EQUAL
        or ${$text_r} =~ /^(<)[^<]/ #Matches token LESSER (supoe que o LESSER_EQUAL JA FOI ACHADO)
        or ${$text_r} =~ /^(>)[^>]/ #Matches token GREATER (supoe que o GREATER_EQUAL JA FOI ACHADO)
        or ${$text_r} =~ /^(<<|>>)/ #Matches tokens DOUBLE_LESSER, DOUBLE_GREATER
        or ${$text_r} =~ /^(\[)[^\[]/ #Matches token SQUARE_OPEN
        or ${$text_r} =~ /^(\])[^\]]/ #Matches token SQUARE_CLOSE
        or ${$text_r} =~ /^(\.)[^\.]/ #Matches token DOT
        or ${$text_r} =~ /^(\.\.)[^\.]/ #Matches token DOUBLE_DOT
        or ${$text_r} =~ /^(\.\.\.)/ #Matches token TRIPLE_DOT
    ){
        my $match = $1;
        #This is necessary so, in the substitution line below, special characters like / and ( are properly escaped.
        my $escaped_match = quotemeta($match);
        ${$text_r} =~ s/$escaped_match//;
        return build_token($symbol_names{$match}, "None", $line);
    } 
    #If no match is found, returns an empty list. This can be treated by the calling function.
    return ();
}

sub scan_string {
    # scan_string (\$TEXT, $LINE)
    # This function scans $TEXT for a STRING token. If one is found, it is consumed and returned.
    (my $text_r, my $line) = @_;
    if (${$text_r} =~ /^(")([^"]*)"/      #String: "foo"
        or ${$text_r} =~ /^(')([^']*)'/   #String: 'foo'
        or ${$text_r} =~ /^(\[\[)([^(\]\])]*)\]\]/ #String: [[foo]]
    ){
        my $enclosing = $1;
        my $match = $2;
        #Multi-line strings (with actual line breaks instead of control sequence) are not supported.
        #If a line break is found, returns an ERROR token, which can be treated by the calling function.
        return build_token("ERROR", "Unexpected newline parsing string literal.", $line) if $match =~ /\n/;
        my $escaped_match = quotemeta($match);
        if ($enclosing ne "[["){
            #Consumes token from input, for formats "foo" and 'foo'.
            ${$text_r} =~ s/(["'])$escaped_match\g1//;
            #Evaluates special sequences of characters '\n', '\r', and '\t'. This is not done on [[foo]] format, as per Lua specification.
            $match =~ s/\\([rnt'"\\])/"qq|\\$1|"/gee; 
        }
        else {
            #Consumes token from input, for format [[foo]].
            ${$text_r} =~ s/\[\[$escaped_match\]\]//; 
        }
        return build_token("STRING", $match, $line);
    } 
    #If no match is found, returns an empty list. This can be treated by the calling function.
    return ();
}

sub scan_number {
    # scan_number (\$TEXT, $LINE)
    # This function scans $TEXT for a number token. If one is found, it is consumed and returned.
    (my $text_r, my $line) = @_;
    if (${$text_r} =~ /^(([0-9]+\.?[0-9]*)([eE][-+]?[0-9]+)?)/){
        my $match = $1;
        ${$text_r} =~ s/$match//; 
            return build_token("NUMBER", $match+0, $line); #MATCH is being implicitly converted to a number.
    }
    #If no match is found, returns an empty list. This can be treated by the calling function.
    return ();
}

sub scan_identifier {
    # scan_identifer (\$TEXT, $LINE)
    # This functions scans $TEXT for an keyword or identifier token. If one is found, it is consumed and returned.
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
    #Matches a valid identifier (possibly a keyword), which can contain only alphanumeric characters, not starting with a digit.
    if (${$text_r} =~ /^([a-zA-Z_][0-9a-zA-Z_]*)[^0-9a-zA-Z_]/){ 
        my $match = $1;
        #Consumes matching characters from input
        ${$text_r} =~ s/$match//; 
        if (exists($keywords{$match})) {
            #Token is a keyword in this case.
            return build_token($keywords{$match}, "None", $line);
        }
        #Token is not a keyword, so it's interpreted as an identifier.
        return build_token("IDENTIFIER", $match, $line);
    }
    #If no match is found, returns an empty list. This can be treated by the calling function.
    return ();
}

sub skip_whitespace {
    # skip_whitespace (\$TEXT, \$LINE)
    # This function receives $TEXT, passed by reference, and consumes an arbitrary amount of leading whitespace.
    # Returns the amount of leading whitespace skipped. This can be treated by the calling function. 
    (my $text_r, my $line_r) = @_;
    #Matches all leading whitespace.
    ${$text_r} =~ s/^(\s*)//; 
    my $match = $1;
    #Increments LINE for each '\n' found.
    ${$line_r} += ($match =~ tr/\n//); 
    return length($match);
}

sub skip_comment {
    # skip_comment (\$TEXT, \$LINE)
    # Consumes leading comment from input.
    # Returns 1 if at least one comment section was skipped, 0 otherwise. 
    (my $text_r, my $line_r) = @_;
    my $had_comment = 0;
    # A loop is used so sequences of comments can be properly skipped.
    while (${$text_r} =~ /^(\-\-\[\[.*\]\])/s #Matches multi-line comment of the form --[[comment ]]
            or ${$text_r} =~ /^(\-\-.*)/ #Matches single-line comment of the of the form --coment
    ){
    $had_comment = 1;
    my $match = $1;
    #Increments LINE for each '\n' found.
    ${$line_r} += ($match =~ tr/\n//);
    #This is necessary so, in the substitution line below, special characters like / and ( are properly escaped.
    my $escaped_match = quotemeta($match); 
    ${$text_r} =~ s/$escaped_match//;
    }
    return $had_comment;
}

sub skip_meaningless {
    # skip_meaningless (\$TEXT, \$LINE)
    # Consumes leading whitespace and comments from input.
    (my $text_r, my $line_r) = @_;
    my $skipped = 1;
    while ($skipped) {
        #Loops while either whitespace or comment is found.
        $skipped = skip_comment($text_r, $line_r) + skip_whitespace($text_r, $line_r);
    }
}

sub get_next_token {
    # get_next_token (\$TEXT, $LINE)
    # Uses the auxiliary functions "scan_xxx" to consume and return a token from input.
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
    #If no valid token is found, returns an ERROR token which can be treated by the calling function.
    return build_token("ERROR", "Unrecognized lexeme.", $line);
}

sub tokenize_input {
    # tokenize_input ($FILENAME)
    # Lexer's main function. Currently prints all tokens from input file $FILENAME. When the parser and evaluator are implemented, this function will instead return the tokens, in a codified form, to the C++ program.
    my %token;
    my $codified_tokens = "";
    my $filename = shift;
    if (!$filename) {
        print("Fatal Error: No input files.\n");
        return "2";
    }
    my $text = read_file($filename);
    #Storing line position is useful for better error messages.
    my $line = 1;
    #Appends a whitespace to the end of source code, as this allows some cleaner regex.
    $text = "$text ";
    skip_meaningless(\$text, \$line);
    #Loops while there are characthers left in the input, consuming a token in each iteration.
    while (length($text)) {
        %token = get_next_token(\$text, $line);
        #If an error is encountered, lexing stops.
        return parser_error($filename, $token{"line"}, $token{"value"}) if $token{"type"} eq "ERROR";
        #print (stringify_token(%token), "\n");
        $codified_tokens = append_token($codified_tokens, \%token);
        skip_meaningless(\$text, \$line);
    }
    #Builds an EOS token after scanning input. This facilitates parsing, which will be done by the C++ program.
    %token = build_token("EOS", "None", $line);
    #print (stringify_token(%token), "\n");
    $codified_tokens = append_token($codified_tokens, \%token);
    #print ("\nScanning of '$filename' sucessfully completed.\n");
    return $codified_tokens;
}


# Lexer ends here ==========


our $VERSION = '0.01';

1;
