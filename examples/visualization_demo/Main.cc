/*
 * Copyright (C) 2021 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#if defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#elif _WIN32
  #define NOMINMAX
  // Must include this before GL/gl.h
  #include <windows.h>
  #include <GL/glew.h>
  // OpenGL utilities header file
  #include <GL/glu.h>
  // OpenGL utilities header file
  #include <GL/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif

#include <iostream>
#include <vector>

#include <ignition/common/Console.hh>
#include <ignition/rendering.hh>

#include "GlutWindow.hh"

using namespace ignition;
using namespace rendering;

//////////////////////////////////////////////////
void buildScene(ScenePtr _scene)
{
  // initialize _scene
  _scene->SetAmbientLight(0.3, 0.3, 0.3);
  VisualPtr root = _scene->RootVisual();

  // create directional light
  DirectionalLightPtr light0 = _scene->CreateDirectionalLight();
  light0->SetDirection(-0.5, 0.5, -1);
  light0->SetDiffuseColor(0.5, 0.5, 0.5);
  light0->SetSpecularColor(0.5, 0.5, 0.5);
  root->AddChild(light0);

  // create point light
  PointLightPtr light1 = _scene->CreatePointLight();
  light1->SetDiffuseColor(0.5, 0.5, 0.5);
  light1->SetSpecularColor(0.5, 0.5, 0.5);
  light1->SetLocalPosition(5, -5, 10);
  root->AddChild(light1);

  // create point light
  PointLightPtr light2 = _scene->CreatePointLight();
  light2->SetDiffuseColor(0.5, 0.5, 0.5);
  light2->SetSpecularColor(0.5, 0.5, 0.5);
  light2->SetLocalPosition(3, 5, 5);
  root->AddChild(light2);

  // create green material
  MaterialPtr green = _scene->CreateMaterial();
  green->SetAmbient(0.0, 0.5, 0.0);
  green->SetDiffuse(0.0, 0.7, 0.0);
  green->SetSpecular(0.5, 0.5, 0.5);
  green->SetShininess(50);
  green->SetReflectivity(0);

  // create center visual
  VisualPtr center = _scene->CreateVisual();
  center->AddGeometry(_scene->CreateSphere());
  center->SetLocalPosition(3, 0, 0);
  center->SetLocalScale(0.1, 0.1, 0.1);
  center->SetMaterial(green);
  root->AddChild(center);

  // create red material
  MaterialPtr red = _scene->CreateMaterial();
  red->SetAmbient(0.5, 0.0, 0.0);
  red->SetDiffuse(1.0, 0.0, 0.0);
  red->SetSpecular(0.5, 0.5, 0.5);
  red->SetShininess(50);
  red->SetReflectivity(0);
  red->SetRenderOrder(3);

  // create sphere visual
  VisualPtr sphere = _scene->CreateVisual();
  sphere->AddGeometry(_scene->CreateSphere());
  sphere->SetOrigin(0.0, -0.5, 0.0);
  sphere->SetLocalPosition(3, -1, 0);
  sphere->SetLocalRotation(0, 0, 0);
  sphere->SetLocalScale(1, 1, 1);
  sphere->SetMaterial(red);
  sphere->SetWireframe(true);
  root->AddChild(sphere);

  // create blue material
  MaterialPtr blue = _scene->CreateMaterial();
  blue->SetAmbient(0.0, 0.0, 0.5);
  blue->SetDiffuse(0.0, 0.0, 1.0);
  blue->SetSpecular(0.5, 0.5, 0.5);
  blue->SetShininess(50);
  blue->SetReflectivity(0);
  blue->SetTransparency(0.5);
  blue->SetDepthWriteEnabled(false);

  // create box visual
  VisualPtr box = _scene->CreateVisual("parent_box");
  box->AddGeometry(_scene->CreateBox());
  box->SetOrigin(0.0, 0.0, 0.0);
  box->SetLocalPosition(4.5, 0.0, 0.0);
  box->SetLocalRotation(0, 0, 0);
  box->SetMaterial(blue);
  root->AddChild(box);

  // create white material
  MaterialPtr white = _scene->CreateMaterial();
  white->SetAmbient(0.5, 0.5, 0.5);
  white->SetDiffuse(0.8, 0.8, 0.8);
  white->SetReceiveShadows(true);
  white->SetReflectivity(0);
  white->SetRenderOrder(0);

  // create plane visual
  VisualPtr plane = _scene->CreateVisual();
  plane->AddGeometry(_scene->CreatePlane());
  plane->SetLocalScale(5, 8, 1);
  plane->SetLocalPosition(3, 0, -0.5);
  plane->SetMaterial(white);
  root->AddChild(plane);

  // create inertia visual
  InertiaVisualPtr inertiaVisual = _scene->CreateInertiaVisual();
  ignition::math::MassMatrix3d massMatrix(1.0, {0.1, 0.1, 0.1}, {0.0, 0.0, 0.0});
  ignition::math::Pose3d p(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  ignition::math::Inertiald inertial{massMatrix, p};
  inertiaVisual->SetInertial(inertial);
  inertiaVisual->SetLocalPosition(1, 0, 0);
  root->AddChild(inertiaVisual);

  // create CoM visual
  COMVisualPtr comVisual = _scene->CreateCOMVisual();
  ignition::math::MassMatrix3d comMassMatrix(
      5.0, {0.1, 0.1, 0.1}, {0.0, 0.0, 0.0});
  ignition::math::Pose3d comPose(
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  ignition::math::Inertiald comVisualInertial{comMassMatrix, comPose};
  comVisual->SetInertial(comVisualInertial);
  comVisual->SetParentLink("parent_box");
  box->AddChild(comVisual);

  // create camera
  CameraPtr camera = _scene->CreateCamera("camera");
  camera->SetLocalPosition(0.0, 0.0, 0.0);
  camera->SetLocalRotation(0.0, 0.0, 0.0);
  camera->SetImageWidth(800);
  camera->SetImageHeight(600);
  camera->SetAntiAliasing(2);
  camera->SetAspectRatio(1.333);
  camera->SetHFOV(IGN_PI / 2);
  root->AddChild(camera);

  // track target
  camera->SetTrackTarget(center);
}

//////////////////////////////////////////////////
CameraPtr createCamera(const std::string &_engineName)
{
  // create and populate scene
  RenderEngine *engine = rendering::engine(_engineName);
  if (!engine)
  {
    std::cout << "Engine '" << _engineName
              << "' is not supported" << std::endl;
    return CameraPtr();
  }
  ScenePtr scene = engine->CreateScene("scene");
  buildScene(scene);

  // return camera sensor
  SensorPtr sensor = scene->SensorByName("camera");
  return std::dynamic_pointer_cast<Camera>(sensor);
}

//////////////////////////////////////////////////
int main(int _argc, char** _argv)
{
  glutInit(&_argc, _argv);

  // Expose engine name to command line because we can't instantiate both
  // ogre and ogre2 at the same time
  std::string engine("ogre");
  if (_argc > 1)
  {
    engine = _argv[1];
  }

  common::Console::SetVerbosity(4);
  std::vector<std::string> engineNames;
  std::vector<CameraPtr> cameras;

  engineNames.push_back(engine);
  engineNames.push_back("optix");

  for (auto engineName : engineNames)
  {
    try
    {
      CameraPtr camera = createCamera(engineName);
      if (camera)
      {
        cameras.push_back(camera);
      }
    }
    catch (...)
    {
      std::cerr << "Error starting up: " << engineName << std::endl;
    }
  }
  run(cameras);
  return 0;
}
