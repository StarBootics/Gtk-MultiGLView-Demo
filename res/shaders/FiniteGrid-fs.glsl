#version 330

in vec3 WorldPos;

layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 BrightColor;

uniform float GridSize;
uniform int PlaneID;          // 0: X-Z, 1: X-Y, 2: Y-Z
uniform float GridCellSize;   // Grid spacing
uniform float MajorLineSpacing = 5.0; // Draw a thick line every N cells
uniform vec4 GridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 GridColorThick = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 AxisColorX = vec4(0.0, 1.0, 0.0, 1.0); // Green
uniform vec4 AxisColorY = vec4(0.0, 0.0, 1.0, 1.0); // Blue
uniform vec4 AxisColorZ = vec4(1.0, 0.0, 0.0, 1.0); // Red

float max2(vec2 v) {
    return max(v.x, v.y);
}

void main() {
    vec2 coords;
    vec4 HorizontalAxisColor = GridColorThick;
    vec4 VerticalAxisColor = GridColorThick;
    
    if (PlaneID == 0) {
        coords = WorldPos.xz;
        HorizontalAxisColor = AxisColorX; // X-axis (green)
        VerticalAxisColor = AxisColorZ;   // Z-axis (red)
    } else if (PlaneID == 1) {
        coords = WorldPos.xy;
        HorizontalAxisColor = AxisColorX; // X-axis (green)
        VerticalAxisColor = AxisColorY;   // Y-axis (blue)
    } else if (PlaneID == 2) {
        coords = WorldPos.yz;
        HorizontalAxisColor = AxisColorY; // Z-axis (red)
        VerticalAxisColor = AxisColorZ;   // Y-axis (blue)
    }
    
    if (coords.x < -GridSize || coords.x > GridSize ||
        coords.y < -GridSize || coords.y > GridSize) {
        discard;
    }

    vec2 dudv = vec2(length(vec2(dFdx(coords.x), dFdy(coords.x))),
                     length(vec2(dFdx(coords.y), dFdy(coords.y)))) + 0.001;
    dudv *= 1.5; // Adjusted thickness (tweak as needed)

    vec2 mod_div_dudv = mod(coords, GridCellSize) / dudv;
    float grid = max2(vec2(1.0) - abs(clamp(mod_div_dudv, 0.0, 1.0) * 2.0 - 1.0));

    vec2 mod_thick = mod(coords, GridCellSize * MajorLineSpacing) / dudv;
    float thick = max2(vec2(1.0) - abs(clamp(mod_thick, 0.0, 1.0) * 2.0 - 1.0));

    // Origin lines
    float origin = 0.0;
    vec2 originDist = abs(coords) / dudv;
    if (originDist.x < 1.0 || originDist.y < 1.0) {
        origin = 1.0 - min(min(originDist.x, originDist.y), 1.0);
    }

    // Edge lines
    float edge = 0.0;
    vec2 edgeDist = abs(abs(coords) - GridSize) / dudv;
    if (edgeDist.x < 1.0 || edgeDist.y < 1.0) {
        edge = 1.0 - min(min(edgeDist.x, edgeDist.y), 1.0);
    }
    
    // Color selection
    vec4 color;
    
    if (originDist.x < 1.0) {
        color = VerticalAxisColor; // Vertical origin line
    } else if (originDist.y < 1.0) {
        color = HorizontalAxisColor;  // Horizontal origin line
    } else if (thick > 0.0) {
        color = GridColorThick;      // Major lines
    } else {
        color = GridColorThin;       // Minor lines or edges
    }
    
    color.a *= max(max(max(grid, thick), origin), edge);

    FragColor = color;
    BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
}
