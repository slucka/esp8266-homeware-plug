
var connection = null;


window.onload = function () {


	// var joySpeed = new JoyStick('joySpeed');
	// var joyDir = new JoyStick('joyDir');

	// var joySpeedVal = 0;
	// var joyDirVal = 0;

	// setInterval(function(){ 
	// 	var y = joySpeed.GetY(); 
	// 	var x = joyDir.GetX(); 
	// 	var valSpeed = Math.trunc(y/10);
	// 	var valDir = Math.trunc(x/10);
	// 	if (valSpeed != joySpeedVal || valDir != joyDirVal){
	// 		joySpeedVal = valSpeed;
	// 		joyDirVal = valDir;
	// 		console.log('valSpeed: ', valSpeed, ', valDir: ', valDir);
	// 		sendData('rg01', 'down', valSpeed, valDir);
	// 	}

	// }, 50);

	$('#btnSave').click(function () {
		///TODO VALIDAR
		let data = {
			command: "Set",
			deviceid: $("#txtDeviceId").val(),
			ssid: $("#txtSSID").val(),
			password: $("#txtPass").val(),
			mqtt_server: $("#txtMqttServer").val(),
		}

		callWs(data);
	});

	$('#btnReset').click(function () {
		///TODO VALIDAR
		let data = {
			command: "Reset",
		}

		callWs(data);
	});
	

	connectWs();


	setTimeout(function () {
		let data = {
			command: "Get",
		}
		callWs(data, function(json){
			let result = JSON.parse(json);
			//console.log(json);
			$("#txtDeviceId").val(result.deviceid);
			$("#txtSSID").val(result.ssid);
			$("#txtPass").val(result.password);
			$("#txtMqttServer").val(result.mqtt_server);
		});
	}, 50);

};



function connectWs() {

	connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);


	connection.onopen = function () {
		console.log('Connected: ');
		//$(".btA").removeClass('btOff');
		//$(".btA").addClass('btOn');
	};

	connection.onerror = function (error) {
		console.log('WebSocket Error ', error);
	};

	connection.onmessage = function (e) {
		console.log('Server: ', e.data);
		//processData(e.data);
		if (connection.fnReturn){
			connection.fnReturn(e.data);
		}
	};

	connection.onclose = function () {
		console.log('WebSocket connection closed');
		//$(".btA").removeClass('btOn');
		//$(".btA").addClass('btOff');
	};

}



// function sendData(data) {

// 	let json = JSON.stringify(data);

// 	connection.send(json);
// }

// function getData(data, fnReturn) {

// 	let json = JSON.stringify(data);
	
// 	connection.fnReturn = fnReturn;
// 	connection.send(json);
// }

function callWs(data, fnReturn) {

	let json = JSON.stringify(data);
	
	connection.fnReturn = fnReturn;
	connection.send(json);
}


// function processData(data) {
// 	let json = JSON.parse(data);
// 	console.log(json);
// }

