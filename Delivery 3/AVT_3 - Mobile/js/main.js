var renderer, scene, effect, controls;
var material, geometry, mesh;

var clock;
var directionalLight;
var aspect;
var cars = [];
var table;
var msgplane;

var billboardsNr = 28;
var billboards = [];

var stereo_mode = true;

var tick =0;
var inc = 0;
var halfway = false;

//assorted flags and "flags"
var speed_inc_timing = false;
var speed_inc = false;
var nightday = false;
var disable_lighting = false;
var pause = false;
var headlights = false;
var end = false;
var restart = false;
var wirefr_cur = false;
var change_wf = false;
var lives = 5;
var change_shading = false;
var turn_off_lights = false;
var fog = false;
var cur_shading = 'phong';
var cam = 'fpv';
var new_cam = 'fpv';
var last_dir = 0;          //1=forwards, 2=backwards, for braking
//flags for current car direction
var forward = true;
var backward = false;
var go_left = false;
var go_right = false;
var car_first_pos = new THREE.Vector3(0, 0, 10);

function intersects(obj1, obj2) {
    var rad1 = obj1.radius;
    var rad2 = obj2.radius;
    var dx = obj1.center.x - obj2.center.x;
    var dy = obj1.center.y - obj2.center.y;
    var dz = obj1.center.z - obj2.center.z;
    var d_squared = dx*dx + dy*dy + dz*dz;
    return (rad1 + rad2)*(rad1 + rad2) >= d_squared;
  }
  
  function dist(obj1, obj2) {
    var dx = obj1.position.x - obj2.position.x;
    var dy = obj1.position.y - obj2.position.y;
    var dz = obj1.position.z - obj2.position.z;
    var d_squared = dx*dx + dy*dy + dz*dz;
    return Math.sqrt(d_squared);
  }
  
  function vectorbetween(obj1, obj2) {
    var dx = obj1.position.x - obj2.position.x;
    var dy = obj1.position.y - obj2.position.y;
    var dz = obj1.position.z - obj2.position.z;
    return new THREE.Vector3(-dx, -dy, -dz);
  }

  function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min + 1) ) + min;
  }
  
  function setRendererWidth(){
    if(window.innerWidth / window.innerHeight >= aspect)
      return window.innerHeight * aspect;
    else
      return window.innerWidth;
  }
  
  function setRendererHeight(){
    if(window.innerWidth / window.innerHeight >= aspect)
      return window.innerHeight;
    else
      return window.innerWidth / aspect;
  }

  function onResize() {
    'use strict';
    renderer.setSize( setRendererWidth(), setRendererHeight() );
    if(cam != "ortogonal")
      camera.aspect = aspect;
    camera.updateProjectionMatrix();
    livesCamera.updateProjectionMatrix();
  
  if(stereo_mode && new_cam != 'ortogonal') effect.setSize( window.innerWidth, window.innerHeight );
  }
  
  function onKeyDown(e) {
    'use strict';
    switch (e.keyCode) {
      case 82: //r
        restart = !restart;
        break;
      case 83: //s
        pause = !pause;
        break;
      case 76: //l
        disable_lighting = true;
        break;
      case 72: //h
        headlights = true;
        break;
      case 78: //n
        nightday = true;
        break;
      case 79: //o
        go_left = true;
        break;
      case 81: //q
        forward = true;
        break;
      case 80: //p
        go_right = true;
        break;
      case 65: //a
        backward = true;
        break;
      case 49:
        new_cam = 'ortogonal';
        break;
       case 50:
        new_cam = 'perspective';
        break;
      case 51:
        new_cam = 'fpv';
        break;
	  case 52:
        stereo_mode = !stereo_mode;
        break;
      case 67:
        turn_off_lights = true;
        break;
      case 70:
        fog = !fog;
        break;
    }
  }
  
  function onKeyUp(e){
    'use strict';
    switch (e.keyCode) {
      case 79: //o
        go_left = false;
        break;
      case 81: //q
        forward = false;
        if(car.cur_veloc < 0)
          last_dir = 2;
        else last_dir = 1;
        break;
      case 80: //p
        go_right = false;
        break;
      case 65: //a
        backward = false;
        if(car.cur_veloc > 0)
          last_dir = 1;
        else last_dir = 2;
        break;
    }
  }

function render() {
  'use strict';
  renderer.setViewport(0, 0, setRendererWidth(), setRendererHeight());
  renderer.setScissor(0, 0, setRendererWidth(), setRendererHeight());
  renderer.setScissorTest (true);
  renderer.render (scene,camera);
  if(stereo_mode && new_cam != 'ortogonal') effect.render( scene, camera );
  renderer.setViewport (0, 0, setRendererWidth()/12, setRendererHeight()/10);
  renderer.setScissor(0, 0, setRendererWidth()/12, setRendererHeight()/10);
  renderer.setScissorTest (true);
  renderer.render (scene,livesCamera);
  if(pause && msgplane.material != msgplane.pauseMat){
    msgplane.material = msgplane.pauseMat;
  }
  if(end && msgplane.material != msgplane.endMat){
    msgplane.material = msgplane.endMat;
  }
  if(end || pause){
    renderer.setViewport ((setRendererWidth()/2)-256, (setRendererHeight()/2)-256, 512, 512);
    renderer.setScissor((setRendererWidth()/2)-256, (setRendererHeight()/2)-256, 512, 512);
    renderer.setScissorTest (true);
    renderer.render (scene,msgCamera);
  }
}

function createBillboards() {
  var spriteMap = new THREE.TextureLoader().load( "assets/tree.png" );
  var spriteMaterial = new THREE.SpriteMaterial( { map: spriteMap, color: 0xffffff, fog: true } );
	var pos_x = [ 5,10,15,22,22,22,22,22,15,10,
					5,0,-5,-10,-15,-22,-22,-22,-22,-22,
					-22,-22,-22,-22,-15,-10,-5,0 ];
	var pos_y = [ 0,0,0,0,5,10,15,22,22,22,
					22,22,22,22,22,22,22,15,10,5,
					0,-5,-10,-15,-10,-5,0,0 ];
  for (var i = 0; i < billboardsNr; i++) {
    var sprite = new THREE.Sprite( spriteMaterial );
    sprite.position.set(pos_x[i], 3, -pos_y[i]);
    sprite.scale.set(10, 10, 10);
    billboards.push(sprite);
    scene.add(sprite);
  }
}

function createScene() {
    'use strict';
    scene = new THREE.Scene();
    for(var i = 0;i < 5; i++){
      cars.push(createCar(1000 + (3*i), 0, 1000));
      cars[i].rotateY(Math.PI/2);
      cars[i].traverse(function (node) {
        if (node instanceof THREE.Object3D)
        node.name = "lives_car";
          if (node instanceof THREE.Mesh && node.basicMaterial) {
            node.material = node.basicMaterial;
            node.material.wireframe = wirefr_cur;
        node.name = "lives_car";
            delete node.phongMaterial;
            delete node.gouraudMaterial;
          }
        });
    }
    car = createCar(car_first_pos.x, car_first_pos.y, car_first_pos.z);
    createRoad();
    createTable();
    createOranges();
    createButters();
    createLights();
    createBillboards();
    createParticleSystem();
    msgplane = createMsgPlane();
}

function restartGame(){
    clock.start();
    lives = 5;
    end = false;
      scene.traverse(function (node) {
          if(node.name == "cheerio"){
              cheerioReset(node);
          }
          else if(node.name == "car"){
              carReset(node);
          }
          else if(node.name == "orange"){
              orangeReset(node);
          }
      });
    for (var i = 0; i < cars.length; i++) {
      scene.add(cars[i]);
    }
    restart = false;
  }

function animate() {
    'use strict';
    requestAnimationFrame(animate);
    if(!lives) end = true;
    if(pause || end) clock.stop(); //pause or end mode
    if(restart) restartGame();
    var delta = clock.getDelta();
    rollOranges(delta);
    if(!speed_inc_timing){
      speed_inc_timing = true;
      setTimeout(function(){speed_inc = true;}, 1000);
    }
  
    if(headlights){
      car.headlights.traverse(function (node) {
        if (node instanceof THREE.SpotLight) {
          node.visible = !node.visible;
        }
      });
      headlights = false;
    }
  
    if(disable_lighting){
      scene.traverse(function (node) {
        if (node instanceof THREE.Mesh && node.basicMaterial && node.phongMaterial) { //2nd condition hammered in that shit tbqh with u fam, checks if there's 2 materials there.
          if (node.material instanceof THREE.MeshLambertMaterial || node.material instanceof THREE.MeshPhongMaterial){
            node.material = node.basicMaterial;
            node.material.wireframe = wirefr_cur;
          }
          else if (cur_shading === "gouraud"){
            node.material = node.gouraudMaterial;
            node.material.wireframe = wirefr_cur;
          }
          else{
            node.material = node.phongMaterial;
            node.material.wireframe = wirefr_cur;
          }
        }
      });
      if(cheerios[0].material instanceof THREE.MeshBasicMaterial){
        for(i=0; i< candles_n; i++){
          candles[i].traverse(function (node) {
            if (node instanceof THREE.PointLight && node.intensity == 1) {
            node.intensity = 0;
            }
          });
        }
        directionalLight.intensity = 0;
      }
      else{
        for(i=0; i< candles_n; i++){
          candles[i].traverse(function (node) {
            if (node instanceof THREE.PointLight && node.intensity == 0) {
            node.intensity = 1;
            }
          });
        }
        directionalLight.intensity = 0.8;
      }
      disable_lighting = false;
    }
  
    if(turn_off_lights){
  
        for(i=0; i< pointLightsNr; i++){
          pointLights[i].traverse(function (node) {
              if (node instanceof THREE.PointLight){
          if(node.intensity == 1) {
                   node.intensity = 0;
                }
                else{
                   node.intensity = 1;
                }
        }
        })
        }
        turn_off_lights = false;
    }
  
    if(nightday){
      if(!directionalLight.intensity)
        directionalLight.intensity = 0.8;
      else
        directionalLight.intensity = 0;
      nightday = false;
    }

    if (fog) {
      scene.fog = new THREE.Fog(0xbac8e0, 0.0025, 20);
    }
    else {
      scene.fog = null;
    }

    if (new_cam == 'ortogonal') {
      for (var i=0; i < billboardsNr; i++) {
        billboards[i].visible = false;
      }
    }
    else {
      for (var i=0; i < billboardsNr; i++) {
        billboards[i].visible = true;
      }
    }
  
    if(cam != new_cam)
      changeCamera();
    for (i = 0; i != butter_num; i++) {
      if (intersects(butters[i].contour, car.contour)) {
        car.butterCollision = true;
        break;
      }
    }
    for (i = 0; i != orange_num; i++) {
      if (intersects(oranges[i].contour, car.contour)) {
        car.orangeCollision = true;
        lives -=1;
        scene.remove(cars[lives]);
        go_left = go_right = false;
        break;
      }
    }
    for (i = 0; i != cheerios.length; i++) {
      if (intersects(cheerios[i].contour, car.contour)) {
        cheerios[i].collision = true;
          car.cheerioCollision = true;
        break;
      }
    }
    for (i = 0; i != cheerios.length; i++) {
      cheerios[i].hasCalculated = false;
      for (var j = 0; j != cheerios.length; j++) {
        if (i != j) {
          if (intersects(cheerios[i].contour, cheerios[j].contour)) {
            cheerios[i].cheerioCollision = true;
            cheerios[j].cheerioCollision = true;
            cheerios[i].collidesWith = j;
            cheerios[j].collidesWith = i;
            break;
          }
        }
      }
    }
    cheerioHit(delta);
    setTimeout(function() {
      moveCar(delta);
      if (car.position.x > 31) {
        go_left = true;
        if (car.cur_angle < -1.4) {
          go_left = false;
          if (car.position.z < -30) {
            go_left = true;
            if (car.cur_angle < -3.14) {
              go_left = false;
            }
          }
        }
      }
      if (car.position.x < -30) {
        go_left = true;
        if (car.cur_angle < -4.5) {
          go_left = false;
          if (car.position.z > 38) {
            go_left = true;
            if (car.cur_angle < -3.14 * 2) {
              go_left = false;
              halfway = true;
            }
          }
        }
      }
      if (car.position.x > -23 && halfway) {
        go_left = true;
        if (car.cur_angle < -7) {
          go_left = false;
          if (car.position.x > -10) {
            go_right = true;
            if (car.cur_angle > -3.14 * 2) {
              go_right = false;
            }
          }
        }
      }
    }, 7000);

    if(car.position.x > 100 || car.position.x < -100 || car.position.z > 60 || car.position.z < -60){
      lives -=1;
      scene.remove(cars[lives]);
      car.position.set(car_first_pos.x, car_first_pos.y, car_first_pos.z);
      car.cur_veloc = 0;
      car.rotateY(car.cur_angle);
      car.cur_angle = 0;
      direction.x = 1;
      direction.y = 0;
      direction.z = 0;
      forward = true;
      backward = false;
      go_left = false;
      go_right = false;
    }


      /*var deltaTime = delta * spawnerOptions.timeScale;

      tick += deltaTime;

      if ( tick < 0 ) tick = 0;

      if ( deltaTime > 0 ) {

        options.position.x = car.position.x - 1.5*Math.cos(car.cur_angle);
        options.position.y = car.position.y +1 ;
        options.position.z = car.position.z - 1.5*Math.sin(car.cur_angle);


        for ( var x = 0; x < spawnerOptions.spawnRate * deltaTime ; x ++ ) {

          // Yep, that's really it. Spawning particles is super cheap, and once you spawn them, the rest of
          // their lifecycle is handled entirely on the GPU, driven by a time uniform updated below

          particleSystem.spawnParticle( options );

        }

      }

      particleSystem.update( tick );*/

    render();
    controls.update();

    clock.start();
  }

function init() {
    'use strict';
    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.shadowMap.enabled = true;
    renderer.shadowMap.type = THREE.PCFSoftShadowMap;
    aspect = window.innerWidth / window.innerHeight;
    document.body.appendChild(renderer.domElement);
	
	effect = new THREE.StereoEffect( renderer );
	effect.setSize( window.innerWidth, window.innerHeight );
	
    createScene();
    createCameras();
    controls = new THREE.DeviceOrientationControls(camera);
    //render();
    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    clock = new THREE.Clock(false);
}