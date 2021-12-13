// #include <iostream>
// #include <vector>

// #include <GL/gl.h>
// #include <glm/glm.hpp>
// #include "../wolf/stb_image.h"


// unsigned int loadTexture(const char *path);
// unsigned int loadCubemap(vector<std::string> faces);

// int main()
// {
//     // "6.1.cubemaps.vs", "6.1.cubemaps.fs");
//     // "6.1.skybox.vs", "6.1.skybox.fs");

//     // cube VAO
//     unsigned int cubeVAO, cubeVBO;
//     glGenVertexArrays(1, &cubeVAO);
//     glGenBuffers(1, &cubeVBO);
//     glBindVertexArray(cubeVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     // skybox VAO
//     unsigned int skyboxVAO, skyboxVBO;
//     glGenVertexArrays(1, &skyboxVAO);
//     glGenBuffers(1, &skyboxVBO);
//     glBindVertexArray(skyboxVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

//     // load textures
//     // -------------
//     unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());

//     vector<std::string> faces
//     {
//         FileSystem::getPath("resources/textures/skybox/right.jpg"),
//         FileSystem::getPath("resources/textures/skybox/left.jpg"),
//         FileSystem::getPath("resources/textures/skybox/top.jpg"),
//         FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
//         FileSystem::getPath("resources/textures/skybox/front.jpg"),
//         FileSystem::getPath("resources/textures/skybox/back.jpg")
//     };
//     unsigned int cubemapTexture = loadCubemap(faces);

//     // render loop
//     // -----------
//     while (!glfwWindowShouldClose(window))
//     {
//         // draw scene as normal
//         shader.use();
//         glm::mat4 model = glm::mat4(1.0f);
//         glm::mat4 view = camera.GetViewMatrix();
//         glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//         shader.setMat4("model", model);
//         shader.setMat4("view", view);
//         shader.setMat4("projection", projection);
//         // cubes
//         glBindVertexArray(cubeVAO);
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, cubeTexture);
//         glDrawArrays(GL_TRIANGLES, 0, 36);
//         glBindVertexArray(0);

//         // draw skybox as last
//         glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
//         skyboxShader.use();
//         view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
//         skyboxShader.setMat4("view", view);
//         skyboxShader.setMat4("projection", projection);
//         // skybox cube
//         glBindVertexArray(skyboxVAO);
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//         glDrawArrays(GL_TRIANGLES, 0, 36);
//         glBindVertexArray(0);
//         glDepthFunc(GL_LESS); // set depth function back to default
//     }

//     // optional: de-allocate all resources once they've outlived their purpose:
//     // ------------------------------------------------------------------------
//     glDeleteVertexArrays(1, &cubeVAO);
//     glDeleteVertexArrays(1, &skyboxVAO);
//     glDeleteBuffers(1, &cubeVBO);
//     glDeleteBuffers(1, &skyboxVBO);
// }

// unsigned int loadTexture(char const * path)
// {
//     unsigned int textureID;
//     glGenTextures(1, &textureID);

//     int width, height, nrComponents;
//     unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
//     if (data)
//     {
//         GLenum format;
//         if (nrComponents == 1)
//             format = GL_RED;
//         else if (nrComponents == 3)
//             format = GL_RGB;
//         else if (nrComponents == 4)
//             format = GL_RGBA;

//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         stbi_image_free(data);
//     }
//     else
//     {
//         std::cout << "Texture failed to load at path: " << path << std::endl;
//         stbi_image_free(data);
//     }

//     return textureID;
// }

// unsigned int loadCubemap(vector<std::string> faces)
// {
//     unsigned int textureID;
//     glGenTextures(1, &textureID);
//     glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

//     int width, height, nrChannels;
//     for (unsigned int i = 0; i < faces.size(); i++)
//     {
//         unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
//         if (data)
//         {
//             glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//             stbi_image_free(data);
//         }
//         else
//         {
//             std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
//             stbi_image_free(data);
//         }
//     }
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

//     return textureID;
// }
