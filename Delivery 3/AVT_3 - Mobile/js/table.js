function createTablePart(table, x, z){
    var texture = new THREE.TextureLoader().load( "assets/table.png" );
    texture.wrapS = THREE.RepeatWrapping;
    texture.wrapT = THREE.RepeatWrapping;
    texture.repeat.set(1,1);
    var texture1 = new THREE.TextureLoader().load( "assets/stone.png" );
    texture1.wrapS = THREE.RepeatWrapping;
    texture1.wrapT = THREE.RepeatWrapping;
    texture1.repeat.set(1,1);
    var materials = [new THREE.MeshLambertMaterial({map:texture1, wireframe: wirefr_cur, opacity: 0.5}), new THREE.MeshLambertMaterial({map:texture, wireframe: wirefr_cur, transparent: true, opacity:0.5})];
    var mesh = THREE.SceneUtils.createMultiMaterialObject(new THREE.BoxGeometry(20,1,15, 10,10,10), materials);
    mesh.position.set(x + 10,-0.5,z + 7.5);
    mesh.receiveShadow = true;
    table.add(mesh);
}
  
function createTable(){
    table = new THREE.Object3D();
    for(j = -100; j != 100; j+=20){
      for(i = -60; i != 60; i+=15){
        createTablePart(table, j, i);
      }
   }
   table.receiveShadow = true;
    scene.add(table);
    // reflectors/mirrors
  /*var geom = new THREE.PlaneGeometry(200,120);
    var groundMirror = new THREE.Reflector( geom, {
      clipBias: 0.003,
      textureWidth: window.innerWidth * window.devicePixelRatio,
      textureHeight: window.innerHeight * window.devicePixelRatio,
      color: 0x777777,
      recursion: 1
    } );
    groundMirror.position.y = -0.1;
    groundMirror.rotateX( - Math.PI / 2 );
    scene.add( groundMirror );*/
}

function createMsgPlane(){
  var pauseMat = new THREE.MeshBasicMaterial({map: new THREE.TextureLoader().load( "assets/pause.png" )});
  var endMat = new THREE.MeshBasicMaterial({map: new THREE.TextureLoader().load( "assets/end.png" )});
  endMat.map.wrapT = pauseMat.map.wrapT = THREE.RepeatWrapping;
  endMat.map.wrapS = pauseMat.map.wrapS = THREE.RepeatWrapping;
  endMat.map.repeat.set(1,1);
  pauseMat.map.repeat.set(1,1);
  var mesh = new THREE.Mesh(new THREE.CubeGeometry(128,0,128), pauseMat);
  mesh.pauseMat = pauseMat;
  mesh.endMat = endMat;
  mesh.position.set(-1000,0,1000);
  scene.add(mesh);
  return mesh;
}