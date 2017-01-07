#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "../scene/Renderable.h"
#include "../math/Vector.h"
#include "../gl/VBOArray.h"
#include "../gl/VAO.h"
#include "../math/Random.h"
#include "../scene/Scene.h"

struct Particle {
	Vec3 pos;
	float size;
	Particle(const Vec3& pos, const float size) : pos(pos), size(size) {;}
};

class ParticleSystem : public Renderable {

private:

	VAO vao;
	VBOArrayStatic<VertexTexture> mesh;
	VBOArrayDynamic<Particle> params;

	//std::vector<Particle> particles;
	Particle* particles;

	Scene* scene;
	Texture* tex;

	Mat4 mat;

	int numParticles = 10240;

public:

	float speed(int idx) {
		return 0.3 + (idx%100)/200.0;
	}

	/** ctor */
	ParticleSystem(Scene* scene) : scene(scene) {

		setShader( scene->getShaderFactory().create("partVert.glsl", "partFrag.glsl") );
		tex = scene->getTextureFactory().create("/apps/workspaces/raptor/data/snowflake.png");
		shader->setInt("tex", 0);

		VertexTexture vnt1(Vec3(-1,-1,0), Vec2(0,0));
		VertexTexture vnt2(Vec3(+1,-1,0), Vec2(1,0));
		VertexTexture vnt3(Vec3(+1,+1,0), Vec2(1,1));
		VertexTexture vnt4(Vec3(-1,+1,0), Vec2(0,1));

		mesh.append(vnt1);
		mesh.append(vnt2);
		mesh.append(vnt3);

		mesh.append(vnt1);
		mesh.append(vnt3);
		mesh.append(vnt4);

		mesh.upload();



		//params.upload();
		particles = (Particle*) params.allocateDynamic(numParticles * sizeof(Particle));

		params.beginSync();
		for (int i = 0; i < numParticles; ++i) {
			const float x = Random::get(-5,5);
			const float y = Random::get(0,5);
			const float z = Random::get(-5,5);
			const float s = Random::get(0.01, 0.04);
			Particle p(Vec3(x,y,z), s);
			particles[i] = p;
			//params.append(p);
		}
		params.endSync();





		vao.bind();

		mesh.bind();
		vao.setVertices(0, sizeof(VertexTexture));
		vao.setTexCoords(1, sizeof(VertexTexture), sizeof(Vertex));

		params.bind();
		vao.setFloats(2, sizeof(Particle)/sizeof(float));

		vao.unbind();

	}

	virtual void render(const RenderStage& rs) override {

		//params.beginSync();
		for (int i = 0; i < numParticles; ++i) {
			const float t = Engine::get()->getDeltaSec() * speed(i);
			particles[i].pos.y -= t;
			if (particles[i].pos.y < 0) {particles[i].pos.y += 5;}
		}
		//params.endSync();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

		shader->bind();
		vao.bind();
		tex->bind(0);

		shader->setVector("camRight", scene->getCamera().getVMatrix().getRow3(0));
		shader->setVector("camUp", scene->getCamera().getVMatrix().getRow3(1));

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 0); // positions : one per quad (its center) -> 1

		glVertexAttribDivisor(2, 1); // color : one per quad -> 1

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, numParticles);

		tex->unbind(0);
		vao.unbind();
		shader->unbind();

		glDisable(GL_BLEND);

	}

	virtual const Mat4& getMatrix() const {
		return mat;
	}

	virtual bool isVisible(const Mat4& MVP) const {
		return true;
	}

private:

};

#endif // PARTICLESYSTEM_H
