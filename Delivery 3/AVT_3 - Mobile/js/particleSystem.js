var options, spawnerOptions, particleSystem;

function createParticleSystem()
{
	particleSystem = new THREE.GPUParticleSystem( {
				maxParticles: 250000
			} );

			scene.add( particleSystem );

			// options passed during each spawned

			options = {
				position: new THREE.Vector3(),
				positionRandomness: 0.2,
				velocity: new THREE.Vector3(),
				velocityRandomness: 0.1,
				color: 0xaa88ff,
				colorRandomness: 0,
				turbulence: 0,
				lifetime: 1,
				size: 13,
				sizeRandomness: 1
			};

			spawnerOptions = {
				spawnRate: 15000,
				horizontalSpeed: 1.5,
				verticalSpeed: 1.33,
				timeScale: 1
			};

}
