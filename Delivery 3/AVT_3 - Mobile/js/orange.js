var orange_num = 5;
var oranges = [];
var interval = 0;

function createStalk(obj, x, y, z) {
  geometry = new THREE.CylinderGeometry(0.25, 0.25, 2, 6, 2);
  material = new THREE.MeshLambertMaterial({color: 0x614126, wireframe: wirefr_cur});
  stalk = new THREE.Mesh(geometry, material);
  stalk.position.set(x, y, z);
  stalk.phongMaterial = new THREE.MeshPhongMaterial({color: 0x614126, shininess: 50, wireframe: wirefr_cur});
  stalk.gouraudMaterial = new THREE.MeshLambertMaterial({color: 0x614126, wireframe: wirefr_cur});
  stalk.basicMaterial = new THREE.MeshBasicMaterial({color: 0x614126, wireframe: wirefr_cur});
  stalk.castShadow = true;
  stalk.receiveShadow = false;
  obj.add(stalk);
}

function createOrange(x, y, z){
  geometry = new THREE.SphereGeometry(3, 9, 9);
  orange = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({color: 0xff5d00, wireframe: wirefr_cur}));
  orange.phongMaterial = new THREE.MeshPhongMaterial({color: 0xff5d00, shininess: 10, wireframe: wirefr_cur, specular: 0x4f3425});
  orange.gouraudMaterial = new THREE.MeshLambertMaterial({color: 0xff5d00, wireframe: wirefr_cur});
  orange.basicMaterial = new THREE.MeshBasicMaterial({color: 0xff5d00, wireframe: wirefr_cur});
  orange.startPosition = new THREE.Vector3(x, y, z);
  orange.position.set(x, y, z);
  orange.name = "orange";
  createStalk(orange, 0, 3, 0);
  scene.add(orange);
  orange.contour = new THREE.Sphere(new THREE.Vector3(x, y, z), 4);                                               //for collision
  orange.angle = Math.random() * Math.PI * 2;                                                                     //for getting a direction/roll vector
  orange.startAngle = orange.angle;
  orange.direction = new THREE.Vector3(Math.cos(orange.angle), 0, Math.sin(orange.angle));                        //random direction for orange
  orange.roll = new THREE.Vector3(Math.cos(orange.angle + Math.PI / 2), 0, Math.sin(orange.angle + Math.PI / 2)); //vector on which the orange rolls around
  orange.speed = Math.floor(Math.random() * 24) + 1;                                                              //random orange velocity
  orange.startSpeed = orange.speed;
  orange.turnRate = 120;
  orange.edgeCollision = false;
  orange.castShadow = true;
  orange.receiveShadow = false;
  return orange;
}

function createOranges(){
  for(i = 0; i != orange_num; i++){
    oranges[i] = createOrange((Math.floor(Math.random() * 121) - 60), 4, (Math.floor(Math.random() * 81) - 40));
  }
}
function rollOranges(delta){
  for(i = 0; i != orange_num; i++){
    var distance = delta * oranges[i].speed;
    oranges[i].position.x += distance * oranges[i].direction.x;
    oranges[i].position.z += distance * oranges[i].direction.z;
    oranges[i].contour.center.x = oranges[i].position.x;
    oranges[i].contour.center.z = oranges[i].position.z;
    if(delta)
      oranges[i].rotateOnAxis(oranges[i].roll, -Math.PI / oranges[i].turnRate);
    if(oranges[i].position.x > 90 || oranges[i].position.x < -90 || oranges[i].position.z > 50 || oranges[i].position.z < -50){
	  if (!interval){
		oranges[i].edgeCollision = true;
		scene.remove(oranges[i]);
		}
	  var orange = oranges[i];
	  if (orange.edgeCollision){
		orange.edgeCollision = false;
		interval = setTimeout(function(){
			orange.rotation.x = 0;
			orange.rotation.y = 0;
			orange.rotation.z = 0;
			orange.position.set((Math.floor(Math.random() * 121) - 60), 5, (Math.floor(Math.random() * 81) - 40));
			orange.angle = Math.random() * Math.PI * 2;
			orange.direction = new THREE.Vector3(Math.cos(orange.angle), 0, Math.sin(orange.angle));
			orange.roll = new THREE.Vector3(Math.cos(orange.angle + Math.PI / 2), 0, Math.sin(orange.angle + Math.PI / 2));
      if(cheerios[0].material instanceof THREE.MeshLambertMaterial)
        orange.material = orange.gouraudMaterial;
      else if(cheerios[0].material instanceof THREE.MeshPhongMaterial)
        orange.material = orange.phongMaterial;
      else
        orange.material = orange.basicMaterial;
      orange.material.wireframe = wirefr_cur;
			scene.add(orange);
			interval = 0;
		  }, 1500, orange);
		}
	}
  }
  if(speed_inc){
    for(i = 0; i != orange_num; i++){
      oranges[i].speed += 1;
	  if (oranges[i].turnRate >= 20)
		oranges[i].turnRate -= 1;
	}
    speed_inc = false;
    speed_inc_timing = false;
  }
}

function orangeReset(orange){
	orange.position.copy(orange.startPosition);
	orange.contour.center.copy(orange.startPosition);
	orange.speed = orange.startSpeed;
	orange.angle = orange.startAngle;
}