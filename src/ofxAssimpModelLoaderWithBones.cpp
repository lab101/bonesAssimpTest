/*
 *  ofxAssimpModelLoaderWithBones.cpp
 *  bonesAssimpTest
 *
 *  Created by Kris Meeusen on 09/08/11.
 *  Copyright 2011 Lab101. All rights reserved.
 *
 */


#include "ofxAssimpModelLoaderWithBones.h"

bool ofxAssimpModelLoaderWithBones::setup(string fileName)
{
	nrOfJoints = 0;
	
	if(loadModel(fileName,true)){
    	setAnimation(0);
    	setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
    	//createLightsFromAiModel();
    	//model.disableTextures();
    	//model.disableMaterials();
		
		/*
    	mesh = model.getMesh(0);
    	position = model.getPosition();
    	normScale = model.getNormalizedScale();
    	scale = model.getScale();
    	sceneCenter = model.getSceneCenter();
    	material = model.getMaterialForMesh(0);
        tex = model.getTextureForMesh(0);
		*/
		
		
		// currently checking the root node but a better way would be to 
		// read the library_visual_scenes in collada file.
		
		aiNode* root = scene->mRootNode->FindNode("root");
		addChildNodes(root,"-");
		return true;
	}
	return false;		
}


bool ofxAssimpModelLoaderWithBones::addChildNodes(aiNode* node,string levelPrefix)
{
	if (node->mNumChildren > 0) {
		for (int i=0 ;i < (int)node->mNumChildren;i++) {
			//cout << "child " << node->mChildren[i]->mName.data << endl;
			
			aiNode* childNode = node->mChildren[i];
			joints[nrOfJoints] = childNode->mName.data;
			jointsLabels[nrOfJoints] = levelPrefix + childNode->mName.data;
			
			nrOfJoints++;
			
			if (childNode->mNumChildren > 0) {
				addChildNodes(childNode,levelPrefix + "--");
			}
		}
	}
	return false;
}

void ofxAssimpModelLoaderWithBones::rotateX(string boneName,float angle)
{
	aiNode* bone = scene->mRootNode->FindNode(boneName);
	aiMatrix4x4 rotation;
	aiMatrix4x4::RotationX(angle,rotation);
	bone->mTransformation *= rotation;
}

void ofxAssimpModelLoaderWithBones::rotateY(string boneName,float angle)
{
	aiNode* bone = scene->mRootNode->FindNode(boneName);
	aiMatrix4x4 rotation;
	aiMatrix4x4::RotationY(angle,rotation);
	bone->mTransformation *= rotation;
}

void ofxAssimpModelLoaderWithBones::rotateZ(string boneName,float angle)
{
	aiNode* bone = scene->mRootNode->FindNode(boneName);
	aiMatrix4x4 rotation;
	aiMatrix4x4::RotationZ(angle,rotation);
	bone->mTransformation *= rotation;
}




// adjusted copy of updateanimation.
// todo find out which lines are code are really important to update the model

void ofxAssimpModelLoaderWithBones::updateModel(){
	//for (unsigned int i = 0; i < modelMeshes.size(); ++i){
	
	// current mesh we are introspecting
	//const aiMesh* mesh = modelMeshes[i].mesh;
	//modelMeshes[i].hasChanged = true;
	//}
	
	
	
	// update mesh position for the animation
	for (unsigned int i = 0; i < modelMeshes.size(); ++i){
		
		// current mesh we are introspecting
		const aiMesh* mesh = modelMeshes[i].mesh;
		
		// calculate bone matrices
		std::vector<aiMatrix4x4> boneMatrices( mesh->mNumBones);
		for( size_t a = 0; a < mesh->mNumBones; ++a)
		{
			const aiBone* bone = mesh->mBones[a];
			
			// find the corresponding node by again looking recursively through the node hierarchy for the same name
			aiNode* node = scene->mRootNode->FindNode(bone->mName);
			
			// start with the mesh-to-bone matrix
			boneMatrices[a] = bone->mOffsetMatrix;
			// and now append all node transformations down the parent chain until we're back at mesh coordinates again
			const aiNode* tempNode = node;
			while( tempNode)
			{
				// check your matrix multiplication order here!!!
				boneMatrices[a] = tempNode->mTransformation * boneMatrices[a];
				// boneMatrices[a] = boneMatrices[a] * tempNode->mTransformation;
				tempNode = tempNode->mParent;
			}
			modelMeshes[i].hasChanged = true;
			modelMeshes[i].validCache = false;
		}
		
		
		modelMeshes[i].animatedPos.assign(modelMeshes[i].animatedPos.size(),0);
		if(mesh->HasNormals()){
			modelMeshes[i].animatedNorm.assign(modelMeshes[i].animatedNorm.size(),0);
		}
		// loop through all vertex weights of all bones
		for( size_t a = 0; a < mesh->mNumBones; ++a)
		{
			const aiBone* bone = mesh->mBones[a];
			const aiMatrix4x4& posTrafo = boneMatrices[a];
			
			
			for( size_t b = 0; b < bone->mNumWeights; ++b)
			{
				const aiVertexWeight& weight = bone->mWeights[b];
				
				size_t vertexId = weight.mVertexId;
				const aiVector3D& srcPos = mesh->mVertices[vertexId];
				
				modelMeshes[i].animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
			}
			if(mesh->HasNormals()){
				// 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
				aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo);
				for( size_t b = 0; b < bone->mNumWeights; ++b)
				{
					const aiVertexWeight& weight = bone->mWeights[b];
					size_t vertexId = weight.mVertexId;
					
					const aiVector3D& srcNorm = mesh->mNormals[vertexId];
					modelMeshes[i].animatedNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);
					
				}
			}
		}
	}
}