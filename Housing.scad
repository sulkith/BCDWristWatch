// CSG.scad - Basic example of CSG usage


module housing(){
translate([0,0,0]) {
    difference(){
    union() {
        hull(){
        translate([0,0,-5]) {//rim
           cylinder(5,17.,17.5);
        }
        
        cylinder(5,17.5,17.5);//bottom
        
        translate([0,0,2.5]) {//wrist mounting
            scale([4,2.5,0.5]) cube(10 ,center=true);
        }
        translate([0,0,4]) {//wrist mounting bottom
            scale([4.5,2.5,0.25]) cube(10 ,center=true);
        }
        translate([-20,12.5,2.5]) {//wrist smoothing
            rotate(a=[90,0,0])
            {
                cylinder(25,2.5,2.5);
            }
        }
        translate([20,12.5,2.5]) {//wrist smoothing
            rotate(a=[90,0,0])
            {
                cylinder(25,2.5,2.5);
            }
        }
        
    }    
    }
    
    union() {
        translate([0,0,-6]) {
          cylinder(2.5,16,16);//glass
          cylinder(22,15,15);//mounted parts
            translate([0,0,5]) {//PCB Mounting
                cylinder(20,15.5,15.5);
            }
            
            translate([-27.5,0,10]) {//wrist mounting
                cube(20, center = true);
            }
            translate([27.5,0,10]) {
                //wrist mounting
                cube(20, center = true);
            }
            
        }
        translate([0,20,0]) {//Holes for buttons
            rotate(a=[90,0,0])
            {
                translate([20,2.5,8])
                {
                    cylinder(24,0.5,0.5); 
                }
                translate([-20,2.5,8])
                {
                    cylinder(24,0.5,0.5); 
                }
            }
        }
        translate([3,20,1]) {//Holes for buttons
            rotate(a=[90,0,0])
            {
                
                cylinder(40,1.5,1.5);
                translate([0,0,4])
                {
                    cylinder(32,2.5,2.5); 
                }
            }
        }
    translate([14.5,11,0]) {
        cylinder(100,0.7,0.7);
    }
    translate([-14.5,11,0]) {
        cylinder(100,0.7,0.7);
    }
    translate([-14.5,-11,0]) {
        cylinder(100,0.7,0.7);
    }
    translate([14.5,-11,0]) {
        cylinder(100,0.7,0.7);
    }
    }
    }
}
}
module bottom(offs){
    //color([255,0,0])
    {
translate([0,0,5+offs]) {
    difference(){
    union() {
        hull(){
        
        cylinder(1,17.5,17.5);//bottom
        translate([0,0,0.5]) {//wrist mounting bottom
            scale([4.5,2.5,0.1]) cube(10 ,center=true);
        }
        
        }
        translate([0,0,-0.5])cylinder(1.5,15.5,15.5);
        
    }
    
    union() {
            translate([-32.5,0,4]) {//wrist mounting
                cube(30, center = true);
            }
            translate([32.5,0,4]) {
                //wrist mounting
                cube(30, center = true);
            }
    translate([14.5,11,0]) {
        cylinder(100,0.7,0.7);
    }
    translate([-14.5,11,0]) {
        cylinder(100,0.7,0.7);
    }
    translate([-14.5,-11,0]) {
        cylinder(100,0.7,0.7);
    }
    translate([14.5,-11,0]) {
        cylinder(100,0.7,0.7);
    }
    }
    }
}}
}

module buttons(offs, dir){
    color([0,0,0])
    {
        translate([3,-15-offs,1]) {
            rotate(a=[dir+90,0,0])
            {
                cylinder(3.7,1.4,1.4);
                cylinder(1,2.4,2.4);
            }
        }
    }
}

module printall(){
    
    $fs = 0.01;
    $fa = 1;
    housing();
    translate([0,0,4]) buttons(-35,90);
    translate([-10,0,4]) buttons(-35,90);
    
    translate([0,40,4]) bottom();
    translate([-3,17,4]) scale([0.5,7,1]) cube(1);
    translate([-5,19.5,4]) scale([6,0.5,1]) cube(1);
}

//housing();
//bottom(3);
//buttons(0,0);
//buttons(-30,180);

printall();

echo(version=version());
// Written by Marius Kintel <marius@kintel.net>
//
// To the extent possible under law, the author(s) have dedicated all
// copyright and related and neighboring rights to this software to the
// public domain worldwide. This software is distributed without any
// warranty.
//
// You should have received a copy of the CC0 Public Domain
// Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
