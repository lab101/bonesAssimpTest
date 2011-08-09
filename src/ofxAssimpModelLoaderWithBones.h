/*
 *  ofxAssimpModelLoaderWithBones.h
 *  bonesAssimpTest
 *
 *  Created by Kris Meeusen on 09/08/11.
 *  Copyright 2011 Lab101. All rights reserved.
 *
 */

#ifndef _AssimpModelLoaderWithBones
#define _AssimpModelLoaderWithBones

#include "ofxAssimpModelLoader.h";

class ofxAssimpModelLoaderWithBones : public ofxAssimpModelLoader
{
	public:
		bool setup(string fileName);
		string joints[100];
		string jointsLabels[100];
	
		int nrOfJoints;
	
		void rotateX(string boneName,float angle);	
		void rotateY(string boneName,float angle);
		void rotateZ(string boneName,float angle);

		bool addChildNodes(aiNode* node,string levelPrefix);
		
		void updateModel();

};

#endif