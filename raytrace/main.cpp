#include "icg_common.h"
#include "camera.h"
#include "imageplane.h"
#include "sphere.h"
#include "light.h"
#include "plane.h"
#include "object.h"
#include <Eigen/Geometry>
#include <math.h>


#ifndef WITH_OPENCV
    #error OpenCV required for this exercise
#endif

typedef cv::Vec3b Colour;
typedef cv::Vec3f Coefficient;
Colour red() { return Colour(0, 0, 255); }
Colour blue() { return Colour(255, 0, 0); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

Coefficient objectRed() { return Coefficient(0.0f, 0.0f, 1.0f); }
Coefficient objectBlue() { return Coefficient(1.0f, 0.0f, 0.0f); }
Coefficient objectWhite() { return Coefficient(1.0f, 1.0f, 1.0f); }
Coefficient objectBlack() { return Coefficient(0.0f, 0.0f, 0.0f); }

enum Special { TEXTURE, NO_TEXTURE, REFLECTION };

struct MyImage{
    /// Data (not private for convenience)
    int cols = 600;
    int rows = 600;
    ///  Channel with [0..255] range image (aka uchar)
    cv::Mat image = cv::Mat(rows, cols, CV_8UC3, cv::Scalar(255,255,255));

    Colour& operator()(int row, int col)
    {
        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);
        return image.at<cv::Vec3b>(cv::Point(col, row));
    }

    /// Displays the image and pauses program execution (until key pressed)
    void show(){
        /// Display the image
        cv::imshow("image", image);
        cv::waitKey(0);
    }

    /// Use files with "*.png" extension
    void save(const std::string& filename){
        cv::imwrite(filename, image);
    }
};

/**
 * @brief priorityObjectIndex
 * @param intersectionObjects
 * @return Int
 * Returns the winning index of which object to paint for each pixel
 */
int priorityObjectIndex(vector<float> intersectionObjects)
{
    int minimumValue;
    if(intersectionObjects.size() == 0)
    {
        return -1;
    } else if(intersectionObjects.size() == 1)
    {
        if(intersectionObjects.at(0) > 0)
        {
            return 0;
        }

        return -1;
    }
    float max = 0;
    for(int i = 0; i < intersectionObjects.size(); ++i)
    {
        if(max < intersectionObjects.at(i)) {
            max = intersectionObjects.at(i);
        }
    }
    if(max > 0)
    {
        for(int i = 0; i < intersectionObjects.size(); ++i)
        {
            if(intersectionObjects.at(i) > 0 && intersectionObjects.at(i) <= max)
            {
                max = intersectionObjects.at(i);
                minimumValue = i;
            }
        }
        return minimumValue;
    }
    return -1;
}

Colour colourIt(ParametrizedLine<float, 3> const &reflectedRay, vector<Object*> scene, Colour light)
{
    vector<float> reflectedIntersections;

    for(int k = 0; k < scene.size(); ++k)
    {
        reflectedIntersections.push_back(scene.at(k)->intersectRayValue(reflectedRay));
    }
    int priorityReflectionIndex = priorityObjectIndex(reflectedIntersections);
    Colour reflection_colour = scene.at(priorityReflectionIndex)->ambient(light);
    return reflection_colour;
}

int main(int, char**){
    /// Rays and vectors represented with Eigen
    typedef Eigen::Vector3f vec3;
    typedef Eigen::ParametrizedLine<float, 3> ray3;
    
    MyImage image;
    
    /// Define camera, light, and image plane

    Camera camera(vec3(0,-1,-8));
    Light light(vec3(-3.5f, -2, -1), white());
    Light light2(vec3(-3.5f, 0, -1), white());
    Light light3(vec3(-3.5f, 2, -1), white());
    Light light4(vec3(-3.5f, 0, -3), white());
    Light light5(vec3(-3.5f, 0, 1), white());
    ImagePlane plane(vec3(-4,-4,-1), vec3(4,4,1), image.rows, image.cols);

    /// Define sphere and plane
    Sphere sphere(vec3(0,0,-1), 0.4f, Coefficient(0.5f, 0.0f, 0.0f), TEXTURE);
    Sphere sphere2(vec3(-0.8f,-1,-1), 0.5f, Coefficient(0.5f, 0.25f, 0.25f), NO_TEXTURE);
    Sphere sphere3(vec3(0,-1,-3), 0.5f, Coefficient(0.34f, 0.75f, 0.69f), REFLECTION);
    Plane floorPlane(vec3(1, 0, 0), vec3(1, 0, 0), Coefficient(1, 1, 1), TEXTURE);
    floorPlane.setKd(Coefficient(0.2f, 0.2f, 0.2f));
    Plane rightPlane(vec3(1, 3, 0), vec3(1, 60, 0), Coefficient(0, 0.392f, 0), NO_TEXTURE);
    Plane leftPlane(vec3(1, -2, 0), vec3(1, -190, 0), Coefficient(0, 0, 1), NO_TEXTURE);
    Plane wallPlane(vec3(1, 0, 3), vec3(1, 0, 20), Coefficient(0.176f, 0.322f, 0.627), NO_TEXTURE);
    Plane ceilingPlane(vec3(-3.5f, 0, -1), vec3(15, 0, -1), Coefficient(0.176f, 0.322f, 0.627), NO_TEXTURE);
    /// Define Object vector to push all the spheres and planes into
    vector<Object*> scene;
    /// Create Area lights
    vector<Light> lightScene;
    lightScene.push_back(light);
    lightScene.push_back(light2);
    lightScene.push_back(light3);
    lightScene.push_back(light4);
    lightScene.push_back(light5);
    scene.push_back(dynamic_cast<Object*>(&floorPlane));
    scene.push_back(dynamic_cast<Object*>(&rightPlane));
    scene.push_back(dynamic_cast<Object*>(&leftPlane));
    scene.push_back(dynamic_cast<Object*>(&wallPlane));
    scene.push_back(dynamic_cast<Object*>(&ceilingPlane));
    scene.push_back(dynamic_cast<Object*>(&sphere));
    scene.push_back(dynamic_cast<Object*>(&sphere2));
    scene.push_back(dynamic_cast<Object*>(&sphere3));
    /// for floating point
    float accuracy = 0.00000001;
    /// sets the shade coefficient for shadows
    float shade = 0.5f;
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            vec3 pt = plane.generatePixelPos(row, col);
            ray3 ray = camera.generateRay(pt);
            vector<float> intersections;
            /// push into intersections vector
            for(int i = 0; i < scene.size(); ++i)
            {
                intersections.push_back(scene.at(i)->intersectRayValue(ray));
            }
            /// get winning index of the object to paint
            int indexPriorityOfObject = priorityObjectIndex(intersections);
            if(indexPriorityOfObject == -1)
            {
                image(row, col) = black();
            } else
            {
                if(intersections.at(indexPriorityOfObject) > accuracy)
                {
                    /// go through sphere items
                    if(Sphere* c = dynamic_cast<Sphere*>(scene[indexPriorityOfObject]))
                    {
                        bool shadow = false;
                        float spherePt = c->intersectRayValue(ray);
                        vec3 sphereHitPt = c->getIntersectPoint(ray, spherePt);
                        ray3 rayToLight = light.generateRay(sphereHitPt);
                        for(int i = 0; i < scene.size(); ++i)
                        {
                            if(Sphere* e = dynamic_cast<Sphere*>(scene[i]))
                            {
                                if(e != c)
                                {
                                    /// check if object is in shadow
                                    for(auto & element : lightScene)
                                    {
                                        if(e->intersectRayForShadow(element.generateRay(sphereHitPt)))
                                        {
                                            shadow = true;
                                        }
                                    }
                                }

                            }
                        }
                        /// perform phong shadings
                        ray3 sphereNormal = c->getNormal(sphereHitPt);
                        ray3 rayToCamera = camera.rayToCamera(sphereHitPt);
                        cv::Vec3b ambientComponent = c->ambient(light.getColour());
                        cv::Vec3b diffuseComponent = c->diffuse(sphereNormal, sphereHitPt, lightScene);
                        cv::Vec3b specularComponent = c->specular(sphereNormal, sphereHitPt, rayToCamera, lightScene);
                        cv::Vec3b illumination = diffuseComponent + ambientComponent + specularComponent;
                        cv::Vec3b textureComponent = c->textureValue(sphereHitPt);
                        /// check if sphere has texture
                        if(c->getSpecial() == TEXTURE)
                        {
                            illumination = textureComponent + diffuseComponent + specularComponent;
                        } else if(c->getSpecial() == REFLECTION)
                        {
                            vec3 reflection = sphereHitPt - 2 * (sphereHitPt.dot(sphereNormal.direction())) * sphereNormal.direction();
                            reflection.normalize();
                            ray3 reflectionRay(sphereHitPt, reflection);
                            vector<float> reflectedIntersections;

                            for(int k = 0; k < scene.size(); ++k)
                            {
                                reflectedIntersections.push_back(scene.at(k)->intersectRayValue(reflectionRay));
                            }

                            int priorityReflectionIndex = priorityObjectIndex(reflectedIntersections);
                            Colour reflection_colour;
                            if(priorityReflectionIndex != -1) {
                                reflection_colour = colourIt(ray3(sphereHitPt + reflection * 0.0001f, reflection), scene, light.getColour());
                            } else {
                                reflection_colour = black();
                            }

                            illumination = reflection_colour + diffuseComponent + specularComponent;
                        }
                        if(shadow)
                        {   if(c->getSpecial() == TEXTURE)
                            {
                                illumination = textureComponent * shade;
                            } else
                            {
                                illumination = ambientComponent * shade;
                            }
                        }

                        image(row, col) = illumination;
                        /// go through plane objects
                    } else if(Plane* b = dynamic_cast<Plane*>(scene[indexPriorityOfObject]))
                    {
                        bool shadow = false;
                        float planePt = b->intersectRayValue(ray);
                        vec3 planeHitPt = b->getIntersectPoint(ray, planePt);
                        ray3 rayToLight = light.generateRay(planeHitPt);
                        for(int i = 0; i < scene.size(); ++i)
                        {
                            /// check if plane is in shadow from sphere
                            if(Sphere* d = dynamic_cast<Sphere*>(scene[i]))
                            {
                                for(auto & element : lightScene) {
                                    float distanceToLight = (b->getPosition() - element.getPosition()).norm();
                                    float distanceToObject = (b->getPosition() - d->getCentre()).norm();
                                    if(d->intersectRayForShadow(element.generateRay(planeHitPt)) && (distanceToLight > distanceToObject))
                                    {
                                        shadow = true;
                                    }
                                }
                            }
                        }
                        /// perform phong shading
                        ray3 planeNormal = b->getNormal(planeHitPt);
                        ray3 rayToCamera = camera.rayToCamera(planeHitPt);
                        cv::Vec3b ambientComponent = b->ambient(light.getColour());
                        cv::Vec3b diffuseComponent = b->diffuse(planeNormal, planeHitPt, lightScene);
//                        cv::Vec3b specularComponent = b->specular(planeNormal, rayToLight, rayToCamera, light.getColour());
                        cv::Vec3b illumination = diffuseComponent + ambientComponent;
                        /// check if floor has a texture
                        if(b->getSpecial() == TEXTURE)
                        {
                            illumination = light.getColour().mul(b->checkerBoard(planeHitPt)) + diffuseComponent;
                        }

                        if(shadow)
                        {
                            if(b->getSpecial() == TEXTURE)
                            {
                                illumination = illumination * shade;
                            } else
                            {
                                illumination = ambientComponent * shade;
                            }
                        }
                        image(row, col) = illumination;
                    }
                }
            }
       }
    }
    
    image.show();
    // image.save("output.png"); ///< Does not work on Windows!

    return EXIT_SUCCESS;
}
