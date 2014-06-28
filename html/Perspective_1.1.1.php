<!DOCTYPE html>
<html>
        <head>
                <title>Perspective</title>
                <meta charset='utf-8'>
                <meta name='viewport' content='width=device-width, initial-scale=1'>
                <link rel='stylesheet' href='http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.css' />
                <script src='http://code.jquery.com/jquery-1.9.1.min.js'></script>
                <script src='http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.js'></script>
                <style>
                        .ui-header .ui-title { margin-left: 1em; margin-right: 1em; text-overflow: clip; }
                </style>
        </head>
		<body>
<div data-role="page" id="page1">
    <div data-theme="a" data-role="header" data-position="fixed">
        <h3>
            Perspective Configuration
        </h3>
        <div class="ui-grid-a">
            <div class="ui-block-a">
                <input id="cancel" type="submit" data-theme="c" data-icon="delete" data-iconpos="left"
                value="Cancel" data-mini="true">
            </div>
            <div class="ui-block-b">
                <input id="save" type="submit" data-theme="b" data-icon="check" data-iconpos="right"
                value="Save" data-mini="true">
            </div>
        </div>
    </div>
    <div data-role="content">
<?php
	if (!isset($_GET['nightstart'])) {
		$nightstart = 19;
	} else {
		$nightstart = $_GET['nightstart'];
	}

	if (!isset($_GET['nightstop'])) {
		$nightstop = 7;
	} else {
		$nightstop = $_GET['nightstop'];
	}
?>
      <div data-role="rangeslider">
<legend>Daytime Range<br />Choose the same value for both to disable night mode :</legend>
<?php
  echo '<input type="range" name="nightstop" id="nightstop" min="0" max="24" value="' . $nightstop . '">';
  echo '<input type="range" name="nightstart" id="nightstart" min="0" max="24" value="' . $nightstart . '">';
?>
      </div>
	</div>
</div>

    <script>
      function saveOptions() {
        var options = {
			'nightstart': parseInt($("#nightstart").val(), 10),
			'nightstop': parseInt($("#nightstop").val(), 10),
        }
        return options;
      }

      $().ready(function() {
        $("#cancel").click(function() {
          console.log("Cancel");
          document.location = "pebblejs://close#";
        });

        $("#save").click(function() {
          console.log("Submit");
          
          var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(saveOptions()));
          console.log("Close: " + location);
          console.log(location);
          document.location = location;
        });

      });
    </script>
</body>
</html>
