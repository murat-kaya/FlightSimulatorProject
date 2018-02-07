//Define Variables
var radius = 40;
var point_size = 4;
var center_x = 50;
var center_y = 50;
var font_size = "20px";

var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");


function drawCircle() {
  ctx.beginPath();
  ctx.arc(center_x, center_y, radius, 0, 2 * Math.PI);
  ctx.stroke();
}

//Calculate plane gyro position (Gyroscope calculation for 1100 LCD)
function drawPoint(angle, distance) {
  var x = center_x + radius * Math.cos(-angle * Math.PI / 180) * distance;
  var y = center_y + radius * Math.sin(-angle * Math.PI / 180) * distance;

  ctx.beginPath();
  ctx.moveTo(center_x, center_y);
  ctx.lineTo(x, y);
  ctx.stroke();



  ctx.font = font_size;
  var lbl = "x: " + Math.round(x) + " , y: " + Math.round(y);
  ctx.fillText(lbl, x + 10, y);
}

//Execution
drawCircle();
var planeAngleNow = 50;
drawPoint(planeAngleNow, 1);
drawPoint(180 + planeAngleNow, 1);
ctx.beginPath();
ctx.moveTo(center_x - radius, center_y);
ctx.lineTo(90, 50);
ctx.strokeStyle = "#FF0000";
ctx.stroke();
ctx.fillText("Horizon Line", 95, 52);
