#version 330

out vec3 WorldPos;

uniform mat4 ProjectionMatrix; // Projection matrix for this view
uniform mat4 ViewMatrix; // View matrix for this view

uniform float GridSize;
uniform int PlaneID;       // 0: X-Z, 1: X-Y, 2: Y-Z

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0), // Bottom left
    vec3( 1.0, 0.0, -1.0), // Bottom right
    vec3( 1.0, 0.0,  1.0), // Top right
    vec3(-1.0, 0.0,  1.0)  // Top left
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main() {
    int Index = Indices[gl_VertexID];
    vec3 vPos3 = Pos[Index];
    vec3 worldPos;

    if (PlaneID == 0) { // X-Z
        worldPos.x = mix(-GridSize, GridSize, (vPos3.x + 1.0) * 0.5);
        worldPos.y = 0.0;
        worldPos.z = mix(-GridSize, GridSize, (vPos3.z + 1.0) * 0.5);
    } else if (PlaneID == 1) { // X-Y
        worldPos.x = mix(-GridSize, GridSize, (vPos3.x + 1.0) * 0.5);
        worldPos.y = mix(-GridSize, GridSize, (vPos3.z + 1.0) * 0.5);
        worldPos.z = 0.0;
    } else if (PlaneID == 2) { // Y-Z
        worldPos.x = 0.0;
        worldPos.y = mix(-GridSize, GridSize, (vPos3.x + 1.0) * 0.5);
        worldPos.z = mix(-GridSize, GridSize, (vPos3.z + 1.0) * 0.5);
    }

    gl_Position = ProjectionMatrix * ViewMatrix * vec4(worldPos, 1.0);
    WorldPos = worldPos;
}
