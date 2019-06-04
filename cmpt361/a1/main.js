let canvas;
let cannon;
let enemies = [];
let gl;
let color;
const NUM_ENEMIES = 8;

//Cannon and Enemy Object 
function Square(x, y, speed, is_cannon) {
    this.speed = speed;
    this.is_cannon = is_cannon;
    this.square_size = 0.15;
    this.square = [
        vec2(x, y),
        vec2(x + this.square_size,this.square_size + y),
        vec2(x, this.square_size + y),
        vec2(x, y),
        vec2(x + this.square_size, y),
        vec2(x + this.square_size,this.square_size + y),
    ];

    this.move = function (speed) {
        if (this.square[0][0] < -1 && speed < 0 || this.square[1][0] > 1 && speed > 0) {
            if (this.is_cannon == false)
                this.speed = -this.speed;
            else
                return;
        }
        for (let i = 0; i < this.square.length; i++) {
            this.square[i][0] += speed;
        }
    }

    this.point_inside = function (p) {
        return ((p[0] >= this.square[0][0]) && (p[0] <= this.square[0][0] + this.square_size) && (p[1] >= this.square[0][1]) && (p[1] <= this.square[0][1] + this.square_size));
    }

    this.collide = function(square) {
        for (let i = 0; i < this.square.length; i++) {
            if (square.point_inside(this.square[i])) {
                return true;
            }
        }
        return false;
    }
}
//Player's square move
window.onkeydown = function check_key(e) {
    e = e || window.event;
    //LEFT KEY
    if (e.keyCode == '37') {
       cannon.move(-cannon.speed)
    }
    //RIGHT KEY
    if (e.keyCode == '39') {
       cannon.move(cannon.speed);
    }
}

window.onload = function init() {
    canvas = document.getElementById("canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    cannon = new Square(0, -1, 0.05, true);

    //Initialize enemies
    for (let i = 0; i < NUM_ENEMIES/2; i++) {
        enemies.push(new Square((i+1)/3 - 1, 0.6, 0.005, false));  // Top row
        enemies.push(new Square((i+1)/3 - 1, 0.8, 0.005, false));  // Bottom row
    }
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor( 0.0, 0.0, 0.0, 1.0 );

    //  Load shaders and initialize attribute buffers
    let program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);
    color = gl.getUniformLocation(program, "fColor");
    
    let vBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cannon.square), gl.STATIC_DRAW);    
    
    // Associate out shader letiables with our data buffer
    let vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray(vPosition);
    render();
};

function render() {
    gl.clear(gl.COLOR_BUFFER_BIT);

    //Render cannon
    gl.uniform4f(color, 0, 1, 0, 1);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cannon.square), gl.STATIC_DRAW); 
    gl.drawArrays(gl.TRIANGLES, 0, 6);

    //First row enemies
    for (let i = 0; i < NUM_ENEMIES/2; i++) {
        for (let j = 0; j < NUM_ENEMIES/2; j++) {
            if (enemies[i].collide(enemies[j]) && i != j) {
                enemies[i].speed = -enemies[i].speed;
            }
        }
        enemies[i].move(enemies[i].speed);
    }

    //Second row enemies
    for (let i = NUM_ENEMIES/2; i < NUM_ENEMIES; i++) {
        for (let j = NUM_ENEMIES/2; j < NUM_ENEMIES; j++) {
            if (enemies[i].collide(enemies[j]) && i != j) {
                enemies[i].speed = -enemies[i].speed;
            }
        }
        enemies[i].move(enemies[i].speed);
    }

    //Render enemies
    for (let i = 0; i < NUM_ENEMIES; i++) {
        gl.uniform4f(color, 1, 0, 0, 1);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(enemies[i].square), gl.STATIC_DRAW); 
        gl.drawArrays(gl.TRIANGLES, 0, 6);
    }

    //Continually render animations
    window.requestAnimationFrame(render);
};