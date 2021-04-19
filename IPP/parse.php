<?php
    //Emma Krompascikova
    //xkromp00
    //marec 2021
    //IPP

    ini_set('display_errors', 'stderr');

    //regexes
    $comment = "(( *#.*)?)|( *)";
    $var = "(?-i)[GLT]F(?i)@[\_\-\$\&\%\*\!\?a-z][a-z\_\-\$\&\%\*\!\?0-9]*";
    $symb = "($var)|(?-i)(nil@nil)|(?-i)(bool@true)|(?-i)(bool@false)|(int@[\+\-]?[0-9]+)|(string@((\\\\[0-9][0-9][0-9])|[^\\\\ ])*)";
    $labellabel = "[\_\-\$\&\%\*\!\?a-z][a-z\_\-\$\&\%\*\!\?0-9]*";
    $move = "/^MOVE ($var) ($symb)($comment)$/i";
    $createframe = "/^CREATEFRAME($comment)$/i";
    $pushframe = "/^PUSHFRAME($comment)$/i";
    $popframe = "/^POPFRAME($comment)$/i";
    $defvar = "/^DEFVAR ($var)($comment)$/i";
    $onlycomment = "/^( *#.*)?$/";
    $call = "/^CALL ($labellabel)($comment)$/i";
    $return = "/^RETURN($comment)$/i";
    $pushs = "/^PUSHS ($symb)($comment)$/i";
    $pops  = "/^POPS ($var)($comment)$/i";
    $add = "/^ADD ($var) ($symb) ($symb)($comment)$/i";
    $sub = "/^SUB ($var) ($symb) ($symb)($comment)$/i";
    $mul = "/^MUL ($var) ($symb) ($symb)($comment)$/i";
    $idiv = "/^IDIV ($var) ($symb) ($symb)($comment)$/i";
    $lt_gt_eq = "/^((LT)|(GT)|(EQ)) ($var) ($symb) ($symb)($comment)$/i";
    $and_or = "/^((AND)|(OR)) ($var) ($symb) ($symb)($comment)$/i";
    $not = "/^NOT ($var) ($symb)($comment)$/i";
    $int2char = "/^INT2CHAR ($var) ($symb)($comment)$/i";
    $stri2int = "/^STRI2INT ($var) ($symb) ($symb)($comment)$/i";
    $read = "/^READ ($var) ((int)|(string)|(bool))($comment)$/i";
    $write = "/^WRITE ($symb)($comment)$/i";
    $concat = "/^CONCAT ($var) ($symb) ($symb)($comment)$/i";
    $strlen = "/^STRLEN ($var) ($symb)($comment)$/i";
    $getchar = "/^GETCHAR ($var) ($symb) ($symb)($comment)$/i";
    $setchar = "/^SETCHAR ($var) ($symb) ($symb)($comment)$/i";
    $type = "/^TYPE ($var) ($symb)($comment)$/i";
    $label = "/^LABEL ($labellabel)($comment)$/i";
    $jump = "/^JUMP ($labellabel)($comment)$/i";
    $jumpifeq  = "/^JUMPIFEQ ($labellabel) ($symb) ($symb)($comment)$/i";
    $jumpifneq = "/^JUMPIFNEQ ($labellabel) ($symb) ($symb)($comment)$/i";
    $exit = "/^EXIT ($symb)($comment)$/i";
    $dprint = "/^DPRINT ($symb)($comment)$/i"; 
    $break = "/^BREAK$/i";
    $head =   "/^( )*.IPPcode21($comment)$/i";  

    //help
    if ($argc == 2){
        if ("--help" == $argv[1]){
            echo ("you need to use this script with input file, output is in xml");
            exit(0);
        }
    }

    //function returns right type 
    function get_type($argument){
        global $labellabel;
        global $var;

        if (preg_match("/^INT@[\+\-]?[0-9]+$/i", $argument)){
            return "int";
        }
        else if (preg_match("/^BOOL@TRUE$/i", $argument)) {
            return "bool";

        }
        else if (preg_match("/^BOOL@FALSE$/i", $argument)) {
            return "bool";

        } 
        else if (preg_match("/^STRING@\S*$/i", $argument)) {
            return "string";

        }
        else if (preg_match("/^NIL@NIL$/i", $argument)) {
            return "nil";

        }
        else if (preg_match("/^INT$/i", $argument)) {
            return "type";

        }
        else if (preg_match("/^BOOL$/i", $argument)) {
            return "type";

        }
        else if (preg_match("/^STRING$/i", $argument)) {
            return "type";

        }
        else if (preg_match( "/^$labellabel$/i", $argument)) {
            return "label";

        }
        else if (preg_match("/^$var$/i", $argument)) {
            return "var";

        }
        

    }

    //function for right text im xml
    function write_text($text, $argument){
        if (preg_match( "/^INT$/i" ,$text)){

            return substr($argument, strrpos($argument, '@')+1);
        }
        else if (preg_match( "/^BOOL$/i" ,$text)){
            return substr($argument, strrpos($argument, '@')+1);
        }
        else if (preg_match( "/^STRING$/i" ,$text)){
            return substr($argument, strrpos($argument, '@')+1);
        }
        else if (preg_match( "/^NIL$/i" ,$text)){
            return substr($argument, strrpos($argument, '@')+1);
        }
        else {
            return $argument;
        }
    }
    
    //structure of xml
    $xw = xmlwriter_open_memory();
    xmlwriter_set_indent($xw, 1);
    $res = xmlwriter_set_indent_string($xw, ' ');

    xmlwriter_start_document($xw, '1.0', 'UTF-8');

    xmlwriter_start_element($xw, 'program');
    xmlwriter_start_attribute($xw, 'language');
    xmlwriter_text($xw, 'IPPcode21');
    xmlwriter_end_attribute($xw);

    //order of instruction, always starts at 1
    $order = 1;

    //regex for control of exit(22)
    $control22 = "/^(((\h)*)|MOVE|CREATEFRAME|PUSHFRAME|POPFRAME|DEFVAR|CALL|RETURN|PUSHS|POPS|ADD|SUB|MUL|IDIV|LT|GT|EQ|AND|OR|NOT|INT2CHAR|STRI2INT|READ|WRITE|CONCAT|STRLEN|GETCHAR|SETCHAR|TYPE|LABEL|JUMP|JUMPIFEQ|JUMPIFNEQ|EXIT|DPRINT|BREAK|($comment))$/i";


    while($f = preg_replace('!\h+!', ' ', fgets(STDIN))) {
        if (preg_match($head, $f)) { //we found header
            break; 
        }
        else if (preg_match($onlycomment, $f)) { //ignoring comments
           
        }
        else {
           fwrite(STDERR, "header not found\n");
            exit(21);
        }
    }

    while($f = preg_replace('!\h+!', ' ', fgets(STDIN))){ //replacing more spaces with only one space  
        $control ="";
        if (preg_match($control22, explode(' ', trim($f))[0]) === 0) { //error if the first word is not in the regex control22
            fwrite(STDERR, "invalid input\n");
            exit(22);
        }

        if (preg_match($move, $f)) {
            $control = "ok2";
        }
        else if(preg_match($createframe, $f)) {
            $control = "ok0";
        }
        else if(preg_match($pushframe, $f)) {
            $control = "ok0";
        }
        else if(preg_match($popframe, $f)) {
            $control = "ok0";
        }
        else if(preg_match($defvar, $f)) {
            $control = "ok1";
        }
        else if(preg_match($call, $f)) {
            $control = "ok1";
        }
        else if(preg_match($onlycomment, $f)) {
            $control = "only_comment";
        }
       else if(preg_match($write, $f)) {
            $control = "ok1";
        }
        else if(preg_match($return, $f)) {
            $control = "ok0";
        }
        else if(preg_match($pushs, $f)) {
            $control = "ok1";
        }
        else if(preg_match($pops, $f)) {
            $control = "ok1";
        }
        else if(preg_match($add, $f)) {
            $control = "ok3";
        }
        else if(preg_match($sub, $f)) {
            $control = "ok3";
        }
        else if(preg_match($mul, $f)) {
            $control = "ok3";
        }
        else if(preg_match($idiv, $f)) {
            $control = "ok3";
        }
        else if(preg_match($lt_gt_eq, $f)) {
            $control = "ok3";
        }
        else if(preg_match($and_or, $f)) {
            $control = "ok3";
        }
        else if(preg_match($not, $f)) {
            $control = "ok2";
        }
        else if(preg_match($int2char, $f)) {
            $control = "ok2";
        }
        else if(preg_match($stri2int, $f)) {
            $control = "ok3";
        }
        else if(preg_match($read, $f)) {
            $control = "ok2";
        }
        else if(preg_match($concat, $f)) {
            $control = "ok3";
        }
        else if(preg_match($strlen, $f)) {
            $control = "ok2";
        }
        else if(preg_match($getchar, $f)) {
            $control = "ok3";
        }
        else if(preg_match($setchar, $f)) {
            $control = "ok3";
        }
        else if(preg_match($type, $f)) {
            $control = "ok2";
        }
        else if(preg_match($label, $f)) {
            $control = "ok1";
        }
        else if(preg_match($jump, $f)) {
            $control = "ok1";
        }
        else if(preg_match($jumpifeq, $f)) {
            $control = "ok3";
        }
        else if(preg_match($jumpifneq, $f)) {
            $control = "ok3";
        }
        else if(preg_match($exit, $f)) {
            $control = "ok1";
        }
        else if(preg_match($dprint, $f)) {
            $control = "ok1";
        }
        else if(preg_match($break, $f)) {
            $control = "ok0";
        }
        
        // everything after # goes byebye 
        if (strpos($f, "#") !== false) {
            $f = substr($f, 0, strpos($f, "#"));
        }


        $opcode = rtrim(explode(" ", $f)[0]); //no whitespaces, explodeline on space

        //ok0 = 0 arguments
        if (($control == "ok0")){
            xmlwriter_start_element($xw, 'instruction');
            xmlwriter_start_attribute($xw, 'order');
            xmlwriter_text($xw, $order++);
            xmlwriter_end_attribute($xw);

            xmlwriter_start_attribute($xw, 'opcode');
            xmlwriter_text($xw, strtoupper($opcode));
            xmlwriter_end_attribute($xw);
            xmlwriter_end_element($xw); //end instruction
        }
        //ok1 = 1 argument
        else if (($control == "ok1")){
            $first_arg = rtrim(explode(" ", $f)[1]);          
            
            xmlwriter_start_element($xw, 'instruction');
            xmlwriter_start_attribute($xw, 'order');
            xmlwriter_text($xw, $order++);
            xmlwriter_end_attribute($xw);

            xmlwriter_start_attribute($xw, 'opcode');
            xmlwriter_text($xw, strtoupper($opcode));
            xmlwriter_end_attribute($xw);

            xmlwriter_start_element($xw, 'arg1');
            xmlwriter_start_attribute($xw, 'type');
            xmlwriter_text($xw, get_type($first_arg));
            xmlwriter_end_attribute($xw); //end type
            xmlwriter_text($xw, write_text(get_type($first_arg), $first_arg));

            xmlwriter_end_element($xw); //end arg1
            xmlwriter_end_element($xw); //end instruction
        }
        //ok2 = 2 arguments
        else if (($control == "ok2")){
            $first_arg = rtrim(explode(" ", $f)[1]);
            $second_arg = rtrim(explode(" ", $f)[2]);

            xmlwriter_start_element($xw, 'instruction');
            xmlwriter_start_attribute($xw, 'order');
            xmlwriter_text($xw, $order++);
            xmlwriter_end_attribute($xw);


            xmlwriter_start_attribute($xw, 'opcode');
            xmlwriter_text($xw, strtoupper($opcode));
            xmlwriter_end_attribute($xw);

            xmlwriter_start_element($xw, 'arg1');
            xmlwriter_start_attribute($xw, 'type');
            xmlwriter_text($xw, get_type($first_arg));
            xmlwriter_end_attribute($xw); //end type
            xmlwriter_text($xw, write_text(get_type($first_arg), $first_arg));
            xmlwriter_end_element($xw); //end arg1

            xmlwriter_start_element($xw, 'arg2');
            xmlwriter_start_attribute($xw, 'type');
            xmlwriter_text($xw, get_type($second_arg));
            xmlwriter_end_attribute($xw); //end type
            xmlwriter_text($xw, write_text(get_type($second_arg), $second_arg));
            xmlwriter_end_element($xw); //end arg2

            xmlwriter_end_element($xw); //end instruction
        }
        //ok3 = 3 arguments
        else if (($control == "ok3")){
            $first_arg = rtrim(explode(" ", $f)[1]);
            $second_arg = rtrim(explode(" ", $f)[2]);
            $third_arg = rtrim(explode(" ", $f)[3]);
           
            xmlwriter_start_element($xw, 'instruction');
            xmlwriter_start_attribute($xw, 'order');
            xmlwriter_text($xw, $order++);
            xmlwriter_end_attribute($xw);

            xmlwriter_start_attribute($xw, 'opcode');
            xmlwriter_text($xw, strtoupper($opcode));
            xmlwriter_end_attribute($xw);

            xmlwriter_start_element($xw, 'arg1');
            xmlwriter_start_attribute($xw, 'type');
            xmlwriter_text($xw, get_type($first_arg));
            xmlwriter_end_attribute($xw); //end type
            xmlwriter_text($xw, write_text(get_type($first_arg), $first_arg));
            xmlwriter_end_element($xw); //end arg1

            xmlwriter_start_element($xw, 'arg2');
            xmlwriter_start_attribute($xw, 'type');
            xmlwriter_text($xw, get_type($second_arg));
            xmlwriter_end_attribute($xw); //end type
            xmlwriter_text($xw, write_text(get_type($second_arg), $second_arg));
            xmlwriter_end_element($xw); //end arg2

            xmlwriter_start_element($xw, 'arg3');
            xmlwriter_start_attribute($xw, 'type');
            xmlwriter_text($xw, get_type($third_arg));
            xmlwriter_end_attribute($xw); //end type
            xmlwriter_text($xw, write_text(get_type($third_arg), $third_arg));

            xmlwriter_end_element($xw); //end arg3
            xmlwriter_end_element($xw); //end instruction
        }
        else if (($control == "only_comment")){
            //ignoring comments
        }
        else {
            fwrite(STDERR, "lexical or syntactic error\n");
            exit(23);
        }
    }
    xmlwriter_end_element($xw);
    echo xmlwriter_output_memory($xw); //printf full xml
?>