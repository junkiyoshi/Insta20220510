#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(2);
	ofNoFill();

	this->radius = 20;

	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds(20, 20, 680, 680);
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	ofColor color;
	for (int i = 0; i < 50; i++) {
		
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.5, 0.63, 0.1);
		circle->setup(this->box2d.getWorld(), ofRandom(30, 670), ofRandom(30, 670), this->radius);
		this->circle_list.push_back(circle);

		color.setHsb(ofMap(i, 0, 50, 0, 255), 180, 255);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	for (int i = 0; i < this->circle_list.size(); i++) {

		for (int j = i + 1; j < this->circle_list.size(); j++) {

			float distance = this->circle_list[i]->getPosition().distance(this->circle_list[j]->getPosition());
			if (distance < this->radius * 4) {

				this->circle_list[i]->addForce(this->circle_list[i]->getPosition() - this->circle_list[j]->getPosition(), ofMap(distance, this->radius, this->radius * 4, 1, 2));
				this->circle_list[j]->addForce(this->circle_list[j]->getPosition() - this->circle_list[i]->getPosition(), ofMap(distance, this->radius, this->radius * 4, 1, 2));
			}
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofMesh line;
	line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	auto threshold = 100;
	for (int i = 0; i < this->circle_list.size(); i++) {

		glm::vec2 location = this->circle_list[i]->getPosition();

		ofSetColor(this->color_list[i]);
		ofDrawCircle(location, 10);

		for (int k = 0; k < this->circle_list.size(); k++) {

			glm::vec2 other = this->circle_list[k]->getPosition();
			if (location == other) { continue; }

			auto distance = glm::distance(location, other);
			if (distance < threshold) {

				auto direction_rad = std::atan2(other.y - location.y, other.x - location.x);
				auto direction = direction_rad * RAD_TO_DEG;
				auto width = ofMap(distance, 0, threshold, 360, 0);

				ofNoFill();

				ofBeginShape();
				for (auto deg = direction - width * 0.65; deg <= direction + width * 0.65; deg++) {

					ofVertex(location.x + this->radius * cos(deg * DEG_TO_RAD), location.y + this->radius * sin(deg * DEG_TO_RAD));
				}
				ofEndShape();

				line.addVertex(glm::vec3(location + glm::vec2(this->radius * cos(direction * DEG_TO_RAD), this->radius * sin(direction * DEG_TO_RAD)), 0));
				line.addVertex(glm::vec3(other + glm::vec2(this->radius * cos((180 + direction) * DEG_TO_RAD), this->radius * sin((180 + direction) * DEG_TO_RAD)), 0));

				line.addColor(this->color_list[i]);
				line.addColor(this->color_list[k]);

				line.addIndex(line.getNumVertices() - 1);
				line.addIndex(line.getNumVertices() - 2);
			}
		}
	}

	ofSetColor(255);
	ofDrawRectangle(20, 20, 680, 680);
	line.drawWireframe();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}