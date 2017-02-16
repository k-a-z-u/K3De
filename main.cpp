
//// Include standard headers
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>

//#include <GL/glew.h>

//#include <GLFW/glfw3.h>

//#include "Engine.h"
//#include "GPUParams.h"

//#include "mesh/Mesh.h"
//#include "mesh/MeshFactory.h"
//#include "mesh/Water.h"
//#include "mesh/TerrainFactory.h"

//#include "scene/Scene.h"

//#include "gl/Renderbuffer.h"
//#include "gl/Framebuffer.h"


//#include "math/Math.h"



//Engine* Engine::instance;


#ifdef WITH_TESTS
#include "tests/Tests.h"
int runTests(int argc, char** argv) {

	//::testing::GTEST_FLAG(filter) = "*DemoScene1.*";
	::testing::GTEST_FLAG(filter) = "*7Z*";

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}

#endif


int main(int argc, char** argv) {

//	ThreadPool& gtq = GlobalThreadPool::get();
//	gtq.add( [] () {std::cout << "123" << std::endl;} );
//	gtq.add( [] () {std::cout << "abc" << std::endl;} );
//	gtq.add( [] () {std::cout << "xyz" << std::endl;} );

//	gtq.join();

#ifdef WITH_TESTS
	runTests(argc, argv);
	//exit(0);
#endif




//	Engine::init();

//	if( !glfwInit() ) {
//		std::cout << "err" << std::endl;
//		return -1;
//	}

//	int width = 1024;
//	int height = 600;

//	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

//	// Open a window and create its OpenGL context
//	GLFWwindow* window; // (In the accompanying source code, this variable is global)
//	window = glfwCreateWindow( width, height, "Tutorial 01", NULL, NULL);
//	if( window == NULL ){
//		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window); // Initialize GLEW
//	glewExperimental=true; // Needed in core profile
//	if (glewInit() != GLEW_OK) {
//		fprintf(stderr, "Failed to initialize GLEW\n");
//		return -1;
//	}

//	// Ensure we can capture the escape key being pressed below
//	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


//	glClearColor(0.0f, 0.2f, 0.8f, 0.0f);
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
//	//glDepthMask(GL_TRUE);
//	glDepthFunc(GL_LESS);

//	GPUParams::show();


//	Scene scene;



//	Texture* tStone = scene.getTextureFactory().create("/apps/workspaces/openGL/data/stone.jpg");
//	Texture* tStoneNormals = scene.getTextureFactory().create("/apps/workspaces/openGL/data/stone_normals.jpg");
//	//ITexture* tWater = scene.getTextureFactory().create("/apps/workspaces/openGL/data/textures/water1/waterWaves_", ".jpg", 0, 23, 25);
//	Texture* tWaterDuDv = scene.getTextureFactory().create("/apps/workspaces/openGL/data/water_dudv.jpg");
//	Texture* tWaterNormals = scene.getTextureFactory().create("/apps/workspaces/openGL/data/water_normals.jpg");

//	Texture* tChecker = scene.getTextureFactory().create("/apps/workspaces/openGL/data/checkerboard.png");

//	Texture* tRock = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/rock.jpg");
//	Texture* tRockNormals = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/rock_normals.jpg");

//	Texture* tSandGround = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/sand_ground.jpg");
//	Texture* tSandWater = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/sand_water.jpg");
//	Texture* tSandWaterNormals = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/sand_water_normals.jpg");

//	Texture* tGrass = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/grass.jpg");
//	Texture* tGrassNormals = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/grass_normals.jpg");


//	Texture* tBlend = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/terrain_mix.png");

//	scene.getShaderFactory().setIncludePath("/apps/workspaces/openGL/data/shader/");
//	Shader* sNormal = scene.getShaderFactory().create("shaderStandardVertex.glsl", "shaderStandardFragment.glsl");
//	sNormal->setInt("tex", 0);
//	sNormal->setInt("texNormals", 1);
//	sNormal->setInt("texShadowMap", 7);

//	Shader* s2D = scene.getShaderFactory().create("shaderStandard2DVertex.glsl", "shaderStandard2DFragment.glsl");
//	s2D->setInt("tex", 0);




//	IndexedMesh* ma = scene.getMeshFactory().createMesh("/apps/workspaces/openGL/data/minicooper.obj");
//	ma->setScale(0.015, 0.015, 0.015);
//	ma->setRotation(-90,0,0);
//	ma->setPosition(-2, 0.2, 1.5);
//	ma->setTexture(0, tStone);
//	ma->setTexture(1, tStoneNormals);
//	ma->setShader(sNormal);
//	scene.add(ma);

//	IndexedMesh* mb = scene.getMeshFactory().createMesh("/apps/workspaces/openGL/data/gate1.obj");
//	mb->setScale(0.05, 0.05, 0.05);
//	mb->setPosition(0, 0, 0);
//	mb->setTexture(0, tGrass);
//	mb->setTexture(1, tStoneNormals);
//	mb->setShader(sNormal);
//	scene.add(mb);

//	IndexedMesh* mc = scene.getMeshFactory().createMesh("/apps/workspaces/openGL/data/cage1.obj");
//	mc->setScale(0.02, 0.02, 0.02);
//	mc->setPosition(-2, 0.0, 1.8);
//	mc->setTexture(0, tGrass);
//	mc->setTexture(1, tStoneNormals);
//	mc->setShader(sNormal);
//	scene.add(mc);

//	IndexedMesh* md = scene.getMeshFactory().createMesh("/apps/workspaces/openGL/data/teapot.obj");
//	md->setPosition(0, 0, -1.5);
//	md->setTexture(0, tStone);
//	md->setTexture(1, tStoneNormals);
//	md->setShader(sNormal);
//	scene.add(md);
//	md->setRotation(0,90,0);


//	IndexedMesh* m2 = scene.getMeshFactory().createMesh("/apps/workspaces/openGL/data/teapot.obj");
//	m2->setTexture(0, tGrass);
//	m2->setTexture(1, tStoneNormals);
//	m2->setShader(sNormal);
//	scene.add(m2);
//	m2->setScale(0.4, 0.4, 0.4);




//	scene.setNumLights(1);
//	scene.getLight(0).setColor(1,1,1);
//	scene.getLight(0).setPosition(1, 1, 0);
//	m2->setPosition(1, 1, 0);


//	int j = 0;
//	double size = 16;

//	Shader* sWater= scene.getShaderFactory().create("shaderWaterVertex.glsl", "shaderWaterFragment.glsl");
//	sWater->setInt("texReflect", 0);
//	sWater->setInt("texRefract", 1);
//	sWater->setInt("texNormals", 2);
//	sWater->setInt("texDuDv", 3);


//	int rw = width/2;
//	int rh = height/2;
//	int sw = width;
//	int sh = height;

//	// water reflection
//	Framebuffer fbReflect;
//	Renderbuffer rbReflect;
//	Texture* texReflect = scene.getTextureFactory().createRenderTexture(rw, rh);
//	texReflect->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
//	fbReflect.attachTextureColor(0, texReflect);
//	fbReflect.attachRenderbufferDepth(&rbReflect, rw, rh);

//	// water refraction
//	Framebuffer fbRefract;
//	Renderbuffer rbRefract;
//	Texture* texRefract = scene.getTextureFactory().createRenderTexture(rw, rh);
//	texRefract->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
//	fbRefract.attachTextureColor(0, texRefract);
//	fbRefract.attachRenderbufferDepth(&rbRefract, rw, rh);

//	// shadows
//	Framebuffer fbShadows;
//	//Renderbuffer rbShadows;
//	Texture* texShadows = scene.getTextureFactory().createDepthTexture(sw, sh);
//	texShadows->setFilter(TextureFilter::NEAREST, TextureFilter::NEAREST);

//	fbShadows.attachTextureDepth(texShadows);
//	//Texture* texShadows = scene.getTextureFactory().createRenderTextureFloat(sw, sh);
//	//fbShadows.attachRenderbufferDepth(&rbShadows, sw, sh);
//	//fbShadows.attachTextureColor(0, texShadows);
//	Shader* sShadowGen = scene.getShaderFactory().create("shaderShadowMapVertex.glsl", "shaderShadowMapFragment.glsl");


//	Water w(Vec3(0,0,0), Vec2(12,12), 5);
//	w.setShader(sWater);
//	w.setReflect(texReflect);
//	w.setRefract(texRefract);
//	w.setNormals(tWaterNormals);
//	w.setDuDv(tWaterDuDv);
//	scene.add(&w);

//	Sprite2D* s1 = scene.getSpriteFactory().createSprite(0.1, 0.1, 0.3, 0.3);
//	s1->setTexture(0, texShadows);
//	s1->setShader(s2D);
//	scene.add(s1);

//	Shader* sTerrain = scene.getShaderFactory().create("shaderTerrainVertex.glsl", "shaderTerrainFragment.glsl");
//	sTerrain->setInt("texBlend", 0);
//	sTerrain->setInt("tex[0]", 1);
//	sTerrain->setInt("tex[1]", 2);
//	sTerrain->setInt("tex[2]", 3);
//	sTerrain->setInt("tex[3]", 4);
//	sTerrain->setInt("tex[4]", 5);
//	sTerrain->setInt("tex[5]", 6);
//	sTerrain->setInt("texShadowMap", 7);


//	const std::string map = "/apps/workspaces/openGL/data/levels/level1/terrainBW.png";
//	TerrainFactory fac;

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	//glBlendFunc(GL_ONE, GL_ONE);

//	IndexedMesh* palm = scene.getMeshFactory().createMesh("/apps/workspaces/openGL/data/palm1.obj");
//	Texture* tPalm = scene.getTextureFactory().create("/apps/workspaces/openGL/data/levels/level1/palm1.png");
//	palm->setTexture(0, tPalm);
//	palm->setShader(sNormal);
//	palm->setScale(0.4,0.4,0.4);
//	palm->setPosition(1,0,0);
//	scene.add(palm);

//	Terrain* t = fac.create(Vec3(0,-0.5,0), Vec3(0.2,2,0.2), map);
//	t->setShader(sTerrain);
//	t->setTexture(0, tBlend);
//	t->setTexture(2, tSandWater);
//	t->setTexture(1, tRock);
//	t->setTexture(3, tGrass);
//	t->setTexture(5, tSandWaterNormals);
//	t->setTexture(4, tRockNormals);
//	t->setTexture(6, tGrassNormals);
//	scene.add(t);


//	Engine::get()->setScene(&scene);
//	scene.getCamera().setScreenSize(width, height);

//	do {

//		Engine::get()->next();

//		;
//		float i = Engine::get()->getTimeSec() * 30.0f;
//		float k = Engine::get()->getTimeSec() * 30.0f;

//		float x = sin(i/90.0) * 3.0;
//		float z = cos(i/90.0) * 3.0;
//		scene.getCamera().setPosition(x, 4, z);
//		//scene.getCamera().setPosition(3, 3, 3);

//		float x2 = sin(1+k/90.0) * 5.0;
//		float y2 = sin(1+k/90.0) * 5;
//		float z2 = cos(1+k/90.0) * 5.0;
//		scene.getLight(0).setPosition(x2, 5, z2);//y2, z2);
//		m2->setPosition(x2, 10, z2);

//		md->setRotation(i, 0, i);
//		mb->setRotation(0,i,0);

//		// before pre-processing
//		scene.getCamera().push();

//		// render water textures
//		w.setEnabled(false);
//		s1->setEnabled(false);
//		scene.getCamera().setScreenSize(rw, rh);

//		// reflect
//		scene.getCamera().mirrorY(0);
//		scene.getLight(0).mirrorY(0);
//		scene.setClipY(-0.05, 9999);		// render everything below the water level (camera is mirrored!)
//		fbReflect.enable();
//		scene.render();
//		fbReflect.disable();
//		scene.getCamera().mirrorY(0);
//		scene.getLight(0).mirrorY(0);

//		// refract
//		scene.setClipY(-9999, +0.05);		// render everything below the water level (camera is normal!)
//		fbRefract.enable();
//		scene.render();
//		fbRefract.disable();

//		// done
//		scene.setClipY(-0.05, 9999);		// render only things above the water level (camera is normal!)

//		// shadows
//		scene.getCamera().setScreenSize(sw, sh);
//		glCullFace(GL_FRONT);
//		scene.setOverwriteShader(sShadowGen);
//		scene.getCamera().setPosition(scene.getLight(0).getPosition());
//		scene.setShadowPV(scene.getCamera().getPVMatrix());
//		fbShadows.enable();
//		scene.render();
//		fbShadows.disable();
//		scene.setOverwriteShader(nullptr);
//		glCullFace(GL_BACK);



//		// re-enable the water
//		w.setEnabled(true);
//		s1->setEnabled(true);

//		// reset the camera
//		scene.getCamera().pop();
//		texShadows->bind(7);

//		// render the final image
//		scene.render();

//		// make visible
//		glfwSwapBuffers(window);
//		glfwPollEvents();

//	} // Check if the ESC key was pressed or the window was closed
//	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
//	glfwWindowShouldClose(window) == 0 );


}
