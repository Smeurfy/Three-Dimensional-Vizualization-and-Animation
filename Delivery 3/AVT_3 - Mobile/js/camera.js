var camera, livesCamera, msgCamera; // camera being used at the moment
var orto, follow, persp; // different cameras available

function createOrtoCamera() {
  'use strict';
  orto = new THREE.OrthographicCamera( -window.innerWidth/16, window.innerWidth/16, window.innerHeight/12, -window.innerHeight/12, 1, 1000);
  orto.position.x = 0;
  orto.position.y = 100;
  orto.position.z = 0;
  orto.lookAt(scene.position);
}

function createFollowCamera(){
  follow = new THREE.PerspectiveCamera(100, 1, 0.1, 1000);
  follow.position.set(-5, 5, 0);
  follow.lookAt(direction);
  //follow.updateProjectionMatrix();
  car.add(follow);
}

function createPerspCamera(){
  persp = new THREE.PerspectiveCamera(100, 1, 0.1, 1000);
  persp.position.x = 60;
  persp.position.y = 60;
  persp.position.z = 60;
  persp.lookAt(scene.position);
}

function createLivesCamera() {
  'use strict';
  livesCamera = new THREE.OrthographicCamera( -window.innerWidth/200, window.innerWidth/200, window.innerHeight/200, -window.innerHeight/200, 1, 1000);
  livesCamera.position.x = 1006;
  livesCamera.position.y = 10;
  livesCamera.position.z = 1000;
  livesCamera.lookAt(new THREE.Vector3(livesCamera.position.x,0,livesCamera.position.z));
}

function createMsgCamera() {
  'use strict';
  msgCamera = new THREE.OrthographicCamera( -64, 64, 64, -64, 1, 1000);
  msgCamera.position.x = -1000;
  msgCamera.position.y = 10;
  msgCamera.position.z = 1000;
  msgCamera.lookAt(new THREE.Vector3(msgCamera.position.x,0,msgCamera.position.z));
}

function createCameras() {
  createOrtoCamera();
  createPerspCamera();
  createFollowCamera();
  createLivesCamera();
  createMsgCamera();
  camera = follow;
}

function changeCamera(){
  if (new_cam == 'fpv'){
    cam = 'fpv';
    camera = follow;
  }
  if (new_cam == 'ortogonal'){
	  cam = 'orto';
    camera = orto;
  }
  if (new_cam == 'perspective'){
	  cam = 'perspective';
    camera = persp;
  }
}