var butter_num = 5;
var butters = [];
var butterTexture, butterBumpMap;

function addButterStick(obj, x, y, z){
	'use strict';
	var colour = 0xffa100;
	butterTexture = new THREE.TextureLoader().load("assets/fieldstone.jpg");
	butterBumpMap = new THREE.TextureLoader().load("assets/fieldstone-normal.jpg");
	geometry = new THREE.CubeGeometry(4.5*1.1, 2*1.1, 1.8*1.15);
	material = new THREE.MeshPhongMaterial({color: colour, wireframe: wirefr_cur, shininess: 10, specular: 0x3d311c, map:butterTexture,normalMap:butterBumpMap});
	mesh = new THREE.Mesh(geometry, material);
	mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: colour, wireframe: wirefr_cur});
	mesh.phongMaterial = new THREE.MeshPhongMaterial({color: colour, wireframe: wirefr_cur, shininess: 10, specular: 0x3d311c, map:butterTexture,normalMap:butterBumpMap});
	mesh.basicMaterial = new THREE.MeshBasicMaterial({color: 0xffa100, wireframe: wirefr_cur});
	mesh.position.set(x, y, z);
	/*mesh.castShadow = true;
  mesh.receiveShadow = false;*/
	obj.add(mesh);
}


function createButter(x, y, z, roty) {
  'use strict';
  var butter = new THREE.Object3D();
  addButterStick(butter, 0, 0, 0);
  butter.contour = new THREE.Sphere(new THREE.Vector3(x, y, z), 2);

  butter.position.set(x, y, z);
  butter.rotation.y += roty;
  scene.add(butter);
  return butter;
}

function createButters(){
	var butter_x = [ -30.0, 20.0, 30.0, -15.0 ];
	var butter_y = [ 20.0, -20.0, 30.0, -25.0 ];
  for(i = 0; i != butter_num; i++){
    butters[i] = createButter(butter_x[i], 0.5, -butter_y[i], 0);
  }
}