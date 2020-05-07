//"Adds on click functions to the buttons with that send commands via REST API"
document.getElementById("readfile").onclick = function () {
   $('#command').load('/arduino/readfile/');
   //alert('readfile command has been sent!');
   return false;
};
document.getElementById("decode").onclick = function () {
  $('#command').load('/arduino/decode/');
  //alert('decoding command has been sent!');
  return false;
};
document.getElementById("reset").onclick = function () {
  $('#LED_content').load('/arduino/reset/');
  //alert('reset command has been sent!');
  return false;
};
document.getElementById("read").onclick = function () {
  $('#command').load('/arduino/read/');
  //alert('read command has been sent!');
  return false;
};
document.getElementById("play").onclick = function () {
  $('#command').load('/arduino/play/');
  //alert('play command has been sent!');
  return false;
};
document.getElementById("readstack").onclick = function () {
  $('#command').load('/arduino/display_stack/');
  //alert('play command has been sent!');
  return false;
};