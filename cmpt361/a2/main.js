let program;
let vBuffer, cubeBuffer, coneBuffer, normalBuffer, vPosition;
let normalAttribLoc;
let bVertices = get_vertices();
let bFaces = get_faces();
let bFlatVertices = flatten(bVertices);
let bFlatFaces = flatten(bFaces);
let bVertFaces = [];
let normalVertFaces = [];

const cubeVLen = 0.25;
const cubeV = [
  // Front face
  vec3(-cubeVLen, -cubeVLen,  cubeVLen),
  vec3(cubeVLen, -cubeVLen,  cubeVLen),
  vec3(cubeVLen,  cubeVLen,  cubeVLen),
  vec3(cubeVLen,  cubeVLen,  cubeVLen),
  
  // Back face
  vec3(-cubeVLen, -cubeVLen, -cubeVLen),
  vec3(-cubeVLen,  cubeVLen, -cubeVLen),
   vec3(cubeVLen,  cubeVLen, -cubeVLen),
   vec3(cubeVLen, -cubeVLen, -cubeVLen),
  
  // Top face
  vec3(-cubeVLen,  cubeVLen, -cubeVLen),
  vec3(-cubeVLen,  cubeVLen,  cubeVLen),
   vec3(cubeVLen,  cubeVLen,  cubeVLen),
   vec3(cubeVLen,  cubeVLen, -cubeVLen),
  
  // Bottom face
  vec3(-cubeVLen, -cubeVLen, -cubeVLen),
  vec3( cubeVLen, -cubeVLen, -cubeVLen),
   vec3(cubeVLen, -cubeVLen,  cubeVLen),
  vec3(-cubeVLen, -cubeVLen,  cubeVLen),
  
  // Right face
   vec3(cubeVLen, -cubeVLen, -cubeVLen),
   vec3(cubeVLen,  cubeVLen, -cubeVLen),
   vec3(cubeVLen,  cubeVLen,  cubeVLen),
   vec3(cubeVLen, -cubeVLen,  cubeVLen),
  
  // Left face
  vec3(-cubeVLen, -cubeVLen, -cubeVLen),
  vec3(-cubeVLen, -cubeVLen,  cubeVLen),
  vec3(-cubeVLen,  cubeVLen,  cubeVLen),
  vec3(-cubeVLen,  cubeVLen, -cubeVLen)
];
const coneV =[
    vec3(1.5, 0, 0), 
    vec3(-1.5, 1, 0), 
    vec3(-1.5, 0.809017, 0.587785),
    vec3(-1.5, 0.309017, 0.951057), 
    vec3(-1.5, -0.309017, 0.951057), 
    vec3(-1.5, -0.809017, 0.587785),
    vec3(-1.5, -1, 0), 
    vec3(-1.5, -0.809017, -0.587785),
    vec3(-1.5, -0.309017, -0.951057), 
    vec3(-1.5, 0.309017, -0.951057), 
    vec3(-1.5, 0.809017, -0.587785)];

// Projection Matrix and ModelView Matrix
let projectionMatrixLoc, modelViewMatrixLoc, cubeLightLoc, coneLightLoc, modelViewMatrix, origModelViewMatrix;
let modelViewMatrixCube, modelViewMatrixCone;
let fovy = 60.0;
let far = 30.0;
let near = 0.1;
let aspect = 1.0;

// Initial camera perspective
let eye;
let eyex = 0.0;
let eyey = 0.0;
let eyez = 10.0;

// Translations
let prevX = 0;
let prevY = 0;
let x = 0.05;
let y = 0.05; 
let z = 0.5;

// Rotations
let rotX = 1.5;
let rotY = 1.5;
let lightRotation = true;
let coneRotation = true;

// Constants
const at = vec3(0.0, 0.0, 0.0);
const up = vec3(0.0, 1.0, 0.0);

// Part (b and c) x, y translations + rotations
let leftDown = false;
let rightDown = false;
canvas.addEventListener('mousedown', function(e) {
  if(e.button == 0)
  	leftDown = true;
  if (e.button == 2)
  	rightDown = true;
},false);
canvas.addEventListener('mousemove', function(e) {
 if (leftDown) {
    // Translate on x axis
    if (e.clientX > prevX)
        modelViewMatrix = mult(modelViewMatrix, translate(x, 0, 0));
    else if (e.clientX  < prevX) 
        modelViewMatrix = mult(modelViewMatrix, translate(-x, 0, 0));

    // Translate on y axis
    if (e.clientY > prevY)
    	modelViewMatrix = mult(modelViewMatrix, translate(0, -y, 0));
    else if (e.clientY < prevY) 
    	modelViewMatrix = mult(modelViewMatrix, translate(0, y, 0));


    prevX = e.clientX;
    prevY = e.clientY;
 }
 if (rightDown) {
 	//Rotate on x axis
 	if (e.clientX > prevX)
 		modelViewMatrix = mult(modelViewMatrix, rotate(rotX, [0, 1, 0]));
 	else if (e.clientX < prevX)
 		modelViewMatrix = mult(modelViewMatrix, rotate(-rotX, [0, 1, 0]));

 	//Rotate on y axis
 	if (e.clientY > prevY)
    	modelViewMatrix = mult(modelViewMatrix, rotate(rotY, [1, 0, 0]));
 	else if (e.clientY < prevY)
 		modelViewMatrix = mult(modelViewMatrix, rotate(-rotY, [1, 0, 0]));

 	prevX = e.clientX;
 	prevY = e.clientY;
 }
},false);
canvas.addEventListener('mouseup',function() {
    leftDown = false;
    rightDown = false;
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
    // 'r' Part (c) for resetting position and orientation
    if (e.keyCode == '82') {
    	modelViewMatrix = origModelViewMatrix;
    }

    if (e.keyCode == '80') {
    	if (lightRotation)
    		lightRotation = false
    	else
    		lightRotation = true
    }

    if (e.keyCode == '83') {
        if (coneRotation)
            coneRotation = false;
        else
            coneRotation = true;
    }
}

window.onload = function init() {

    // WebGL Inits
    canvas = document.getElementById("canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    if ( !gl ) { alert( "WebGL isn't available" ); }
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.0, 1.0, 0.0, 1);
    gl.enable(gl.DEPTH_TEST);
    gl.enable(gl.CULL_FACE);

    //  Load shaders and initialize attribute buffers
    program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);
    //color = gl.getUniformLocation(program, "fColor");

    // Bind buffer
    vBuffer = gl.createBuffer();
    cubeBuffer = gl.createBuffer();  
    coneBuffer = gl.createBuffer();
    normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeBuffer);
    gl.bindBuffer(gl.ARRAY_BUFFER, coneBuffer);
    gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);
    normalAttribLoc = gl.getAttribLocation(program, 'vertNormal');
    gl.vertexAttribPointer(normalAttribLoc, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(normalAttribLoc);

     // Associate out shader letiables with our data buffer
    vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);
    projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");
    modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");
    cubeLightLoc = gl.getUniformLocation(program, 'sunlightDir');
    coneLightLoc = gl.getUniformLocation(program, 'conelightDir');


    //Construct complete bunny array of vertices and indices

    for (let i = 0; i < bFlatFaces.length; i++) {
        bFlatFaces[i]--;
        bVertFaces.push(bVertices[bFlatFaces[i]]);
        let b = [...bVertices[bFlatFaces[i]]];
        normalVertFaces.push(normalize(b));
    }

    eye = vec3(eyex, eyey, eyez);
    modelViewMatrix = lookAt(eye, at, up);
    modelViewMatrixCube = lookAt(eye, at, up);
    modelViewMatrixCone = lookAt(eye, at, up);
    origModelViewMatrix = modelViewMatrix;
    for (let i = 0; i < coneV.length; i++) {
        coneV[i][0] = coneV[i][0]/3;
        coneV[i][1] = coneV[i][1]/3;
        coneV[i][2] = coneV[i][2]/3;
    }
    modelViewMatrixCone = mult(modelViewMatrixCone, translate(0, 4, 2));
    modelViewMatrixCone = mult(modelViewMatrixCone, rotate(270, [0,0,1]));
    render();
};

let lightRot = 0;
let i = 1;
let j = 30;
let rot = 1;
function render() {
    let projectionMatrix = perspective(fovy, aspect, near, far);

    //Part (f) realistic bunny
    gl.bufferData(gl.ARRAY_BUFFER, flatten(normalVertFaces), gl.STATIC_DRAW);

    //Part (d) auto rotating light cube
    eye = vec3(eyex, eyey, eyez);
    modelViewMatrixCube = lookAt(eye, at, up);
    if (lightRotation)
    	lightRot++;
    modelViewMatrixCube = mult(modelViewMatrixCube, rotate(lightRot, [0, 1, 0]));
    modelViewMatrixCube = mult(modelViewMatrixCube, translate(5, 5, 0));
    gl.uniform3f(cubeLightLoc, modelViewMatrixCube[0][0], modelViewMatrixCube[0][1], modelViewMatrixCube[0][2]);
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrixCube));
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cubeV), gl.STATIC_DRAW);
    gl.drawArrays(gl.LINE_STRIP, 0, cubeV.length);

    //Part (e) cone
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrixCone));
    if (coneRotation) {
        if (i % j == 0) {
            rot = -rot;
            i = 0;
            j = 60;
        }
        i++;
        modelViewMatrixCone = mult(modelViewMatrixCone, rotate(rot, [0,0,1]));
    }
    gl.uniform3f(coneLightLoc, modelViewMatrixCone[0][0], modelViewMatrixCone[0][1], modelViewMatrixCone[0][2]);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(coneV), gl.STATIC_DRAW);
    gl.drawArrays(gl.LINE_LOOP, 0, coneV.length);

    // //Draw bunny
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));
    gl.bufferData(gl.ARRAY_BUFFER, flatten(bVertFaces), gl.STATIC_DRAW);
    gl.drawArrays(gl.TRIANGLES, 0, bVertFaces.length);

    requestAnimFrame(render);
};