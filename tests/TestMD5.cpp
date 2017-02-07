
#ifdef WITH_TESTS

#include "Tests.h"

#include "../import/MD5/ImportMesh.h"
#include "../import/MD5/ImportAni.h"

/*
TEST(MD5, import) {

	std::string str =
		R"=====(

		  MD5Version 10
		  commandline "xxx"

		  joints {
			  "origin"	-1 ( -0.000000 0.001643 -0.000604 ) ( -0.707107 -0.000242 -0.707107 )		//
			  "sheath"	0 ( 1.100481 -0.317714 3.170247 ) ( 0.307041 -0.578615 0.354181 )		// origin
			  "sword"	1 ( 0.980960 -0.936157 4.075372 ) ( 0.305556 -0.578156 0.353505 )		// sheath
		  }

		  mesh {
			shader "bob_body"

			numverts 10
			vert 0 ( 0.683594 0.455078 ) 0 3
			vert 1 ( 0.773438 0.439453 ) 3 2
			vert 2 ( 0.738281 0.464844 ) 5 2
			vert 3 ( 0.277344 0.306641 ) 7 2
			vert 4 ( 0.318359 0.181641 ) 9 2
			vert 5 ( 0.341797 0.207031 ) 11 2
			vert 6 ( 0.822266 0.429688 ) 13 2
			vert 7 ( 0.867188 0.439453 ) 15 2
			vert 8 ( 0.822266 0.453125 ) 17 2
			vert 9 ( 0.417969 0.320312 ) 19 2

			numtris 628
			tri 0 0 2 1
			tri 1 3 5 4
			tri 2 6 8 7"

			numweights 5
			weight 0 1 0.333333 ( -0.194917 0.111128 -0.362937 )
			weight 1 2 0.333333 ( 0.115237 1.409351 0.894934 )
			weight 2 1 0.333333 ( 0.126628 -0.030269 0.894934 )
			weight 3 2 0.500000 ( 0.054236 1.540521 0.403698 )
			weight 4 1 0.500000 ( 0.167080 0.108620 0.403697 )

		  }

	)=====";


	MD5::Import imp(1);
	imp.loadMeshString(str);

}

#include "../Engine.h"
#include "../material/ambient/MaterialAmbientFixedColor.h"
#include "../material/ambient/MaterialAmbientTexture.h"
#include "../material/bump/MaterialBumpMapTexture.h"
#include "../material/specular/MaterialSpecularFixed.h"
#include "../material/ShaderCompiler.h"

#include "../mesh/MD5Mesh.h"
#include "../3D/Plane.h"
#include "../3D/Box.h"

#include <thread>

TEST(MD5, live) {


	EngineSettings settings;

	Engine::init(settings);
	Engine* eng = Engine::get();

	class MyScene : public Scene {

		void onBeforeRender() override {
			;
		}

	} scene;

	scene.setEnableShadows(true);


	MD5::Import imp(0.05);
	MD5::AniImport ani(0.05);

	//imp.loadMeshFile(getDataFile("md5_test/bob_lamp_update.md5mesh"));
	//Texture* tex = scene.getTextureFactory().create(getDataFile("md5_test/bob_body.jpg"));
	//ani.loadFile(getDataFile("md5_test/bob_lamp_update.md5anim"));

	MD5::ImportedMesh impPlayer = imp.loadMeshFile(getDataFile("md5_2/hellknight.md5mesh"));

	//ani.loadFile(getDataFile("md5_2/hellknight_walk.md5anim"));
	MD5::Animation aniIdle = ani.loadFile(getDataFile("md5_2/hellknight_idle.md5anim")).prepare();


	imp.setScale(0.1);
	ani.setScale(0.1);

	//MD5::ImportedMesh impWeapon = imp.loadMeshFile("/mnt/vm/DOOM/pak002/md5/weapons/rocketlauncher_world/worldrocketlauncher.md5mesh");
	MD5::ImportedMesh impWeapon = imp.loadMeshFile("/mnt/vm/DOOM/pak002/md5/weapons/pistol_world/worldpistol.md5mesh");



	//MD5::Animation aniWeapon = ani.loadFile("/mnt/vm/DOOM/pak002/md5/weapons/rocketlauncher_world/rocketlauncher_aim.md5anim").prepare();
	MD5::Animation aniWeapon = ani.loadFile("/mnt/vm/DOOM/pak002/md5/weapons/pistol_world/pistol_idle.md5anim").prepare();


	Texture* texDiff = scene.getTextureFactory().create(getDataFile("md5_2/hellknight_d.jpg"));
	Texture* texNorm = scene.getTextureFactory().create(getDataFile("md5_2/hellknight_local.jpg"));
	Texture* texWeapon = scene.getTextureFactory().create("/mnt/vm/DOOM/doublebarrel.jpg");

	IndexedMesh* scythe = scene.getMeshFactory().createMesh("/mnt/vm/workspace/IRGame/IndoorUI/res/models/scythe/osiris.obj", false);
	scene.add(scythe);
	scythe->setTransformOrder(AffineTransformOrder::SCALE_TRANSLATE_ROTATE);
	scythe->setScale(0.6, 0.6, 0.6);
	scythe->setPosition(0,-1,0);
	scythe->setRotationDeg(0,90,0);
	Texture* texScythe = scene.getTextureFactory().create("/mnt/vm/workspace/IRGame/IndoorUI/res/models/scythe/diffuse.jpg");
	Material matScythe(true, true, true);
	matScythe.setAmbient(new MaterialAmbientTexture(texScythe));
	scythe->setMaterial(&matScythe);
	scythe->setShader(ShaderCompiler::compileShader(&matScythe, &scene));

	IndexedMesh* keyboard = scene.getMeshFactory().createMesh("/mnt/vm/workspace/IRGame/K3De/tests/data/md5_2/keyboard/Duffle Bag.obj", true, true);
	scene.add(keyboard);
	Texture* texKeyboard = scene.getTextureFactory().create("/mnt/vm/workspace/IRGame/K3De/tests/data/md5_2/keyboard/bm_amb_holdall.png");
	Material matKeyboard(true, true, true);
	matKeyboard.setAmbient(new MaterialAmbientTexture(texKeyboard));
	keyboard->setMaterial(&matKeyboard);
	keyboard->setShader(ShaderCompiler::compileShader(&matKeyboard, &scene));
	keyboard->setTransformOrder(AffineTransformOrder::SCALE_TRANSLATE_ROTATE);
	keyboard->setScale(2,2,2);
	keyboard->setPosition(0.15, -0.45, 0);
	keyboard->setRotationDeg(90,0,90);

	IndexedMesh* hat = scene.getMeshFactory().createMesh("/mnt/vm/workspace/IRGame/K3De/tests/data/md5_2/cap/hat.obj", true, false);
	scene.add(hat);
	hat->setTransformOrder(AffineTransformOrder::SCALE_TRANSLATE_ROTATE);
	hat->setRotationDeg(90,0,0);
	hat->setScale(2.1,2.1,2.1);
	hat->setPosition(0, -0.1, 0.2);
	Texture* texHat = scene.getTextureFactory().create("/mnt/vm/workspace/IRGame/K3De/tests/data/md5_2/hat/leather.jpg");
	Material matHat(true, true, true);
	matHat.setAmbient(new MaterialAmbientTexture(texHat));
	hat->setMaterial(&matHat);
	hat->setShader(ShaderCompiler::compileShader(&matHat, &scene));
	//hat->setScale(0.6, 0.6, 0.6);

	MD5Mesh weapon(impWeapon.getPreparedMesh(0));
	weapon.addAnimation("idle", aniWeapon);
	weapon.setAnimation("idle");
	scene.add(&weapon);

	MD5Mesh mesh(impPlayer.getPreparedMesh(0));
	mesh.addAnimation("idle", aniIdle);
	mesh.setAnimation("idle");
	mesh.attach(scythe, "lmissile");
	mesh.attach(keyboard, "rmissile");
	mesh.attach(hat, "forehead1");
	scene.add(&mesh);




//	{
//		//std::vector<AttrVertexNormalTangent> vnt = prepWeapon.getVertices(impWeapon.getSkeleton().asAbsoluteFrame(), true);
//		std::vector<AttrVertexNormalTangent> vnt = prepWeapon.getVertices(aniWeapon.getFrame(0), true);
//		weapon.vertices.allocateElements(vnt.size());
//		weapon.vertices.upload(vnt, false);
//	}


	Plane3 plane;
	plane.addPlaneY(0, Vec2(-5, -5), Vec2(+5, +5));
	scene.add(&plane);



//	int idx = 0;
//	IndexedMesh mesh;
//	mesh.indices.append(imp.getIndexedMeshIndices(idx));
//	mesh.indices.upload();
//	mesh.configure();
//	scene.add(&mesh);


	Material mat(true, true, true);
	//mat.setAmbient(new MaterialAmbientFixedColor(Vec4(0,0,1,1)));
	mat.setAmbient(new MaterialAmbientTexture(texDiff));
	mat.setBumpMap(new MaterialBumpMapTexture(texNorm));
	mat.setSpecular(new MaterialSpecularFixed(80, 1));
	mesh.setMaterial(&mat);

	Shader* s = ShaderCompiler::compileShader(&mat, &scene);
	mesh.setShader(s);

	Material mat2(true, true, true);
	mat2.setAmbient(new MaterialAmbientFixedColor(Vec4(0.9, 0.9, 0.9, 1)));
	plane.setMaterial(&mat2);

	Shader* s2 = ShaderCompiler::compileShader(&mat2, &scene);
	plane.setShader(s2);

	Material mat3(true, true, true);
	mat3.setAmbient(new MaterialAmbientTexture(texWeapon));
	Shader* s3 = ShaderCompiler::compileShader(&mat3, &scene);
	weapon.setMaterial(&mat3);
	weapon.setShader(s3);

	//scene.add(&weapon);

//	Box3 weapon;
//	weapon.create(1,1,1);
//	weapon.setMaterial(&mat2);
//	weapon.setShader(s2);
//	scene.add(&weapon);

	std::vector<Box3> boxes(40);
	for (int i = 0; i < 40; ++i) {
		const float sc = 0.25;
		boxes[i].create(sc, sc, sc);
		boxes[i].setMaterial(&mat);
		boxes[i].setShader(s);
		//scene.add(&boxes[i]);
	}

	scene.getCamera().setLookAt(0,0,0);


	scene.getLight(0).setColor(1,1,1);
	scene.getLight(0).setPosition(10,10,10);
	scene.setNumLights(1);

	scene.setClearColor(Vec4(1,1,1,1));
	eng->setScene(&scene);

	float radC = 2.5;
	float radL = radC - 0.2;
	float rad = 0;
	int frm = 0;

	// TODO
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	const int boneIdx = aniIdle.getBoneIndex("rhand");
	const int wBoneIdx = aniWeapon.getBoneIndex("pbody");

	while(true) {

		//radC += 0.05;
		//radL += 0.07;
		rad += 0.03;

		const float xc = std::sin(radC) * 8;
		const float yc = std::cos(radC) * 8;

		const float xl = std::sin(radL) * 10;
		const float yl = std::cos(radL) * 10;

		scene.getCamera().setPosition(xc, 8, yc);
		scene.getLight(0).setPosition(xl, 10, yl);
		scene.getCamera().setLookAt(0,3,0);

		const float scale = 1.0 + std::sin(rad) * 0.1;
		mesh.setRotationRad( mesh.getRotationRad() + Vec3(0,0.1,0) );
		mesh.setScale( scale, scale, scale );


		//MD5::AbsoluteFrame absFrm = imp.getSkeleton().asAbsoluteFrame();//MD5::getAbsFrame(imp.getSkeleton(), ani.getFrame(frm));
	//	const MD5::AbsoluteFrame framePlayer = aniIdle.getFrame(frm);

	//	const MD5::AbsoluteFrame frameWeapon = aniWeapon.getFrame(0);
		//const MD5::AbsoluteFrame frameWeapon = impWeapon.getSkeleton().asAbsoluteFrame();

	//	std::vector<AttrVertexNormalTangent> vnt = prepPlayer.getVertices(framePlayer, true);
	//	mesh.vertices.upload(vnt, false);

		//std::vector<AttrVertexNormalTangent> vnt = prepMesh.getVertices(imp.getSkeleton().asAbsoluteFrame());

//		mesh.setPosition(xl*0.1, 1, yl*0.1);
//		mesh.setRotationRad(radL/3, radL/3, radL/3);

	//	weapon.setPosition(framePlayer.bones[boneIdx].pos.swappedYZ() ); // todo
	//	weapon.setRotationRad(framePlayer.bones[boneIdx].orientation.swappedYZ().toEulerAngles() + Vec3(-M_PI/2, 0, M_PI/2)); // todo

	//	weapon.setPosition(framePlayer.bones[boneIdx].pos.swappedYZ() - frameWeapon.bones[wBoneIdx].pos.swappedYZ()); // todo
		//weapon.setRotationRad(framePlayer.bones[boneIdx].orientation.swappedYZ().toEulerAngles() + frameWeapon.bones[wBoneIdx].orientation.inverse().swappedYZ().toEulerAngles()); // todo
		//weapon.setRotationRad( (frameWeapon.bones[wBoneIdx].orientation.inverse() * framePlayer.bones[boneIdx].orientation.swappedYZ() ).toEulerAngles() + Vec3(-M_PI/2, 0, 0) ); // todo


	//	weapon.getTransform().setParent(&mesh.getTransform());




//		for (size_t i = 0; i < absFrm.bones.size(); ++i) {
//			const Vec3 pos = absFrm.bones[i].pos;
//			boxes[i].setPosition(pos.x, pos.z, pos.y);	// SWAP YZ
//		}


		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		eng->render();

		Image img = eng->getFrame();
		ImageFactory::saveJPG("/tmp/3/img_" + std::to_string(mesh.getCurrentFrame()) + ".jpg", img);


	}

}
*/

#endif
