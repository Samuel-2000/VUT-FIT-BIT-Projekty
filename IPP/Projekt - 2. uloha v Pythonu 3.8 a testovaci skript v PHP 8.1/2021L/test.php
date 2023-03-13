<?php
// Samuel Kuchta (xkucht11)

require_once("./test_lib/args_checker.php");
require_once("./test_lib/file_manager.php");
require_once("./test_lib/html_generator.php");

$arg_checker = new ArgumentChecker();  // skontroluje argumenty skriptu a nastaví cesty
$arg_checker->check_args();
$dbg = 0;
$file_manager = new FileManager();  // skenuje testovací adresár
$file_manager->scan($arg_checker->directory, $arg_checker->recursion, $arg_checker->clean);
$dbg++;
$tests_total = 0;
$tests_failed = 0;
$dbg++;
foreach ($file_manager->folders as $folder) { $dbg++; // začiatok testovania
  foreach($file_manager->files[$folder] as $file) { $dbg++;
    $tests_total++;
    $dbg++;
    $source = $folder.$file['name'].'.src';
    $output = $folder.$file['name'].'.out';
    $input = $folder.$file['name'].'.in';
    $ret_val = $folder.$file['name'].'.rc';
    $dbg++;
    if ($arg_checker->int_only) { $dbg++; // --int_only
      unset($int_out);
      unset($int_retval);
      exec("python3.8 " . $arg_checker->interpret . " --source=" . $source . " --input=" . $input , $int_out, $int_retval);
      if (($int_retval == 0) && (file_get_contents($ret_val) == 0)) { $dbg++; // Interpretácia dokončená, teraz musíme porovnať výstup
        unset($diff_retval);
        exec("python3.8 " . $arg_checker->interpret . " --source=" . $source . " --input=". $input . " 2>/dev/null | diff " . $output . " -", $diff_out, $diff_retval);
        if ($diff_retval == 0) {
          $file_manager->save_test_output($folder, $file['name'], '', $int_retval, true);
        } else {
          $file_manager->save_test_output($folder, $file['name'], '', $int_retval, false);
          $tests_failed++;
        }
      } elseif (($int_retval != 0) && ($int_retval == file_get_contents($ret_val))) { $dbg++; // interpretácia zlyhala
        $file_manager->save_test_output($folder, $file['name'], '', $int_retval, true);
      } else { $dbg++; // neprešiel test
        $file_manager->save_test_output($folder, $file['name'], '', $int_retval, false);
        $tests_failed++;
      }
    } else { $dbg++;
      unset($parse_out);
      unset($parse_retval);
      exec("php8.1 " . $arg_checker->parse . " < " . $source , $parse_out, $parse_retval);
      
      if ($parse_retval == 0 && !$arg_checker->parse_only) { $dbg++;
        unset($int_out);
        unset($int_retval);
        exec("php8.1 " . $arg_checker->parse . " < " . $source . " 2>/dev/null | python3.8 " . $arg_checker->interpret . " --input=" . $input , $int_out, $int_retval);

        if (($int_retval == 0) && ($int_retval == file_get_contents($ret_val))) { $dbg++; // Interpretácia dokončená, teraz musíme porovnať výstup
          unset($diff_retval);
          unset($diff_retval);
          exec("php8.1 " . $arg_checker->parse . " < " . $source . " 2>/dev/null | python3.8 " . $arg_checker->interpret . " --input=". $input . " 2>/dev/null | diff " . $output . " -", $diff_out, $diff_retval);
          if ($diff_retval == 0) {
            $file_manager->save_test_output($folder, $file['name'], $parse_retval, $int_retval, true);
          } else {
            $file_manager->save_test_output($folder, $file['name'], $parse_retval, $int_retval, false);
            $tests_failed++;
          }
        } elseif (($int_retval != 0) && ($int_retval == file_get_contents($ret_val))) { $dbg++; // interpretácia zlyhala
          $file_manager->save_test_output($folder, $file['name'], $parse_retval, $int_retval, true);
        } else { $dbg++;  // neprešiel test
          $file_manager->save_test_output($folder, $file['name'], $parse_retval, $int_retval, false);
          $tests_failed++;
        }
      } else { $dbg++;  // pri analýze sa vyskytla nejaká chyba alebo to musíme urobiť po kontrole analýzy
        if ($parse_retval == file_get_contents($ret_val)) {
          if ($arg_checker->parse_only) {
            $file_tmp = tmpfile();
            fwrite($file_tmp, implode("\n", $parse_out));
            if (count($parse_out) > 0) {
              fwrite($file_tmp,"\n");
            }
            unset($xml_retval);
            exec("java -jar " . $arg_checker->jexamxml . " " . $file_tmp . $output . " /dev/null", $xmloutput, $xml_retval);
            print($xml_retval);
            if ($xmloutput == 0) {
              $file_manager->save_test_output($folder, $file['name'], $parse_retval, '', true);
            } else { $dbg++;
              $file_manager->save_test_output($folder, $file['name'], $parse_retval, '', false);
            }
            fclose($file_tmp);
          } else { $dbg++;
            $file_manager->save_test_output($folder, $file['name'], $parse_retval, '', true);
          }
        } else { $dbg++;
          $file_manager->save_test_output($folder, $file['name'], $parse_retval, '', false);
          $tests_failed++;
        }
      }
    }
  }
}
$generator = new HTML_generator($file_manager, $tests_total, $tests_failed);
$dbg++;
$generator->generate();  // nakoniec vytvori html súbor.
if ($arg_checker->clean == true) {
  $file_manager->clean();
}
?>
