let program;
let vBuffer, cubeBuffer, vPosition;
let bVertices = get_vertices();
let bFaces = get_faces();
let bFlatVertices = flatten(bVertices);
let bFlatFaces = flatten(bFaces);
let bVertFaces = [];

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

// Projection Matrix and ModelView Matrix
let projectionMatrixLoc, modelViewMatrixLoc, modelViewMatrix, origModelViewMatrix;
let modelViewMatrixCube, modelViewMatrixLLoc;
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

// Constants
const at = vec3(0.0, 0.0, 0.0);
const up = vec3(0.0, 1.0, 0.0);

//
let la1 = 0.2;
let la2 = 0.2;
let la3 = 0.2;
let la4 = 1.0;
let ld1 = 1.0;
let ld2 = 1.0;
let ld3 = 1.0;
let ld4 = 1.0;
let ls1 = 1.0;
let ls2 = 1.0;
let ls3 = 1.0;
let ls4 = 1.0;
let ma1 = 1.0;
let ma2 = 0.0;
let ma3 = 1.0;
let ma4 = 1.0;
let md1 = 1.0;
let md2 = 0.8;
let md3 = 0.0;
let md4 = 1.0;
let ms1 = 1.0;
let ms2 = 0.8;
let ms3 = 0.0;
let ms4 = 1.0;
let shine = 1.0;

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
}

window.onload = function init() {

    // WebGL Inits
    canvas = document.getElementById("canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    if ( !gl ) { alert( "WebGL isn't available" ); }
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.0, 0.0, 0.0, 1);
    gl.enable(gl.DEPTH_TEST);
    gl.enable(gl.CULL_FACE);

    //  Load shaders and initialize attribute buffers
    program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);
    color = gl.getUniformLocation(program, "fColor");

    // Bind buffer
    vBuffer = gl.createBuffer();
    cubeBuffer = gl.createBuffer();  
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeBuffer);

     // Associate out shader letiables with our data buffer
    vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);
    projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");
    modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");
    modelViewMatrixLLoc = gl.getUniformLocation(program, "modelViewMatrixL");

    //Construct complete bunny array of vertices and indices
    for (let i = 0; i < bFlatFaces.length; i++) {
        bFlatFaces[i]--;
        bVertFaces.push(bVertices[bFlatFaces[i]]);
    }

    eye = vec3(eyex, eyey, eyez);
    modelViewMatrix = lookAt(eye, at, up);
    modelViewMatrixCube = lookAt(eye, at, up);
    modelViewMatrixL = mat4();
    origModelViewMatrix = modelViewMatrix;

    render();
};

let lightRot = 0;
function render() {
    gl.uniform4f(color, 0, 1, 1, 1);
    let projectionMatrix = perspective(fovy, aspect, near, far);

    //Part (e) realistic bunny
    eye = vec3(5, 5, 0);
    modelViewMatrixL = lookAt(eye, at, up);
    modelViewMatrixL = mult(modelViewMatrixL, rotate(-lightRot, [0, 1, 0]));
    modelViewMatrixL = mult(modelViewMatrixL, translate(5, -5, 0));
    gl.uniformMatrix4fv(modelViewMatrixLLoc, false, flatten(modelViewMatrixL));

    let is_light = 0;
    gl.uniform1i(gl.getUniformLocation(program,"is_light"), is_light);

    lightAmbient = vec4(la1, la2, la3, la4);
    lightDiffuse = vec4(ld1, ld2, ld3, ld4);
    lightSpecular = vec4(ls1, ls2, ls3, ls4);
    materialAmbient = vec4(ma1, ma2, ma3, ma4);
    materialDiffuse = vec4(md1, md2, md3, md4);
    materialSpecular = vec4(ms1, ms2, ms3, ms4);
    materialShininess = shine;

    ambientProduct = mult(lightAmbient, materialAmbient);
    diffuseProduct = mult(lightDiffuse, materialDiffuse);
    specularProduct = mult(lightSpecular, materialSpecular);

    gl.uniform4fv(gl.getUniformLocation(program, "ambientProduct"),flatten(ambientProduct));
    gl.uniform4fv(gl.getUniformLocation(program, "diffuseProduct"),flatten(diffuseProduct));
    gl.uniform4fv(gl.getUniformLocation(program, "specularProduct"), flatten(specularProduct));
    gl.uniform1f(gl.getUniformLocation(program, "shininess"), materialShininess);
    gl.uniform1i(gl.getUniformLocation(program, "is_light"), is_light);

    //Part (d) auto rotating light cube
    eye = vec3(eyex, eyey, eyez);
    modelViewMatrixCube = lookAt(eye, at, up);
    if (lightRotation)
    	lightRot++;
    modelViewMatrixCube = mult(modelViewMatrixCube, rotate(lightRot, [0, 1, 0]));
    modelViewMatrixCube = mult(modelViewMatrixCube, translate(5, 5, 0));
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrixCube));
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cubeV), gl.STATIC_DRAW);
    gl.drawArrays(gl.LINE_STRIP, 0, cubeV.length);

    // //Draw bunny
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));
    gl.bufferData(gl.ARRAY_BUFFER, flatten(bVertFaces), gl.STATIC_DRAW);
    gl.drawArrays(gl.TRIANGLES, 0, bVertFaces.length);

    requestAnimFrame(render);
};