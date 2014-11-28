#include "RenderManager.h"
#include "Scene.h"
#include "Light.h"
#include "Camera.h"
#include "FramebufferObject.h"
#include "RenderTarget.h"

namespace Canis
{

    RenderManager::RenderManager(){
        _activeCamera = nullptr;
    }

    RenderManager::~RenderManager(){
        for(size_t i=0; i<_lights.size(); i++)
            delete _lights[i];
        for(size_t i=0; i<_cameras.size(); i++)
            delete _cameras[i];
        for(size_t i=0; i<_scenes.size(); i++)
            delete _scenes[i];
        for(size_t i=0; i<_renderTargets.size(); i++)
            delete _renderTargets[i];
    }

    //This should be called every frame by the host application
    void RenderManager::update(){
        for(size_t i=0; i<_renderTargets.size(); i++){
            _renderTargets[i]->set();

            //TODO: this probably isn't the best way to deal with scenes
            //for(size_t i=0; i<_cameras.size(); i++)
            //  _cameras[i]->update();

            //for(size_t i=0; i<_scenes.size(); i++)
            //   _scenes[i]->render();
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, _width, _height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //setViewMatrix(_activeCamera->getTransform());
        setViewVector(glm::vec4(_activeCamera->getViewVector(), 1.0f));
        //setProjectionMatrix(_projectionMatrix);
        calculateFrustum(glm::mat4(1));

        //TODO: this probably isn't the best way to deal with scenes
        //for(size_t i=0; i<_scenes.size(); i++)
        //    _scenes[i]->render();
    }

    void RenderManager::resize(int width, int height){
        _width = width;
        _height = height;
    }

    void RenderManager::addRenderTarget(RenderTarget* target){
        _renderTargets.push_back(target);
    }

    void RenderManager::calculateFrustum(glm::mat4 transform){
        //glm::mat4 mvp = /*transform*/_viewMatrix*_projectionMatrix;
        glm::mat4 mvp = _projectionMatrix*_viewMatrix*transform;
        mvp = glm::transpose(mvp);

        /*_frustum[0] = mvp[3] + mvp[0]; // left
        _frustum[1] = mvp[3] - mvp[0]; // right
        _frustum[2] = mvp[3] - mvp[1]; // top
        _frustum[3] = mvp[3] + mvp[1]; // bottom
        _frustum[4] = mvp[3] + mvp[2]; // near
        _frustum[5] = mvp[3] - mvp[2]; // far*/

        //left
        _frustum[0] = glm::vec4(mvp[0][3] + mvp[0][0],
                                mvp[1][3] + mvp[1][0],
                                mvp[2][3] + mvp[2][0],
                                mvp[3][3] + mvp[3][0]);

        //right
        _frustum[1] = glm::vec4(mvp[0][3] - mvp[0][0],
                                mvp[1][3] - mvp[1][0],
                                mvp[2][3] - mvp[2][0],
                                mvp[3][3] - mvp[3][0]);

        //bottom
        _frustum[2] = glm::vec4(mvp[0][3] + mvp[0][1],
                                mvp[1][3] + mvp[1][1],
                                mvp[2][3] + mvp[2][1],
                                mvp[3][3] + mvp[3][1]);

        //top
        _frustum[3] = glm::vec4(mvp[0][3] - mvp[0][1],
                                mvp[1][3] - mvp[1][1],
                                mvp[2][3] - mvp[2][1],
                                mvp[3][3] - mvp[3][1]);

        //near
        _frustum[4] = glm::vec4(mvp[0][3] + mvp[0][2],
                                mvp[1][3] + mvp[1][2],
                                mvp[2][3] + mvp[2][2],
                                mvp[3][3] + mvp[3][2]);

        //far
        _frustum[5] = glm::vec4(mvp[0][3] - mvp[0][2],
                                mvp[1][3] - mvp[1][2],
                                mvp[2][3] - mvp[2][2],
                                mvp[3][3] - mvp[3][2]);

        /*_frustum[0] = glm::normalize(_frustum[0]);
        _frustum[1] = glm::normalize(_frustum[1]);
        _frustum[2] = glm::normalize(_frustum[2]);
        _frustum[3] = glm::normalize(_frustum[3]);
        _frustum[4] = glm::normalize(_frustum[4]);
        _frustum[5] = glm::normalize(_frustum[5]);*/
        //_frustum[5].w *= 50.0f;

        /*FILE* fout = fopen("./frustum.txt", "a");
        for(int i=0; i<6; i++)
            fprintf(fout, "%i: %f, %f, %f, %f\n", i, _frustum[i].x, _frustum[i].y, _frustum[i].z, _frustum[i].w);
        fclose(fout);*/
    }

    void RenderManager::addLight(Light* light){
        if(std::find(_lights.begin(), _lights.end(), light) == _lights.end())
            _lights.push_back(light);
    }

    std::pair<std::pair<glm::mat4, glm::mat4>, glm::vec4> RenderManager::getLightsClosestToPoint(glm::vec4 point){
        //TODO: we will eventually only want to sort lights that we can see
        /*for(size_t i=0; i<_lights.size(); i++){
            glm::vec4 lightPos = glm::vec4(_lights[i]->getAbsolutePosition(), 1.0f);
            glm::vec4 lightDir = glm::normalize(lightPos - point);
            glm::vec4 eyeDir = glm::normalize(glm::vec4(_activeCamera->getAbsolutePosition(), 1.0f) - point);

            //_lights[i]->_brightness = glm::dot(lightDir, eyeDir);
            //_lights[i]->_brightness = glm::length(point - lightPos);
            _lights[i]->_brightness = glm::length(lightPos - point);
            //printf("Brightness: %f\n", _lights[i]->_brightness);
        }

        std::sort(_lights.begin(), _lights.end());

        glm::mat4 retPos = glm::mat4(0.0f);
        glm::mat4 retCol = glm::mat4(0.0f);
        glm::vec4 retRad = glm::vec4(0.0f);

        int count = 0;
        if(_lights.size() <= 4)
            count = _lights.size();
        else
            count = 4;

        for(size_t i=0; i<count; i++){
            retPos[i] = glm::vec4(_lights[i]->getAbsolutePosition(), 1.0f);
            retCol[i] = glm::vec4(_lights[i]->getDiffuse(), 1.0f);
            retRad[i] = _lights[i]->getRadius();
        }

        std::pair<std::pair<glm::mat4, glm::mat4>, glm::vec4> ret;
        ret.first.first = glm::mat4(0.0f);
        ret.first.second = glm::mat4(0.0f);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                ret.first.first[i][j] = retPos[i][j];
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                ret.first.second[i][j] = retCol[i][j];
        for(int i=0; i<4; i++)
            ret.second[i] = retRad[i];
        return ret;*/
        std::cout << "Warning: RenderManager deprecated" << std::endl;
        std::pair<std::pair<glm::mat4, glm::mat4>, glm::vec4> ret;
        return ret;
    }

    void RenderManager::addScene(Scene* scene){
        _scenes.push_back(scene); //TODO: enforce uniqueness
    }

    std::vector<Scene*> RenderManager::getScenes(){
        return _scenes;
    }

    void RenderManager::addCamera(Camera* camera){
        if(std::find(_cameras.begin(), _cameras.end(), camera) == _cameras.end())
            _cameras.push_back(camera);
    }

    void RenderManager::setActiveCamera(Camera* camera){
        _activeCamera = camera;
    }

    Camera* RenderManager::getActiveCamera(){
        return _activeCamera;
    }

    void RenderManager::setViewMatrix(glm::mat4 viewMatrix){
        _viewMatrix = viewMatrix;
    }

    glm::mat4 RenderManager::getViewMatrix(){
        return _viewMatrix;
    }

    void RenderManager::setProjectionMatrix(glm::mat4 projectionMatrix){
        _projectionMatrix = projectionMatrix;
    }

    glm::mat4 RenderManager::getProjectionMatrix(){
        return _projectionMatrix;
    }

    void RenderManager::setViewVector(glm::vec4 viewVector){
        _viewVector = viewVector;
    }

    glm::vec4 RenderManager::getViewVector(){
        return _viewVector;
    }

    glm::vec4* RenderManager::getFrustum(){
        return _frustum;
    }
}
