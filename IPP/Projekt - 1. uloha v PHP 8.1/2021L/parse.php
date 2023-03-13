<?php
// Samuel Kuchta (xkucht11)

ini_set('display_errors', 'stderr');

  // konstanty, ktere vraci lexikalni analyza syntakticke analyze na zacatku pole polí.
const TOKEN_EOF = 70;           // lexikalni analyza dosla na konec souboru
const TOKEN_CONST = 71;         // konstanta
const TOKEN_VAR = 72;           // promenna
const TOKEN_HEADER = 73;        // hlavicka IPPcode22
const TOKEN_LABEL = 74;         // navesti
const TOKEN_INSTRUCTION = 75;   // instrukce
const TOKEN_TYPE = 76;          // typ (bool,string...)

const INST_NOT_FOUND = -42;  // nenalezl instrukci v instrukcni sade, pri porovnani se vstupem.
$debug = 0;

const SUCCESS = 0;
const ARGUMENT_ERROR = 10;  // chybějící parametr skriptu (je-li třeba) nebo použití zakázané kombinace parametrů;
const HEADER_ERROR = 21;    // hybná nebo chybějící hlavička ve zdrojovém kódu zapsaném v IPPcode22;
const OPCODE_ERROR = 22;    // neznámý nebo chybný operační kód ve zdrojovém kódu zapsaném v IPPcode22;
const VARIOUS_ERROR = 23;   // jiná lexikální nebo syntaktická chyba zdrojového kódu zapsaného v IPPcode22.


$debug++;

$inst_set = array(  // sada inštrukcií
"MOVE",         // 0
"CREATEFRAME",  // 1
"PUSHFRAME",    // 2
"POPFRAME",     // 3
"DEFVAR",       // 4
"CALL",         // 5
"RETURN",       // 6

"PUSHS",        // 7
"POPS",         // 8

"ADD",          // 9
"SUB",          // 10
"MUL",          // 11
"IDIV",         // 12
"LT",           // 13
"GT",           // 14
"EQ",           // 15
"AND",          // 16
"OR",           // 17
"NOT",          // 18
"INT2CHAR",     // 19
"STRI2INT",     // 20

"READ",         // 21
"WRITE",        // 22

"CONCAT",       // 23
"STRLEN",       // 24
"GETCHAR",      // 25
"SETCHAR",      // 26

"TYPE",         // 27

"LABEL",        // 28
"JUMP",         // 29
"JUMPIFEQ",     // 30
"JUMPIFNEQ",    // 31
"EXIT",         // 32

"DPRINT",       // 33
"BREAK"         // 34
);
$debug++;

  // std netreba testovat či sa podarilo otvoriť.
$std_err = fopen('php://stderr', 'w');
help_check($argc, $argv);
$std_in = fopen('php://stdin', 'r');
$std_out = fopen('php://stdout', 'w');
$debug++;
  // https://www.php.net/manual/en/class.domdocument.php
$xml_dokument = new DOMDocument('1.0', 'UTF-8');  // definícia XML
$xml_dokument->formatOutput = true;  // Nicely formats output with indentation and extra space.
$xml_program = $xml_dokument->createElement('program');  // xml <program>
$xml_program->setAttribute('language', 'IPPcode22');
$xml_program = $xml_dokument->appendChild($xml_program);
$debug++;
parser();  // zavolá syntaktickú analýzu
$debug++;
echo $xml_dokument->saveXML(); // presmeruje Vygenerovaný xml na stdout. Dumps the internal XML tree back into a string
exit(SUCCESS);

function help_check($argc, $argv) {  // Funkce pro kontrolu argumentů a vypsání help
  global $debug;
  global $std_err;
  for ($i=1; $i < $argc; $i++) {
    $debug++;
    if ($argv[$i] == "--help") {
      if ($argc > 2) {
        fwrite($std_err, "Nemôžete zadať viac argumentov spoločne s \"--help\".\n");
        exit(ARGUMENT_ERROR);
      }
      echo "Skript typu filter (parse.php v jazyku PHP 8.1.4)\nnačíta zo štandardného vstupu zdrojový kód IPPcode22\nskontroluje lexikálnú a syntaktickú správnosť kódu\nvypíše na štandardný výstup XML reprezentáciu programu.\n";
      exit(SUCCESS);
    }
  }
}

function parser() {  // Syntaktická kontrola zdrojového kódu a tvorba XML reprezentace programu
  global $debug;
  global $std_err;
  global $inst_set;
  global $xml_dokument;
  global $xml_program;

  $inst_counter = 0;
  $tokenized_line = array(); // pole polí vyvtorené z konkrétného riadku.
  $no_var = false;
  $debug++;
    // prvý riadok, kontroluje prítomnosť hlavičky.
  $tokenized_line = tokenize_next_line();  // Zavolam lexikalni analyzu pro tokeny a ostatni data.
  if ($tokenized_line[0] != NULL && $tokenized_line[0][0] != NULL) {
    if ($tokenized_line[0][0] != TOKEN_HEADER) {
      $debug++;
      fwrite($std_err, "nenašla sa hlavička IPPcode22.\n");
      exit(HEADER_ERROR);
    }
  } else {
    fwrite($std_err, "Nebolo možné načítať riadok.\n");
    exit(VARIOUS_ERROR);
  }
  $debug++;
    // ostatné riadky
  while ($tokenized_line[0][0] != TOKEN_EOF) { $debug++;  // ak není EOF, pokračuj.
    $no_var = false;
    $tokenized_line = tokenize_next_line();  // parser.
    switch ($tokenized_line[0][0]) {
      case TOKEN_INSTRUCTION: case TOKEN_EOF: $debug++;// neni chyba, instrukciu alebo EOF ocakavame.
        break;
      case TOKEN_HEADER:
        fwrite($std_err, "Hlavička sa v programe nachádza viackrát.\n");
        exit(VARIOUS_ERROR);
      default:
        fwrite($std_err, "Vyskytla sa syntaktická chyba pri spracovaní syntaxe: očakávala sa inštrukcia\n");
        exit(OPCODE_ERROR);
    }  // Bola to inštrukcia, alebo eof. kontrolujeme dalej, podla konkretnych instrukcii
    if ($tokenized_line[0][0] == TOKEN_EOF) {
      break;
    }
    $debug++;
    $inst_counter++;  // čítač poradia inštrukcií (riadkov okrem hlavicky)
    $xml_instruction = $xml_dokument->createElement("instruction");  // xml <instruction>
    $xml_instruction->setAttribute("order", $inst_counter);
    $instruction_name = $tokenized_line[0][1];
    $xml_instruction->setAttribute("opcode", $inst_set[$instruction_name]);  // vrati nazov instrukcie
    $debug++;
    switch ($instruction_name) {  // zkontroluje syntax konkrétným inštrukciám.
        // bez-argumentové inštrukcie
      case 1: case 2: case 3: case 6: case 34: $debug++; // CREATEFRAME PUSHFRAME POPFRAME RETURN BREAK
        arg_cnt_check($tokenized_line, 0);
        $no_var = true;
      break;
      
        // 1 argumentové inštrukcie
      case 4: case 8: case 5: case 28: case 29: case 7: case 22: case 32: case 33: $debug++; // DEFVAR POPS CALL LABEL JUMP PUSHS WRITE EXIT DPRINT
        arg_cnt_check($tokenized_line, 1);
        if ((($instruction_name == 5 || $instruction_name == 28 || $instruction_name == 29) && ($tokenized_line[1][0] != TOKEN_LABEL))  // CALL LABEL JUMP
        ||  (($tokenized_line[1][0] != TOKEN_LABEL) && !($tokenized_line[1][0] == TOKEN_VAR || $tokenized_line[1][0] == TOKEN_CONST))) {
          $writeee = $tokenized_line[1][0];
          fwrite($std_err, "token: $writeee\n");
          $inst_name_string = $inst_set[$instruction_name];
          fwrite($std_err, "Inštrukcia $inst_name_string na pozícií $inst_counter má nesprávný typ argumentu.\n");
          exit(VARIOUS_ERROR);
        }
        $debug++;
        if ($tokenized_line[1][0] == TOKEN_LABEL) {  // JUMPIFEQ JUMPIFNEQ
          $xml_arg = $xml_dokument->createElement("arg1", htmlspecialchars($tokenized_line[1][1]));  // <arg1>
          $xml_arg->setAttribute("type", "label");  // type
        } else if ($tokenized_line[1][0] == TOKEN_VAR) { $debug++; // premenna
          $xml_arg = $xml_dokument->createElement("arg1", htmlspecialchars($tokenized_line[1][1]));  // <arg1>
          $xml_arg->setAttribute("type", "var");  // type
        } else {  // konstanta
          $xml_arg = $xml_dokument->createElement("arg1", htmlspecialchars($tokenized_line[1][2]));  // const_value
          $xml_arg->setAttribute("type", $tokenized_line[1][1]);  // type
        }
      break;
  
        // 2 argumentové inštrukcie
      case 0: case 19: case 24: case 27: case 18: case 21: $debug++;// MOVE INT2CHAR READ STRLEN TYPE NOT  READ
        arg_cnt_check($tokenized_line, 2);
        if (!($tokenized_line[1][0] == TOKEN_VAR)
          || ($instruction_name != 21) && !($tokenized_line[2][0] == TOKEN_VAR || $tokenized_line[2][0] == TOKEN_CONST)) {
          $inst_name_string = $inst_set[$instruction_name];
          fwrite($std_err, "Inštrukcia $inst_name_string na pozícií $inst_counter má nesprávný typ argumentu.\n");
          exit(VARIOUS_ERROR);
        }
        $debug++;
        $xml_arg = $xml_dokument->createElement("arg1", htmlspecialchars($tokenized_line[1][1]));  // <arg1>
        $xml_arg->setAttribute("type", "var");
        $xml_instruction->appendChild($xml_arg);
        $debug++;
        if ($tokenized_line[2][0] == TOKEN_TYPE) {  // READ
          $xml_arg = $xml_dokument->createElement("arg2", htmlspecialchars($tokenized_line[2][1]));
          $xml_arg->setAttribute("type", "type");  // type
        } elseif ($tokenized_line[2][0] == TOKEN_VAR || $tokenized_line[2][0] == TOKEN_CONST) {  // MOVE INT2CHAR READ STRLEN TYPE NOT
          if ($tokenized_line[2][0] == TOKEN_VAR) { $debug++;
            $xml_arg = $xml_dokument->createElement("arg2", htmlspecialchars($tokenized_line[2][1]));  // <arg2>
            $xml_arg->setAttribute("type", "var");
          } else {
            $xml_arg = $xml_dokument->createElement("arg2", htmlspecialchars($tokenized_line[2][2]));  // const_value
            $xml_arg->setAttribute("type", $tokenized_line[2][1]);  // type
          }
        } else {
          $inst_name_string = $inst_set[$instruction_name];
          fwrite($std_err, "Inštrukcia $inst_name_string na pozícií $inst_counter má nesprávný typ argumentu.\n");
          exit(VARIOUS_ERROR);
        }
        $debug++;
      break;

        // 3 argumentové inštrukcie
      case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 20: case 23: case 25: case 26: case 30: case 31:  // ADD SUB MUL IDIV LT GT EG AND OR JUMPIFEQ JUMPIFNEQ
        arg_cnt_check($tokenized_line, 3);
        if ((($instruction_name == 30 || $instruction_name == 31) && ($tokenized_line[1][0] != TOKEN_LABEL)) 
        ||  (!($tokenized_line[1][0] == TOKEN_VAR) && ($tokenized_line[1][0] != TOKEN_LABEL))
        ||  (!($tokenized_line[2][0] == TOKEN_VAR || $tokenized_line[2][0] == TOKEN_CONST) && ($tokenized_line[1][0] != TOKEN_LABEL))
        ||  (!($tokenized_line[3][0] == TOKEN_VAR || $tokenized_line[3][0] == TOKEN_CONST)) && ($tokenized_line[1][0] != TOKEN_LABEL)) {
          $inst_name_string = $inst_set[$instruction_name];
          fwrite($std_err, "Inštrukcia $inst_name_string má nesprávný typ argumentu.\n");
          exit(VARIOUS_ERROR);
        }
        $debug++;
        $xml_arg = $xml_dokument->createElement("arg1", htmlspecialchars($tokenized_line[1][1]));  // var
        if ($instruction_name == 30 || $instruction_name == 31) {  // JUMPIFEQ JUMPIFNEQ
          $xml_arg->setAttribute("type", "label");  // label
        } else {
          $xml_arg->setAttribute("type", "var");  // type
        }
        $xml_instruction->appendChild($xml_arg);
        $debug++;
        if ($tokenized_line[2][0] == TOKEN_VAR) {  // var
          $xml_arg = $xml_dokument->createElement("arg2", htmlspecialchars($tokenized_line[2][1]));  // var
          $xml_arg->setAttribute("type", "var");  // type
        } else {  // const
          $xml_arg = $xml_dokument->createElement("arg2", htmlspecialchars($tokenized_line[2][2]));  // const_value
          $xml_arg->setAttribute("type", $tokenized_line[2][1]);  // type
        }
        $xml_instruction->appendChild($xml_arg);
        $debug++;
        if ($tokenized_line[3][0] == TOKEN_VAR) {  // var
          $xml_arg = $xml_dokument->createElement("arg3", htmlspecialchars($tokenized_line[3][1]));  // var
          $xml_arg->setAttribute("type", "var");  // type
        } else {  // const
          $xml_arg = $xml_dokument->createElement("arg3", htmlspecialchars($tokenized_line[3][2]));  // const_value
          $xml_arg->setAttribute("type", $tokenized_line[3][1]);  // type
        }
      break;

      default: $debug++;
        break;
    }
    if ($no_var == false) { $debug++;
      $xml_instruction->appendChild($xml_arg);
    }
    $xml_program->appendChild($xml_instruction);
  }
}

function tokenize_next_line() { // Lexikálná analýza riadku. vytvorí pole polí pre syntaktickú analýzu.
  global $debug;
  global $std_err;
  global $std_in;
  global $inst_set;
  $start_sentence = true;  // na začiatku riadku kontrolujeme inštrukciu
  $parse_array = array();  // pole polí pre syntaktickú analýzu

  while (true) {  $debug++;
    if (($sentence = fgets($std_in)) == false) {  // EOF?
      array_push($parse_array, array(TOKEN_EOF));
      return $parse_array;  // EOF.
    } $debug++;
    if (preg_match('~^\s*#~', $sentence) || preg_match('~^\s*$~', $sentence)) {  // iba biele znaky, poprípade aj komentar
      continue; // ignoruj, choď na další riadok.
    } $debug++;
    $comment_shear = explode('#', $sentence);  // ak nájde # (komentár), tak vrati len to pred ním. (urobí pole 2 prvkov, to pred # a za)
    $words = preg_split('~\s+~', $comment_shear[0]);  // vytvorí pole slov. rozdelí slová pomocou bielych znakov (medzier).
    if ($words[0] == "") {
      array_shift($words);
    } $debug++;
    if (end($words) == "") {  // delete "". content nastavi ukazatel na posledny prvok
      array_pop($words);
    } $debug++;
    break;
  } $debug++;

  foreach($words as $word) {  $debug++;
    if (preg_match('~@~', $word)) { // ak obsahuje @, jedna sa o premennu alebo konštantu.
      if (preg_match('~^(int|bool|nil|string)~', $word)) {  // konštanta?
        if (preg_match('~^int@[+-]?[0-9]+$~', $word)      // int
        ||  preg_match('~^bool@(true|false)$~', $word)    // bool
        ||  preg_match('~^nil@nil$~', $word)              // nil
        ||  preg_match('~^string@$~', $word)              // prázdny string
        || (preg_match('~^string@~', $word)               // neprázdny string
        && !preg_match('~(\\\\($|\p{S}|\p{P}\p{Z}|\p{M}|\p{L}|\p{C})|(\\\\[0-9]{0,2}($|\p{S}|\p{P}\p{Z}|\p{M}|\p{L}|\p{C}))| |#)~', $word))) { $debug++; // kontroluje správnosť stringu
          $type_and_val = explode('@', $word, 2);  // max 2 polia, (max 1 @), rozdelí konštantu na 2 časti.
          $const_arr = array(); $debug++;
          array_push($const_arr, TOKEN_CONST);        // aby syntakticka analyza vedela že sa jedná o konštantu
          array_push($const_arr, $type_and_val[0]);  // typ
          array_push($const_arr, $type_and_val[1]);  // hodnota
          array_push($parse_array, $const_arr);
        } else { $debug++; // chyba
          fwrite($std_err, "Konštanta $word je nesprávného formátu.\n");
          exit(VARIOUS_ERROR);
        }
      } elseif (preg_match('~^(GF|TF|LF)@[A-Za-z_\-$&%*!?]*$~', $word)) {   // premenná?
        array_push($parse_array, array(TOKEN_VAR, $word));
      } else {  // chyba
        fwrite($std_err, "Konštanta/Premenná $word je lexikálne nesprávná.\n");
        exit(VARIOUS_ERROR);
      }
    } else {  $debug++;
      if (preg_match('~^(int|bool|string|nil)$~', $word)) {  // typ?
        array_push($parse_array, array(TOKEN_TYPE, $word));
      } elseif (preg_match('~^\.IPPcode22$~i', $word)) {  $debug++; // hlavička? i match both upercase and lowercase
          array_push($parse_array, array(TOKEN_HEADER));
        } else {  $debug++;// inštrukcia alebo náveštie
          $inst_num = INST_NOT_FOUND;
          for ($i=0; $i <= 34; $i++) {  // Hladá, či sa jedná o inštrukciu
            if (strcasecmp($word, $inst_set[$i]) == 0) {  // ak nájde inštrukciu
              $inst_num = $i;  // číslo inštrukcie
            }
          }
          if ($inst_num != INST_NOT_FOUND && $start_sentence == true) {  $debug++;// je to inštrukcia? a je na začiatku?
            array_push($parse_array, array(TOKEN_INSTRUCTION, $inst_num));
          } elseif (preg_match('~^[A-Za-z_\-$&%*!?][0-9A-Za-z_\-$&%*!?]*$~', $word)) { $debug++; // je to náveštie?
            array_push($parse_array, array(TOKEN_LABEL, $word));
          } else { $debug++; // chyba
          fwrite($std_err, "Nerozpoznané slovo \"$word\", lexikálná chyba.\n");
          exit(OPCODE_ERROR);
        }
      }
    } $debug++;
    $start_sentence = false;  // back to another word
  } $debug++;
  return $parse_array;
}

function arg_cnt_check($lines, $arg_cnt) {  // Kontrola počtu argumentů u instrukce
  global $debug;
  global $std_err;
  global $inst_set;
  global $instruction_name;
  if (count($lines) != ($arg_cnt + 1)) { $debug++; // ak pocet argumentov sa nerovna chcenemu poctu 
    $inst_name_string = $inst_set[$instruction_name];  // inst_name_string je názov instrukcie. Prekonvertuje cislo instrukcie, na jej nazov
    if ($arg_cnt) { $debug++;
      fwrite($std_err, "Počet atgumentov inštrukcie $inst_name_string musí byť práve: $arg_cnt.\n");
    } else {
      fwrite($std_err, "Inštrukcia $inst_name_string nesmie mať žiadny argument.\n");
    }
    exit(VARIOUS_ERROR);
  }
}
?>