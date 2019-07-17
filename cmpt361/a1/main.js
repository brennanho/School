let canvas;
let cannon;
let top_enemies = [];
let bot_enemies = [];
let bullets = [];
let enemy_bullets = [];
let gl;
let color;
const NUM_ENEMIES_ROW = 4;
const RANDOM = 0.99;
const ENEMY_SPEED_ACC = 1.001;
const ALIEN_SHOOT_TIME = 1000;

//Cannon and Enemy Object 
function Square(x, y, speed, is_cannon) {
    this.speed = speed;
    this.fall_speed = 0.001;
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

    this.move = function (speed, enemies=null) {
        if (this.square[0][0] < -1 && speed < 0 || this.square[1][0] > 1 && speed > 0) {
            if (this.is_cannon == false) {
                if (enemies == null)
                    this.speed = -this.speed;
                else {
                    for (let i = 0; i < enemies.length; i++) {
                        enemies[i].speed = -enemies[i].speed;
                    }
                }
            }
            else
                return;
        }
        for (let i = 0; i < this.square.length; i++) {
            this.square[i][0] += speed;
            if (this.is_cannon == false) {
                this.square[i][1] -= this.fall_speed;
                if (this.square[i][1] <= -1)
                    game_over("You lose ):");
            }
        }
        if (!this.is_cannon)
            this.speed *= 1.0001;
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
    this.shoot = function() {
        if (this.is_cannon) {
            return [
                vec2(this.square[0][0] + 0.25*this.square_size, this.square[0][1] + this.square_size),
                vec2(this.square[0][0] + this.square_size/2, this.square[0][1] + 1.5*this.square_size),
                vec2(this.square[0][0] + 0.75*this.square_size, this.square[0][1] + this.square_size)
            ]
        }
        return [
            vec2(this.square[0][0] + 0.25*this.square_size, this.square[0][1]),
            vec2(this.square[0][0] + this.square_size/2, this.square[0][1] - 0.5*this.square_size),
            vec2(this.square[0][0] + 0.75*this.square_size, this.square[0][1])
        ]
    }
}

function compute_enemies(enemies) {
    for (let i = 0; i < enemies.length; i++) {
        enemies[i].move(enemies[i].speed, enemies);
    }
}

function render_enemies(enemies) {
    gl.uniform4f(color, 1, 0, 0, 1);
    for (let i = 0; i < enemies.length; i++) {
        gl.bufferData(gl.ARRAY_BUFFER, flatten(enemies[i].square), gl.STATIC_DRAW); 
        gl.drawArrays(gl.TRIANGLES, 0, 6);
    }
}

function check_cannon_hits(enemies, bullet) {
    for (let j = 0; j < enemies.length; j++) {
        if (bullet == undefined)
            break;
        if (enemies[j].point_inside(bullet[0]) || enemies[j].point_inside(bullet[1]) || enemies[j].point_inside(bullet[2])) {
            bullets.splice(bullets.indexOf(bullet), 1);
            enemies.splice(j, 1);
            break;
        }
    }
}

//For part (e), left and right movement becomes faster and faster
function increase_enemy_speed(enemies) {
    for (let i = 0; i < enemies.length; i++) {
        enemies[i].speed *= ENEMY_SPEED_ACC;
    }
}

//For part (a), aliens move left and right uniformly randomly
function change_direction(enemies) {
    if (Math.random() >= RANDOM) {
        for (let i = 0; i < enemies.length; i++) {
            enemies[i].speed = -enemies[i].speed;
        }
    }
}

function remove_bullets(bullets) {
    for (let i = 0; i < bullets.length; i += 1) {
        if (bullets[i][1][1] <= -1 - cannon.square_size/2) {
            bullets.splice(i, 1);
        }
    }
}

//Player's cannon move
window.onkeydown = function check_key(e) {
    e = e || window.event;

    //For part (b)
    //LEFT KEY
    if (e.keyCode == '37') {
       cannon.move(-cannon.speed)
    }
    //RIGHT KEY
    if (e.keyCode == '39') {
       cannon.move(cannon.speed);
    }
    //Restart game
    if (e.keyCode == '82') {
        location.reload();
    }

    //Quit game
    if (e.keyCode == '81') {
        window.alert("Confirm to quit...");
        window.alert = () => false;
        close();
    }
}

//Enemy shoots (part (c))
function enemy_shoot() {
    if (bot_enemies.length != 0) {
        for (let i = 0; i < bot_enemies.length; i++) {
            enemy_bullets.push(bot_enemies[i].shoot());
        }
    } else {
        for (let i = 0; i < top_enemies.length; i++) {
            enemy_bullets.push(top_enemies[i].shoot());
        }
    }
    setTimeout(enemy_shoot, ALIEN_SHOOT_TIME);
}

//Cannon shoot
window.onmousedown = function click(event) {
    if (event.button == '0') {
        if (bullets.length < 3)
            bullets.push(cannon.shoot());
    }
} 

function game_over(msg) {
    window.alert(msg);
    window.alert = () => false;
    location.reload();
}

window.onload = function init() {
    canvas = document.getElementById("canvas");
    gl = WebGLUtils.setupWebGL(canvas);
    cannon = new Square(0, -1, 0.05, true);

    //Initialize enemies
    for (let i = 0; i < NUM_ENEMIES_ROW; i++) {
        bot_enemies.push(new Square((i+1)/3 - 1, 0.6, 0.005, false));  // Top row
        top_enemies.push(new Square((i+1)/3 - 1, 0.8, 0.005, false));  // Bottom row
    }
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

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
    enemy_shoot();
};

function render() {
    gl.clear(gl.COLOR_BUFFER_BIT);

    //Render cannon
    gl.uniform4f(color, 0, 1, 0, 1);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cannon.square), gl.STATIC_DRAW); 
    gl.drawArrays(gl.TRIANGLES, 0, 6);

    //Compute enemy rows
    increase_enemy_speed(top_enemies);
    increase_enemy_speed(bot_enemies);
    change_direction(top_enemies);
    change_direction(bot_enemies);
    compute_enemies(top_enemies);
    compute_enemies(bot_enemies);

    //Render enemies
    render_enemies(top_enemies);
    render_enemies(bot_enemies);

    remove_bullets(enemy_bullets);

    //Render Player's bullets
    gl.uniform4f(color, 0, 1, 0, 1);
    for (let i = 0; i < bullets.length; i++) {
        if (bullets[i] == undefined)
            continue;
        for (let j = 0; j < bullets[i].length; j++) {
            bullets[i][j][1] += 0.02;
            if (bullets[i][j][1] >= 1 + cannon.square_size/2) {
                bullets.splice(i, 1);
                break;
            }
        }
        check_cannon_hits(bot_enemies, bullets[i]);
        check_cannon_hits(top_enemies, bullets[i]);

        if (bullets[i] == undefined)
            continue;
        gl.bufferData(gl.ARRAY_BUFFER, flatten(bullets[i]), gl.STATIC_DRAW); 
        gl.drawArrays(gl.TRIANGLES, 0, 3);
    }
    //Render enemy bullets
    gl.uniform4f(color, 1, 0, 0, 1);
    for (let i = 0; i < enemy_bullets.length; i++) {
        for (let j = 0; j < enemy_bullets[i].length; j++) {
            enemy_bullets[i][j][1] -= 0.01;
        }
        if (cannon.point_inside(enemy_bullets[i][0]) || cannon.point_inside(enemy_bullets[i][1]) || cannon.point_inside(enemy_bullets[i][2])) 
            game_over("You lose ):");

        gl.bufferData(gl.ARRAY_BUFFER, flatten(enemy_bullets[i]), gl.STATIC_DRAW); 
        gl.drawArrays(gl.TRIANGLES, 0, 3);
    }

    if (top_enemies.length == 0 && bot_enemies.length == 0)
        game_over("You win!")

    //Continually render animations
    window.requestAnimationFrame(render);
};