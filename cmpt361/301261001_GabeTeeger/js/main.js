"use strict";

var canvas;
var gl;

var numVertices;

var vBuffer;
var vPosition;
var cubeBuffer;
var coneBuffer;

var near;
var far;
var radius;
var theta;
var phi;
var aspect;

var dr = 5.0 * Math.PI / 180.0;
var left_mouseDown = 0;
var right_mouseDown = 0;

var x_coord_new = 0.0;
var y_coord_new = 0.0;

var x_coord_old = 0.0;
var y_coord_old = 0.0;

var x_trans = 0;
var y_trans = 0;
var z_trans = 0;

var x_rot = 0;
var y_rot = 0;
var reset = 0;

var modelViewMatrix, projectionMatrix;
var modelViewMatrix_L, projectionMatrix;

var modelViewMatrixLoc, projectionMatrixLoc;
var normalMatrix, normalMatrixLoc;

var is_lightLoc, modelViewMatrix_LLoc;
//var transformationMatrixLoc, transformationMatrix;


var eye;
const at = vec3(0.0, 0.0, 0.0);
const up = vec3(0.0, 1.0, 0.0);
var fovy = 60.0;

var vertices;
var faces;
var new_vertices = [];
var new_faces = [];
var random_colors = [];

var pointsArray = [];
var normalsArray = [];

var lightPosition;
var lightAmbient;
var lightDiffuse;
var lightSpecular;

var materialAmbient;
var materialDiffuse;
var materialSpecular;
var materialShininess;

var ambientColor, diffuseColor, specularColor;
var index = 0

var ambientProduct;
var diffuseProduct;
var specularProduct;

var spot_pan = 1;



var i = 0.0;
var j = 0.0;

//var lp1 = 5;

var lp4 = 1;

var la1;
var la2;
var la3;
var la4;

var ld1;
var ld2;
var ld3;
var ld4;

var ls1;
var ls2;
var ls3;
var ls4;

var ma1;
var ma2;
var ma3;
var ma4;

var md1;
var md2;
var md3;
var md4;

var ms1;
var ms2;
var ms3;
var ms4;

var shine;

var eyex;
var eyey;
var eyez;



var lp1_temp = 0;
var lp2_temp = 0;
var light_rot = 1;

var cone_vertices = [1.5, 0, 0,
    -1.5, 1, 0,
    -1.5, 0.809017, 0.587785,
    -1.5, 0.309017, 0.951057,
    -1.5, -0.309017, 0.951057,
    -1.5, -0.809017, 0.587785,
    -1.5, -1, 0,
    -1.5, -0.809017, -0.587785,
    -1.5, -0.309017, -0.951057,
    -1.5, 0.309017, -0.951057,
    -1.5, 0.809017, -0.587785];

var cone_indices = [0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,
    0, 8, 9,
    0, 9, 10,
    0, 10, 1];

var cube_vertices = [
    vec3(-0.5, -0.5, 0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5, 0.5, -0.5),
    vec3(0.5, 0.5, -0.5),
    vec3(0.5, -0.5, -0.5)
];

var cube_points = []

function quad(a, b, c, d) {


    cube_points.push(cube_vertices[a]);
    cube_points.push(cube_vertices[b]);
    cube_points.push(cube_vertices[c]);
    cube_points.push(cube_vertices[a]);
    cube_points.push(cube_vertices[c]);
    cube_points.push(cube_vertices[d]);
}


function colorCube() {
    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}


function triangle(a, b, c) {

    var t1 = subtract(b, a);
    var t2 = subtract(c, a);
    var normal = normalize(cross(t2, t1));
    normal = vec4(normal);
    normal[3] = 0.0;

    normalsArray.push(normal);
    normalsArray.push(normal);
    normalsArray.push(normal);

    pointsArray.push(a);
    pointsArray.push(b);
    pointsArray.push(c);

    index += 3;
}

var full_array = [];

function normals() {



    for (var i = 0; i < (full_array.length); i += 3) {

        triangle(full_array[index], full_array[index + 1], full_array[index + 2]);
    }

}

var program;
var Index_Buffer;

window.onload = function init() {
    canvas = document.getElementById("gl-canvas");
    //setInterval(showView, 1000);
    gl = WebGLUtils.setupWebGL(canvas);
    if (!gl) { alert("WebGL isn't available"); }

    vertices = get_vertices();
    faces = get_faces();

    aspect = canvas.width / canvas.height;

    for (var i = 0; i < vertices.length; i++) {
        new_vertices = new_vertices.concat(vertices[i]);

        // random_colors[i] = i % 4;
    }

    for (var i = 0; i < faces.length; i++) {
        new_faces = new_faces.concat(faces[i]);
    }


    for (var i = 0; i < new_faces.length; i++) {

        new_faces[i]--;
        //now we have indices for each vertex that make up a face starting from zero
    }

    for (var i = 0; i < new_faces.length; i++) {

        full_array[i] = vertices[new_faces[i]];
    }

    //THIS HAS TO GO BEFORE ALL THE SHADER STUFF
    program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);


    normals();
    

    //now push in cub vertices;

    var nBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, nBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(normalsArray), gl.STATIC_DRAW);

    var vNormal = gl.getAttribLocation(program, "vNormal");
    gl.vertexAttribPointer(vNormal, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vNormal);


   

    vBuffer = gl.createBuffer();
    cubeBuffer = gl.createBuffer();
    coneBuffer = gl.createBuffer();

    vPosition = gl.getAttribLocation(program, "vPosition");
    gl.enableVertexAttribArray(vPosition);


    Index_Buffer = gl.createBuffer();



    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    gl.enable(gl.DEPTH_TEST);
    gl.enable(gl.CULL_FACE);

    //
    //  Load shaders and initialize attribute buffers
    //

    modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");
    projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");
    normalMatrixLoc = gl.getUniformLocation(program, "normalMatrix");

    is_lightLoc = gl.getUniformLocation(program, "is_light");


    modelViewMatrix_LLoc = gl.getUniformLocation(program, "modelViewMatrix_L");



    document.getElementById("zFarSlider").onchange = function (event) {
        far = event.target.value;
        console.log("far=" + far);
    };
    document.getElementById("zNearSlider").onchange = function (event) {
        near = event.target.value;
        console.log("near=" + near);
    };

    document.getElementById("aspectSlider").onchange = function (event) {
        aspect = event.target.value;
    };
    document.getElementById("fovSlider").onchange = function (event) {
        fovy = event.target.value;
    };


    document.getElementById("la1").onchange = function (event) {
        la1 = event.target.value;
    };
    document.getElementById("la2").onchange = function (event) {
        la2 = event.target.value;
    };
    document.getElementById("la3").onchange = function (event) {
        la3 = event.target.value;
    };


    document.getElementById("ld1").onchange = function (event) {
        ld1 = event.target.value;
    };
    document.getElementById("ld2").onchange = function (event) {
        ld2 = event.target.value;
    };
    document.getElementById("ld3").onchange = function (event) {
        ld3 = event.target.value;
    };



    document.getElementById("ls1").onchange = function (event) {
        ls1 = event.target.value;
    };
    document.getElementById("ls2").onchange = function (event) {
        ls2 = event.target.value;
    };
    document.getElementById("ls3").onchange = function (event) {
        ls3 = event.target.value;
    };




    document.getElementById("ma1").onchange = function (event) {
        ma1 = event.target.value;
    };
    document.getElementById("ma2").onchange = function (event) {
        ma2 = event.target.value;
    };
    document.getElementById("ma3").onchange = function (event) {
        ma3 = event.target.value;
    };


    document.getElementById("md1").onchange = function (event) {
        md1 = event.target.value;
    };
    document.getElementById("md2").onchange = function (event) {
        md2 = event.target.value;
    };
    document.getElementById("md3").onchange = function (event) {
        md3 = event.target.value;
    };



    document.getElementById("ms1").onchange = function (event) {
        ms1 = event.target.value;
    };
    document.getElementById("ms2").onchange = function (event) {
        ms2 = event.target.value;
    };
    document.getElementById("ms3").onchange = function (event) {
        ms3 = event.target.value;
    };


    document.getElementById("shine").onchange = function (event) {
        shine = event.target.value;
    };


    document.getElementById("eyex").onchange = function (event) {
        eyex = event.target.value;
    };

    document.getElementById("eyey").onchange = function (event) {
        eyey = event.target.value;
    };

    document.getElementById("eyez").onchange = function (event) {
        eyez = event.target.value;
    };





    /* DEFAULT: 
    lightPosition = vec4(0.0, 1.0, 0.0, 0.0);
    lightAmbient = vec4(0.2, 0.2, 0.2, 1.0);
    lightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
    lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);

    materialAmbient = vec4(1.0, 0.0, 1.0, 1.0);
    materialDiffuse = vec4(1.0, 0.8, 0.0, 1.0);
    materialSpecular = vec4(1.0, 0.8, 0.0, 1.0);
    materialShininess = 1000.0;
    */

    near = 0.1;
    far = 30.0;
    radius = 10;
    theta = 0.0;
    phi = 0.0;
    aspect;




    la1 = 0.2;
    la2 = 0.2;
    la3 = 0.2;
    la4 = 1.0;

    ld1 = 1.0;
    ld2 = 1.0;
    ld3 = 1.0;
    ld4 = 1.0;

    ls1 = 1.0;
    ls2 = 1.0;
    ls3 = 1.0;
    ls4 = 1.0;

    ma1 = 1.0;
    ma2 = 0.0;
    ma3 = 1.0;
    ma4 = 1.0;

    md1 = 1.0;
    md2 = 0.8;
    md3 = 0.0;
    md4 = 1.0;

    ms1 = 1.0;
    ms2 = 0.8;
    ms3 = 0.0;
    ms4 = 1.0;
    shine = 1.0;

    i = 0;
    j = 0;

    eyex = 0.0;
    eyey = 0.0;
    eyez = 10.0;


    const container = document.querySelector('.container');

    //document.body.onmousemove = showView;
    container.onmousemove = showView;


    container.onmousedown = mouse_click;

    container.onmouseup = function () {
        left_mouseDown = 0;
        right_mouseDown = 0;
    }

    document.onkeydown = checkKey;

    //cube_vertices = mult(cube_vertices, rotate(45, [1, 0, 0]));
    colorCube();

    
    modelViewMatrix_L = mat4();




    render();
}

var keyPress;

function checkKey(event) {

    switch (event.keyCode) {

        case 38:
            //up arrow
            keyPress = 'up';

            z_trans += 1;
            
            break;
        case 40:
            //down arrow
            keyPress = 'down';

            z_trans -= 1;
            break;

        case 82:
            //r
          
            reset = 1;
            break;
        case 80:
            //p
            light_rot = !light_rot;
            break;

        case 83:
            //s
            spot_pan = !spot_pan;
            //console.log("here");
            break;

        default:
            //keyPress = 'NA';



    }
}

function mouse_click(event) {

    switch (event.button) {
        case 0:
            left_mouseDown = 1;
            break;
        case 2:
            right_mouseDown = 1;
            break;
        default:
            right_mouseDown = 0;
            left_mouseDown = 0;

    }
}


function showView(event) {
    //
    
    x_coord_old = x_coord_new;
    y_coord_old = y_coord_new;

    if (left_mouseDown || right_mouseDown) {
        
        x_coord_new = event.clientX/2;
        y_coord_new = event.clientY/2;
    }




}


var deg_to_rad = (Math.PI * 45) / 180;

var lp1_new;
var lp2_new;
var j2 = 0;
var pan_amount = 0;
var rot_angle = 0;
var i2 = 0;
//the first thing we do is draw lights
var is_light = 1;
function render() {

    projectionMatrix = perspective(fovy, aspect, near, far);
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));


    eye = vec3(eyex, eyey, eyez);


    if (light_rot == 1) {

        if (i2 >= 5) {
            //j2 = 0;
            i2 = 0;




            rot_angle++;
            j2+=0.1;
        }
        i2++;
    }

    eye = vec3(5, 5, 0);
    modelViewMatrix_L = lookAt(eye, at, up);
    modelViewMatrix_L = mult(modelViewMatrix_L, rotate(-rot_angle, [0, 1, 0]));
    modelViewMatrix_L = mult(modelViewMatrix_L, translate(5, -5, 0));
    gl.uniformMatrix4fv(modelViewMatrix_LLoc, false, flatten(modelViewMatrix_L));
    

    eye = vec3(eyex, eyey, eyez);


    //must happen before any drawing
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    //this cannot change, everthing is in the same perspective
   

    // gl.uniform1f(gl.getUniformLocation(program,
    //     "shininess"), materialShininess);

    // is_light = 1;
    // gl.uniform1i(gl.getUniformLocation(program,
    //     "is_light"), is_light);

    modelViewMatrix = lookAt(eye, at, up);

    modelViewMatrix = mult(modelViewMatrix, rotate(rot_angle, [0, 1, 0]));

    modelViewMatrix = mult(modelViewMatrix, translate(5, 5, 0));


    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cube_points), gl.STATIC_DRAW);
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.drawArrays(gl.LINE_STRIP, 0, cube_points.length);


/***************************************************************************************/
/***************************************************************************************/

/*  CONE PART:
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, Index_Buffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cone_indices), gl.STATIC_DRAW);


    modelViewMatrix = lookAt(eye, at, up);

    //we have to move the pointy part of the cone to the origin at first(0,0,0 ) 

    modelViewMatrix = mult(modelViewMatrix, translate(0, 4, 2));

    //using trig to calculate angle between cone and origin
    var rotate_amount = (Math.atan(2)) * (180 / Math.PI);

    //modelViewMatrix = mult(modelViewMatrix, rotate(-45, [0, 1, 0]));


    //we have to move the pointy part of the cone to the origin at first(0,0,0 ) 

    //SCALING:
    for (var i = 0; i < modelViewMatrix.length; i++) {
        for (var j = 0; j < 3; j++) {
            modelViewMatrix[i][j] *= 0.35;
        }
    }

    if (spot_pan) {
        pan_amount+=0.01;
    }
    modelViewMatrix = mult(modelViewMatrix, translate(6 * Math.cos(pan_amount), 0, 0));

    modelViewMatrix = mult(modelViewMatrix, translate(-1.5, 0, 0));


    modelViewMatrix = mult(modelViewMatrix, rotate(-rotate_amount, [1, 0, 0]));

    modelViewMatrix = mult(modelViewMatrix, rotate(-90, [0, 1, 0]));


    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
    gl.bindBuffer(gl.ARRAY_BUFFER, coneBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(cone_vertices), gl.STATIC_DRAW);
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.drawElements(gl.LINE_STRIP, cone_indices.length, gl.UNSIGNED_SHORT, 0);



*/
/***************************************************************************************/
/***************************************************************************************/


    is_light = 0;
    gl.uniform1i(gl.getUniformLocation(program,
        "is_light"), is_light);




    //we must reset the modelViewMatrix
    //eye = vec3(5, 5, 0);
    modelViewMatrix = lookAt(eye, at, up);


    // var lp1_temp = 5.0;
    // var lp2_temp = 0.0;

    // var  rot_angle_rad = (Math.PI * rot_angle) / 180;
    // lp1_new = (lp1_temp * Math.cos(rot_angle_rad)) - (lp2_temp * Math.sin(rot_angle_rad));
    // lp2_new = (lp1_temp * Math.sin(rot_angle_rad)) + (lp2_temp * Math.cos(rot_angle_rad));
    

   // lightPosition = mult(vec4(5.0, 0.0, 0.0, 0.0), modelViewMatrix) ;


    lightAmbient = vec4(la1, la2, la3, la4);
    lightDiffuse = vec4(ld1, ld2, ld3, ld4);
    lightSpecular = vec4(ls1, ls2, ls3, ls4);

    materialAmbient = vec4(ma1, ma2, ma3, ma4);
    materialDiffuse = vec4(md1, md2, md3, md4);
    materialSpecular = vec4(ms1, ms2, ms3, ms4);
    materialShininess = shine;






    
    //transformationMatrix = rotate(j, [0, 1, 1]);
    // modelViewMatrix = rotate(j, [0, 1, 1]);




    ambientProduct = mult(lightAmbient, materialAmbient);
    diffuseProduct = mult(lightDiffuse, materialDiffuse);
    specularProduct = mult(lightSpecular, materialSpecular);

    gl.uniform4fv(gl.getUniformLocation(program, "ambientProduct"),
        flatten(ambientProduct));
    gl.uniform4fv(gl.getUniformLocation(program, "diffuseProduct"),
        flatten(diffuseProduct));
    gl.uniform4fv(gl.getUniformLocation(program, "specularProduct"),
        flatten(specularProduct));
   // gl.uniform4fv(gl.getUniformLocation(program, "lightPosition"),
     //   (lightPosition));

    gl.uniform1f(gl.getUniformLocation(program,
        "shininess"), materialShininess);

    gl.uniform1i(gl.getUniformLocation(program,
        "is_light"), is_light);


    //check old values of mouse pointer.
    //adding or subtracting 1 removes jitter

    if (right_mouseDown) {
        if (x_coord_new > (x_coord_old + 1)) {
            y_rot += x_coord_new + x_coord_old;
        }
        else if (x_coord_new < (x_coord_old - 1)) {
            y_rot -= x_coord_new + x_coord_old;
        }

        if (y_coord_new > (y_coord_old + 1)) {
            x_rot += x_coord_new + x_coord_old;

        }
        else if (y_coord_new < (y_coord_old - 1)) {
            x_rot -= x_coord_new + x_coord_old;
        }
       


    }

    if (left_mouseDown) {

        if (x_coord_new > (x_coord_old+1)) {
            x_trans += x_coord_new + x_coord_old;
        }
        else if (x_coord_new < (x_coord_old-1)) {
            x_trans -= x_coord_new + x_coord_old;
        }

        if (y_coord_new > (y_coord_old+1)) {
            y_trans -= x_coord_new + x_coord_old;

        }
        else if (y_coord_new < (y_coord_old-1)) {
            y_trans += x_coord_new + x_coord_old;
        }

    }

    if (keyPress == 'up') {
        modelViewMatrix = mult(modelViewMatrix, translate(0, 0, z_trans));
        keyPress == 'NA';
    }
    else if (keyPress == 'down') {
        modelViewMatrix = mult(modelViewMatrix, translate(0, 0, z_trans));
        keyPress == 'NA';
    }

    if (reset == 1) {
        

        x_trans = 0;
        y_trans = 0;
        z_trans = 0;
        x_rot = 0;
        y_rot = 0;
        reset = 0;


    }


    modelViewMatrix = mult(modelViewMatrix, translate(x_trans / 3000.0, 0, 0));
    modelViewMatrix = mult(modelViewMatrix, translate(0, y_trans / 3000.0, 0));

    modelViewMatrix = mult(modelViewMatrix, rotate(x_rot / 75.0, [1, 0, 0]));
    modelViewMatrix = mult(modelViewMatrix, rotate(y_rot / 75.0, [0, 1, 0]));


    //modelViewMatrix = mult(modelViewMatrix, translate();


    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));


    gl.uniformMatrix3fv(normalMatrixLoc, false, flatten(normalMatrix));

    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(pointsArray), gl.STATIC_DRAW);

    
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);

    gl.drawArrays(gl.TRIANGLES, 0, pointsArray.length);






    //gl.drawElements(gl.TRIANGLES, new_faces.length, gl.UNSIGNED_SHORT, 0);

    requestAnimFrame(render);
}




    // array element buffer




/*
    // color array atrribute buffer

    var cBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(vertexColors), gl.STATIC_DRAW);

    var vColor = gl.getAttribLocation(program, "vColor");
    gl.vertexAttribPointer(vColor, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vColor);
*/

    // vertex array attribute buffer

