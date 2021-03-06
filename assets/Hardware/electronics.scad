$fn = 50;
outerDiam = 6;
screwDiam = 3;
pilarHeight = 8;

boardX = 180;
boardY = 180;
boardZ = 2.4;

boardHoleDiam = 25;
boardHoleSpace = 0.2 * boardHoleDiam;

// First is the board x, y second is the position of the first hole
megaDim = [ [ 101.6, 53.34 ], [ 14, 2.5 ] ];

mega = [
  [ 0, 0 ], [ 79.4, 48.26 ], [ 0, 48.26 ], [ 82.55, 0 ], [ 50.8, 0 ],
  [ 50.8, 33 ]
];

unoDim = [ [ 68.58, 53.34 ], [ 2.58, 7.62 ] ];
uno = [ [ 0, 0 ], [ 0, 27.94 ], [ 50.76, 43.18 ] ];
// https://learn.adafruit.com/introducing-the-raspberry-pi-model-b-plus-plus-differences-vs-model-b/mounting-holes
// https://learn.adafruit.com/assets/17950
raspberryDim = [ [ 85.6, 54 ], [ 25.5, 18 ] ];
raspberryBPlus = [ [], [], [], [], [] ];
raspberryB = [ [ 0, 0 ], [ 55.1, 23.5 ] ];
raspberry3 = [ [], [], [], [], [] ];

LEDDiverDim = [ [ 42.5, 24 ], [ 2, 2 ] ];
LEDDriver = [ [ 0, 0 ], [ 37.6, 0 ], [ 0, 20 ], [ 37.6, 20 ] ];

relaisDim = [ [ 70.15, 50 ], [ 3, 3 ] ];
relais = [ [ 0, 0 ], [ 65.72, 0 ], [ 0, 45 ], [ 65.72, 45 ] ];

PHBoardDim = [ [ 40.5, 20 ], [ 1, 1 ] ];
PHBoard = [ [ 0, 0 ], [ 35.7, 0 ], [ 0, 15.7 ], [ 35.7, 15.7 ] ];

module pillar(base) {
  translate([ 0, 0, boardZ ]) difference() {
    cylinder(h = pilarHeight, d1 = outerDiam, d2 = outerDiam);
    translate([ 0, 0, -1 ])
        cylinder(h = pilarHeight + 2, d1 = screwDiam, d2 = screwDiam);
  }
  if(base){
  cylinder(h = boardZ, d = boardHoleDiam/2);}
}

module mount(component,basePillar) {

  color("Lime", 1.0) {
    for (i = [0:len(component)]) {
      translate([ component[i][0], component[i][1], 0 ]) pillar(basePillar);
    }
  }
}

// This prints the pillars and the board footprint
module fullmodule(boardDim, component, base,basePillar) {

  if (base) {
    cube([ boardDim[0][0], boardDim[0][1], 3 ]);
  }
  translate([ boardDim[1][0], boardDim[1][1], 0 ]) mount(component,basePillar);
}
// mount(mega);

module holes() {
  for (y = [0:boardHoleDiam +
           boardHoleSpace:boardY - 2 * (boardHoleDiam + boardHoleSpace)]) {

    for (x = [0:boardHoleDiam +
             boardHoleSpace:boardX - 2 * (boardHoleDiam + boardHoleSpace)]) {
      translate([ x, y, 0 ]) cylinder(h = 10, d = boardHoleDiam);
    }
  }
}

module board() {

  color("Gray", 1.0) {
    difference() {
      cube([ boardX, boardY, boardZ ]);
      translate([
        (boardHoleDiam + boardHoleSpace), (boardHoleDiam + boardHoleSpace), -5
      ]) holes();
    }
  }
}


module baseSupport(){
    cube([boardX/2-0.2*boardX,10,20]);
    
    
}


module electronicsBase(){
board();
baseShow = false;
basePillar = true;
basePPH = false;
// translate([0,0,0])rotate([0,0,0])mount(uno);
translate([ raspberryDim[0][0] + 10, raspberryDim[0][1] + 10, 0 ])
    rotate([ 0, 0, 180 ])
        fullmodule(raspberryDim, raspberryB, baseShow,basePillar); // mount(raspberryB);
translate([ 70, 120, 0 ]) rotate([ 0, 0, 180 ])
    fullmodule(unoDim, uno, baseShow, basePillar);

translate([ 170, 10, 0 ]) rotate([ 0, 0, 90 ])
    fullmodule(relaisDim, relais, baseShow, basePillar);
translate([ 129, 95, 0 ]) rotate([ 0, 0, 0 ])
    fullmodule(LEDDiverDim, LEDDriver, baseShow, basePillar);
//translate([ 129, 135, 0 ]) rotate([ 0, 0, 0 ])
//    fullmodule(LEDDiverDim, LEDDriver, baseShow, basePillar);

translate([ 140, 155, 0 ]) rotate([ 0, 0, 0 ])
    fullmodule(PHBoardDim, PHBoard, baseShow, basePPH);

// fullmodule(raspberryDim,raspberryB);
// translate([raspberryDim[0][0] + 20,0,0])fullmodule(unoDim,uno);
}


//baseSupport();
pillar();