#ifdef WITH_TESTS

#include "Tests.h"
#include "../K3De.h"
#include "../material/Material2.h"
#include "../material/MaterialFactory.h"

#include "../3D/Plane.h"

#include <thread>
#include <chrono>

class MyScene2359123123 : public Scene {
	virtual void onKeyEvent(const int key, const int scancode, const int action, const int mods) {;}
	virtual void onBecomesActive() {;}
	virtual void onBeforeRender(const SceneState& ss) {;}
};
/*

TEST(Material2, build1) {

	EngineSettings set;
	Engine::init(set);

	MyScene2359123123 scene;
	scene.setEnableShadows(true);

	MaterialFactory& fac = scene.getMaterialFactory();
	//scene.getTextureFactory().setIncludePath(getDataFile("mat1/"));

	Material2* mat = fac.load(getDataFile("mat1/material.xml"));
	//ASSERT_EQ(2, scene.getTextureFactory().getNumLoaded());

	const std::string cVert = mat->getVertexParams().getCode();
	const std::string cFrag = mat->getFragmentParams().getCode();

	std::cout << cVert << std::endl;
	std::cout << "***************************************************************" << std::endl;
	std::cout << cFrag << std::endl;

	Plane3 plane1;
	plane1.addPlaneY(0, Vec2(-2, -2), Vec2(+2, +2));
	plane1.setMaterial(mat);

	Plane3 plane2;
	plane2.addPlaneY(1.0, Vec2(-0.4, -0.4), Vec2(+0.4, +0.4));
	plane2.setMaterial(mat);

	scene.add(&plane1);
	scene.add(&plane2);

	scene.getCamera().setLookAt(0,0,0);
	scene.getCamera().setPosition(-2, 2, -2);

	scene.getLight(0).setColor(1,1,1);



	scene.setClearColor(Vec4(1,0,0,1));

	Engine::get()->setScene(&scene);


	float rad = 1;

	while(true) {

		//rad += 0.05;

		const float s = 6;
		scene.getLight(0).setPosition(cos(rad)*s, s, sin(rad)*s);

//		Engine::get()->tick();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}

}
*/


#endif
