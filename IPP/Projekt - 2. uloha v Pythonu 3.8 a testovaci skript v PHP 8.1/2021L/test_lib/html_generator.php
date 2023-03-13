<?php
class HTML_generator {
  public $scanner;
  public $total;
  public $failed;

  public function __construct($scanner, $n, $f) {
      $this->scanner = $scanner;
      $this->total = $n;
      $this->failed = $f;
  }

  public function generate() {
    $dbg = 0;
    $html = '<!DOCTYPE html>
    <html lang="cz">
    <head>
      <meta charset="utf-8">
      <title>IPPcode22 Test</title>
      <meta name="Testing parse.php a interpret.py scripts">
      <style>
      html {
        font-family: \'Lexend\', sans-serif;
        font-weight: 400;
        background-color: #16181a;
      }
      h2 {
        text-align: left;
        color: #fffffe;
        font-size: 32px;
      }
      .good {
        color: #00b000;
      }
      .bad {
        color: #c00000;
      }
      p {
        text-align: right;
        color: #94a1b2;
      }
      #summary {
        width: 210px;
        color: #94a1b2;
      }
      td, th {
        width: 120px;
        text-align: center;
        padding: 2px;
      }
      table, th, td {
        border: 1px solid #7f5af0;
        border-collapse: collapse;
        color: #94a1b2;
      }
      tr:hover {background-color: #242629;}
      #tests {
        padding-top: 20px;
      }
      button:hover {background-color: #242629; color: #fffffe;} {
        padding: 5px;
      }
      .completed {
        display: none;
      }
      .completed_shown {
        display: block;
      }
      


      </style>
    </head>
    <body>
      <div id="main">
        <h2>Test summary<h2>
      </div>
      <div id="summary">
        <p>Tests total: ';
        $dbg++;
        $html = $html.$this->total;
        $html = $html.' </p>';
        $dbg++;
        $html = $html.'<p class="good">Passed: ';
          $html = $html.($this->total - $this->failed);
        $html = $html.' </p>';
        $dbg++;
        $html = $html.'<p class="bad">Failed: ';
          $html = $html.$this->failed;
        $html = $html.' </p>';
        $html = $html.'<p class="good">Success rate: ';
        $dbg++;
        if ($this->total != 0) {
            $html = $html.number_format((($this->total - $this->failed) / $this->total * 100), 1, '.', '');
        } $dbg++;
        $html = $html.'% </p>';
      $html = $html.'</div>
    <div id="button">
      <button type="button" id="tlacitko">Show passed tests</button>
    </div>
    <div id="tests">
    <table>
    <thead>
      <tr>
        <th>Number</th>
        <th>File</th>
        <th>parse.php</th>
        <th>interpret.py</th>
        <th>Expected</th>
        <th>Result</th>
      </tr>
    </thead>
    <tbody>';
    $dbg++;
    $test_counter = 0;

    foreach ($this->scanner->folders as $folder) { $dbg++;
      foreach($this->scanner->files[$folder] as $file) { $dbg++;
        $test_counter++;
        if ($file['completed']) {
          $html = $html.'<tr class="completed">';
        } else {
          $html = $html.'<tr>';
        } $dbg++;
        $html = $html.'<td>';
          $html = $html.$test_counter;
        $html = $html.'</td>'; $dbg++;
        $html = $html.'<td>';
          $html = $html.$file['name'].'.src';
        $html = $html.'</td>'; $dbg++;
        $html = $html.'<td>';
          $html = $html.$file['parser'];
        $html = $html.'</td>'; $dbg++;
        $html = $html.'<td>';
          $html = $html.$file['interpret'];
        $html = $html.'</td>'; $dbg++;
        $html = $html.'<td>';
          $html = $html.file_get_contents($folder.$file['name'].'.rc');
        $html = $html.'</td>'; $dbg++;
        $dbg++;
        if ($file['completed']) {
          $html = $html.'<td class="good">';
          $html = $html.'&#10004';
          $html = $html.'</td>';
        } else {
          $html = $html.'<td class="bad">';
          $html = $html.'&#10007';
          $html = $html.'</td>';
        } $dbg++;
        $html = $html.'</tr>';
      } $dbg++;
    } $dbg++;
    $dbg++;
    $html = $html.'</tbody></table></div>
    </body>
    <script>
      document.getElementById("tlacitko").onclick = function() {show_them_all()};
      var shown = false;
      function show_them_all() {
        if (!shown) {
          var x = document.getElementsByClassName("completed");
          for (i = 0; i < x.length; i++) {
            x[i].style.display = "table-row";
          } 
          document.getElementById("tlacitko").innerHTML = "Hide passed tests"
        } else {
          var x = document.getElementsByClassName("completed");
          for (i = 0; i < x.length; i++) {
            x[i].style.display = "none";
          }
          document.getElementById("tlacitko").innerHTML = "Show passed tests"
        }
        shown = !shown;
      }
      </script>
    </html>
    '; $dbg++;
    echo $html;
    $dbg++;
  }
}
?>