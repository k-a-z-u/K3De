
#ifdef WITH_TESTS

#include "Tests.h"

#include "../K3De.h"
#include "../import/OBJ/ImportMesh.h"
#include "../import/OBJ/ImportAni.h"
#include "../mesh/OBJMesh.h"

#include <cstring>

const char* frame1 = R"(
o Plane
v -1.000000 0.000000 0.384984
v 1.000000 0.000000 0.384984
v -1.000000 0.000000 -1.615016
v 1.000000 0.000000 -1.615016
vn 0.0000 1.0000 0.0000
s off
f 1//1 2//1 4//1 3//1
)";

const char* frame2 = R"(
o Plane
v -1.000000 0.000000 0.689410
v 1.000000 0.000000 0.689410
v -1.000000 0.000000 -1.310590
v 1.000000 0.000000 -1.310590
vn 0.0000 1.0000 0.0000
s off
f 1//1 2//1 4//1 3//1
)";

const char* matA = R"(
   <material name="matA">
	   <config>
		   <diffuse>
			   <color r="1" g="0" b="0" a="0"/>
		   </diffuse>
	   </config>
   </material>
)";

class MyScene1002943 : public Scene {
	virtual void onKeyEvent(const int key, const int scancode, const int action, const int mods) {;}
	virtual void onBecomesActive() {;}
	virtual void onBeforeRender(const SceneState& ss) {;}
};





TEST(OBJ, import) {

	Data d1 = Data::wrapUnmanaged((void*)frame1, strlen(frame1));
	Data d2 = Data::wrapUnmanaged((void*)frame2, strlen(frame2));

	OBJ::Mesh mesh1 = OBJ::ImportMesh::load(d1);
	OBJ::Mesh mesh2 = OBJ::ImportMesh::load(d2);

	// 6 triangles
	ASSERT_EQ(6, mesh1.getIndexedMeshIndices().size());
	ASSERT_EQ(6, mesh2.getIndexedMeshIndices().size());

}

TEST(OBJ, importAni) {

	Data d1 = Data::wrapUnmanaged((void*)frame1, strlen(frame1));
	Data d2 = Data::wrapUnmanaged((void*)frame2, strlen(frame2));

	OBJ::Mesh mesh = OBJ::ImportMesh::load(d1);

	OBJ::ImportAni aimp;
	aimp.add(d1);
	aimp.add(d2);
	OBJ::Animation ani = aimp.get();

	ASSERT_EQ(2, ani.frames.size());
	ASSERT_EQ(ani.frames[0].vertices.size(), ani.frames[1].vertices.size());
	ASSERT_TRUE(ani.frames[0].vertices.size() > 0);

}

TEST(OBJ, liveScene) {

	Data d1 = Data::wrapUnmanaged((void*)frame1, strlen(frame1));
	Data d2 = Data::wrapUnmanaged((void*)frame2, strlen(frame2));
	Data dm = Data::wrapUnmanaged((void*)matA, strlen(matA));

	OBJ::Mesh mesh = OBJ::ImportMesh::load(d1);

	OBJ::ImportAni aimp;
	aimp.add(d1);
	aimp.add(d2);
	OBJ::Animation ani = aimp.get();

	ASSERT_EQ(2, ani.frames.size());
	ASSERT_EQ(ani.frames[0].vertices.size(), ani.frames[1].vertices.size());
	ASSERT_TRUE(ani.frames[0].vertices.size() > 0);


	EngineSettings set;
	set.windowWidth = 800;
	set.windowHeight = 600;
	Engine::init(set);

	MyScene1002943 scene;
	Engine::get()->setScene(&scene);

	OBJMesh omesh(mesh);
	omesh.addAnimation("ani1", ani);
	omesh.setAnimation("ani1");
	omesh.setMaterial(scene.getMaterialFactory().load(dm));
	scene.add(&omesh);

	Engine::get()->run(20);

}


#endif
