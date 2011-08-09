#include "testApp.h"
#include "ofxSimpleGuiToo.h"



float	translateX,translateXPrev;
float	translateY,translateYPrev;
float	translateZ,translateZPrev;
float	rotX,rotXPrev = 0;
float	rotY,rotYPrev = 0;
float	rotZ,rotZPrev = 0;

float rotationView;

int selectedJointIndex = - 1;
bool autoRotate = false;


//--------------------------------------------------------------
void testApp::setup(){
	
	//ofSetFrameRate(60);
	
	gui.setAutoSave(false);
	
	// not working because the rotation matrix resets the translation matrix
	
//	gui.addTitle("Translate (not working)");
//	gui.addSlider("translate X", translateX, -2, 2); 
//	gui.addSlider("translate Y", translateY, -3, 3); 
//	gui.addSlider("translate Z", translateZ, -5, 5); 
	gui.addSlider("Rotation", rotationView, -360, 360); 
	gui.addToggle("Auto rotate", autoRotate);	
	
    //ofSetLogLevel(OF_LOG_VERBOSE);

    // we need GL_TEXTURE_2D for our models coords.
    ofDisableArbTex();

	if (model.setup("Seymour_triangulate.dae"))
	{
		gui.addComboBox("Select Joint", selectedJointIndex, model.nrOfJoints,  model.jointsLabels);
	}
	
	gui.addTitle("Rotate bones");
	
	gui.addSlider("X", rotX,-5, 5); 
	gui.addSlider("Y", rotY, -5, 5); 
	gui.addSlider("Z", rotZ, -5, 5); 

	gui.loadFromXML();
	gui.show();
	

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	glEnable(GL_DEPTH_TEST);

    //some model / light stuff
    glShadeModel(GL_SMOOTH);
    light.enable();
    ofEnableSeparateSpecularLight();
}

//--------------------------------------------------------------
void testApp::update(){
    	
//	aiMatrix4x4 matrix =bone->mTransformation;

//			if (abs(translateX - translateXPrev) >0) {
//				aiMatrix4x4 trans;
//				bone->mTransformation.Translation(aiVector3D(translateX - translateXPrev,0,0),matrix);
//				translateXPrev = translateX;
//				//matrix *= trans;			
//				matrix.a4 = translateX;
//			}
		
		if (abs(rotX - rotXPrev) >0) {
			model.rotateX(model.joints[selectedJointIndex],rotX - rotXPrev);			
			rotXPrev = rotX;
		}
	
		if (abs(rotY - rotYPrev) >0) {
			model.rotateY(model.joints[selectedJointIndex],rotY - rotYPrev);			
			rotYPrev = rotY;
		}				

		if (abs(rotZ - rotZPrev) >0) {
			model.rotateZ(model.joints[selectedJointIndex],rotZ - rotZPrev);			
			rotZPrev = rotZ;
		}				
	
		// update model is some extract from update animation.
		// have to clear this out because this is to much overhead at the moment
		model.updateModel();				
}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(50, 50, 50, 0);
    ofSetColor(255, 255, 255, 255);

	glEnable(GL_DEPTH_TEST);

    ofPushMatrix();
		ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
		ofRotate(rotationView + ((autoRotate) ? ofGetElapsedTimef() * 10 : 0), 0, 1, 0);
		ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

		model.drawFaces();

    ofPopMatrix();
		
	glDisable(GL_DEPTH_TEST);

   	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
 }

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

