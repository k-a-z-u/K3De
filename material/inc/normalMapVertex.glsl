R"=====(

out mat3 normalRotMat;

/** get the matrix describing the bump-maps coordinate system (normal, tangent, bitangent) */
mat3 getNormalRotationMatrix(const vec3 normal, const vec3 tangent) {
    vec3 bitangent = cross(normal, tangent);
    return mat3(tangent, bitangent, normal);
}

)====="
