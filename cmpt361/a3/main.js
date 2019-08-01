let canvas, gl, color, program, myRobot, mySphere;
let mouseX, mouseY, mouseX2, mouseY2;
let modelViewMatrix, projectionMatrix;
let theta= [0, 0, 0];
let angle = 0;
let modelViewMatrixLoc;
let vBuffer, sBuffer, cBuffer;

scale4 = function(a, b, c) {
    let result = mat4();
    result[0][0] = a;
    result[1][1] = b;
    result[2][2] = c;
    return result;
}

function robotArm() {
    this.BASE_HEIGHT = 2.0;
    this.BASE_WIDTH = 4.0;
    this.UPPER_ARM_HEIGHT = 3.0;
    this.LOWER_ARM_HEIGHT = 4.0;
    this.UPPER_ARM_WIDTH = 0.5;
    this.LOWER_ARM_WIDTH = 0.75;
    this.points = [];
    this.verts = [
        vec4( -0.5, -0.5,  0.5, 1.0 ),
        vec4( -0.5,  0.5,  0.5, 1.0 ),
        vec4(  0.5,  0.5,  0.5, 1.0 ),
        vec4(  0.5, -0.5,  0.5, 1.0 ),
        vec4( -0.5, -0.5, -0.5, 1.0 ),
        vec4( -0.5,  0.5, -0.5, 1.0 ),
        vec4(  0.5,  0.5, -0.5, 1.0 ),
        vec4(  0.5, -0.5, -0.5, 1.0 )
    ];
    this.quad = function(a, b, c, d) {
        this.points.push(this.verts[a]);
        this.points.push(this.verts[b]);
        this.points.push(this.verts[c]);
        this.points.push(this.verts[a]);
        this.points.push(this.verts[c]);
        this.points.push(this.verts[d]);
    }
    this.colorCube = function() { //Shape of cube
        this.quad( 1, 0, 3, 2 );
        this.quad( 2, 3, 7, 6 );
        this.quad( 3, 0, 4, 7 );
        this.quad( 6, 5, 1, 2 );
        this.quad( 4, 5, 6, 7 );
        this.quad( 5, 4, 0, 1 );
    }
    this.applyTransform = function(instanceMatrix, modelViewMatrix, modelViewMatrixLoc) {
        let t = mult(modelViewMatrix, instanceMatrix);
        gl.uniformMatrix4fv(modelViewMatrixLoc,  false, flatten(t));
        gl.bufferData(gl.ARRAY_BUFFER, flatten(this.points), gl.STATIC_DRAW);
        gl.drawArrays(gl.TRIANGLES, 0, this.points.length);
    }
    this.base = function(modelViewMatrix, modelViewMatrixLoc) {
        let s = scale4(this.BASE_WIDTH, this.BASE_HEIGHT, this.BASE_WIDTH);
        let instanceMatrix = mult(translate( 0.0, 0.5 * this.BASE_HEIGHT, 0.0 ), s);
        this.applyTransform(instanceMatrix, modelViewMatrix, modelViewMatrixLoc);
    }
    this.upperArm = function(modelViewMatrix, modelViewMatrixLoc) {
        let s = scale4(this.UPPER_ARM_WIDTH, this.UPPER_ARM_HEIGHT, this.UPPER_ARM_WIDTH);
        let instanceMatrix = mult(translate( 0.0, 0.5 * this.UPPER_ARM_HEIGHT, 0.0 ), s);
        this.applyTransform(instanceMatrix, modelViewMatrix, modelViewMatrixLoc);
    }
    this.lowerArm = function(modelViewMatrix, modelViewMatrixLoc) {
        let s = scale4(this.LOWER_ARM_WIDTH, this.LOWER_ARM_HEIGHT, this.LOWER_ARM_WIDTH);
        let instanceMatrix = mult( translate( 0.0, 0.5 * this.LOWER_ARM_HEIGHT, 0.0 ), s);
        this.applyTransform(instanceMatrix, modelViewMatrix, modelViewMatrixLoc);
    }
}
function sphere() {
    this.verts = [];
    this.detail = 1000;
    this.r = 1.0;
    this.size = 1.0;
    this.bands = 500;
    this.xOffset = -10;
    this.yOffset = 8;
    this.init = function() {
        for (let latNumber = 0; latNumber <= this.bands; ++latNumber) {
          let theta = latNumber * Math.PI / this.bands;
          let sinTheta = Math.sin(theta);
          let cosTheta = Math.cos(theta);

          for (let longNumber = 0; longNumber <= this.bands; ++longNumber) {
            let phi = longNumber * 2 * Math.PI / this.bands;
            let sinPhi = Math.sin(phi);
            let cosPhi = Math.cos(phi);

            let x = this.r * cosPhi * sinTheta;
            let y = this.r * cosTheta;
            let z = this.r * sinPhi * sinTheta;

            this.verts.push(vec4(x, y, z, 1));
          }
        }
    }
    this.draw = function(modelViewMatrix, modelViewMatrixLoc) {
        let s = scale4(this.size, this.size, this.size);
        let instanceMatrix;
        if (mouseX2 && mouseY2) {
        	instanceMatrix = mult(translate(mouseX2/25 + this.xOffset, -mouseY2/25 + this.yOffset, 0.0), s);
        }
        else {
        	instanceMatrix = mult(translate(5, 3, 0), s);
        	mouseX2 = undefined;
        	mouseY2 = undefined;
        }
        let t = mult(mat4(), instanceMatrix);
        gl.uniformMatrix4fv(modelViewMatrixLoc,  false, flatten(t));
        gl.bufferData(gl.ARRAY_BUFFER, flatten(this.verts), gl.STATIC_DRAW);
        gl.drawArrays(gl.LINES, 0, this.verts.length);
    }
}

function mousePos(e) {
	mouseX = e.clientX;
	mouseY = e.clientY;
}
function click(e) {
	mouseX2 = mouseX;
	mouseY2 = mouseY;
	mySphere.draw(modelViewMatrix, modelViewMatrixLoc);
}

window.onload = function init() {

    canvas = document.getElementById("canvas");
    canvas.onmousemove = mousePos;
    canvas.onclick = click;
    mouseX2 = undefined;
    mouseY2 = undefined;

    gl = WebGLUtils.setupWebGL(canvas);
    if ( !gl ) { alert( "WebGL isn't supported" ); }

    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.enable(gl.DEPTH_TEST);
    gl.enable(gl.CULL_FACE);

    program = initShaders(gl, "vertex-shader", "fragment-shader" );
    gl.useProgram(program);

    myRobot = new robotArm();
    myRobot.colorCube();
    mySphere = new sphere();
    mySphere.init();

    vBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);

    let vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray(vPosition);

    let vColor = gl.getAttribLocation(program, "vColor");
    gl.vertexAttribPointer(vColor, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vColor);

    document.getElementById("slider1").onchange = function(event) {
        theta[0] = event.target.value;
    };
    document.getElementById("slider2").onchange = function(event) {
         theta[1] = event.target.value;
    };
    document.getElementById("slider3").onchange = function(event) {
         theta[2] =  event.target.value;
    };


    modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");
    projectionMatrix = ortho(-10, 10, -10, 10, -10, 10);
    gl.uniformMatrix4fv(gl.getUniformLocation(program, "projectionMatrix"),  false, flatten(projectionMatrix));

    render();
}


let i = 0;

let render = function() {

    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );

    modelViewMatrix = rotate(theta[0], 0, 1, 0 );
    
     //Robot arm base
    myRobot.base(modelViewMatrix, modelViewMatrixLoc);

    // //Robot lower arm
    modelViewMatrix = mult(modelViewMatrix, translate(0.0, myRobot.BASE_HEIGHT, 0.0));
    modelViewMatrix = mult(modelViewMatrix, rotate(theta[1], 0, 0, 1 ));
    myRobot.lowerArm(modelViewMatrix, modelViewMatrixLoc);

    // //Robot upper arm
    modelViewMatrix  = mult(modelViewMatrix, translate(0.0, myRobot.LOWER_ARM_HEIGHT, 0.0));
    modelViewMatrix  = mult(modelViewMatrix, rotate(theta[2], 0, 0, 1));
    myRobot.upperArm(modelViewMatrix, modelViewMatrixLoc);

    // Ball
    //mySphere.draw(modelViewMatrix, modelViewMatrixLoc);

    requestAnimFrame(render);
}
