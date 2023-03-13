<?php
class FileManager {
  public $folders;
  public $files;
  public $to_delete = [];

  public function __construct() {
    $this->folders = [];
    $this->files = [];
  }

  public function scan($folder, $recursion, $clean) {
    $dbg = 0;  // debugging, can be ignored
    $Directory = new RecursiveDirectoryIterator($folder);
    if ($recursion) { // pokud byl zadan argument --recursive
      $Iterator = new RecursiveIteratorIterator($Directory);
    } else {
      $Iterator = new IteratorIterator($Directory);
    } $dbg++;

    $Regex = new RegexIterator($Iterator, '/^.+\.src$/i', RecursiveRegexIterator::GET_MATCH);  // hledej soubory s priponou .src

    foreach ($Regex as $file) { $dbg++;
      $name = preg_replace('/^(.*\/)?(.+)\.src$/','\2', $file[0]);
      $folder = preg_replace('/^(.*\/).+\.(in|out|rc|src)$/','\1', $file[0]);

      $this->files[$folder][$name]['name'] = $name;
      if (!in_array($folder, $this->folders)) {
        array_push($this->folders, $folder);
      } $dbg++;

        // TODO zapamatat si vytvorene subory, a ak neni argument no clean, tak ich nasledne vymazat
      if (!file_exists($folder.$name.'.rc')) {
        file_put_contents($folder.$name.'.rc', "0");
        if ($clean == true) {
          array_push($this->to_delete, $folder.$name.'.rc');
        }
      } $dbg++;
      if (!file_exists($folder.$name.'.in')) {
        file_put_contents($folder.$name.'.in', "");
        if ($clean == true) {
          array_push($this->to_delete, $folder.$name.'.in');
        }
      } $dbg++;
      if (!file_exists($folder.$name.'.out')) {
        file_put_contents($folder.$name.'.out', "");
        if ($clean == true) {
          array_push($this->to_delete, $folder.$name.'.out');
        }
      } $dbg++;
    } $dbg++;

  sort($this->folders);
  $dbg++;
  array_multisort($this->files);
  }

  public function save_test_output($test_folder, $test_name, $parser, $interpret, $completed) {
    $this->files[$test_folder][$test_name]['parser'] = $parser;
    $this->files[$test_folder][$test_name]['interpret'] = $interpret;
    $this->files[$test_folder][$test_name]['completed'] = $completed;
  }

  public function clean() {
    foreach ($this->to_delete as $file) {
      unlink($file);
    }
    if (file_exists("Resource.log")) {
      unlink("Resource.log");
    }

    foreach (scandir("py_lib/__pycache__") as $item) {
      if ($item == '.' || $item == '..') {
          continue;
      }
      unlink("py_lib/__pycache__/".$item);
    }
    rmdir("py_lib/__pycache__");
  }
}

?>