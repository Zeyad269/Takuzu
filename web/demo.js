Module.onRuntimeInitialized = () => { start(); }

/* ******************** global variables ******************** */

// load images
var background = new Image();
background.src = "background.jpg";

var takuzu = new Image();
takuzu.src = "Takuzu.png"; 

var asteroide = new Image();
asteroide.src = "asteroide.png";

var asteroide2 = new Image();
asteroide2.src = "asteroide2.png";

var etoile = new Image();
etoile.src = "etoile-filante.png";

var etoile2 = new Image();
etoile2.src = "etoile-filante2.png";

var victoire = new Image();
victoire.src = "victory.png";

// Retrieve canva
var canvas = document.getElementById('mycanva');

var ctx = canvas.getContext('2d');

// Retrieve button
var listOfButtons = document.getElementsByClassName('mybutton');

// Set canva dimensions
canvas.width = window.innerWidth / 2.5;
canvas.height = window.innerHeight / 1.5;

// Set buttons dimensions
for (var i = 0; i < listOfButtons.length; i++) {
    listOfButtons[i].style.width = window.innerWidth * 0.06 + 'px';
    listOfButtons[i].style.height = window.innerHeight * 0.06 + 'px';
}

// initial dimensions of canvas
var width ;
var height ;

// game
var g ;

// dimensions of the game
var game_width ;
var game_height ;

// boundaries of the grid in canvas
var grid_x1 ;
var grid_y1 ;
var grid_x2 ;
var grid_y2 ;

// initals positions of the first case
var inital_x ;
var inital_y ;

// dimensions of a case
var case_height ;
var case_width ;

// array to store the positions of the cases
var tabOfXPositions ;
var tabOfYPositions ;


/* ******************** register events ******************** */


window.addEventListener('load', windowLoad);              // window load
window.addEventListener('resize', resize);                // window resize
canvas.addEventListener('click', canvasLeftClick);        // left click event



/* ******************** event callback ******************** */

function windowLoad() {
    drawCanvas();
    victory();
        
}

function resize(){

    //resize canva
    canvas.width = window.innerWidth / 2.5;
    canvas.height = window.innerHeight / 1.5;

    //resize the buttons
    for (var i = 0; i < listOfButtons.length; i++) {
        listOfButtons[i].style.width = window.innerWidth * 0.06 + 'px';
        listOfButtons[i].style.height = window.innerHeight * 0.06 + 'px';
    }

    drawCanvas();
}

function undo(){
    Module._undo(g);
    drawCanvas();
}

function redo(){
    Module._redo(g);
    drawCanvas(); 
}

function solve(){
    if ( Module._is_equal(g, Module._new_default()) ){
        g=Module._default_solution();     
    }
        
    else {
        Module._solve(g);
    }
    drawCanvas(); 
}

function reset(){
    Module._restart(g);
    drawCanvas(); 
}

function random(){
    g=Module._new_random(Module._nb_rows(g),Module._nb_cols(g),0,0);
    resize();
    drawCanvas(); 
   
}

function setDimensions(i,j){
    g=Module._new_empty(i,j,0,0);
    resize();
    drawCanvas(); 
}


function canvasLeftClick(event) {

    // get relative cursor position in canvas    
    // Checks if the mouse is inside the Grid 
    if ((event.offsetX > grid_x1 && event.offsetX < grid_x2) &&
        (event.offsetY > grid_y1 && event.offsetY < grid_y2)) {

        x = -1;
        // Get the the x position of the mouse in the grid
        for (i = 0; i <= (event.offsetX - grid_x1); i += case_width) {
            x++;
        }

        y = -1;
        // Get the the y position of the mouse in the grid
        for (i = 0; i <= (event.offsetY - grid_y1); i += case_height) {
            y++;
        }

        if (!Module._is_immutable(g,x,y)){

            if (Module._get_number(g,x,y)==0){
                Module._play_move(g,x,y,2)
            } 
            else if (Module._get_number(g,x,y)==1){
                Module._play_move(g,x,y,0);
            }           
            else{
                Module._play_move(g,x,y,1); 
            }
        }

        
                   
    }
    
    drawCanvas(); 
}




/* ******************** canvas drawing ******************** */

function drawCanvas() {

    game_width = Module._nb_rows(g);
    game_height = Module._nb_cols(g);

    width = canvas.width;
    height = canvas.height;

    grid_x1 = width / 11;
    grid_y1 = height / 9;
    grid_x2 = width - grid_x1;
    grid_y2 = height - grid_y1;

    case_height = ( grid_y2 - grid_y1 ) / game_height ;
    case_width  = ( grid_x2 - grid_x1 ) / game_width ;

    tabOfXPositions=[];
    tabOfYPositions=[];

    inital_x = grid_x1 + case_width / 8;
    inital_y = grid_y1 + case_height / 8;

    for (i=0; i < game_width ; i++){
        tabOfXPositions.push( (inital_x) + i * case_width );
    }
    
    for ( i=0; i < game_height ; i++){
        tabOfYPositions.push( (inital_y) + i * case_height );
    } 
    
    ctx.clearRect(grid_x1, grid_y1 , grid_x2-grid_x1 , grid_y2 - grid_y1);

    // draw images    
    ctx.drawImage(background, 0, 0, width, height);

    // draw the grid
    ctx.fillStyle = 'lightsteelblue';
    ctx.fillRect(grid_x1, grid_y1 , grid_x2-grid_x1 , grid_y2 - grid_y1);
    ctx.restore();
    ctx.save();
    ctx.strokeStyle = 'white';  
    ctx.save();

    // draw horizontal lines
    for (var i = 0; i <= game_height; i+=1){
        ctx.moveTo(grid_x1 , grid_y1 + i * case_height);
        ctx.lineTo(grid_x2 , grid_y1 + i * case_height);
    }
    // draw vertical lines
    for (var i = 0; i <= game_width; i+=1){
        ctx.moveTo(grid_x1 + i * case_width , grid_y1);
        ctx.lineTo(grid_x1 + i * case_width , grid_y2);
    }
    
    ctx.stroke(); // the stroke will actually paint the current lines
    ctx.restore();

    // load the grid

    for (x=0; x<Module._nb_rows(g); x++ ){
        for (y=0; y<Module._nb_cols(g); y++ ){

            if (Module._has_error(g,x,y)!=0){
                ctx.fillStyle = 'red';
                ctx.fillRect(grid_x1 + x * case_width, grid_y1 + y * case_height , case_width , case_height);    
            }
            if(Module._get_number(g, x, y) == 1 && Module._is_immutable(g, x, y))
                ctx.drawImage(etoile2, tabOfXPositions[x] , tabOfYPositions[y], case_width / 1.3, case_height/ 1.3);
            if(Module._get_number(g, x, y) == 1 && !(Module._is_immutable(g, x, y)))
                ctx.drawImage(etoile, tabOfXPositions[x] , tabOfYPositions[y], case_width / 1.3, case_height/ 1.3);
            if(Module._get_number(g, x, y) == 0 && Module._is_immutable(g, x, y))
                ctx.drawImage(asteroide2, tabOfXPositions[x] , tabOfYPositions[y], case_width / 1.3, case_height/ 1.3);
            if(Module._get_number(g, x, y) == 0 && !(Module._is_immutable(g, x, y)))
                ctx.drawImage(asteroide, tabOfXPositions[x] , tabOfYPositions[y], case_width / 1.3, case_height/ 1.3);
        }
    }
    ctx.restore();

    // Check if the game is won
    if (Module._is_over(g)){

        ctx.drawImage(victoire, width / 6, grid_y1 * 1.5 , width / 1.5, height / 1.5);

        // Fill the canva with a brigthness filter
        ctx.fillStyle = 'rgba(0, 0, 0, 0.3)';
        ctx.globalCompositeOperation = 'source-atop';
        ctx.fillRect(grid_x1, grid_y1 , grid_x2-grid_x1 , grid_y2 - grid_y1);
    }

    // Remove the victory filter 
    ctx.globalCompositeOperation = 'source-over';
    

}

/* ******************** start ******************** */

function start() {
    console.log("call start routine");
    g = Module._new_default();
    drawCanvas();       
} 