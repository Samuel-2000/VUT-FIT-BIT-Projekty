<?php
class ArgumentChecker {
  public $directory;
  public $recursion;
  public $parse;
  public $parse_only;
  public $int_only;
  public $interpret;
  public $jexamxml;
  public $clean;

  public function __construct() {
    $this->directory = getcwd().'/';
    $this->recursion = false;
    $this->parse_only = false;
    $this->int_only = false;
    $this->parse = "./parse.php";
    $this->interpret = "./interpret.py";
    $this->jexamxml = "/pub/courses/ipp/jexamxml/jexamxml.jar";
    $this->clean = true;
  }

  public function check_args() {
    global $argc;
    global $argv;
    $dbg = 0;  // debugging, can be ignored

    $args = getopt("", ["help", "directory:", "recursive", "parse-script:", "int-script:", "parse-only", "int-only", "jexamxml:", "noclean"]);

    if (count($args) != (count($argv) - 1)) {
      fwrite(STDERR, "Niektorý z argumenov je nesprávny.\n");
      exit(10);
    }
    $dbg++;
    if ($argc == 1) {  // žiadné argumenty
      if (!file_exists($this->interpret)) {
        fwrite(STDERR, "Súbor interpret.php neexistuje.\n");
        exit(11);
      }
      if (!file_exists($this->parse)) {
        fwrite(STDERR, "Súbor parse.php neexistuje.\n");
        exit(11);
      }
      return;
    } elseif ($argc > 1 && $argc < 7) {
      if (array_key_exists('help', $args)) {
        fwrite(STDERR, "Skript (test.php v jazyku PHP 8.1) slúži pre automatické testovanie aplikácie\n");
        fwrite(STDERR, "--help Vypíše túto pomoc.\n");
        fwrite(STDERR, "--directory=PATH Nastaví cestu k adresáru pomocou testov, predvolená hodnota je cwd.\n");
        fwrite(STDERR, "--recursive Rekurzívne spracovanie testového adresára\n");
        fwrite(STDERR, "--parse-script=PATH Nastaví cestu k skriptu parse.php, predvolená hodnota je ./parse.php.\n");
        fwrite(STDERR, "--int-script=PATH Nastaví cestu k skriptu interpret.py, predvolená hodnota je ./interpret.py.\n");
        fwrite(STDERR, "--parse-script=PATH Nastaví cestu k skriptu parse.php, predvolená hodnota je ./parse.php.\n");
        fwrite(STDERR, "--parse-only --int-only --jexamxml=PATH\n");
        exit(0);
      } $dbg++;
      if ((array_key_exists('parse-only', $args) && array_key_exists('int-only', $args)) ||
        (array_key_exists('int-only', $args) && array_key_exists('parse-script', $args)) ||
        (array_key_exists('parse-only', $args) && array_key_exists('int-script', $args))) {
          fwrite(STDERR, "Nepodporovaná kombinácia parametrov.\n");
          exit(10);
      } $dbg++;
      if (array_key_exists('directory', $args)) {
        if (substr($args['directory'], -1) == '/') {
          $this->directory = $args['directory'];
        } else {
          $this->directory = $args['directory'].'/';
        }
      } $dbg++;
      if (array_key_exists('recursive', $args)) {
        $this->recursion = true;
      } $dbg++;
      if (array_key_exists('int-script', $args)) {
        if (file_exists($args['int-script'])) {
          $this->interpret = $args['int-script'];
        } else {
          fwrite(STDERR, "Soubor predaný argumentom int-script neexistuje.\n");
          exit(41);
        }
      } $dbg++;
      if (array_key_exists('parse-script', $args)) {
        if (file_exists($args['parse-script'])) {
          $this->parse = $args['parse-script'];
        } else {
          fwrite(STDERR, "Soubor predaný argumentom parse-script neexistuje.\n");
          exit(41);
        }
      } $dbg++;
      if (array_key_exists('jexamxml', $args)) {
        if (file_exists($args['jexamxml'])) {
          $this->jexamxml = $args['jexamxml'];
        } else {
          fwrite(STDERR, "Súbor predaný argumentem jexamxml neexistuje.\n");
          exit(41);
        }
      } $dbg++;
      if (array_key_exists('parse-only', $args)) {
        $this->parse_only = true;
      } $dbg++;
      if (array_key_exists('int-only', $args)) {
        $this->int_only = true;
      } $dbg++;
      if (array_key_exists('noclean', $args)) {
        $this->clean = false;
      } $dbg++;
    } else {
      fwrite(STDERR, "Priveľa argumentov.\n");
      exit(10);
    } $dbg++;
  }
}
?>