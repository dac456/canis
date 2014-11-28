#if defined(__WIN32__) || defined(_WIN32)

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "assimp.lib")

#ifndef _DEBUG
#pragma comment(lib, "SOILr.lib")
#pragma comment(lib, "boost_filesystem-vc100-mt-1_47.lib")
#else
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "boost_filesystem-vc100-mt-gd-1_47.lib")
#endif

#pragma comment(lib, "Canis.lib")

#endif

#include "CanisViewer.h"

#include <math.h>

#define M_PI 3.14159265358979323846

Canis::GlShader* basicShd;
Canis::Mesh* mesh;
Canis::Mesh* mesh2;
Canis::SkyBox* skyBox;
glm::mat4 projMatrix;
Canis::Camera* cam;
Canis::Engine* engine;

Canis::Scene* scene;
Canis::SceneNode* newNode;
Canis::SceneNode* lightNode;
Canis::SceneNode* propNode;
Canis::SceneNode* propNode2;
Canis::StaticMesh* sm;
Canis::Prop* p1;
Canis::Prop* p2;
Canis::Light* dynamicLight;
Canis::Light* dynamicLight2;

int frame = 0;
int currentTime;
int lastTime = 0;

float cX, lX = 0.0f;
glm::vec3 angles;

bool mouseLock = true;

int main(int argc, char* argv[])
{
    fprintf(stdout, "%s\n", argv[0]);
    initialize(argc, argv);
    glutMainLoop();

    delete engine;
    exit(EXIT_SUCCESS);
}

void initialize(int argc, char* argv[])
{
    initWindow(argc, argv);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glewExperimental = GL_TRUE;
    glewInit();

    engine = new Canis::Engine();

    cam = new Canis::Camera("default", glm::vec3(0.0, 1.8, 0.0), glm::vec3(0.0, 1.8, 5.0));
    Canis::RenderManager::getSingleton().addCamera(cam);
    Canis::RenderManager::getSingleton().setActiveCamera(cam);

    //mesh = new Canis::Mesh(new Canis::DXSLoader("./Media/DXS/WareHouse.dxs"));
    //mesh2 = new Canis::Mesh(new Canis::DXSLoader("./Media/DXS/scene.dxs"));
    scene = new Canis::Scene("Scene01");
    Canis::RenderManager::getSingleton().addScene(scene);

    //FBO test
    Canis::Texture* col0 = new Canis::Texture("rtt0", 512, 512, Canis::TEXTURE_RGBA_UBYTE, 0);
    Canis::Texture* depth = new Canis::Texture("rtt_depth", 512, 512, Canis::TEXTURE_DEPTH_UBYTE, 0);
    Canis::TextureManager::getSingleton().addTexture(col0);
    Canis::TextureManager::getSingleton().addTexture(depth);
    std::vector<Canis::Texture*> attachments;
    attachments.push_back(col0);
    Canis::FramebufferObject* fbo = new Canis::FramebufferObject(512, 512, attachments, depth);
    Canis::RenderManager::getSingleton().addRenderTarget(new Canis::RenderTarget("mirror", cam, fbo));

        Canis::StringMap args;
        /*args["mass"] = "1.0f";
        args["shape"] = std::to_string((long long)Canis::BOX_SHAPE);
        args["mesh"] = "./Media/Models/testobj/testobj.obj";
        Canis::IEntity* ent = Canis::EntityManager::getSingleton().getEntityFactory("Prop")->createEntity("Ball", glm::mat4(1.0f), args);*/

        args.clear();
        args["mesh"] = "./Media/Models/terrain2.ms3d";
        Canis::IEntity* ent2 = Canis::EntityManager::getSingleton().getEntityFactory("Static Mesh")->createEntity("Terrain", glm::mat4(1.0f), args);

        Canis::SceneNode* root = new Canis::SceneNode("Root");
        //Canis::SceneNode* ball = new Canis::SceneNode("BallNode", glm::translate(glm::vec3(0.0f, 30.0f, 0.0f)));
        scene->addSceneNode(root);
        //scene->addSceneNode(ball);

        root->attachEntity(ent2);
        //ball->attachEntity(ent);

        srand(time(NULL));
        for(size_t i=0; i<10000; i++){
            Canis::StringMap args;
            args["mass"] = "1.0f";
            args["shape"] = std::to_string((long long)Canis::BOX_SHAPE);
            args["mesh"] = "./Media/Models/testobj/testobj.obj";

            std::stringstream ss;
            ss << "Box_" << i;
            Canis::IEntity* ent = Canis::EntityManager::getSingleton().getEntityFactory("Prop")->createEntity(ss.str(), glm::mat4(1.0f), args);

            ss << "_node";
            Canis::SceneNode* node = new Canis::SceneNode(ss.str(), glm::translate(glm::vec3(rand() % 100, (rand() % 100) + 200, rand() % 100)));

            scene->addSceneNode(node);
            node->attachEntity(ent);
        }

        Canis::Light* light = new Canis::Light("Light0", glm::vec3(1.0f, 1.0f, 1.0f), 10000.0f, glm::translate(glm::vec3(0.0f, 2000.0f, 0.0f)));
        Canis::RenderManager::getSingleton().addLight(light);
        root->attachLight(light);

    Canis::DIFLoader* dif = new Canis::DIFLoader("./Media/DIF/cathedral.dif");
    delete dif;
    //mesh = new Canis::Mesh(new Canis::LW14Loader("./Media/3DW/DwarvenCapital.3dw"));
    //mesh = new Canis::Mesh(new Canis::CSXLoader("./Media/CSX/cathedral.csx"));
    //mesh2 = new Canis::Mesh(new Canis::AssimpLoader("./Media/Models/sphere.ms3d"));
    //skyBox = new Canis::SkyBox("test");

    fprintf(stdout, "GL Version: %s\n", glGetString(GL_VERSION));
    fprintf(stdout, "Canis Version: %i.%i.%i '%s'\n", CS_MAJOR_REVISION, CS_MINOR_REVISION, CS_BUILD_REVISION, CS_CODENAME);
}

void initWindow(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitWindowPosition(200, 32);
    glutInitWindowSize(1024,512);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    int wHandle = glutCreateWindow("-");

    if(wHandle < 1){
        fprintf(stderr, "Could not create rendering window.");
        exit(EXIT_FAILURE);
    }

    //projMatrix = glm::perspective(45.0f, (float)(800/600), 0.9f, 10000.0f);
    //fprintf(stdout, "%i\n", sizeof(char));

    glutReshapeFunc(resize);
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyboard);
}

void resize(int width, int height)
{
    glViewport(0,0,width,height);
    projMatrix = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 10000.0f);
    Canis::RenderManager::getSingleton().setProjectionMatrix(projMatrix);
    Canis::RenderManager::getSingleton().resize(width, height);
}

float x = 0.0f;
void render()
{
    frame++;
    currentTime=glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lastTime > 1000) {
        char s[20];
        sprintf(s,"FPS:%4.2f",
            frame*1000.0/(currentTime-lastTime));
        glutSetWindowTitle(s);
        lastTime = currentTime;
        frame = 0;
    }

    //if(mouseLock)
        //mouse();
    glEnable(GL_DEPTH_TEST);

    Canis::RenderManager::getSingleton().setViewMatrix(cam->getTransform());
    Canis::RenderManager::getSingleton().update();

    x += 0.5;
    if(x > 360.0f)
        x = 0.0f;

    glutSwapBuffers();
    glutPostRedisplay();
}

void idle()
{
    glutPostRedisplay();
}

void mouse(int x, int y){
    //POINT mousePos;
    //GetCursorPos(&mousePos);

    if(mouseLock){
        int mX = glutGet(GLUT_WINDOW_X)+(glutGet(GLUT_WINDOW_WIDTH))/2;
        int mY = glutGet(GLUT_WINDOW_Y)+(glutGet(GLUT_WINDOW_HEIGHT))/2;

        if((x == mX) && (y == mY))
            return;

        float dX = ((float)mX - (float)x)/100.0f;
        float dY = ((float)mY - (float)y)/100.0f;

        angles.x += dX*4.0f;
        angles.y += dY*4.0f;

        /*if(angles.x < -M_PI)
          angles.x += M_PI * 2;
        else if(angles.x > M_PI)
          angles.x -= M_PI * 2;

        if(angles.y < -M_PI / 2)
          angles.y = -M_PI / 2;
        if(angles.y > M_PI / 2)
          angles.y = M_PI / 2;*/

        cam->setRotation(glm::radians(angles.y), glm::radians(angles.x), 0.0f);

        //SetCursorPos(mX, mY);
        glutWarpPointer(mX, mY);
    }
}

void keyboard(unsigned char key, int x, int y){
    if(key == 119){
        cam->move(glm::vec3(-1.5f, -1.5f, -1.5f));
    }
    if(key == 115){
        cam->move(glm::vec3(1.5f, 1.5f, 1.5f));
    }
    if(key == 100){
        cam->strafe(-1.5f);
    }
    if(key == 97){
        cam->strafe(1.5f);
    }
    if(key == 27){
        exit(0);
    }
    if(key == 109){
        mouseLock = !mouseLock;
    }
    glutPostRedisplay();

}
