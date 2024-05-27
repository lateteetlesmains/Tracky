document.addEventListener("DOMContentLoaded", function() {
    var slider_1 = document.getElementById("servoSlider_1");
	var slider_2 = document.getElementById("servoSlider_2");
	
    var servoP1 = document.getElementById("servoPos1");
	var servoP2 = document.getElementById("servoPos2");
	
    servoP1.innerHTML = slider_1.value;
	servoP2.innerHTML = slider_2.value;
	
    slider_1.oninput = function() {
        slider_1.value = this.value;
		servoP1.innerHTML = this.value;
				
    }
	
	 slider_2.oninput = function() {
        slider_2.value = this.value;
		servoP2.innerHTML = this.value;
    }

    $.ajaxSetup({timeout: 1000});

  
function servo_1(pos) {
    $.get("/position_servo_1?value=" + pos + "&", {Connection: close});
}
function servo_2(pos) {
    $.get("/position_servo_2?value=" + pos + "&", {Connection: close});
}
    slider_1.addEventListener("change", function() {
        servo_1(this.value);
    });
	slider_2.addEventListener("change", function() {
        servo_2(this.value);
    });
});
