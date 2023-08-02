#include "Libraries/GLM/ext/matrix_clip_space.hpp"
#include "Libraries/GLM/ext/matrix_transform.hpp"
#include "Libraries/GLM/ext/quaternion_geometric.hpp"
#include "Libraries/GLM/ext/vector_float3.hpp"
#include "Libraries/GLM/geometric.hpp"

#include "Libraries/PL/ClassShader.h"
#include "Libraries/PL/ClassCamera.h"

#include "Libraries/GLAD/glad/KHR/khrplatform.h"
#include "Libraries/GLAD/glad/glad.h"

#include "Libraries/GLFW/include/GLFW/glfw3.h"
#include "Libraries/GLM/glm.hpp"
#include "Libraries/GLM/gtc/matrix_transform.hpp"
#include "Libraries/GLM/gtc/type_ptr.hpp"
#include "Libraries/GLM/gtx/string_cast.hpp"

#include "Libraries/IMGUI/imgui.h"
#include "Libraries/IMGUI/imgui_impl_opengl3.h"
#include "Libraries/IMGUI/imgui_impl_glfw.h"


#include <gl/gl.h>
#include <iterator>
#include <stdlib.h>
#include <iostream>
#include <cmath>




#define print std::cout
#define end std::endl
#define IMGUI_ENABLE_WIN32_DEFAULT_IME_FUNCTIONS


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);

float *CalculateNormals(float verticies[], int arraySize);

void ImGuiSetup(GLFWwindow *window);
void GLSetup();
void GLWindowSetup(GLFWwindow *window);

const unsigned int SCR_WIDTH =  1920;
const unsigned int SCR_HEIGHT = 1080;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;
bool stopPollEvents = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
    // glfw: initialize and configure
    GLSetup();    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLWindowSetup(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //IMGUI setup
    ImGuiSetup(window);

    // build and compile our shader program
    Shader waveShader("Resources/Shaders/Wave.shader");
    Shader LightingShader("Resources/Shaders/Lighting.shader");

    ShaderProgramSource waveSource = Parse("Resources/Shaders/Wave.shader");    
    ShaderProgramSource lightingSource = Parse("Resources/Shaders/Lighting.shader");

    float verticesP[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
}; 

    float planeVertices[]{
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    glm::vec3 postions[]{
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 2.0),
        glm::vec3(0.0, 0.0, 5.0)
    };

   /* unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    }; */
    

//Uncolored cube
    
    unsigned int cVBO, cVAO;

    glGenVertexArrays(1, &cVAO);
    glBindVertexArray(cVAO);
    

    glGenBuffers(1, &cVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesP), verticesP, GL_DYNAMIC_DRAW);  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    

    //normals
    int arrayLength = *(&verticesP + 1) - verticesP;
    unsigned int normCVBO;
    float normArr[108] = {};
    
    float* result = CalculateNormals(verticesP, arrayLength);

    for(int i = 0; i < arrayLength; i++){
        normArr[i] = result[i];
    };
    for(int j = 0; j < arrayLength; j += 3){
        print << normArr[j] << " " << normArr[j + 1] << " " << normArr[j + 2] << std::endl;
    };


    glGenBuffers(1, &normCVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normCVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normArr), normArr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3 ,GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindVertexArray(0);
 
//PLANE
    
    int planeArrLength = *(&planeVertices + 1) - planeVertices;
    float planeNormArr[18] = {};
    float *planeResult = CalculateNormals(planeVertices, planeArrLength);
    unsigned int pNormVBO;

    unsigned int pVAO, pVBO;

    glGenVertexArrays(1, &pVAO);
    glBindVertexArray(pVAO);

    glGenBuffers(1, &pVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glGenBuffers(1, &pNormVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pNormVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeNormArr), planeNormArr, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3 ,GL_FLOAT, GL_FALSE, 0, (void*)0);

    
    glBindVertexArray(0);


    glEnable(GL_DEPTH_TEST);
    
    //declare these out of loop so they can be changed by imGui
    //
    //Uniforms for lighting shader
    glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 cubePosition(0.0f, 0.5f, 0.0f);
    glm::vec3 planePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 cubeColor(0.0f, 1.0f, 0.0f);
    float cubeRotate = 0.0f;
    float cubeRotateX , cubeRotateY , cubeRotateZ;
    bool rotateX = false, rotateY = false, rotateZ = false;

    float ambientIntensity = 0.35f;
    float specValue = 32.0f;

    bool lightType = false;
    bool useBlinn = false;
    //Uniforms for lighting shader
    //
    bool polygonMode;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        if(polygonMode){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        };



        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //imGui init
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
            

        LightingShader.use();


        //Light Uniforms
        int _lightCol = glGetUniformLocation(LightingShader.ID, "lightColor");
        glUniform3fv(_lightCol, 1, glm::value_ptr(lightColor));

        int _lightPos = glGetUniformLocation(LightingShader.ID, "lightPos");
        glUniform3fv(_lightPos, 1, glm::value_ptr(lightPos));

        int _lightPosition = glGetUniformLocation(LightingShader.ID, "light.position");
        glUniform3fv(_lightPosition, 1, glm::value_ptr(lightPos));

        int _lightLinear = glGetUniformLocation(LightingShader.ID, "light.linear");
        glUniform1f(_lightLinear, .09f);

        int _lightQuadratic = glGetUniformLocation(LightingShader.ID, "light.quadratic");
        glUniform1f(_lightQuadratic, .032f);

        int _lightConstant = glGetUniformLocation(LightingShader.ID, "light.constant");
        glUniform1f(_lightConstant, 1.0f);

        int _lightType = glGetUniformLocation(LightingShader.ID, "lightType");
        glUniform1i(_lightType, lightType);

        int _useBlinn = glGetUniformLocation(LightingShader.ID, "useBlinn");
        glUniform1i(_useBlinn, useBlinn);

        //cube uniforms
        int _cubeColor = glGetUniformLocation(LightingShader.ID, "objectColor");
        glUniform3fv(_cubeColor, 1, glm::value_ptr(cubeColor));
        int _cubeAmbInt = glGetUniformLocation(LightingShader.ID, "ambientIntensity");
        glUniform1f(_cubeAmbInt, ambientIntensity);
        int _cubeSpecVal = glGetUniformLocation(LightingShader.ID, "specValue");
        glUniform1f(_cubeSpecVal, specValue);



        //For ImGui Menu
        if(rotateX){
            cubeRotateX = 1.0f;
        }
        else{
            cubeRotateX = 0.0f;
        };
        if(rotateY){
            cubeRotateY = 1.0f;
        }
        else{
            cubeRotateY = 0.0f;
        };
        if(rotateZ){
            cubeRotateZ = 1.0f;
        }
        else{
            cubeRotateZ = 0.0f;
        };

                //FLOOR PLANE
        glm::mat4 planeModel = glm::mat4(1.0f);
        planeModel = glm::rotate(planeModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 planeProj = glm::mat4(1.0f);
        planeProj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 planeView = glm::mat4(1.0f);
        planeView = camera.GetViewMatrix();


        //Time uniform
        float _time = glGetUniformLocation(LightingShader.ID, "time");
        glUniform1f(_time, currentFrame);


        //For Lighitng Shader
        int planeMLoc = glGetUniformLocation(LightingShader.ID, "model");
        int planePLoc = glGetUniformLocation(LightingShader.ID, "proj");
        int planeVLoc = glGetUniformLocation(LightingShader.ID, "view");

        glUniformMatrix4fv(planeMLoc, 1 , GL_FALSE, glm::value_ptr(planeModel));
        glUniformMatrix4fv(planePLoc, 1 , GL_FALSE, glm::value_ptr(planeProj));
        glUniformMatrix4fv(planeVLoc, 1 , GL_FALSE, glm::value_ptr(planeView));



        glBindVertexArray(pVAO);
        

        for(int i = 0; i < 50; i++){
            glm::mat4 planeModel = glm::mat4(1.0f);
            planeModel = glm::rotate(planeModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            planeModel = glm::scale(planeModel, glm::vec3(0.1f, 0.1f, 0.1f));
            planeModel = glm::translate(planeModel, glm::vec3(static_cast<float>(i), 0.0f, 0.0f) + planePosition);

           
           
            glUniformMatrix4fv(planeMLoc, 1, GL_FALSE, glm::value_ptr(planeModel));            

            glDrawArrays(GL_TRIANGLES, 0, 6); 
            
            for(int j = 0; j < 50; j++){
                glm::mat4 planeModel = glm::mat4(1.0f);
                planeModel = glm::rotate(planeModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                planeModel = glm::scale(planeModel, glm::vec3(0.1f, 0.1f, 0.1f));
                planeModel = glm::translate(planeModel, glm::vec3(static_cast<float>(i), static_cast<float>(j), 0.0f) + planePosition);
           
               
                glUniformMatrix4fv(planeMLoc, 1, GL_FALSE, glm::value_ptr(planeModel));

                glDrawArrays(GL_TRIANGLES, 0, 6); 
            };
        
        };

        





        //glDrawArrays(GL_TRIANGLES, 0, 6);

        //CUBES
        glm::mat4 cModel = glm::mat4(1.0f);
        cModel = glm::translate(cModel, cubePosition);
        cModel = glm::rotate(cModel,  glm::radians(cubeRotate) , glm::vec3(cubeRotateX, cubeRotateY, cubeRotateZ));

        glm::mat4 cProjection = glm::mat4(1.0f);
        cProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 cView = glm::mat4(1.0f);
        cView = camera.GetViewMatrix();

        int cmodelLoc = glGetUniformLocation(LightingShader.ID, "model");
        int cprojLoc = glGetUniformLocation(LightingShader.ID, "proj");
        int cviewLoc = glGetUniformLocation(LightingShader.ID, "view");
        glUniformMatrix4fv(cmodelLoc, 1, GL_FALSE, glm::value_ptr(cModel)); 
        glUniformMatrix4fv(cprojLoc, 1, GL_FALSE, glm::value_ptr(cProjection)); 
        glUniformMatrix4fv(cviewLoc, 1, GL_FALSE, glm::value_ptr(cView));

        int viewPosLoc = glGetUniformLocation(LightingShader.ID, "viewPos");
        glUniform3fv(viewPosLoc,1, glm::value_ptr(camera.Position));
        
        glBindVertexArray(cVAO);
        for(int i = 0; i < 3; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, postions[i] + cubePosition);
            model = glm::rotate(model,  glm::radians(cubeRotate) , glm::vec3(cubeRotateX, cubeRotateY, cubeRotateZ));

            glUniformMatrix4fv(cmodelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        };
        for(int j = 0; j < 1; j++){
            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.25f, 0.25f, 0.25f));

            glUniformMatrix4fv(cmodelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));

            glDrawArrays(GL_TRIANGLES, 0 , 36);
        };




        //imGui menu 
        ImGui::Begin("Options Menu");
        ImGui::BulletText("Press Tab to use menu");
        ImGui::BulletText("Press Tilde to exit options");
        if(ImGui::CollapsingHeader("Light")){
            ImGui::Checkbox("Use Point Light?", &lightType);
            ImGui::Checkbox("Use Blinn-Phong?", &useBlinn);
            if(ImGui::CollapsingHeader("Point Light")){
                ImGui::SliderFloat3("Light Position", &lightPos.x, -10.0f, 10.0f);
                ImGui::ColorEdit3("Light Color", &lightColor.x);
            };
            if(ImGui::CollapsingHeader("Directional Light")){
                ImGui::SliderFloat3("Light Position", &lightPos.x, -10.0f, 10.0f);
                ImGui::ColorEdit3("Light Color", &lightColor.x);
            };

        };
        if(ImGui::CollapsingHeader("Draw Options")){
                ImGui::Checkbox("Wireframe", &polygonMode);
        };
        if(ImGui::CollapsingHeader("Cube Options")){
            ImGui::SliderFloat3("Cube Position", &cubePosition.x, -50.0f, 50.0f);
            ImGui::SliderFloat("CubeRotation", &cubeRotate, 0.0f, 360.0f);
            if(ImGui::BeginTable("Rotation Axis", 3)){
                ImGui::TableNextColumn();  ImGui::Checkbox("Rotate X", &rotateX);
                ImGui::TableNextColumn();  ImGui::Checkbox("Rotate Y", &rotateY);
                ImGui::TableNextColumn();  ImGui::Checkbox("Rotate Z", &rotateZ);
                ImGui::EndTable();
            };
            if(ImGui::CollapsingHeader("Cube Shader Options")){
                ImGui::ColorEdit3("Cube Color", &cubeColor.x);
                ImGui::SliderFloat("Cube Ambient Intensity", &ambientIntensity, 0.0f, 1.0f);
                ImGui::SliderFloat("Cube Specular Value", &specValue, 0.0f, 256.0f);
            };

        };
        if(ImGui::CollapsingHeader("Plane Options")){
            ImGui::SliderFloat3("Plane Position", &planePosition.x, -50.0f, 50.0f);
        };

        ImGui::End();

        ImGui::Begin("Stats");

        ImGui::Text(" deltaTime = %f", 1 / deltaTime);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(!stopPollEvents){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        };
        if(stopPollEvents){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        };

    }

    
    //imGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    //ALWAYS CALL AT END
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// Checks if escape is pressed and sets window close flag to true if it is
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        stopPollEvents = true;
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
        stopPollEvents = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //Check if we are using UI so we dont move camera while doing so
    if(!stopPollEvents)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



//NEEDS WORK
float *CalculateNormals(float vertices[], int arraySize){

    int arrayLength = arraySize;
    std::cout << arrayLength << std::endl;
    //float *result = new float[arrayLength];
    static float result[108] = {};
    glm::vec3 crossResult = glm::vec3(0.0f, 0.0f, 0.0f);
    
    int resultIndex = 0;
    int crossIndex = 0;
    int swap = 0;
    for(int i = 0; i < arrayLength; i += 9){
        
        glm::vec3 vecA = glm::vec3(vertices[crossIndex], vertices[crossIndex + 1], vertices[crossIndex + 2]);
        glm::vec3 vecB = glm::vec3(vertices[crossIndex + 3], vertices[crossIndex + 4], vertices[crossIndex + 5]);
        glm::vec3 vecC = glm::vec3(vertices[crossIndex + 6], vertices[crossIndex + 7], vertices[crossIndex + 8]);
        
        print << glm::to_string(vecA) << " " << glm::to_string(vecB) << " " << glm::to_string(vecC) << std::endl;

        glm::vec3 edgeAB = vecB - vecA;
        glm::vec3 edgeAC = vecC - vecA;

        print << "Edge AB: " << glm::to_string(edgeAB) << std::endl;
        print << "Edge AC: " << glm::to_string(edgeAC) << std::endl;

        //hyper-cope fix, maybe one day i will be smart enough to actually find the answer  
        if(swap < 2){
            crossResult = glm::normalize(glm::cross(edgeAC, edgeAB));
            swap++;
        }
        else if(swap >= 2 && swap < 6){
            crossResult = glm::normalize(glm::cross(edgeAB, edgeAC));
            swap++;
        }
        else if(swap >= 6  && swap < 8){
            crossResult = glm::normalize(glm::cross(edgeAC, edgeAB));
            swap++;
        }
        else if(swap >= 8 && swap < 10){
            crossResult = glm::normalize(glm::cross(edgeAB, edgeAC));
            swap++;
        }
        else if (swap >= 10 && swap < 12) {
            crossResult = glm::normalize(glm::cross(edgeAC, edgeAB));
            swap++;
            if(swap == 12){
                swap = 0;
            };
        } 

        crossIndex += 9;
        for(int j = 0; j < 9; j += 9){
            result[resultIndex] = crossResult.x;
            result[resultIndex + 1] = crossResult.y;
            result[resultIndex + 2] = crossResult.z;

            result[resultIndex + 3] = crossResult.x;
            result[resultIndex + 4] = crossResult.y;
            result[resultIndex + 5] = crossResult.z;

            result[resultIndex + 6] = crossResult.x;
            result[resultIndex + 7] = crossResult.y;
            result[resultIndex + 8] = crossResult.z;
            

            resultIndex += 9;
            //crossResult = glm::normalize(glm::cross(edgeAB, edgeAC));
            //crossResult *= glm::vec3(-1.0f,-1.0f,-1.0f);
            
        };  
    };
    print << sizeof(result) << std::endl;
    return result;
};


void GLSetup(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
};

void GLWindowSetup(GLFWwindow *window){
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};
void ImGuiSetup(GLFWwindow *window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
};
