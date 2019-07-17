let vBuffer, vPosition;
let bVertices = get_vertices();
let bFaces = get_faces();
let bFlatVertices = flatten(bVertices);
let bFlatFaces = flatten(bFaces);
let bVertFaces = [];

// Projection Matrix and ModelView Matrix
let projectionMatrixLoc, modelViewMatrixLoc, modelViewMatrix;
let fovy = 60.0;
let far = 30.0;
let near = 0.1;
let aspect = 1.0;

// Initial camera perspective
let eyex = 0.0;
let eyey = 0.0;
let eyez = 10.0;

// Translations
let prevX = 0;
let prevY = 0; 
let x = 0.05;
let y = 0.05; 
let z = 1;

// Constants
const at = vec3(0.0, 0.0, 0.0);
const up = vec3(0.0, 1.0, 0.0);

// Part (b) x, y translations
var isMouseDown=false;
canvas.addEventListener('mousedown', function() {
    isMouseDown=true
},false);
canvas.addEventListener('mousemove', function(e) {
 if(isMouseDown) {
    // Translate on x axis
    if (e.clientX > prevX)
        modelViewMatrix = mult(modelViewMatrix, translate(x, 0, 0));
    else if (e.clientX < prevX) 
        modelViewMatrix = mult(modelViewMatrix, translate(-x, 0, 0));

    // Translate on y axis
    if (e.clientY > prevY)
        modelViewMatrix = mult(modelViewMatrix, translate(0, -y, 0));
    else if (e.clientY < prevY) 
        modelViewMatrix = mult(modelViewMatrix, translate(0, y, 0));

    prevX = e.clientX;
    prevY = e.clientY;
 }
},false);
canvas.addEventListener('mouseup',function () {
    isMouseDown = false;
},false);

window.onkeydown = function check_key(e) {
    e = e || window.event;

    // Part (b) z translation
    // Up arrow
    if (e.keyCode == '38') {
        modelViewMatrix = mult(modelViewMatrix, translate(0, 0, z));
    }
    // Down arrow
    if (e.keyCode == '40') {
        modelViewMatrix = mult(modelViewMatrix, translate(0, 0, -z));
    }
}

window.onload = function init() {

    // WebGL Inits
    canvas = document.getElementById("canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    if ( !gl ) { alert( "WebGL isn't available" ); }
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.enable(gl.CULL_FACE);

    //  Load shaders and initialize attribute buffers
    let program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);
    color = gl.getUniformLocation(program, "fColor");

    // Bind buffer
    vBuffer = gl.createBuffer();  
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);

     // Associate out shader letiables with our data buffer
    vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);
    projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");
    modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");

    //Construct complete bunny array of vertices and indices
    for (let i = 0; i < bFlatFaces.length; i++) {
        bFlatFaces[i]--;
        bVertFaces.push(bVertices[bFlatFaces[i]]);
    }

    let eye = vec3(eyex, eyey, eyez);
    modelViewMatrix = lookAt(eye, at, up);
    render();
};

function render() {
    gl.uniform4f(color, 0, 1, 0, 1);
    let projectionMatrix = perspective(fovy, aspect, near, far);

    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));

    gl.bufferData(gl.ARRAY_BUFFER, flatten(bVertFaces), gl.STATIC_DRAW);
    gl.drawArrays(gl.TRIANGLES, 0, bVertFaces.length);

    requestAnimFrame(render);
};