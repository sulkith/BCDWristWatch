// CSG.scad - Basic example of CSG usage


module housingBody(height,inside,lid){
        if (inside)
        {
            //space for wrist strap
            translate([-43.5+5,-10,0])scale([20,20,height+5])cube(1);
            translate([23.5-5,-10,0])scale([20,20,height+5])cube(1);
            
            translate([-21,11.5,height-2.5]) {//holes for Mounting
                
                rotate(a=[90,0,0])cylinder(23,0.5,0.5);
            }
            translate([21,11.5,height-2.5]) {//holes for Mounting
                rotate(a=[90,0,0])cylinder(23,0.5,0.5);
            }
            
            
            translate([0,0,-1])cylinder(2.2,16,16);//glass
            cylinder(height+2,15,15);//mounted parts
            translate([0,0,2.2+1.5]) {//PCB Mounting
                cylinder(height,15.5,15.5);
            }
            
            translate([-1,20,7]) {//Holes for buttons
                rotate(a=[90,0,0])
                {
                    
                    cylinder(40,1.5,1.5);
                    translate([0,0,3])
                    {
                        cylinder(34,2.3,2.3); 
                    }
                }
            }
            translate([14.5,11,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([-14.5,11,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([-14.5,-11,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([14.5,-11,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([0,0,height+3])scale([37.1,40,6])cube(1,center=true);
        }
        else
        {
            difference(){
                hull()union(){
                    //translate([0,0,height-1])scale([47,25,2])cube(1, center=true);
                    cylinder(5,17,18.5);
                    translate([0,0,5])cylinder(height-5,18.5,18.5);
                    translate([0,0,height])cylinder(2,18.5,17.5);
                    translate([-21,12.5,height-2.5]) {//wrist smoothing
                        rotate(a=[90,0,0])
                        {
                            cylinder(25,2.5,2.5);
                        }
                    }
                    translate([21,12.5,height-2.5]) {//wrist smoothing
                        rotate(a=[90,0,0])
                        {
                            cylinder(25,2.5,2.5);
                        }
                    }
                }
                if(lid)
                {
                    union()
                    {   
                        difference(){
                            cube(50,center=true);
                            translate([0,0,height+3])scale([36.9,40,7])cube(1,center=true);
                            translate([0,0,height-1])cylinder(2,15.4,15.4);
                        }
                        translate([14.5,11,9]) {
                            cylinder(4,0.7,0.7);
                        }
                        translate([-14.5,11,9]) {
                            cylinder(4,0.7,0.7);
                        }
                        translate([-14.5,-11,9]) {
                            cylinder(4,0.7,0.7);
                        }
                        translate([14.5,-11,9]) {
                            cylinder(4,0.7,0.7);
                        }
                    }
                }
            }
        }
        
}
module button(){
    color([0,0,0])rotate([180,0,0])
    {
        cylinder(5,1.4,1.4);
        cylinder(1,2.2,2.2);
    }
}

module pcb(){
    color([0,0,0]) cylinder(1,15.5,15.5);
    translate([5,0,1])cylinder(4,10,10);
    translate([-9,0,-0.5])scale([5,5,1])cube(1,center=true);
    translate([-3,0,-0.5])scale([1.2,1.8,1])cube(1,center=true);
    translate([-3,4,-0.5])scale([0.8,1.6,1])cube(1,center=true);
    translate([-3,-4,-0.5])scale([0.8,1.6,1])cube(1,center=true);
    color([1,0,0]){
        translate([0,-2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([0,2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([0,6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([0,-6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        
        translate([3,-2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([3,2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([3,6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([3,-6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        
        translate([6,-2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([6,2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([6,6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([6,-6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        
        translate([9,-2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([9,2,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([9,6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([9,-6,-0.5])scale([0.8,1.6,1])cube(1,center=true);
    }
    translate([5,11.7,1.4])scale([5.1,3,0.5])cube(1,center=true);
    translate([5,-11.7,1.4])scale([5.1,3,0.5])cube(1,center=true);
    
    translate([-2,12.5,1.4])scale([6.4,3.5,1.4])cube(1,center=true);
    color([0,0,1])translate([-2,12.7,1.4])scale([2.6,4.5,2.4])cube(1,center=true);
    translate([-2,-12.5,1.4])scale([6.4,3.5,1.4])cube(1,center=true);
    color([0,0,1])translate([-2,-12.7,1.4])scale([2.6,4.5,2.4])cube(1,center=true);
}
module housingBodyAll(height){
    difference()
    {
        housingBody(height,false);
        housingBody(height,true);
    }
}

module printall(){
    
    $fs = 0.01;
    $fa = 1;
    
    housingBodyAll(10);
    translate([0,0,20])housingBody(10,false,true);
    translate([0,0,20.7])pcb();
    translate([5,7,7])button();
    translate([5,-7,7])button();
    translate([5,0,6.5])scale([0.5,35,1])cube(1,center=true);
    translate([0,0,6.5])scale([32,0.5,1])cube(1,center=true);
    translate([5,11,6.5])scale([0.5,1,24])cube(1);
    translate([5,-12,6.5])scale([0.5,1,24])cube(1);
    translate([-9.5,-0.5,6.5])scale([0.5,1,24])cube(1);

}

module rendering()
{
    //$fs = 0.01;
    //$fa = 1;
    housingBodyAll(10);
    //translate([0,0,10])lid();
    housingBody(10,false,true);
    translate([0,0,3.7])pcb();
    translate([-1,15,7])rotate([90,0,0])button();
    translate([-1,-15,7])rotate([-90,0,0])button();
}
//printall();

rendering();
//translate([0,0,0])housingBody(10,false);
//translate([0,0,10])lid();
//for Debugging
//housingBodyAll(10);
//housingBody(10,false,true);
//translate([0,50,0])housingBody(10,true);


echo(version=version());