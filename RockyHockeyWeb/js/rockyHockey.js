
var logElem = document.getElementById("log");
var settings = document.getElementById("settings");
var canvas = document.getElementById("canvas");

function setSettings(settingsJson){
  settings.innerHTML = syntaxHighlight(settingsJson) + "<br />\n";
}
function logAppend(string){
  logElem.innerHTML = string + "<br />\n" + logElem.innerHTML;
}

function syntaxHighlight(json) {
    if (typeof json != 'string') {
         json = JSON.stringify(json, undefined, 2);
    }
    json = json.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return json.replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g, function (match) {
        var cls = 'number';
        if (/^"/.test(match)) {
            if (/:$/.test(match)) {
                cls = 'key';
            } else {
                cls = 'string';
            }
        } else if (/true|false/.test(match)) {
            cls = 'boolean';
        } else if (/null/.test(match)) {
            cls = 'null';
        }
        return '<span class="' + cls + '">' + match + '</span>';
    });
}

function drawCanvas(data){
  var ctx = canvas.getContext("2d");
  ctx.fillStyle = "#000000";
  ctx.fillRect(0, 0, 640, 480);

  canvas_circle(ctx, data.puck.position, 11, "red");

  destination = {
    "x" : data.puck.position.x + (data.puck.direction.x * 1000),
    "y" : data.puck.position.y + (data.puck.direction.y * 1000),
  }
  console.log(destination);
  canvas_arrow(ctx, data.puck.position, destination, 10, "green");
}

function canvas_circle(context, position, radius, color){
  context.beginPath();
  context.fillStyle = color;
  context.arc(position.x * 2, position.y * 2, radius * 2, 0, 2 * Math.PI);
  context.fill();
  context.closePath();
}

function canvas_arrow(context, fromPosition, toPosition, r,  color){
    fromPosition.x *=2;
    fromPosition.y *=2;
    toPosition.x *= 2;
    toPosition.y *= 2;

    context.beginPath();
    context.moveTo(fromPosition.x, fromPosition.y);
    context.lineTo(toPosition.x,toPosition.y);
    context.strokeStyle = color;
    context.lineWidth = 5;
    context.stroke();
    context.closePath();
}

function onOpen(e){
  logAppend("Etablished connection");
};

function onClose(e){
  logAppend("Lost the websocket connection");
}

function onMessage(e) {
  data = JSON.parse(e.data);
  setSettings(data);
  drawCanvas(data);
};

logAppend("Try to open websocket connection");

let ws = new WebSocket("ws://127.0.0.1:9003");

ws.onmessage = onMessage;
ws.onopen = onOpen;
ws.onClose = onClose;
