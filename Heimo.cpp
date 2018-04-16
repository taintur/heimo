//#define OGRE_STATIC_LIB
#include <Ogre.h>
#include <OgreVector3.h>
#include <math.h>
#include <OgreApplicationContext.h>
typedef double Real;

class HeimoContext: public OgreBites::ApplicationContext, public OgreBites::InputListener
{
  //Application context is apparently some sort of an abstract class
  public:
  HeimoContext();
  void setup();
  bool keyPressed(const OgreBites::KeyboardEvent& evt);
  void rotateCamera(int dir);
  void zoomCamera(int dir);
  Ogre::SceneNode* camNode;
  private:
  Real camPhi = 0;
  Real camTheta = Ogre::Math::HALF_PI;
  Real camR = 16;
  Real camR_inc = 0.1;
  Real camAngle_inc = 0.1;

  Ogre::Vector3 world_center = Ogre::Vector3(0, 0, 0);
};

HeimoContext::HeimoContext() : OgreBites::ApplicationContext("Heimo")
{

}

#define PHI_POSITIVE 1
#define PHI_NEGATIVE -1
#define THETA_POSITIVE 2
#define THETA_NEGATIVE -2

void HeimoContext::rotateCamera(int dir){
  //dir == 1 means positive phi, dir == -1 negative phi, dir == 2 means positive
  //theta and dir == -2 means negative theta, create enum sometime in the future
    switch(dir){
      case PHI_POSITIVE:{
        camPhi = camPhi + camAngle_inc;
        break;
      }
      case PHI_NEGATIVE:{
        camPhi = camPhi - camAngle_inc;
        break;
      }
      case THETA_POSITIVE:{
        camTheta = camTheta + camAngle_inc;
        break;
      }
      case THETA_NEGATIVE:{
        camTheta = camTheta - camAngle_inc;
        break;
      }
    }

    Real z = camR * cos(camPhi) * sin(camTheta);
    Real x = camR * sin(camPhi) * sin(camTheta);
    Real y = camR * cos(camTheta);

    Ogre::Vector3 new_pos = Ogre::Vector3(x, y, z);
    camNode->setPosition(new_pos);
    camNode->lookAt(world_center, Ogre::Node::TS_PARENT);

    #ifdef DEBUG
    printf("Current camera position: (%f, %f, %f)\n",new_pos[0],new_pos[1],new_pos[2]);
    #endif
}



void HeimoContext::zoomCamera(int dir){
  //dir == 1 meas outward and dir == -1 means inward
  //the radius gets scaled with the camDist_inc multiplier
      Ogre::Vector3 current_pos = camNode->getPosition();
      Ogre::Vector3 new_pos = dir * camR_inc * current_pos + current_pos;
      camR = new_pos.length();
      camNode->setPosition(new_pos);
      camNode->lookAt(world_center, Ogre::Node::TS_PARENT);
      #ifdef DEBUG
      printf("Current camera position: (%f, %f, %f)\n",new_pos[0],new_pos[1],new_pos[2]);
      #endif
}

bool HeimoContext::keyPressed(const OgreBites::KeyboardEvent& evt)
{
  if(evt.keysym.sym == SDLK_ESCAPE)
  {
    getRoot()->queueEndRendering();
  }

  else if(evt.keysym.sym == SDLK_UP)
  {
    rotateCamera(THETA_NEGATIVE);
  }

  else if(evt.keysym.sym == SDLK_DOWN)
  {
    rotateCamera(THETA_POSITIVE);
  }

  else if(evt.keysym.sym == SDLK_RIGHT)
  {
    rotateCamera(PHI_POSITIVE);
  }

  else if(evt.keysym.sym == SDLK_LEFT)
  {
    rotateCamera(PHI_NEGATIVE);
  }

  else if(evt.keysym.sym == SDLK_LCTRL)
  {
    zoomCamera(1); //outward zoom
  }

  else if(evt.keysym.sym == SDLK_LSHIFT)
  {
    zoomCamera(-1); //inward zooming
  }

  return true;
}

void HeimoContext::setup(void)
{
  //this is the base setup function
  OgreBites::ApplicationContext::setup();

  //creates a thread for listening the SDL input handling?
  addInputListener(this);

  //setting up some other stuff?
  Ogre::Root* root = getRoot();
  Ogre::SceneManager* scnMgr = root->createSceneManager();

  //setting up the real-time Shading system?
  Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr);

  //set up lighting to the scene
  Ogre::Light* light = scnMgr->createLight("MainLight");
  Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  lightNode->setPosition(0, 10, 15);
  lightNode->attachObject(light);

  //set up the camera node
  camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  camNode->setPosition(0, 0, camR);
  camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT); //mikä on tämä toinen argumentti?

  //create the camera element
  Ogre::Camera* cam = scnMgr->createCamera("myCam");
  cam->setNearClipDistance(5);
  cam->setAutoAspectRatio(true);
  camNode->attachObject(cam);

  //put that shit to the main window
  getRenderWindow()->addViewport(cam);

  //add an object to be rendered
  // scnMgr.getRootSceneNode()
  Ogre::Entity* ent = scnMgr->createEntity("WoodPallet.mesh");
  Ogre::SceneNode* modelNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  //modelNode->setOrientation(0,0,0,0); //input type is Real?
  modelNode->attachObject(ent);
}

int main(int argc, char *argv[])
{
  HeimoContext app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
