var cheerios_radius = 0.5;
var cheerios_tube = 0.3;
var cheerios_radsegm = 6;
var cheerios_tubsegm = 6;
var cheerios = []

function createRoad(){
  
  var cheerios_x = [  //OUTER RING
							-46.0,  -42.0,  -38.0,  -34.0,  -30.0,  -26.0, 
							-22.0,  -18.0,  -14.0,  -10.0,    -6.0,    -2.0,   
							  2.0,      6.0,     10.0,    14.0,    18.0,    22.0,  
							 26.0,    30.0,    34.0,    38.0,    42.0,    46.0,  

							 46.0,   46.0,   46.0,    46.0,    46.0,    46.0,  
							 46.0,    46.0,    46.0,    46.0,    46.0,    46.0,  
							 46.0,    46.0,    46.0,    46.0,    46.0,

							 46.0,    42.0,    38.0,    34.0,    30.0,    26.0,  
							 22.0,    18.0,   14.0,    10.0,      6.0,   2.0,   
							 -2.0,    -6.0,   -10.0,  -14.0,  -18.0,  -22.0, 
							-26.0,  -30.0,  -34.0,  -38.0,  -42.0,  -46.0,

							-46.0,  -46.0,  -46.0,  -46.0,  -46.0,  -46.0, 
							-46.0,  -46.0,  -46.0,  -46.0,  -46.0,  -46.0,
							-46.0,  -46.0,  -46.0,  -46.0,  -46.0,  -46.0, 
							-46.0,  -46.0,  -46.0,  -46.0,  -46.0, 

							//INNER RING
																								  -26.0, 
							-22.0,  -18.0,  -14.0,  -10.0,    -6.0,    -2.0,   
							  2.0,      6.0,     10.0,    14.0,    18.0,    22.0,  
							 26.0,

							 26.0,    26.0,    26.0,    26.0,    26.0,    26.0,  
							 26.0,  

							 24.0,    20.0,    16.0,    12.0,     8.0,      4.0,   
							  0.0,    -4.0,    -8.0,  -12.0,  -16.0,  -20.0, 
							-24.0, 

							-26.0,  -26.0,  -26.0,  -26.0,  -26.0,  -26.0, 
							-26.0,  -26.0,  -26.0, -26.0,  -26.0,  -26.0, 
							-26.0 ];

	var cheerios_y = [-46.0, -46.0,  -46.0,  -46.0,  -46.0,  -46.0,
							-44.0,  -40.0,  -36.0,  -32.0,  -28.0,  -24.0, 
							-22.0,  -22.0,  -22.0,  -22.0, 	-22.0,  -22.0,
							-22.0,  -22.0,  -22.0,  -22.0, 	-22.0,  -22.0, 

							-20.0,  -16.0,  -12.0,   -8.0,    -4.0,     0.0,   
							  4.0,   ,   8.0,    12.0,    16.0,    20.0,    24.0,  
							 28.0,    32.0,    36.0,    40.0,    44.0,

							 46.0,    46.0,    46.0,    46.0,    46.0,    46.0,  
							 46.0,    46.0,    46.0,    46.0,    46.0,    46.0,  
							 46.0,    46.0,    46.0,    46.0,    46.0,    46.0,  
							 46.0,    46.0,    46.0,    46.0,    46.0,    46.0,

							 46.0,    42.0,    38.0,    34.0,    30.0,    26.0,  
							 22.0,    18.0,    14.0,    10.0,      6.0,      2.0,   
							 -2.0,    -6.0,   -10.0,  -14.0,  -18.0,  -22.0, 
							-26.0,  -30.0,  -34.0,  -38.0,  -42.0, 

							//INNER RING
																								  -26.0, 
							-24.0,  -20.0,  -16.0,  -12.0,   -8.0,    -4.0,  
							 -2.0,    -2.0,    -2.0,    -2.0,    -2.0,    -2.0,  
							 -2.0,

							  0.0,      4.0,      8.0,    12.0,    16.0,    20.0,  
							 24.0,  

							 26.0,   26.0,    26.0,    26.0,    26.0,    26.0, 
							 26.0,    26.0,    26.0,    26.0,    26.0,    26.0,  
							 26.0, 

							 24.0,    20.0,    16.0,    12.0,     8.0,      4.0,   
							  0.0,    -4.0,    -8.0,  -12.0,  -16.0,  -20.0, 
							-24.0 ];
	for(i=0; i<cheerios_x.length; i++){						
		createTorus(cheerios_x[i], 0, -cheerios_y[i], 1,0)
	}
}

function createTorus(x, y, z, l, s){
  var colour = 0xffa500;
  var geometry = new THREE.TorusGeometry( cheerios_radius, cheerios_tube, cheerios_radsegm, cheerios_tubsegm ); //radius, tube, radialSegments, tubularSegments, arc
  var torus = new THREE.Mesh( geometry, new THREE.MeshLambertMaterial( { color: colour, wireframe: wirefr_cur } ));
  torus.phongMaterial = new THREE.MeshPhongMaterial( { color: colour, shininess: 5, wireframe: wirefr_cur, specular: 0x443211 } );
  torus.gouraudMaterial = new THREE.MeshLambertMaterial( { color: colour, wireframe: wirefr_cur } );
  torus.basicMaterial = new THREE.MeshBasicMaterial({color: 0xffa500, wireframe: wirefr_cur});
  torus.name = "cheerio";
  scene.add( torus );

  torus.position.x = x ;
  torus.position.z = z;
  torus.position.y = y + cheerios_tube;
  
  torus.startPosition = new THREE.Vector3(x, y+cheerios_tube,z);
  torus.collision = false;
  torus.speed = 0;
  torus.dir = new THREE.Vector3(0,0,0);
  torus.fb = 1; //1 = going forward, 2 = going backwards
  torus.contour = new THREE.Sphere(new THREE.Vector3(x, y + cheerios_tube, z), cheerios_radius + cheerios_tube);
  cheerios.push(torus);
  torus.rotation.x = Math.PI / 2;
  torus.hasCalculated = false;
}

function cheerioHit(delta){
  for (i=0; i < cheerios.length; i++){
  	if (cheerios[i].collision){
  	  cheerios[i].collision = false;
  	  cheerios[i].speed = car.cur_veloc * 1.7;
      cheerios[i].dir = new THREE.Vector3(Math.cos(car.cur_angle), 0, Math.sin(car.cur_angle));
      if(cheerios[i].speed > 0)
  	    cheerios[i].fb = 1;
      if(cheerios[i].speed < 0)
        cheerios[i].fb = 2;
  	}
    if (cheerios[i].cheerioCollision) {
      if (cheerios[i].collidesWith &&
        cheerios[i].hasCalculated === false &&
        cheerios[cheerios[i].collidesWith].hasCalculated === false) {
          if(!cheerios[i].speed && !cheerios[cheerios[i].collidesWith].speed){
            var vec = vectorbetween(cheerios[i], cheerios[cheerios[i].collidesWith]);
            cheerios[i].position.x -= (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * vec.x;
            cheerios[i].position.z -= (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith])  + 0.05)/2 * vec.z;
            cheerios[cheerios[i].collidesWith].position.x += (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * vec.x;
            cheerios[cheerios[i].collidesWith].position.z += (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * vec.z;
          }
          else if (cheerios[i].speed >= cheerios[cheerios[i].collidesWith].speed) {
            cheerios[i].position.x -= (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[i].dir.x;
            cheerios[i].position.z -= (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith])  + 0.05)/2 * cheerios[i].dir.z;
            cheerios[cheerios[i].collidesWith].position.x += (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[i].dir.x;
            cheerios[cheerios[i].collidesWith].position.z += (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[i].dir.z;
            cheerios[cheerios[i].collidesWith].speed = cheerios[i].speed * 0.9;
            cheerios[i].speed *= 0.05;
            cheerios[cheerios[i].collidesWith].dir = cheerios[i].dir;
          }
          else {
            cheerios[i].position.x += (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[cheerios[i].collidesWith].dir.x;
            cheerios[i].position.z += (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[cheerios[i].collidesWith].dir.z;
            cheerios[cheerios[i].collidesWith].position.x -= (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[cheerios[i].collidesWith].dir.x;
            cheerios[cheerios[i].collidesWith].position.z -= (2 * cheerios_radius - dist(cheerios[i], cheerios[cheerios[i].collidesWith]) + 0.05)/2 * cheerios[cheerios[i].collidesWith].dir.z;
            cheerios[i].speed = cheerios[cheerios[i].collidesWith].speed * 0.9;
            cheerios[i].speed *= 0.05;
            cheerios[i].dir = cheerios[cheerios[i].collidesWith].dir;
          }
        cheerios[i].hasCalculated = true;
        cheerios[cheerios[i].collidesWith].hasCalculated = true;
      }
      cheerios[i].cheerioCollision = false;
    }
    if(cheerios[i].speed){
      if(cheerios[i].fb === 1){
        if(cheerios[i].speed > 0){
      	  cheerios[i].speed -= delta * 80;
      	}
        else if (cheerios[i].speed < 0){
          cheerios[i].speed = 0;
        }
      }
      if(cheerios[i].fb === 2){
        if(cheerios[i].speed < 0){
          cheerios[i].speed += delta * 80;
        }
        else if (cheerios[i].speed > 0){
          cheerios[i].speed = 0;
        }
      }
      var distance = cheerios[i].speed * delta;
    	cheerios[i].position.x += distance * cheerios[i].dir.x;
      cheerios[i].position.z += distance * cheerios[i].dir.z;
      cheerios[i].contour.center.x = cheerios[i].position.x;
      cheerios[i].contour.center.z = cheerios[i].position.z;
    }
  }
}

function cheerioReset(cheerio){
	cheerio.position.copy(cheerio.startPosition);
	cheerio.contour.center.copy(cheerio.startPosition);
	cheerio.speed = 0;
	cheerio.dir.set(0,0,0);
}