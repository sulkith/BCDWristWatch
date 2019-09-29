// CSG.scad - Basic example of CSG usage

ClockHeight = 10;
module housingBody(_height,inside,lid){
    glassClamp = 0.1;
    glassSize = 1.5;
    PCBSize = 1.6;
    PCBFrontKeepout = 1.4;
    PCBBackKeepout = 3.9;
    LIDSize=1.5;
    height=glassClamp+glassSize+PCBSize+PCBFrontKeepout+PCBBackKeepout+LIDSize;
    echo("checking ig the requested Size is met");
    assert(_height==height);
    
    
    mHx=14.;
    mHy=10.5;
    
        if (inside)
        {
            //space for wrist strap
            translate([-43.5+5,-10,0])scale([20,20,height+5])cube(1);
            translate([23.5-5,-10,0])scale([20,20,height+5])cube(1);
            
            translate([-21,11.5,height-3.5]) {//holes for Mounting
                
                rotate(a=[90,0,0])cylinder(23,0.5,0.5);
            }
            translate([21,11.5,height-3.5]) {//holes for Mounting
                rotate(a=[90,0,0])cylinder(23,0.5,0.5);
            }
            
            
            translate([0,0,-1])cylinder(glassClamp+1.1,15.9,15.9);//glass Clamp
            translate([0,0,glassClamp])cylinder(glassSize,16,16);//glass
            cylinder(height+2,15,15);//mounted parts
            translate([0,0,glassSize+PCBFrontKeepout]) {//PCB Mounting
                cylinder(height,15.5,15.5);
            }
            /*
            translate([-2,20,glassSize+PCBFrontKeepout+PCBSize+2.3]) {//Holes for buttons
                rotate(a=[90,0,0])
                {
                    
                    cylinder(40,1.5,1.5);
                    translate([0,0,3])
                    {
                        cylinder(34,2.4,2.4);
                    }
                }
            }
            */
            translate([mHx,mHy,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([-mHx,mHy,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([-mHx,-mHy,height-5]) {
                cylinder(6,0.7,0.7);
            }
            translate([mHx,-mHy,height-5]) {
                cylinder(6,0.7,0.7);
            }
            //translate([0,0,height+3-LIDSize])scale([37.1,40,6])cube(1,center=true);
            translate([0,0,height-LIDSize-1]){
                //cylinder(6,18,18);
                translate([11.5,7,0])cylinder(1,3.5,3.5);
                translate([11.5,-7,0])cylinder(1,3.5,3.5);
            }
            translate([0,0,height-LIDSize-0.1])hull()
            {
                cylinder(6,17,17);
                //translate([-14.5,-11,0]) {
                //    cylinder(6,3,3);
                //}
                translate([14.5,11,0]) {
                    cylinder(6,2,2);
                }
                translate([-14.5,11,0]) {
                    cylinder(6,2,2);
                }
                translate([-14.5,-11,0]) {
                    cylinder(6,2,2);
                }
                translate([14.5,-11,0]) {
                    cylinder(6,2,2);
                }
                //cylinder(6,16,16);
            }
        }
        else
        {
            difference(){
                hull()
                union(){
                    //translate([0,0,height-1])scale([47,25,2])cube(1, center=true);
                    minkowski(){
                        translate([0,0,0])cylinder(5,16.5,18.5);
                        sphere(0.7);
                    }
                    translate([0,0,5])cylinder(height-5-LIDSize,18.5,18.5);
                    translate([0,0,height-LIDSize])cylinder(LIDSize,18.5,17.5);
                    translate([-21,12.5,height-3.5]) {//wrist smoothing
                        rotate(a=[90,0,0])
                        {
                            cylinder(25,2.5,2.5);
                        }
                    }
                    translate([21,12.5,height-3.5]) {//wrist smoothing
                        rotate(a=[90,0,0])
                        {
                            cylinder(25,2.5,2.5);
                        }
                    }
                    translate([14.5,11,height-LIDSize]) {
                        cylinder(LIDSize,2.5,2.5);
                    }
                    translate([-14.5,11,height-LIDSize]) {
                        cylinder(LIDSize,2.5,2.5);
                    }
                    translate([-14.5,-11,height-LIDSize]) {
                        cylinder(LIDSize,2.5,2.5);
                    }
                    translate([14.5,-11,height-LIDSize]) {
                        cylinder(LIDSize,2.5,2.5);
                    }
                }
                if(lid)
                {
                    union()
                    {   
                        difference(){
                            cube(100,center=true);
                            //translate([0,0,height+3-LIDSize+0.1])scale([36.9,40,6])cube(1,center=true);
                            //translate([0,0,height-LIDSize+0.1])cylinder(LIDSize-1,15.4,15.4);
                            translate([0,0,height-LIDSize])hull()
                            {
                                cylinder(6,16.9,16.9);
                                //translate([-14.5,-11,0]) {
                                //    cylinder(6,3,3);
                                //}
                                translate([14.5,11,0]) {
                                    cylinder(6,1.9,1.9);
                                }
                                translate([-14.5,11,0]) {
                                    cylinder(6,1.9,1.9);
                                }
                                translate([-14.5,-11,0]) {
                                    cylinder(6,1.9,1.9);
                                }
                                translate([14.5,-11,0]) {
                                    cylinder(6,1.9,1.9);
                                }
                                //cylinder(6,16,16);
                            }
                        }
                        translate([mHx,mHy,height-6]) {
                            cylinder(9,0.7,0.7);
                        }
                        translate([-mHx,mHy,height-6]) {
                            cylinder(9,0.7,0.7);
                        }
                        translate([-mHx,-mHy,height-6]) {
                            cylinder(9,0.7,0.7);
                        }
                        translate([mHx,-mHy,height-6]) {
                            cylinder(9,0.7,0.7);
                        }
                        translate([mHx,mHy,height-0.8]) {
                            cylinder(0.9,0.7,1.8);
                        }
                        translate([-mHx,mHy,height-0.8]) {
                            cylinder(0.9,0.7,1.8);
                        }
                        translate([-mHx,-mHy,height-0.8]) {
                            cylinder(0.9,0.7,1.8);
                        }
                        translate([mHx,-mHy,height-0.8]) {
                            cylinder(0.9,0.7,1.8);
                        }
                        translate([-2,20,glassSize+PCBFrontKeepout+PCBSize+2.3]) {//Holes for buttons
                            rotate(a=[90,0,0])
                            {
                                
                                cylinder(40,1.5,1.5);
                                translate([0,0,3])
                                {
                                    translate([0,0,0])cylinder(6,2.5,2.5);
                                    translate([0,0,28])cylinder(6,2.5,2.5); 
                                }
                            }
                        }
                    }
                }
            }
        }
     echo("Housing Size is ");
     echo(height);
}
module button(){
    color([0,0,0])rotate([180,0,0])
    {
        cylinder(5,1.4,1.4);
        cylinder(0.8,2.2,2.2);
    }
}

module pcb(){
    color([0,0,0]) cylinder(1,15.5,15.5);
    translate([5,0,1])cylinder(3.9,10,10);
    translate([-9,0,-0.5])scale([5,5,1])cube(1,center=true);
    translate([-3,0,-0.5])scale([1.2,1.8,1])cube(1,center=true);
    translate([-3,4,-0.5])scale([0.8,1.6,1])cube(1,center=true);
    translate([-3,-4,-0.5])scale([0.8,1.6,1])cube(1,center=true);
    color([1,0,0]){
        translate([0,-3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([0,3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([0,8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([0,-8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        
        translate([3,-3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([3,3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([3,8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([3,-8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        
        translate([6,-3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([6,3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([6,8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([6,-8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        
        translate([9,-3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([9,3,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([9,8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
        translate([9,-8,-0.5])scale([0.8,1.6,1])cube(1,center=true);
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
    
    housingBodyAll(ClockHeight);
    translate([0,0,8])housingBody(ClockHeight,false,true);
    //translate([0,0,20.7])pcb();
    //translate([5,7,6.1])rotate([180,0,0])button();
    //translate([5,-7,6.1])rotate([180,0,0])button();
    translate([5,0,6.5])scale([0.5,35,0.8])cube(1,center=true);
    translate([0,0,6.5])scale([32,0.5,0.8])cube(1,center=true);
    translate([5,11,6.0])scale([0.5,0.8,12])cube(1);
    translate([5,-12,6.0])scale([0.5,0.8,12])cube(1);
    translate([-9.5,-0.5,6.0])scale([0.5,0.8,12])cube(1);

}
module printSLASideBySide(){
    
    translate([0,0,ClockHeight])rotate([180,0,0])housingBodyAll(ClockHeight);
    translate([0,37,-ClockHeight+1.5])housingBody(ClockHeight,false,true);
    //translate([0,0,20.7])pcb();
    //translate([5,7,0])rotate([180,0,0])button();
    //translate([5,-7,0])rotate([180,0,0])button();
}

module rendering()
{
    //$fs=0.05;
    //$fa=5;

    housingBodyAll(ClockHeight);
    housingBody(ClockHeight,false,true);
    //translate([0,0,-15])housingBody(10,false,true);
    translate([0,0,2.8])pcb();
    //translate([-2,15,6.1])rotate([90,0,0])button();
    //translate([-2,-15,6.1])rotate([-90,0,0])button();
}
//printall();

rendering();
//printSLASideBySide();
//translate([0,0,0])housingBody(10,false);
//translate([0,0,10])lid();
//for Debugging
//housingBodyAll(ClockHeight);
//housingBody(10,false,true);
//translate([0,50,0])housingBody(10,true);


echo(version=version());
