var nightstart = 0;
var nightstop = 0;

function logVariables() {
	console.log("	nightstart: " + nightstart);
	console.log("	nightstop: " + nightstop);
}

Pebble.addEventListener("ready", function() {
	console.log("Ready Event");
	
	nightstart = localStorage.getItem("nightstart");
	if (!nightstart) {
		nightstart = 19;
	}

	nightstop = localStorage.getItem("nightstop");
	if (!nightstop) {
		nightstop = 7;
	}

	logVariables();
						
	Pebble.sendAppMessage(JSON.parse('{"nightstart":'+nightstart+',"nightstop":'+nightstop+'}'));

});

Pebble.addEventListener("showConfiguration", function(e) {
	console.log("showConfiguration Event");

	logVariables();
						
	Pebble.openURL("http://www.famillemattern.com/jnm/pebble/Perspective/Perspective_1.1.1.php?nightstart=" + nightstart +
				   "&nightstop=" + nightstop);
});

Pebble.addEventListener("webviewclosed", function(e) {
	console.log("Configuration window closed");
	console.log(e.type);
	console.log(e.response);

	var configuration = JSON.parse(e.response);
	Pebble.sendAppMessage(configuration);
						
	nightstart = configuration["nightstart"];
	localStorage.setItem("nightstart", nightstart);

	nightstop = configuration["nightstop"];
	localStorage.setItem("nightstop", nightstop);
});
