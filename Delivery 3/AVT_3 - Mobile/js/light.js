var pointLightsNr = 6;
var posLights = [[-45, 25], [-45, -25], [45, -25], [45, 25], [0, 25], [0, -25]];
var pointLights = [];
var lensflare;

function createLights() {
    // directional light
    directionalLight = new THREE.DirectionalLight( 0xffffff, 0.8 );
    directionalLight.position.set(-100, 100, 60);
    /*directionalLight.castShadow = true;
    directionalLight.shadow.mapSize.width = 256;
    directionalLight.shadow.mapSize.length = 256;
    directionalLight.shadow.camera.top = 300;
    directionalLight.shadow.camera.right = 300;
    directionalLight.shadow.camera.bottom = -300;
    directionalLight.shadow.camera.left = -300;
    directionalLight.shadow.camera.near = 0.5;
    directionalLight.shadow.camera.far = 100;*/
    scene.add(directionalLight);
    direction = new THREE.Vector3(1,0,0);

    // point lights
    for (var i = 0; i < pointLightsNr; i++) {
        var light = new THREE.PointLight( 0xffffff, 1, 50, 2 );
        if(i == 2)
        {
        	//createLensFlare();
        	//light.add(lensflare);
        }
        
        light.position.set(posLights[i][0], 1, posLights[i][1]);
        //light.castShadow = true;
        pointLights.push(light);
        scene.add(light);
    }
}

function addHeadlight(obj, x, y, z) {
    var spotlight = new THREE.SpotLight(0xffffff, 1.5, 100, Math.PI/5, 0.3, 2);
    obj.add(spotlight);
    spotlight.position.x = x;
    spotlight.position.y = y;
    spotlight.position.z = z;
    spotlight.target = targetObj;
    //spotlight.castShadow = true;
}

function createLensFlare() {
    var loader = new THREE.TextureLoader();
    var tex0 = loader.load("assets/sunburst.png");
    var tex1 = loader.load("assets/sun.png");
    var tex2 = loader.load("assets/ring.png");
    var tex3 = loader.load("assets/black.png");
    var tex4 = loader.load("assets/shine.png");
    var tex5 = loader.load("assets/alpha.png");

    lensflare = new THREE.Lensflare();
    lensflare.addElement( new THREE.LensflareElement( tex0, 200, 0 ) );
    lensflare.addElement( new THREE.LensflareElement(tex1, 200, 0.2 ));
    lensflare.addElement( new THREE.LensflareElement(tex2, 200, 0.4 ));
    lensflare.addElement( new THREE.LensflareElement(tex3, 200, 0.6 ));
    lensflare.addElement( new THREE.LensflareElement(tex4, 200, 0.8 ));
    lensflare.addElement( new THREE.LensflareElement(tex5, 200, 1 ));

}