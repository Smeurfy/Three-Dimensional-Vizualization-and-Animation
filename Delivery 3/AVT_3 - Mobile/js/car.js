var car;
var direction;
var targetObj;

function addTriPrism(obj, x, y, z, lgh, hgh, wdh, rot, pr_color, shine, spec){
    var geometry = new THREE.Geometry();
    geometry.vertices.push( new THREE.Vector3( -lgh/2, -hgh/2, wdh/2 ) );
    geometry.vertices.push( new THREE.Vector3( -lgh/2, -hgh/2, -wdh/2) );
    geometry.vertices.push( new THREE.Vector3( -lgh/2, hgh/2, wdh/2) );
    geometry.vertices.push( new THREE.Vector3( -lgh/2, hgh/2, -wdh/2 ) );
    geometry.vertices.push( new THREE.Vector3( lgh/2, -hgh/2, wdh/2 ) );
    geometry.vertices.push( new THREE.Vector3( lgh/2, -hgh/2, -wdh/2 ) );
    geometry.faces.push(new THREE.Face3(2,1,0));
    geometry.faces.push(new THREE.Face3(4,3,2));
    geometry.faces.push(new THREE.Face3(4,1,0));
    geometry.faces.push(new THREE.Face3(1,2,3));
    geometry.faces.push(new THREE.Face3(3,4,5));
    geometry.faces.push(new THREE.Face3(1,4,5));
    geometry.faces.push(new THREE.Face3(0,2,4));
    geometry.faces.push(new THREE.Face3(1,3,5));
    geometry.computeFaceNormals();
    geometry.computeVertexNormals();
    mesh = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({color: pr_color, wireframe: wirefr_cur, side: THREE.DoubleSide}));
    mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: pr_color, wireframe: wirefr_cur, side: THREE.DoubleSide});
    mesh.phongMaterial = new THREE.MeshPhongMaterial({color: pr_color, shininess: shine, wireframe: wirefr_cur, side: THREE.DoubleSide, specular: spec});
    mesh.basicMaterial = new THREE.MeshBasicMaterial({color: pr_color, wireframe: wirefr_cur, side: THREE.DoubleSide});
    mesh.position.set(x, y, z);
    mesh.rotateY(rot);
    mesh.castShadow = true;
    mesh.receiveShadow = false;
    obj.add(mesh);
  }

  function addTrapPrism(obj, x, y, z, lgh, hgh, wdh, rot, pr_color, trap_ratio){ //only 2.19% gay dont judge me k (note: this has length/height/width as opposed to TriPrism!)
    var geometry = new THREE.Geometry();
    //lower back vertices
    geometry.vertices.push(new THREE.Vector3( -lgh/2, -hgh/2, wdh/2 ));
    geometry.vertices.push( new THREE.Vector3( -lgh/2, -hgh/2, -wdh/2) );
    //upper vertices
    geometry.vertices.push(new THREE.Vector3( -lgh/2, hgh/2, wdh/(2* trap_ratio) ));
    geometry.vertices.push( new THREE.Vector3( -lgh/2, hgh/2, -wdh/(2*trap_ratio)) );
    geometry.vertices.push(new THREE.Vector3( lgh/2, hgh/2, wdh/(2* trap_ratio) ));
    geometry.vertices.push( new THREE.Vector3( lgh/2, hgh/2, -wdh/(2*trap_ratio)) );
    //lower vertices
    geometry.vertices.push( new THREE.Vector3( lgh/2, -hgh/2, wdh/2) );
    geometry.vertices.push( new THREE.Vector3( lgh/2, -hgh/2, -wdh/2 ) );
    //faces
    geometry.faces.push(new THREE.Face3(0,2,3));
    geometry.faces.push(new THREE.Face3(0,3,1));
    geometry.faces.push(new THREE.Face3(0,6,7));
    geometry.faces.push(new THREE.Face3(0,1,7));
    geometry.faces.push(new THREE.Face3(6,4,5));
    geometry.faces.push(new THREE.Face3(6,5,7));
    geometry.faces.push(new THREE.Face3(3,5,1));
    geometry.faces.push(new THREE.Face3(1,5,7));
    geometry.faces.push(new THREE.Face3(0,2,4));
    geometry.faces.push(new THREE.Face3(0,4,6));
    geometry.faces.push(new THREE.Face3(2,3,4));
    geometry.faces.push(new THREE.Face3(3,4,5));
    geometry.computeFaceNormals();
    mesh = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({color: pr_color, wireframe: wirefr_cur, side: THREE.DoubleSide}));
    mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: pr_color, wireframe: wirefr_cur, side: THREE.DoubleSide});
    mesh.phongMaterial = new THREE.MeshPhongMaterial({color: pr_color, shininess: 100, wireframe: wirefr_cur, side: THREE.DoubleSide, specular: 0x255eba});
    mesh.basicMaterial = new THREE.MeshBasicMaterial({color: pr_color, wireframe: wirefr_cur, side: THREE.DoubleSide});
    mesh.position.set(x, y, z);
    mesh.rotateY(rot);
    mesh.castShadow = true;
    mesh.receiveShadow = false;
    obj.add(mesh);
  }
  
function addCarPart(obj, x, y, z, length, height, width){
    'use strict';
    var geometry = new THREE.CubeGeometry(length * car.sz_multi, height * car.sz_multi, width * car.sz_multi);
    mesh = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({color: 0x4286f4, wireframe: wirefr_cur}));
    mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: 0x4286f4, wireframe: wirefr_cur});
    mesh.basicMaterial = new THREE.MeshBasicMaterial({color: 0x4286f4, wireframe: wirefr_cur});
    mesh.phongMaterial = new THREE.MeshPhongMaterial({color: new THREE.Color(0.5, 0, 0), shininess: 100, wireframe: wirefr_cur, specular: 0x255eba});
    mesh.position.set(x * car.sz_multi, y * car.sz_multi, z * car.sz_multi);
    mesh.castShadow = true;
    mesh.receiveShadow = false;
    obj.add(mesh);
  }
  
function addWheel(obj, x, y, z){
	var geometry = new THREE.Geometry();
  geometry.vertices.push(new THREE.Vector3(1 * car.sz_multi * 2, 0 * car.sz_multi * 2, 0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(0.5 * car.sz_multi * 2, Math.sqrt(3)/2 * car.sz_multi * 2, 0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(-0.5 * car.sz_multi * 2, Math.sqrt(3)/2 * car.sz_multi * 2, 0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(-1 * car.sz_multi * 2, 0 * car.sz_multi * 2, 0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(-0.5 * car.sz_multi * 2, -Math.sqrt(3)/2 * car.sz_multi * 2, 0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(0.5 * car.sz_multi * 2, -Math.sqrt(3)/2 * car.sz_multi * 2, 0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(1 * car.sz_multi * 2, 0 * car.sz_multi * 2, -0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(0.5 * car.sz_multi * 2, Math.sqrt(3)/2 * car.sz_multi * 2, -0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(-0.5 * car.sz_multi * 2, Math.sqrt(3)/2 * car.sz_multi * 2, -0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(-1 * car.sz_multi * 2, 0 * car.sz_multi * 2, -0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(-0.5 * car.sz_multi * 2, -Math.sqrt(3)/2 * car.sz_multi * 2, -0.25 * car.sz_multi * 2));
  geometry.vertices.push(new THREE.Vector3(0.5 * car.sz_multi * 2, -Math.sqrt(3)/2 * car.sz_multi * 2, -0.25 * car.sz_multi * 2));
  geometry.faces.push(new THREE.Face3(0, 1, 2));
  geometry.faces.push(new THREE.Face3(0, 2, 3));
  geometry.faces.push(new THREE.Face3(0, 3, 4));
  geometry.faces.push(new THREE.Face3(0, 4, 5));
  geometry.faces.push(new THREE.Face3(6, 7, 8));
  geometry.faces.push(new THREE.Face3(6, 8, 9));
  geometry.faces.push(new THREE.Face3(6, 9, 10));
  geometry.faces.push(new THREE.Face3(6, 10, 11));
  geometry.faces.push(new THREE.Face3(0, 1, 7));
  geometry.faces.push(new THREE.Face3(0, 6, 7));
  geometry.faces.push(new THREE.Face3(1, 2, 8));
  geometry.faces.push(new THREE.Face3(1, 7, 8));
  geometry.faces.push(new THREE.Face3(2, 3, 9));
  geometry.faces.push(new THREE.Face3(2, 8, 9));
  geometry.faces.push(new THREE.Face3(3, 4, 10));
  geometry.faces.push(new THREE.Face3(3, 9, 10));
  geometry.faces.push(new THREE.Face3(4, 5, 11));
  geometry.faces.push(new THREE.Face3(4, 10, 11));
  geometry.faces.push(new THREE.Face3(5, 0, 6));
  geometry.faces.push(new THREE.Face3(5, 11, 6));
  geometry.computeFaceNormals();
  geometry.computeVertexNormals();
  var colour = new THREE.Color(0.02, 0.02, 0.02);
  mesh = new THREE.Mesh( geometry, new THREE.MeshLambertMaterial({color: colour, side: THREE.DoubleSide, wireframe: wirefr_cur}));
  mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: colour, side: THREE.DoubleSide, wireframe: wirefr_cur});
  mesh.phongMaterial = new THREE.MeshPhongMaterial({color: colour, shininess: 5, side: THREE.DoubleSide, wireframe: wirefr_cur, specular: 0x000000});
  mesh.basicMaterial = new THREE.MeshBasicMaterial({color: 0x333333, wireframe: wirefr_cur, side: THREE.DoubleSide});
  mesh.position.set(x * car.sz_multi, y * car.sz_multi, z * car.sz_multi);
  mesh.castShadow = true;
  mesh.receiveShadow = false;
  obj.add(mesh);
}
  
  
  function addTargetObject(x, y, z) {
    targetObj = new THREE.Object3D();
    scene.add(targetObj);
    targetObj.position.set(x, y, z);
  }
  
  function createCar(x,y,z){
    "use strict";
    car = new THREE.Object3D();
    car.name = 'car';
    car.body = new THREE.Object3D();
    car.wheels = new THREE.Object3D();
    car.headlights = new THREE.Object3D();
    car.add(car.wheels);
    car.add(car.body);
    car.add(car.headlights);

    car.sz_multi = 0.2;         //testing var which keeps proportions of car while changing overall size, remove later! or not
    car.brake_accel = 100;    //""negative"" acceleration when braking
    car.norm_accel = 40;      //normal acceleration
    car.back_accel = 20;      //backwards acceleration
    car.max_veloc = 40;       //max speed
    car.max_veloc_back = 30;  //max backwards speed
    car.turn_rate = 56;       //fraction of pi which the car turns at each frame
    car.cur_angle = 0;        //current car rotation for turns

    addCarPart(car.body, -2, 1.5, 0, 6, 1.5, 5); //body
    addCarPart(car.body, -3.25, 2.75, 0, 3.5, 1, 5); //body
    addCarPart(car.body, 3.5, 2, 0, 5, 2, 3);  //front
    addTriPrism(car.body, 7 * car.sz_multi, 2.25 * car.sz_multi, 0 * car.sz_multi, 2 * car.sz_multi, 1.5 * car.sz_multi, 6 * car.sz_multi, 0, 0x4286f4, 100, 0x255eba);  //front spoiler
    addCarPart(car.body, -5.5, 2.5, 0, 1, 1, 2); //back piece
    addCarPart(car.body, -6.5, 2.5, 0, 1, 1, 8);   //back spoiler
    addTriPrism(car.body, -6.5 * car.sz_multi, 3.5 * car.sz_multi, 0 * car.sz_multi, 1 * car.sz_multi, 1 * car.sz_multi, 8 * car.sz_multi, 0, 0x4286f4, 100, 0x255eba);  //back spoiler, top
    addTriPrism(car.body, -3.25 * car.sz_multi, 3.75 * car.sz_multi, 0 * car.sz_multi, 3.5 * car.sz_multi, 1 * car.sz_multi, 2.5 * car.sz_multi, Math.PI, 0x4286f4, 100, 0x255eba);  //seat
    addTrapPrism(car.body, -0.25 * car.sz_multi, 2.75 * car.sz_multi, 0 * car.sz_multi, 2.5 * car.sz_multi, 1 * car.sz_multi, 5 * car.sz_multi, 0, 0x4286f4, 2); //trapezoid

    addWheel(car.wheels, -2.5, 2, 3.125);
    addWheel(car.wheels, 3.5, 2, 2);
    addWheel(car.wheels, -2.5, 2, -3.125);
    addWheel(car.wheels, 3.5, 2, -2);

    car.position.x = x;
    car.position.y = y;
    car.position.z = z;
    
    addTargetObject(car.position.x + 5, car.position.y, car.position.z);
    addHeadlight(car.headlights, 1, 0.5, -0.25);
    addHeadlight(car.headlights, 1, 0.5, 0.25);
    car.contour = new THREE.Sphere(new THREE.Vector3(x, y, z), 8.06 * car.sz_multi);
    car.butterCollision = false;
    car.orangeCollision = false;
    car.cheerioCollision = false;
    scene.add(car);
    return car;
  }

  function moveCar(delta){
    'use strict';
    var distance = 0;
    if(go_right && delta){
      car.cur_angle += Math.PI / car.turn_rate;
      direction.x = Math.cos(car.cur_angle);
      direction.z = Math.sin(car.cur_angle);
      car.rotateY(-Math.PI / car.turn_rate);
    }
    if(go_left && delta){
      car.cur_angle -= Math.PI / car.turn_rate;
      direction.x = Math.cos(car.cur_angle);
      direction.z = Math.sin(car.cur_angle);
      car.rotateY(Math.PI / car.turn_rate);
    }
    if(forward){
      if(car.cur_veloc < car.max_veloc){
        car.cur_veloc += delta * car.norm_accel;
        distance = car.cur_veloc * delta;
      }
      else{
        car.cur_veloc = car.max_veloc;
        distance = car.cur_veloc * delta;
      }
    }
    if(backward){
      if(car.cur_veloc > -car.max_veloc_back){
        car.cur_veloc -= delta * car.back_accel;
        distance = car.cur_veloc * delta;
      }
      else{
        car.cur_veloc = -car.max_veloc_back;
        distance = car.cur_veloc * delta;
      }
    }
    if(!forward && !backward && car.cur_veloc != 0){
      if(last_dir === 1){
        if(car.cur_veloc > 0)
          car.cur_veloc -= delta * car.brake_accel;
        else if (car.cur_veloc < 0){
          car.cur_veloc = 0;
        }
        var distance = car.cur_veloc * delta;
      }
      if(last_dir === 2){
        if(car.cur_veloc < 0)
          car.cur_veloc += delta * car.brake_accel;
        else if (car.cur_veloc > 0)
          car.cur_veloc = 0;
        var distance = car.cur_veloc * delta;
      }
    }
    if (car.butterCollision) {
      if (forward) {
        car.cur_veloc = -20;
        last_dir = 2;
      }
      else if (backward) {
        car.cur_veloc = 10;
        last_dir = 1;
      }
      forward = false;
      backward = false;
      car.butterCollision = false;
    }
    if (car.orangeCollision) {
      carReset(car);
      car.orangeCollision = false;
    }
    if (car.cheerioCollision){
      car.cur_veloc *= 1;
      car.cheerioCollision = false;
    }
    car.position.x += distance * direction.x;
    car.position.z += distance * direction.z;
    car.contour.center.x = car.position.x;
    car.contour.center.z = car.position.z;
    targetObj.position.x = car.position.x + 5*direction.x;
    targetObj.position.z = car.position.z + 5*direction.z;

  }

  function carReset(car){
    car.position.set(car_first_pos.x, car_first_pos.y, car_first_pos.z);
    car.contour.center.set(car_first_pos.x, car_first_pos.y, car_first_pos.z);
    car.rotateY(car.cur_angle);
    car.cur_veloc = 0;
    car.cur_angle = 0;
    direction.set(1,0,0);
  }