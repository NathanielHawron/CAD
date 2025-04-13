const char *vertexShaderSrc = 
"#version 330 core\n" \
"layout (location = 0) in vec3 pos;\n" \
"void main(){\n" \
    "gl_Position = vec4(pos, 1.0);\n"
"}\0";

const char *fragmentShaderSrc =
"#version 330 core\n" \
"out vec4 color;\n" \
"void main(){\n" \
    "color = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";