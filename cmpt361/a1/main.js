let canvas;
let gl;
const cannon_speed = 0.05;
const square_size = 0.1

// Three square        
let square = [
        vec2(0,0),
        vec2(0,square_size),

        vec2(0,0),
        vec2(square_size,0),

        vec2(0,square_size),
        vec2(square_size,square_size),

        vec2(square_size,square_size),
        vec2(square_size,0) 
];

function move_cannon(x) {
    for (let i = 0; i < square.length; i++) {
        square[i][0] += x;
    }
}  

//Player's Cannon
document.onkeydown = function check_key(e) {
    e = e || window.event;

    if (e.keyCode == '37') {
       console.log("LEFT");
       move_cannon(-cannon_speed);
    }
    if (e.keyCode == '39') {
        console.log("RIGHT");
       move_cannon(cannon_speed);
    }
}    


window.onload = function init() {
    canvas = document.getElementById("canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor( 0.0, 0.0, 0.0, 1.0 );

    //  Load shaders and initialize attribute buffers
    let program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);
    
    
    let vBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bufferData( gl.ARRAY_BUFFER, flatten(square), gl.DYNAMIC_DRAW);    
    
    // Associate out shader letiables with our data buffer
      let vPosition = gl.getAttribLocation(program, "vPosition");
      gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0 );
      gl.enableVertexAttribArray(vPosition);    
      render();
};

function render() {
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.bufferData( gl.ARRAY_BUFFER, flatten(square), gl.DYNAMIC_DRAW); 
    gl.drawArrays(gl.LINES, 0, 8);
    //Continually render animations
    requestAnimationFrame(render);
};