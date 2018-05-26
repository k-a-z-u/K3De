

private:

	std::unordered_map<std::string, GLint> uniformLocByName;
	GLint uniformLocByEnum[32] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

	/** fetch all uniforms for this program */
	void getAllActiveUniforms() {

		// total number of uniforms
		GLint numUniforms;
		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);

		// fetch details for each uniform
		for (int i = 0; i < numUniforms; ++i) {

			// fetch details
			GLenum type;
			GLchar name[128];
			GLsizei nameLen;
			GLsizei size;
			glGetActiveUniform(programID, i, 128, &nameLen, &size, &type, name);

			// get location
			std::cout << "#name " << name << std::endl;
			//GLint location = getUniformLocation(name);
			const GLint location = glGetUniformLocation(programID, name);
			std::cout << "#loc " << location << std::endl;

			// remember
			if (location >= 0) {
				Uniform u(programID, name, type, size, location);
				std::cout << u.asString() << std::endl;
				addUniform(u);
			}

			// array uniform? texShadow[4] -> texShadow[0],texShadow[1],...
			for (int i = 1; i < size; ++i) {

				std::string curName = name;
				String::replace(curName, "[0]", "["+std::to_string(i)+"]");

				const GLint location = glGetUniformLocation(programID, curName.c_str());
				if (location >= 0) {
					Uniform uBase(programID, curName, type, size, location);
					std::cout << uBase.asString() << std::endl;
					addUniform(uBase);
				}

			}

			// compound uniform? e.g. Lights.light[7].attenuationQuadratic -> Lights.light[7]
			const std::string baseName = Uniform::getBaseName(name);
			if (baseName != name) {

				const GLint location = glGetUniformLocation(programID, baseName.c_str());
				if (location >= 0) {
					Uniform uBase(programID, baseName, type, size, location);
					std::cout << uBase.asString() << std::endl;
					addUniform(uBase);
				}

			}

			std::cout << "--------" <<std::endl;

		}

	}

	void addUniform(const Uniform& u) {
		uniformLocByName[u.name] = u.location;
		int enumVal = getDefaultUniform(u.name);
		if (enumVal != -1) {
			uniformLocByEnum[enumVal] = u.location;
		}
	}

	int getDefaultUniform(const std::string& name) {
		if ("P" == name) {return DefaultUniform::PROJECTION_MATRIX;}
		if ("M" == name) {return DefaultUniform::MODEL_MATRIX;}
		if ("V" == name) {return DefaultUniform::VIEW_MATRIX;}
		if ("PV" == name) {return DefaultUniform::VIEW_PROJECTION_MATRIX;}
		if ("PVM" == name) {return DefaultUniform::MODEL_VIEW_PROJECTION_MATRIX;}
		if ("camPos" == name) {return DefaultUniform::CAMERA_POSITION;}
		if ("clipZ" == name) {return DefaultUniform::CLIPPING;}
		if ("time" == name) {return DefaultUniform::TIME;}
		if ("screenWidth" == name) {return DefaultUniform::SCREEN_WIDTH;}
		if ("screenHeight" == name) {return DefaultUniform::SCREEN_HEIGHT;}
		return -1;
	}

	/** get the ID for a uniform named "name" */
	inline GLuint getUniformLocation(const std::string& name) const {
		std::unordered_map<std::string, int>::const_iterator it = uniformLocByName.find(name);
		if (it != uniformLocByName.end()) {
			return it->second;
		} else {
			throw Exception("uniform not found: " + name);
		}
	}

	/** get the ID for the given default uniform  */
	inline GLuint getUniformLocation(const DefaultUniform def) const {
		GLint loc = uniformLocByEnum[def];
		if (loc == -1) {throw Exception("uniform not found: " + def);}
		return loc;
	}

public:


	/** does the program have this default uniform */
	inline bool hasUniform(const DefaultUniform def) const {
		return uniformLocByEnum[(int)def] != -1;
	}

	/** does the program have a uniform with the given name? */
	inline bool hasUniform(const std::string& name) const {

#ifdef WITH_ASSERTIONS

		// from program
		const GLint _loc1 = glGetUniformLocation(programID, name.c_str());

		// from cache
		std::unordered_map<std::string, int>::const_iterator _it = uniformLocByName.find(name);
		const GLint _loc2 = (_it == uniformLocByName.end()) ? (-1) : (_it->second);

		// difference?
		if (_loc1 != _loc2) {
			throw Exception("uniform cache mismatch");
		}

#endif
		std::unordered_map<std::string, int>::const_iterator it = uniformLocByName.find(name);
		return it != uniformLocByName.end();

	}



	inline void setInt(const std::string& name, const int i) const {
		assertBound();
		glUniform1i(getUniformLocation(name), i);
		Error::assertOK();
	}
	inline void setInt(const DefaultUniform def, const int i) const {
		assertBound();
		glUniform1i(getUniformLocation(def), i);
		Error::assertOK();
	}

	inline void setFloat(const std::string& name, const float val) const {
		assertBound();
		glUniform1f(getUniformLocation(name), val);
		Error::assertOK();
	}
	inline void setFloat(const DefaultUniform def, const float val) const {
		assertBound();
		glUniform1f(getUniformLocation(def), val);
		Error::assertOK();
	}

	inline void setVector(const std::string& name, const Vec2& vec) const {
		assertBound();
		glUniform2f(getUniformLocation(name), vec.x, vec.y);
		Error::assertOK();
	}
	inline void setVector(const DefaultUniform def, const Vec2& vec) const {
		assertBound();
		glUniform2f(getUniformLocation(def), vec.x, vec.y);
		Error::assertOK();
	}

	inline void setVector(const std::string& name, const Vec3& vec) const {
		assertBound();
		glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
		Error::assertOK();
	}
	inline void setVector(const DefaultUniform def, const Vec3& vec) const {
		assertBound();
		glUniform3f(getUniformLocation(def), vec.x, vec.y, vec.z);
		Error::assertOK();
	}

	void setMatrix(const std::string& name, const Mat4& mat) const {
		assertBound();
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, mat.data());		// GL_TRUE = transpose from row-major to column-major!
		Error::assertOK();
	}
	void setMatrix(const DefaultUniform def, const Mat4& mat) const {
		assertBound();
		glUniformMatrix4fv(getUniformLocation(def), 1, GL_TRUE, mat.data());		// GL_TRUE = transpose from row-major to column-major!
		Error::assertOK();
	}






	std::unordered_set<std::string> _setValues;
	bool _setValuesEnum[32] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,};

	void setVarOnce(const std::string& name, UBO& ubo) {
		if (_setValues.find(name) == _setValues.end()) {
			setVar(name, ubo);
			_setValues.insert(name);
		}
	}

	void setFloatOnce(const std::string& name, const float val) {
		if (_setValues.find(name) == _setValues.end()) {
			setFloat(name, val);
			_setValues.insert(name);
		}
	}
	void setFloatOnce(const DefaultUniform def, const float val) {
		if (!_setValuesEnum[def]) {
			setFloat(def, val);
			_setValuesEnum[def] = true;
		}
	}









	/** des the programe have a uniform-block with the given name? */
	bool hasUniformBlock(const std::string& name) const {
		return GL_INVALID_INDEX != glGetUniformBlockIndex(programID, name.c_str());
	}

	inline void setVar(const std::string& name, UBO& ubo) {
		assertBound();
		const GLuint idx = glGetUniformBlockIndex(programID, name.c_str());
		glUniformBlockBinding(programID, idx, 0);
		Error::assertOK();
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo.getID());
		Error::assertOK();
	}
