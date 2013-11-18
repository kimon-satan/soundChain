#include "pathContainer.h"

pathContainer::pathContainer(){

    m_corners = 0.25;
    m_spacing = 3;
    m_posC.set(0,0,0);
    m_rotC = 0;
    pathUtils::createRoundedRect(m_polyC, ofPoint(0,0,0), 200, 200, m_corners, m_spacing);
    updateO();

}


void pathContainer::start(ofVec2f b_posO, float s_val){

    m_bPosO.set(b_posO);

    if(m_tData->type == "pivot"){

        shared_ptr<pivotTData> t_piv(static_pointer_cast <pivotTData>(m_tData));

        t_piv->start = s_val;

        if(t_piv->isLocal)
                t_piv->pivotGlobal.set(localToWorldPoint(t_piv->pivotLocal));
            else
                t_piv->pivotGlobal.set(t_piv->pivotLocal);
    }

    if(m_tData->type == "translate"){

        shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));

        if(t_trans->isLocal){
            t_trans->dirGlobal.set(localToWorldVec(t_trans->dirLocal));
        }else{
            t_trans->dirGlobal.set(t_trans->dirLocal);
        }

        if(t_trans->isBoundsFromPath){

            if(t_trans->isPixelBuf){
                pathUtils::setBoundsFromPath(m_polyA, t_trans->bounds ,m_bPosO, t_trans->dirGlobal, t_trans->pixelBuf, true);
            }else{
                pathUtils::setBoundsFromPath(m_polyA, t_trans->bounds ,m_bPosO, t_trans->dirGlobal, t_trans->prop, true);
            }

            ofVec2f v = t_trans->dirGlobal.getNormalized();
            for(int  i =0; i < 2; i++)t_trans->bounds[i] += ofVec2f(m_posO - b_posO);
            t_trans->start = ofVec2f(m_posO - t_trans->bounds[0]).length()/ofVec2f(t_trans->bounds[1] - t_trans->bounds[0]).length();

        }else{

            //this won't always work here because hold is the other way round ?
            t_trans->start = s_val;
            t_trans->bounds.clear();
            t_trans->bounds.push_back(m_posO - t_trans->dirGlobal * t_trans->range * t_trans->start);
            t_trans->bounds.push_back(m_posO + t_trans->dirGlobal * t_trans->range * (1 - t_trans->start));

        }



    }

}

void pathContainer::update(float val){

    //determine transform type and downcast
    //perform relevant transform

    if(m_tData->type == "pivot"){
        shared_ptr<pivotTData> t_piv(static_pointer_cast <pivotTData>(m_tData));
        float ang = utils::valToRot(val, t_piv->range, t_piv->start);
        rotateFromO(ang, t_piv->pivotGlobal);
    }

    if(m_tData->type == "translate"){
        shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));
        translateFromO( utils::valToVec(val, t_trans->bounds, m_posO));
    }

}

void pathContainer::updateO() {

    m_polyO = m_polyC; // incase of resize or resample

    m_rotO = m_rotC;
    m_posO = m_posC;

    vector<ofPoint> t_verts = m_polyO.getVertices();
    for(int i = 0; i < t_verts.size(); i++){
        t_verts[i].rotate(m_rotC, ofVec3f(0,0,1));
        t_verts[i] += m_posC;
    }

    m_polyA.clear();
    m_polyA.addVertices(t_verts);


}


void pathContainer::resizeC(int width, int height) {

    pathUtils::createRoundedRect(m_polyO,
                                 m_polyO.getCentroid2D(),
                                 width, height,
                                 m_corners, m_spacing);

    m_polyC = m_polyO; // need to think how to handle translation etc.

}

void pathContainer::rotateFromO(float angle, ofVec2f pivot) {

    m_rotC = m_rotO + angle;
    m_posC = m_posO.getRotated(angle, pivot, ofVec3f(0,0,1));

}

void pathContainer::translateFromO(ofVec2f v){m_posC = m_posO + v;}

vector<ofVec2f> pathContainer::getIntersects(ofVec2f uv, ofVec2f p){
    return pathUtils::getIntersects(m_polyA, uv, p);
}

ofVec2f pathContainer::localToWorldPoint(ofVec2f local){

    ofVec2f p;
    ofRectangle r = m_polyC.getBoundingBox();
    p.set(m_posC.x + r.width * local.x, m_posC.y + r.height * local.y);
    p.rotate(m_rotC, m_posC);
    return p;
}

float pathContainer::localToWorldRot(float local){
    float r = local + m_rotC;
    while(r > 180)r -= 360;
    while(r < -180)r += 360;
    return r;
}

ofVec2f pathContainer::localToWorldVec(ofVec2f local){
    return local.rotate(m_rotC);
}

void pathContainer::draw() {

    ofPushStyle();
    ofSetLineWidth(2);
    ofSetColor(0);
        ofPushMatrix();
        ofTranslate(m_posC.x, m_posC.y, m_posC.z);
        ofRotate(m_rotC, 0, 0, 1);
        m_polyC.draw();

        //debug draw
        /*vector<ofPoint> t_verts = m_polyC.getVertices();
        for(int i = 0; i < t_verts.size(); i++){
            ofCircle(t_verts[i],1);
        }*/

        ofPopMatrix();
    ofPopStyle();

    /*if(m_tData->type == "translate"){
        shared_ptr<transTData> t_trans(static_pointer_cast <transTData>(m_tData));
        if(t_trans->bounds.size() > 0){
            ofCircle(t_trans->bounds[0],5);
            ofCircle(t_trans->bounds[1],5);
        }
    }*/

}

float pathContainer::getStartVal(){return m_tData->start;}

ofRectangle pathContainer::getDims(){
    return m_polyO.getBoundingBox();
}


ofPoint pathContainer::getPos(){ return m_posC; }
float pathContainer::getRot(){ return m_rotC; }


void pathContainer::setTransform(shared_ptr <baseTData> b){
    m_tData = b;
}

ofPolyline pathContainer::getPolyA(){return m_polyA;}

pathContainer::~pathContainer() {
    //dtor
}
