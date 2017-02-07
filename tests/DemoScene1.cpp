#ifdef WITH_TESTS

#include "Tests.h"
#include "../K3De.h"
#include "../material2/Material2.h"
#include "../material2/MaterialFactory.h"
#include "../shader/ShaderDebug.h"
#include "../3D/Plane.h"

#include <thread>
#include <chrono>

TEST(DemoScene1, scene) {

	EngineSettings set;
	Engine::init(set);

	Scene scene;
	scene.setEnableShadows(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	scene.setClearColor( Vec4(0,0,0,1) );

	MaterialFactory& fac = scene.getMaterialFactory();

	{
		Material2* mat = fac.load(getDataFile("scene1/plane/plane.xml"));
		Plane3* plane = new Plane3();
		plane->addPlaneY(0, Vec2(-2, -2), Vec2(+2, +2));
		plane->setMaterial(mat);
		scene.add(plane);

		mat->getShader()->bind();
		ShaderDebug sd;
		sd.dumpUniforms(mat->getShader());
		//std::cout << mat->getShader()->getUniformLocation("lights") << std::endl;
		//std::cout << mat->getShader()->getUniformLocation("lights.light") << std::endl;
		//std::cout << mat->getShader()->getUniformLocation("lights.light[0].r") << std::endl;


	}

	{
		Material2* mat = fac.load(getDataFile("scene1/wood1/wood1.xml"));
		IndexedMesh* mesh = scene.getMeshFactory().createMesh(getDataFile("scene1/wood1/Wood.obj"), true);
		mesh->setMaterial(mat);
		mesh->setPosition(-0.5, 0, -0.5);
		mesh->setScale(3,3,3);
		mesh->setRotationDeg(0,-33,0);

		scene.add(mesh);
	}

	{
		Material2* mat = fac.load(getDataFile("scene1/wood1/wood1.xml"));
		IndexedMesh* mesh = scene.getMeshFactory().createMesh(getDataFile("scene1/wood1/Wood.obj"), true);
		mesh->setMaterial(mat);
		mesh->setPosition(-0.6, 0, +0.5);
		mesh->setScale(3,3,3);
		mesh->setRotationDeg(0,45,0);
		scene.add(mesh);
	}

	{
		Material2* mat = fac.load(getDataFile("scene1/palm/palm.xml"));
		IndexedMesh* mesh = scene.getMeshFactory().createMesh(getDataFile("scene1/palm/palm.obj"));
		mesh->setMaterial(mat);
		mesh->setRotationDeg(-90,0,0);
		mesh->setScale(0.5, 0.5, 0.5);
		mesh->setPosition(+0.5, 0, +0.5);
		scene.add(mesh);
	}

	{
		Material2* mat = fac.load(getDataFile("scene1/palm/palm.xml"));
		IndexedMesh* mesh = scene.getMeshFactory().createMesh(getDataFile("scene1/palm/palm.obj"));
		mesh->setMaterial(mat);
		mesh->setRotationDeg(-85,45,0);
		mesh->setScale(0.35, 0.35, 0.35);
		mesh->setPosition(+0.5, 0, -0.5);
		scene.add(mesh);
	}



//	scene.getCamera().setLookAt(0,0,0);
//	scene.getCamera().setPosition(-2, 2, -2);

//	scene.setNumLights(1);
//	scene.getLight(0).setColor(1,1,1);

	scene.getLight(0).setColor(1,1,1);
	scene.getLight(0).setImpact(0.5);
	scene.getLight(0).setAttenuationLinear(0.0);
	scene.getLight(0).setAttenuationQuadratic(0.01);
	scene.getLight(0).setActive(true);
	scene.getLight(0).setCastsShadows(true);

	scene.getLight(1).setColor(1,1,1);
	scene.getLight(1).setImpact(0.5);
	scene.getLight(1).setAttenuationLinear(0.0);
	scene.getLight(1).setAttenuationQuadratic(0.01);
	scene.getLight(1).setActive(true);
	scene.getLight(1).setPosition(0, 5, 0);
	scene.getLight(1).setCastsShadows(true);

	scene.getLight(2).setColor(0,0,1); scene.getLight(2).setPosition(+1,1,+1); scene.getLight(2).setImpact(1.99); scene.getLight(2).setAttenuationQuadratic(3.25); scene.getLight(2).setActive(true); scene.getLight(2).setCastsShadows(false);
	scene.getLight(3).setColor(0,1,0); scene.getLight(3).setPosition(-1,1,+1); scene.getLight(3).setImpact(1.99); scene.getLight(3).setAttenuationQuadratic(3.25); scene.getLight(3).setActive(true); scene.getLight(3).setCastsShadows(false);
	scene.getLight(4).setColor(1,0,0); scene.getLight(4).setPosition(+1,1,-1); scene.getLight(4).setImpact(1.99); scene.getLight(4).setAttenuationQuadratic(3.25); scene.getLight(4).setActive(true); scene.getLight(4).setCastsShadows(false);

	scene.setClearColor(Vec4(0,0,0,1));

	Engine::get()->setScene(&scene);


	float rad = 1;
	int cnt = 0;

	while(true) {

		rad += 0.05;

		float s = 5;
		scene.getLight(0).setPosition(cos(rad)*s, s, sin(rad)*s);
		scene.getLight(1).setPosition(cos(rad+2)*s, s, sin(rad+2)*s);

		float r;
		s = 1.0;

		r = rad*2 + 2 * M_PI * 0.0;
		scene.getLight(2).setPosition(cos(r)*s, s*0.5, sin(r)*s);

		r = rad*2 + 2 * M_PI * 0.33;
		scene.getLight(3).setPosition(cos(r)*s, s*0.5, sin(r)*s);

		r = rad*2 + 2 * M_PI * 0.66;
		scene.getLight(4).setPosition(cos(r)*s, s*0.5, sin(r)*s);

		Engine::get()->render();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		Image img = Engine::get()->getFrame();
		ImageFactory::saveJPG("/tmp/2/img_" + std::to_string(cnt) + ".jpg", img); ++cnt;

	}

}


#endif
