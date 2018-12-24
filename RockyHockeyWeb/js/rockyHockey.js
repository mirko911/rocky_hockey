
var logElem = document.getElementById("log");
var settings = document.getElementById("settings");
var canvas = document.getElementById("canvas");
var puckPos = document.getElementById("puckPosition");
var puckDir = document.getElementById("puckDirection");
var predictedPosition = document.getElementById("predictedPosition");

function setHTML(elem, json, highlight = true){
  if(highlight)
    elem.innerHTML = syntaxHighlight(json) + "<br />\n";
  else
    elem.innerHTML = JSON.stringify(json) + "<br />\n";
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
};

function canvas_vectorDirection(context, position, direction, width, color){
  direction.x *= 1000;
  direction.y *= 1000;

  destination = {
    "x":position.x + direction.x,
    "y": position.y + direction.y
  };
  canvas_line(context, position, destination, width, color)
}

function drawCanvas(data){
  var ctx = canvas.getContext("2d");
  ctx.fillStyle = "#000000";
  ctx.fillRect(0, 0, 640, 480);

//Draw Puck
  canvas_circle(ctx, data.puck.position, 11, "red");
  canvas_vectorDirection(ctx, data.puck.position, data.puck.direction, 5, "green");
/*
//Draw Field
  let top_left = {"x": 1, "y": 1};
  let top_right = {"x": data.settings.field.width, "y": 1};
  let bottom_left = {"x": 1, "y": data.settings.field.height};
  let bottom_right = {"x": data.settings.field.width, "y": data.settings.field.height};

  canvas_line(ctx, top_left, top_right, 10, "white");
  canvas_line(ctx, top_left, bottom_left, 10, "white");
  canvas_line(ctx, bottom_left, bottom_right, 10, "yellow");
  canvas_line(ctx, top_right, bottom_right, 10, "orange");
*/
//Draw imaginary borders (walls, defendlines)
  canvas_line(ctx, data.prediction.defendLine.start, data.prediction.defendLine.end, 5, "yellow");
  for(i = 0; i < data.prediction.walls.length; i++){
    wall = data.prediction.walls[i];
    canvas_line(ctx, wall.start, wall.end, 5, "lightgreen");
  }

//Draw Wall Reflections
  for(i = 0; i < data.prediction.reflections.length; i++){
    prediction = data.prediction.reflections[i];
    canvas_vectorDirection(ctx, prediction.position, prediction.direction, 5, "lightblue");
  }
//Draw predicted puck position
  canvas_circle(ctx, data.prediction.predictedPosition, 10, "white");
}

function canvas_circle(context, position, radius, color){
  context.beginPath();
  context.fillStyle = color;
  context.arc(position.x * 2, position.y * 2, radius * 2, 0, 2 * Math.PI);
  context.fill();
  context.closePath();
}

function canvas_line(context, fromPosition, toPosition, lineWidth, color){
    fromPosition.x *=2;
    fromPosition.y *=2;
    toPosition.x *= 2;
    toPosition.y *= 2;

    context.beginPath();
    context.moveTo(fromPosition.x, fromPosition.y);
    context.lineTo(toPosition.x,toPosition.y);
    context.strokeStyle = color;
    context.lineWidth = lineWidth;
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
  setHTML(settings, data.settings);
  setHTML(puckPos, data.puck.position, false);
  setHTML(puckDir, data.puck.direction, false)
  setHTML(predictedPosition, data.prediction.predictedPosition, false);
  drawCanvas(data);
};

var ws;
function connect(){
  ws = new WebSocket("ws://127.0.0.1:9003");

  ws.onmessage = onMessage;
  ws.onopen = onOpen;
  ws.onClose = onClose;
  logAppend("Try to open websocket connection");
}

function disconnect(){
  ws.onclose = function () {}; // disable onclose handler first
  ws.close();
  logAppend("Closed Websocket connection");
}
